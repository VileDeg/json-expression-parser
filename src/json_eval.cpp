#include "json_eval.h"
#include "json_types.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

//  a.b[a.b[1]].c

// a.b[1].c

std::shared_ptr<JsonValue> JsonEval::EvaluateExpression(std::shared_ptr<JsonValue> parent, std::string& expression)
{
    std::string token{};

    int i = 0;
    char ch{};
    for (; i < expression.length(); ++i) {
        ch = expression[i];

        if (ch == '.' || ch == '[' || ch == ']') {
            ++i;
            break;
        } else {
            token += ch;
        }
    }
    
    std::shared_ptr<JsonValue> current{};

    if (_afterArrayIndex) {
        assert(token == "");

        current = parent;

        _afterArrayIndex = false;
    } else {
        // If token is just a literal int index into an array
        try {
            int index = std::stoi(token);

            if (!_inArrayIndex) {
                throw std::runtime_error("Integer literal can only be used as an array index.");
            }

            current = std::make_shared<JsonNumber>(index);
        } catch (const std::runtime_error& e) {
            throw e; // propagate the error
        } catch (...) {
            std::shared_ptr<JsonObject> parentObj = std::dynamic_pointer_cast<JsonObject>(parent);
            if (!parentObj) {
                throw std::runtime_error("Token preceding '.' must be a JSON object.");
            }
            
            // Find current token in parent
            current = parentObj->get(token);
            if (!current) {
                throw std::runtime_error("Key \"" + token + "\" was not found in parent object.");
            }
        }
    }

    expression = expression.substr(i);

    if (ch == '.') {
        return EvaluateExpression(current, expression);
    } else if (ch == '[') {
        std::shared_ptr<JsonArray> currentArr = std::dynamic_pointer_cast<JsonArray>(current);
        if (!currentArr) {
            throw std::runtime_error("Token preceding '[' must be a JSON array.");
        }

        _inArrayIndex = true;

        std::shared_ptr<JsonValue> retVal = EvaluateExpression(_root, expression);
        
        _inArrayIndex = false;

        _afterArrayIndex = true;
        
        std::shared_ptr<JsonNumber> retNum = std::dynamic_pointer_cast<JsonNumber>(retVal);
        if (!retNum) {
            throw std::runtime_error("Expected number value as index in JSON array.");
        }

        if (!retNum->isInteger()) {
            throw std::runtime_error("Expected integer index in JSON array.");
        }

        int index = *retNum;

        std::shared_ptr<JsonValue> arrChild = currentArr->get(index);
        if (!arrChild) {
            throw std::runtime_error("Index '" + std::to_string(index) + "' is out of range.");
        }

        return EvaluateExpression(arrChild, expression);
    } else if (ch == ']') {
        
        return current;
    }

    return current;
}
