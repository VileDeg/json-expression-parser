#include "json_parser.h"
#include <cassert>
#include <stdexcept>

int JsonValue::s_LogDepth = 0;


std::shared_ptr<JsonValue> JsonParser::Parse(std::ifstream& file) {
    nextCharSkipWS(file);

    if (_ch != '{') {
        throwRuntimeError("The root of JSON file must be an object");
    }

    return parseValue(file);
}

std::shared_ptr<JsonValue> JsonParser::parseValue(std::ifstream& file) {
    switch (_ch) {
        case '{':
            return std::make_shared<JsonObject>(parseObject(file));
        case '[':
            return std::make_shared<JsonArray>(parseArray(file));
        case '"':
            return std::make_shared<JsonString>(parseString(file));
        case 't':
        case 'f':
            return std::make_shared<JsonBoolean>(parseBoolean(file));
            break;
        case 'n': 
            return std::make_shared<JsonNull>(parseNull(file));
            break;
    }

    return std::make_shared<JsonNumber>(parseNumber(file));
}

JsonObject JsonParser::parseObject(std::ifstream& file) {
    JsonObject obj;
    
    nextCharSkipWS(file); // Skip ws and read first char after '{'
    while (_ch != '}') {
        
        JsonString key = parseString(file);
        if (key.empty()) {
            // still need to parse to catch json format errors. :(  
        }
        nextCharSkipWS(file);
        if (_ch != ':') {
            throwRuntimeError("Invalid object format");
        }
        nextCharSkipWS(file);
        auto value = parseValue(file);

        if (!key.empty()) {
            obj.add(key, value);
        }

        nextCharSkipWS(file);
        if (_ch != ',' && _ch != '}') {
            throwRuntimeError("Missing comma between members");
        }
    }
    // Invariant: _ch == '}'
    return obj;
}

JsonArray JsonParser::parseArray(std::ifstream& file) {
    JsonArray arr;
    
    while (_ch != ']') {
        nextCharSkipWS(file);
        arr.add(parseValue(file));
        nextCharSkipWS(file);
        if (_ch != ',' && _ch != ']') {
            throwRuntimeError("Missing comma between elements");
        }
    }
    return arr;
}

JsonNumber JsonParser::parseNumber(std::ifstream& file) {
    std::string numberStr;

    // Handle optional negative sign
    if (_ch == '-') {
        numberStr += _ch;
        nextChar(file);
    }

    // Parse integer part
    while (std::isdigit(_ch)) {
        numberStr += _ch;
        nextChar(file);
    }

    // Parse fractional part
    if (_ch == '.') {
        numberStr += _ch;
        nextChar(file);
        while (std::isdigit(_ch)) {
            numberStr += _ch;
            nextChar(file);
        }
    }

    // Parse exponent part
    if (_ch == 'e' || _ch == 'E') {
        numberStr += _ch;
        nextChar(file);
        if (_ch == '+' || _ch == '-') {
            numberStr += _ch;
            nextChar(file);
        }
        while (std::isdigit(_ch)) {
            numberStr += _ch;
            nextChar(file);
        }
    }

    // Convert the parsed string to a number
    try {
        returnChar(file);

        double doubleRep = std::stod(numberStr);

        try {
            int intRep = std::stoi(numberStr);
            return intRep;
        } catch(...) {
            return doubleRep;
        }
        
    } catch (const std::exception& e) {
        throwRuntimeError(e.what());
    }

    throwRuntimeError("Unexpected error in number parsing");
    return 42.42; /* never reached */
}

JsonBoolean JsonParser::parseBoolean(std::ifstream& file) {
    std::string boolStr;
    for (int i = 0; std::isalpha(_ch) && i < 5; ++i) {
        boolStr += _ch;
        nextChar(file);
    }

    if (boolStr == "true") {
        return true;
    } else if (boolStr != "false") {
        throwRuntimeError("Invalid boolean value");
    } 
    
    return false; 
}

JsonNull JsonParser::parseNull(std::ifstream& file) {
    std::string nullStr;
    for (int i = 0; i < 4; ++i) {
        if (_ch != "null"[i]) {
            throwRuntimeError("Invalid null value");
        }
        nullStr += _ch;
        nextChar(file);
    }

    return JsonNull();
}


JsonString JsonParser::parseString(std::ifstream& file) {
    
    if (_ch != '"') {
        throwRuntimeError("String must start with \" sign");
    }

    JsonString str;

    int state = 0; // 2 - escape, 3 - unicode, -1 - done

    int unicode_digits_cnt = 0;

    std::string unicode_hex_string = "";
    
    while (state != -1) {

        nextChar(file);

        switch (state) {
            case 0:
                switch (_ch) {
                    case '\\':
                        state = 2; // escape
                        break;
                    case '"':
                        state = -1; // done
                        break;
                    default:
                        if (_ch < 0x0020 || _ch > 0x10FFF) {
                            // Error: invalid char.
                        }
                        str += _ch;
                        // All good :)
                        break;
                }
                break;
            case 2: // escape mode
                switch (_ch) {
                    case '"':
                        str += '"';
                        state = 0; 
                        break;
                    case '\\':
                        str += '\\';
                        state = 0; 
                        break;
                    case '/':
                        str += '/';
                        state = 0; 
                        break;
                    case 'b':
                        str += '\b';
                        state = 0; 
                        break;
                    case 'f':
                        str += '\f';
                        state = 0; 
                        break;
                    case 'n':
                        str += '\n';
                        state = 0; 
                        break;
                    case 'r':
                        str += '\r';
                        state = 0; 
                        break;
                    case 't':
                        str += '\t';
                        state = 0; 
                        break;
                    case 'u':
                        state = 3;
                        break;
                }
                break;
            case 3: // unicode mode
                if (!std::isxdigit(_ch)) {
                    throwRuntimeError(std::string("Invalid unicode hex digit \'") + _ch + "\'");
                }
                unicode_hex_string += _ch;

                ++unicode_digits_cnt;


                if (unicode_digits_cnt == 4) {
                    unicode_digits_cnt = 0;

                    char unicode_letter = std::stoi(unicode_hex_string, nullptr, 16);

                    str += unicode_letter;

                    state = 0; 
                }
                assert(unicode_digits_cnt < 4);
                
                break;
            default:
                assert(false); // should never happen
                break;
        }
    }

    return str;
}
