#ifndef ebc_parse_hpp
#define ebc_parse_hpp 1

#include <memory>
#include "AST.hpp"
#include "Tokenizer.hpp"

std::unique_ptr<AST> parse(Tokenizer & tokenizer);

#endif // ebc_parse_hpp
