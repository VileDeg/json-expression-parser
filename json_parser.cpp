#include "json_parser.h"
#include <stdexcept>
#include <cmath>

JsonValue JsonParser::Parse(std::ifstream& file) {
    return parseValue(file);
}

JsonValue JsonParser::parseValue(std::ifstream& file) {
    nextCharSkipWS(file);

    switch (_ch) {
        case '{':
            return JsonValue(parseObject(file));
        case '[':
            return JsonValue(parseArray(file));
        case '"':
            return JsonValue(parseString(file));
        case 't':
        case 'f':
            return JsonValue(parseBoolean(file));
            break;
        case 'n': 
            return JsonValue(parseNull(file));
            break;
    }

    return JsonValue(parseNumber(file));
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
        obj[key] = std::make_shared<JsonValue>(value);
        nextCharSkipWS(file);
        if (_ch == ',') {
            nextCharSkipWS(file);
        } else if (_ch != '}') {
            throwRuntimeError("Missing comma between members");
        }
    }
    // Invariant: _ch == '}'
    return obj;
}

JsonArray JsonParser::parseArray(std::ifstream& file) {
    JsonArray arr;
    
    while (_ch != ']') {
        arr.push_back(std::make_shared<JsonValue>(parseValue(file)));
        nextCharSkipWS(file);
        if (_ch == ',') {
            nextCharSkipWS(file);   
        } else if (_ch != ']') {
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
        return std::stod(numberStr);
    } catch (const std::invalid_argument& e) {
        throwRuntimeError("Invalid number format");
    } catch (const std::out_of_range& e) {
        throwRuntimeError("Number out of range");
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

    return nullptr;
}


JsonString JsonParser::parseString(std::ifstream& file) {
    
    if (_ch != '"') {
        // Error: string must start with double-quotes
    }

    JsonString str = "";

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
                    // Error: invalid unicode format
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
                // What?
                break;
        }
    }

    return str;
}
