#ifndef ebc_LLVM_Visitor_hpp
#define ebc_LLVM_Visitor_hpp 1

#include <memory>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include "AST.hpp"
#include "Visitor.hpp"


class LLVM_Visitor : public Visitor {
    private:
        llvm::Value * array_;
        llvm::Value * pos_;
        llvm::LLVMContext * context_;
        llvm::Function * main_;
        llvm::Module * module_;
        llvm::ExecutionEngine * engine_;
        llvm::IRBuilder<> builder_;

        llvm::Value * posptr();
    public:
        LLVM_Visitor(llvm::LLVMContext & context = llvm::getGlobalContext());
        void finalize();
        llvm::Module * module();

        void visit( BlockAST &);
        void visit(  IncrAST &);
        void visit(  DecrAST &);
        void visit(  NextAST &);
        void visit(  PrevAST &);
        void visit(  LoopAST &);
        void visit( InputAST &);
        void visit(OutputAST &);
};

#endif // ebc_LLVM_Visitor_hpp
