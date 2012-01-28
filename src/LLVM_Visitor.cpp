#include <cstdio>
#include <llvm/Analysis/Passes.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/Scalar.h>
#include "AST.hpp"
#include "LLVM_Visitor.hpp"

using namespace llvm;

LLVM_Visitor::LLVM_Visitor(LLVMContext & context)
    : array_(nullptr),
      pos_(nullptr),
      context_(&context),
      main_(nullptr),
      module_(new Module("EBC", *context_)),
      engine_(EngineBuilder(module_).create()),
      builder_(*context_) {
    Function::Create(
        FunctionType::get(
            Type::getVoidTy(*context_),
            {Type::getInt32Ty(*context_)},
            false
        ),
        Function::ExternalLinkage,
        "putchar",
        module_
    );
    Function::Create(
        FunctionType::get(
            Type::getInt32Ty(*context_),
            {},
            false
        ),
        Function::ExternalLinkage,
        "getchar",
        module_
    );
    main_ = Function::Create(
        FunctionType::get(
            Type::getVoidTy(*context_),
            {},
            false
        ),
        Function::ExternalLinkage,
        "main",
        module_
    );
    builder_.SetInsertPoint(BasicBlock::Create(*context_, "entry", main_));
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
    module_->dump();
    verifyFunction(*main_);
    verifyModule(*module_);
    // Optimize it ?
#ifndef NO_OPT
      FunctionPassManager fpm(module_);
    /*Module*/PassManager mpm;

    PassManagerBuilder pmb;
    pmb.OptLevel = 3;
    pmb.SizeLevel = 0;

    pmb.populateFunctionPassManager(fpm);
    pmb.populateModulePassManager(mpm);

    fpm.run(*main_);
    mpm.run(*module_);
#endif
}

llvm::Module * LLVM_Visitor::module() {
    return module_;
}

void LLVM_Visitor::visit(BlockAST & b) {
    for (AST * ast : b.stmts()) {
        ast->accept(*this);
    }
}
void LLVM_Visitor::visit(IncrAST &) {
    Value * ptr = posptr();
    Value * val = builder_.CreateLoad(ptr, "val");
    Value * add = builder_.CreateAdd(val, ConstantInt::get(*context_, APInt(8, 1)), "add");
    builder_.CreateStore(add, ptr);
}
void LLVM_Visitor::visit(DecrAST &) {
    Value * ptr = posptr();
    Value * val = builder_.CreateLoad(ptr, "val");
    Value * add = builder_.CreateSub(val, ConstantInt::get(*context_, APInt(8, 1)), "sub");
    builder_.CreateStore(add, ptr);
}
void LLVM_Visitor::visit(NextAST &) {
    pos_ = builder_.CreateAdd(pos_, ConstantInt::get(*context_, APInt(32, 1)), "nxt");
}
void LLVM_Visitor::visit(PrevAST &) {
    pos_ = builder_.CreateSub(pos_, ConstantInt::get(*context_, APInt(32, 1)), "prv");
}
void LLVM_Visitor::visit(LoopAST & l) {
    // Retrieve data
    BasicBlock * beforeBB = builder_.GetInsertBlock();
    // Create BB, jump to it
    BasicBlock * loopBB = BasicBlock::Create(*context_, "loop", main_);
    builder_.CreateBr(loopBB);
    builder_.SetInsertPoint(loopBB);
    // Generate the PHI node for pos
    PHINode * phi = builder_.CreatePHI(Type::getInt32Ty(*context_), 2, "pos");
    phi->addIncoming(pos_, beforeBB);
    pos_ = phi;
    // Generate content
    l.content()->accept(*this);
    BasicBlock * loopendBB = builder_.GetInsertBlock();
    // Generate end condition
    Value * cond = builder_.CreateICmpNE(
        builder_.CreateLoad(posptr(), "val"),
        ConstantInt::get(*context_, APInt(8, 0)),
        "cond"
    );
    // Generate after-loop block
    BasicBlock * afterBB = BasicBlock::Create(*context_, "end", main_);
    // Generate end-loop dbl-jmp
    builder_.CreateCondBr(cond, loopBB, afterBB);
    // Finalize PHI node
    phi->addIncoming(pos_, loopendBB);
    // Go to end-of-loop block
    builder_.SetInsertPoint(afterBB);
}
void LLVM_Visitor::visit(InputAST &) {
    // Get function, generate new BBs
    BasicBlock *  eofBB = BasicBlock::Create(*context_, "eof" , main_);
    BasicBlock * readBB = BasicBlock::Create(*context_, "read", main_);
    BasicBlock *  endBB = BasicBlock::Create(*context_, "end" , main_);
    // Read on stdin
    Value * in = builder_.CreateCall(module_->getFunction("getchar"), "getc");
    // If there an EOF ?
    Value * eof = builder_.CreateICmpEQ(in, ConstantInt::get(*context_, APInt(32, EOF)), "eof");
    builder_.CreateCondBr(eof, eofBB, readBB);
    // Yes, EOF
    builder_.SetInsertPoint(eofBB);
    Value * val_eof = ConstantInt::get(*context_, APInt(8, 0));
    builder_.CreateBr(endBB);
    // No, not EOF
    builder_.SetInsertPoint(readBB);
    Value * val_read = builder_.CreateTrunc(in, Type::getInt8Ty(*context_), "cast");
    builder_.CreateBr(endBB);
    // Merge all that
    builder_.SetInsertPoint(endBB);
    PHINode * val = builder_.CreatePHI(Type::getInt8Ty(*context_), 2, "in");
    val->addIncoming(val_eof ,  eofBB);
    val->addIncoming(val_read, readBB);
    // And store !
    builder_.CreateStore(val, posptr());
}
void LLVM_Visitor::visit(OutputAST &) {
    Value * out = builder_.CreateLoad(posptr(), "out");
    Value * val = builder_.CreateZExt(out, Type::getInt32Ty(*context_), "putc");
    builder_.CreateCall(module_->getFunction("putchar"), val);
}

Value * LLVM_Visitor::posptr() {
    return builder_.CreateGEP(array_, pos_, "ptr");
}
