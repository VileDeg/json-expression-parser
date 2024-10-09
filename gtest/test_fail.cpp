#include <gtest/gtest.h>

#include "core.h"

#include <memory>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../src/json_parser.h"
#include "../src/json_eval.h"

class FailTest : public EvalTest {
protected:
    void evalExpr_Fail(const std::string& fileName, const std::string& expression) {
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
        try {
            expressionResult = evaluator.EvaluateExpression(root, exprCopy);
            ADD_FAILURE() << "Expected exception was not thrown.";
        } catch (const std::runtime_error& e) {
            TEST_COUT << "Exception: " << e.what() << std::endl;
            SUCCEED();
        }
    }
};


TEST_F(FailTest, missing_key) {
    evalExpr_Fail("test.json", "x");
}

TEST_F(FailTest, not_array_0) {
    evalExpr_Fail("test.json", "a[0]");
}

TEST_F(FailTest, not_array_1) {
    evalExpr_Fail("test.json", "a[?]");
}

TEST_F(FailTest, index_invalid) {
    evalExpr_Fail("test.json", "a.b[?]"); // TODO: throws wrong exception
}

TEST_F(FailTest, index_out_of_range) {
    evalExpr_Fail("test.json", "a.b[42]");
}

TEST_F(FailTest, index_out_of_range_negative) {
    evalExpr_Fail("test.json", "a.b[-1]");
}