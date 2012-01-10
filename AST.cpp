#include <memory>
#include <utility>
#include "AST.hpp"

AST::~AST() { }

void BlockAST::add(std::unique_ptr<AST> stmt) {
    stmts_.push_back(std::move(stmt));
}

LoopAST::LoopAST(std::unique_ptr<AST> content)
    : content_(std::move(content))
{ }
