#ifndef bbc_LLVM_Visitor_hpp
#define bbc_LLVM_Visitor_hpp 1

#include <memory>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include "AST.hpp"
#include "Visitor.hpp"

class LLVM_Visitor : public Visitor {
    public:
        llvm::Value * array_;
        llvm::Value * pos_;
        llvm::LLVMContext * context_;
        llvm::IRBuilder<> builder_;
        llvm::Module module_;

        llvm::Value * posptr();
    public:
        LLVM_Visitor(llvm::LLVMContext & context = llvm::getGlobalContext());
        void finalize();

        void visit( BlockAST &);
        void visit(  IncrAST &);
        void visit(  DecrAST &);
        void visit(  NextAST &);
        void visit(  PrevAST &);
        void visit(  LoopAST &);
        void visit( InputAST &);
        void visit(OutputAST &);
};

#endif // bbc_LLVM_Visitor_hpp