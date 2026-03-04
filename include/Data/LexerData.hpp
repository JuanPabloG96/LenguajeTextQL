#ifndef LEXERDATA_HPP
#define LEXERDATA_HPP

#include <string>
#include <unordered_map>

namespace lexer {
extern const std::unordered_map<int, std::string> LexicalErrorMessages;
extern const std::unordered_map<int, std::string> TokenMap;
} // namespace lexer

#endif
