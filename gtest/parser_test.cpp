#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <fstream>
#include "../src/json_parser.h"

class ParserTest : public ::testing::Test {
protected:
    std::string _testDirectory;

    void SetUp() override {
        // Set the test directory
        _testDirectory = "input/";
    }

    void TearDown() override {
        // Cleanup code here, if needed
    }

    void compareStreamsIgnoringWhitespace(std::istream& stream1, std::istream& stream2) {
        std::string word1, word2;
        while (stream1 >> word1 && stream2 >> word2) {
            EXPECT_EQ(word1, word2);
        }
        EXPECT_FALSE(stream1 >> word1);
        EXPECT_FALSE(stream2 >> word2);
    }

    void parseFile(const std::string& fileName) {
        std::string filePath = _testDirectory + fileName;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }
        std::stringstream jsonInput;
        jsonInput << file.rdbuf();

        // Reset file
        file.clear();
        file.seekg(0);

        JsonParser parser;
        std::stringstream parserOut;

        std::shared_ptr<JsonValue> root = parser.Parse(file);
        std::shared_ptr<JsonObject> jsonObject = std::dynamic_pointer_cast<JsonObject>(root);
        ASSERT_NE(jsonObject, nullptr);

        parserOut << *jsonObject;

        // Compare the contents of the two stringstreams word by word, ignoring whitespace
        compareStreamsIgnoringWhitespace(jsonInput, parserOut);
    }
};

TEST_F(ParserTest, Case_00) {
    parseFile("test.json");
}

TEST_F(ParserTest, Case_01) {
    parseFile("01-empty_key.json");
}
