#include <gtest/gtest.h>

/* Credit: 
https://stackoverflow.com/questions/16491675/how-to-send-custom-message-in-google-c-testing-framework
*/
class TestCout : public std::stringstream {
public:
    ~TestCout()
    {
        std::cout << "\u001b[32m[          ] \u001b[33m" << str() << "\u001b[0m" << std::flush;
    }
};

#define TEST_COUT  TestCout()


class EvalTest : public ::testing::Test {
protected:
    std::string _testDirectory;

    void SetUp() override {
        // Set the test directory
        _testDirectory = "input/";
    }

    void TearDown() override {
        // Cleanup code here, if needed
    }
};