#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <assert.h>

#include "json_types.h"

class JsonParser {
public:
    std::shared_ptr<JsonValue> Parse(std::ifstream& file);

private:
    std::shared_ptr<JsonValue> parseValue(std::ifstream& file);

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

    inline void returnChar(std::ifstream& file) {
        file.putback(_ch);

        if (_ch == '\n') {
            _line--;
            // Assuming previous line length is unknown, reset _column
            _column = 0;
        } else {
            _column--;
        }
    }

    char _ch;

    int _line = 1;
    int _column = 0;
};
