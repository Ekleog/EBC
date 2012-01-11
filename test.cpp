#include <iostream>
#include <memory>
#include "AST.hpp"
#include "helpers.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"

int main(int, char**) {
    Tokenizer tokenizer{std::cin};
    std::unique_ptr<AST> ast = parse(tokenizer);
}
