#include <memory>
#include <utility>
#include "AST.hpp"

AST::~AST() { }

void Block::add(std::unique_ptr<AST> stmt) {
    stmts_.push_back(std::move(stmt));
}

Loop::Loop(std::unique_ptr<AST> content) : content_(std::move(content)) { }
