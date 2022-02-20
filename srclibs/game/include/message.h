#pragma once
#include <string>

enum MSG_TYPE {
    MSG_NONE, MSG_ERROR, MSG_INFO, MSG_WARN
};

void showMessage(const std::string& msg, const std::string& title = "", MSG_TYPE ty=MSG_NONE);
bool showQuestion(const std::string& msg, const std::string& title = "", MSG_TYPE ty=MSG_NONE);