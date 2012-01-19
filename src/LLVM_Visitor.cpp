#include <cstdio>
#include <llvm/LLVMContext.h>
#include "AST.hpp"
#include "LLVM_Visitor.hpp"

using namespace llvm;

extern "C" unsigned char bbc_getc() { return getchar(); }
extern "C" void bbc_putc(unsigned char c) { putchar(c); }

LLVM_Visitor::LLVM_Visitor(LLVMContext & context)
    : value_(nullptr),
      pos_(nullptr),
      context_(&context),
      builder_(*context_),
      module_("BBC", *context_) {
    value_ = ConstantInt::get(*context_, APInt(8, 0));
    pos_ = ConstantInt::get(*context_, APInt(16, 0));
    Function::Create(
        FunctionType::get(
            Type::getInt8Ty(*context_),
            {},
            false
        ),
        Function::ExternalLinkage,
        "bbc_getc",
        &module_
    );
    Function::Create(
        FunctionType::get(
             Type::getVoidTy(*context_),
            {Type::getInt8Ty(*context_)},
            false
        ),
        Function::ExternalLinkage,
        "bbc_putc",
        &module_
    );
    builder_.SetInsertPoint(
        BasicBlock::Create(
            *context_,
            "entry",
            Function::Create(
                FunctionType::get(
                    Type::getVoidTy(*context_),
                    {},
                    false
                ),
                Function::InternalLinkage,
                "bbc_main",
                &module_
            )
        )
    );
}

void LLVM_Visitor::finalize() {
    builder_.CreateRetVoid();
}

void LLVM_Visitor::visit(BlockAST & b) {
    for (AST * ast : b.stmts()) {
        ast->accept(*this);
    }
}
void LLVM_Visitor::visit(IncrAST &) {
    value_ = builder_.CreateAdd(
        value_,
        ConstantInt::get(*context_, APInt(8, 1)),
        "incr"
    );
}
void LLVM_Visitor::visit(DecrAST &) {
    value_ = builder_.CreateSub(
        value_,
        ConstantInt::get(*context_, APInt(8, 1)),
        "decr"
    );
}
void LLVM_Visitor::visit(NextAST &) {
    pos_ = builder_.CreateAdd(
        pos_,
        ConstantInt::get(*context_, APInt(16, 1)),
        "next"
    );
}
void LLVM_Visitor::visit(PrevAST &) {
    pos_ = builder_.CreateSub(
        pos_,
        ConstantInt::get(*context_, APInt(16, 1)),
        "prev"
    );
}
void LLVM_Visitor::visit(LoopAST & l) {
    l.content()->accept(*this);
}
void LLVM_Visitor::visit(InputAST &) {
    value_ = builder_.CreateCall(module_.getFunction("bbc_getc"), "in");
}
void LLVM_Visitor::visit(OutputAST &) {
    builder_.CreateCall(module_.getFunction("bbc_putc"), value_);
}
