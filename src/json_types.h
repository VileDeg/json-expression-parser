#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <assert.h>

// #define JSON_VALUE_PRINT_NL

class JsonValue {
public:
    
    virtual void print(std::ostream& os) const = 0;

    virtual ~JsonValue() = default;

protected:
    static int s_LogDepth;

#ifdef JSON_VALUE_PRINT_NL    
    static inline std::string NLSep() {
        std::string nl_sep = "\n";
        for (int i = 0; i < s_LogDepth; ++i) {
            nl_sep += "    ";
        }
        return nl_sep;
    }
#else
    static inline std::string NLSep() {
        std::string nl_sep = " ";
        return nl_sep;
    }
#endif

    friend inline std::ostream& operator<<(std::ostream& os, const JsonValue& value) {
        value.print(os);
        return os;
    }
};

class JsonString;


class JsonObject : public JsonValue {
public:
    void add(const std::string& key, std::shared_ptr<JsonValue> value) {
        _map[key] = value;
    }

    std::shared_ptr<JsonValue> get(const std::string& key) const {
        auto it = _map.find(key);
        if (it != _map.end()) {
            return it->second;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        _map.erase(key);
    }

    bool contains(const std::string& key) const {
        return _map.find(key) != _map.end();
    }

    void print(std::ostream& os) const override {
        ++s_LogDepth;
        os << "{" << NLSep();
        
        for (auto it = _map.begin(); it != _map.end(); ++it) {
            if (it != _map.begin()) {
                os << "," << NLSep();
            }
            os << "\"" << it->first << "\": ";

            std::shared_ptr<JsonString> strPtr = std::dynamic_pointer_cast<JsonString>(it->second);
            if (strPtr) { // if is string enclose in double quotes
                os << "\"" << *(it->second) << "\"";
            } else {
                os << *(it->second);
            }
        }
        --s_LogDepth;
        os << NLSep() << "}";
    }

private:
    std::unordered_map<std::string, std::shared_ptr<JsonValue>> _map;
};


class JsonArray : public JsonValue {
public:
    void add(std::shared_ptr<JsonValue> value) {
        _array.push_back(value);
    }

    std::shared_ptr<JsonValue> get(size_t index) const {
        if (index < _array.size()) {
            return _array[index];
        }
        return nullptr;
    }

    void remove(size_t index) {
        if (index < _array.size()) {
            _array.erase(_array.begin() + index);
        }
    }

    size_t size() const {
        return _array.size();
    }

    void print(std::ostream& os) const override {
        ++s_LogDepth;
        os << "[" << NLSep();
        
        for (size_t i = 0; i < _array.size(); ++i) {
            if (i > 0) {
                os << "," << NLSep();
            }

            std::shared_ptr<JsonString> strPtr = std::dynamic_pointer_cast<JsonString>(_array[i]);
            if (strPtr) { // if is string enclose in double quotes
                os << "\"" << *(_array[i]) << "\"";
            } else {
                os << *(_array[i]);
            }
            
        }
        --s_LogDepth;
        os << NLSep() << "]";
    }

private:
    std::vector<std::shared_ptr<JsonValue>> _array;
};


class JsonString : public JsonValue {
public:
    JsonString() = default;

    JsonString(const std::string& value) : _value(value) {}

    
    JsonString(const JsonString& other) : _value(other._value) {}

   
    JsonString& operator=(const JsonString& other) {
        if (this != &other) {
            _value = other._value;
        }
        return *this;
    }

    // += operator
    JsonString& operator+=(const std::string& other) {
        _value += other;
        return *this;
    }

    JsonString& operator+=(char ch) {
        _value += ch;
        return *this;
    }


    void print(std::ostream& os) const override {
        os << _value;
    }

    bool empty() {
        return _value.empty();
    }

    operator std::string() const {
        return _value;
    }

private:
    std::string _value;
};


class JsonNumber : public JsonValue {
public:
    JsonNumber(int value) 
        : _value(value), _isInteger(true) {}

    JsonNumber(float value) 
        : _value(value), _isInteger(false) {}

    JsonNumber(double value) 
        : _value(value), _isInteger(false) {}

    void print(std::ostream& os) const override {
        if (_isInteger) {
            os << int(_value);
        } else {
            os << _value;
        }
    }

    bool isInteger() {
        return _isInteger;
    }

    operator int() const {
        return _value;
    }

private:
    bool _isInteger;

    double _value;
};


class JsonBoolean : public JsonValue {
public:
    JsonBoolean(bool value) : _value(value) {}

    void print(std::ostream& os) const override {
        os << (_value ? "true" : "false");
    }

private:
    bool _value;
};


class JsonNull : public JsonValue {
public:
    void print(std::ostream& os) const override {
        os << "null";
    }
};