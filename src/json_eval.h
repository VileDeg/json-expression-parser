#pragma once

#include "json_types.h"
#include <memory>


class JsonEval {

public:
    JsonEval(const std::shared_ptr<JsonValue>& root) 
        : _root(root) {}

    std::shared_ptr<JsonValue> EvaluateExpression(std::shared_ptr<JsonValue> root, std::string& expression);

private:

    void logError(const std::string& message) {
        std::cerr << "[JSON expression]: " << message << std::endl;
    }

    std::shared_ptr<JsonValue> _root;
    std::string _expression;

    bool _inArrayIndex = false;
    bool _afterArrayIndex = false;
};