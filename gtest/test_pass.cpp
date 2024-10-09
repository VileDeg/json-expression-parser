#include <gtest/gtest.h>

#include "core.h"

#include <memory>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../src/json_parser.h"
#include "../src/json_eval.h"

class PassTest : public EvalTest {
protected:
    void evalExpr(const std::string& fileName, const std::string& expression, const std::string& expected) {
        std::string filePath = _testDirectory + fileName;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }

        JsonParser parser;
        std::shared_ptr<JsonValue> root;
        ASSERT_NO_THROW(root = parser.Parse(file));

        file.close();

        JsonEval evaluator(root);
        std::shared_ptr<JsonValue> expressionResult{};


        std::string exprCopy = expression;
        ASSERT_NO_THROW(expressionResult = evaluator.EvaluateExpression(root, exprCopy));

        std::stringstream evalOut;
        evalOut << *expressionResult;
        std::string actual = evalOut.str();

        ASSERT_EQ(actual, expected);
    }
};

TEST_F(PassTest, Case_00) {
    evalExpr("test.json", "a.b[1]", "2");
}

TEST_F(PassTest, Case_01) {
    evalExpr("test.json", "a.b[2].c", "test");
}

TEST_F(PassTest, Case_02) {
    evalExpr("test.json", "a.b", "[ 1, 2, { \"c\": \"test\" }, [ 11, 12 ] ]");
}

TEST_F(PassTest, Case_03) {
    evalExpr("test.json", "a.b[a.b[1]].c", "test");
}

TEST_F(PassTest, Case_04) {
    evalExpr("test.json", "a.b[a.b[a.b[0]]].c", "test");
}
