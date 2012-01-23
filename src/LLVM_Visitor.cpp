#include <cstdio>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LLVMContext.h>
#include "AST.hpp"
#include "LLVM_Visitor.hpp"

using namespace llvm;

extern "C" unsigned char bbc_getc() {
    int c = getchar();
    return c == EOF ? 0 : static_cast<unsigned char>(c);
}
extern "C" void bbc_putc(unsigned char c) {
    putchar(c);
}

LLVM_Visitor::LLVM_Visitor(LLVMContext & context)
    : array_(nullptr),
      pos_(nullptr),
      context_(&context),
      builder_(*context_),
      module_("BBC", *context_) {
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
    array_ = builder_.CreateAlloca(
        Type::getInt8Ty(*context_),
        ConstantInt::get(*context_, APInt(32, 30000)),
        "mem"
    );
    builder_.CreateMemSet(array_, ConstantInt::get(*context_, APInt(8, 0)), 30000, 1);
    pos_ = ConstantInt::get(*context_, APInt(32, 0));
}

void LLVM_Visitor::finalize() {
    // Finalize the function
    builder_.CreateRetVoid();
    // Now, run the JIT
    Function * function = builder_.GetInsertBlock()->getParent();
    EngineBuilder EB(&module_);
    ExecutionEngine * EE = EB.create();
    void * ptr = EE->getPointerToFunction(function);
    void (*FP)() = (void (*)()) (std::intptr_t) ptr;
    FP();
}

void LLVM_Visitor::visit(BlockAST & b) {
    for (AST * ast : b.stmts()) {
        ast->accept(*this);
    }
}
void LLVM_Visitor::visit(IncrAST &) {
    Value * ptr = posptr();
    builder_.CreateStore(
        builder_.CreateAdd(
            builder_.CreateLoad(ptr, "val"),
            ConstantInt::get(*context_, APInt(8, 1)),
            "incr"
        ),
        ptr
    );
}
void LLVM_Visitor::visit(DecrAST &) {
    Value * ptr = posptr();
    builder_.CreateStore(
        builder_.CreateSub(
            builder_.CreateLoad(ptr, "val"),
            ConstantInt::get(*context_, APInt(8, 1)),
            "decr"
        ),
        ptr
    );
}
void LLVM_Visitor::visit(NextAST &) {
    pos_ = builder_.CreateAdd(pos_, ConstantInt::get(*context_, APInt(32, 1)), "next");
}
void LLVM_Visitor::visit(PrevAST &) {
    pos_ = builder_.CreateSub(pos_, ConstantInt::get(*context_, APInt(32, 1)), "prev");
}
void LLVM_Visitor::visit(LoopAST & l) {
    // Retrieve data
    BasicBlock * beforeBB = builder_.GetInsertBlock();
    Function * function = beforeBB->getParent();
    // Create BB, jump to it
    BasicBlock * loopBB = BasicBlock::Create(*context_, "loop", function);
    builder_.CreateBr(loopBB);
    builder_.SetInsertPoint(loopBB);
    // Generate the PHI node for pos
    PHINode * phi = builder_.CreatePHI(Type::getInt32Ty(*context_), 2, "pos");
    phi->addIncoming(pos_, beforeBB);
    pos_ = phi;
    // Generate content
    l.content()->accept(*this);
    // Generate end condition
    Value * cond = builder_.CreateICmpNE(
        builder_.CreateLoad(posptr(), "val"),
        ConstantInt::get(*context_, APInt(8, 0)),
        "cond"
    );
    // Generate after-loop block
    BasicBlock * afterBB = BasicBlock::Create(*context_, "end", function);
    // Generate end-loop dbl-jmp
    builder_.CreateCondBr(cond, loopBB, afterBB);
    // Finalize PHI node
    phi->addIncoming(pos_, loopBB);
    // Go to end-of-loop block
    builder_.SetInsertPoint(afterBB);
}
void LLVM_Visitor::visit(InputAST &) {
    builder_.CreateStore(
        builder_.CreateCall(module_.getFunction("bbc_getc"), "in"),
        posptr()
    );
}
void LLVM_Visitor::visit(OutputAST &) {
    builder_.CreateCall(
        module_.getFunction("bbc_putc"),
        builder_.CreateLoad(posptr(), "out")
    );
}

Value * LLVM_Visitor::posptr() {
    return builder_.CreateGEP(array_, pos_, "ptr");
}
