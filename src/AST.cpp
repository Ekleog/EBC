#include <algorithm>
#include <memory>
#include <utility>
#include "AST.hpp"
#include "Visitor.hpp"

AST::~AST() { }

void BlockAST::add(std::unique_ptr<AST> stmt) {
    stmts_.push_back(std::move(stmt));
}
std::vector<AST *> BlockAST::stmts() {
    std::vector<AST *> ret;
    std::transform(stmts_.begin(), stmts_.end(), std::back_inserter(ret),
                [] (std::unique_ptr<AST> const & ast) {
                    return ast.get();
                });
    return ret;
}

LoopAST::LoopAST(std::unique_ptr<AST> content)
    : content_(std::move(content))
{ }
AST * LoopAST::content() {
    return content_.get();
}

#define ACCEPT(Class)                       \
    void Class::accept(Visitor & visitor) { \
        visitor.visit(*this);               \
    }                                       \

ACCEPT( BlockAST)
ACCEPT(  IncrAST)
ACCEPT(  DecrAST)
ACCEPT(  NextAST)
ACCEPT(  PrevAST)
ACCEPT(  LoopAST)
ACCEPT( InputAST)
ACCEPT(OutputAST)
