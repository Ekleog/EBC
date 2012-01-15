#ifndef bbc_parse_hpp
#define bbc_parse_hpp 1

#include <memory>
#include "AST.hpp"
#include "Tokenizer.hpp"

std::unique_ptr<AST> parse(Tokenizer & tokenizer);

#endif // bbc_parse_hpp
