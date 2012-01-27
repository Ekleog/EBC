#include <iostream>
#include <memory>
#include <llvm/Support/TargetSelect.h>
#include "AST.hpp"
#include "LLVM_Visitor.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"

llvm::Module * compile_ir(std::istream & is) {
    Tokenizer tokenizer(is);
    std::unique_ptr<AST> ast = parse(tokenizer);
    if (!ast) return nullptr;

    LLVM_Visitor visitor;
    ast->accept(visitor);
    visitor.finalize();

    return visitor.module();
}
