#include <iostream>
#include <memory>
#include "AST.hpp"
#include "LLVM_Visitor.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"

int main(int, char**) {
    Tokenizer tokenizer(std::cin);

    std::unique_ptr<AST> ast = parse(tokenizer);
    if (!ast) return 1;

    LLVM_Visitor visitor;
    ast->accept(visitor);
    visitor.finalize();

    visitor.module_.dump();
}
