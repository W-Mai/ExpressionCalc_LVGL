#pragma once

#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <iomanip>




enum class ErrorType {
    Well, FunctionNotFound, EvalError, SyntaxError, BracketNotMatched
};

struct Error {
    ErrorType type = ErrorType::Well;
    std::string msg = "";
};

typedef std::map<char, int> TokenLevel;
typedef std::vector<std::string> Notation;

Notation reversePolishNotation(const std::string& expression, const TokenLevel& tokenLevel, Error& e);

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression);
std::string readNumber(std::string::const_iterator& it, const std::string& expression);
inline bool checkFunc(std::string::const_iterator& it, const std::string& expression);
std::string readFunc(std::string::const_iterator& it, const std::string& expression);

double evalNotation(const Notation& notation, Error& e);

extern const TokenLevel tokenLevel;
extern std::map<ErrorType, std::string> ErrorType2Name;

