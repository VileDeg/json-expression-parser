#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <memory>
#include <assert.h>

class JsonValue;

using JsonObject = std::unordered_map<std::string, std::shared_ptr<JsonValue>>;
using JsonArray = std::vector<std::shared_ptr<JsonValue>>;
using JsonString = std::string;
using JsonNumber = double;
using JsonBoolean = bool;
using JsonNull = std::nullptr_t;


class JsonValue {
public:
    using ValueType = std::variant<JsonObject, JsonArray, JsonString, JsonNumber, JsonBoolean, JsonNull>;

    JsonValue() : value(nullptr) {}
    JsonValue(ValueType v) : value(v) {}

    ValueType value;
};

class JsonParser {
public:
    JsonValue Parse(std::ifstream& file);

private:
    JsonValue parseValue(std::ifstream& file);

    JsonObject parseObject(std::ifstream& file);

    JsonArray parseArray(std::ifstream& file);

    JsonString parseString(std::ifstream& file);

    JsonNumber parseNumber(std::ifstream& file);
    
    JsonBoolean parseBoolean(std::ifstream& file);
    
    JsonNull parseNull(std::ifstream& file);

    inline void throwRuntimeError(const std::string& message) {
        throw std::runtime_error("[Line: " + std::to_string(_line) 
            + "] [Column: " + std::to_string(_column) + "]: " 
            + message);
    }

    inline void nextChar(std::ifstream& file) {
        if (!file.get(_ch)) {
            if (file.eof()) {
                throwRuntimeError("End of file reached");
            } else {
                throwRuntimeError("Error reading character from file");
            }
        }

        if (_ch == '\n') {
            _line++;
            _column = 0;
        } else {
            _column++;
        }
    }

    inline void nextCharSkipWS(std::ifstream& file) {
        do {
            nextChar(file);
        } while (std::isspace(_ch));
    }

    char _ch;

    int _line = 1;
    int _column = 0;
};
