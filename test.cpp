#include <iostream>
#include <memory>
#include "AST.hpp"
#include "helpers.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"
#include "Visitor.hpp"

struct TestVisitor : public Visitor {
    virtual void visit(BlockAST & b) {
        std::cout << "{";
        for (AST * ast : b.stmts()) {
            ast->accept(*this);
        }
        std::cout << "}";
    }
    virtual void visit(IncrAST &) {
        std::cout << "+";
    }
    virtual void visit(DecrAST &) {
        std::cout << "-";
    }
    virtual void visit(NextAST &) {
        std::cout << ">";
    }
    virtual void visit(PrevAST &) {
        std::cout << "<";
    }
    virtual void visit(LoopAST & l) {
        std::cout << "[";
        l.content()->accept(*this);
        std::cout << "]";
    }
    virtual void visit(InputAST &) {
        std::cout << ",";
    }
    virtual void visit(OutputAST &) {
        std::cout << ".";
    }
};

int main(int, char**) {
    Tokenizer tokenizer{std::cin};
    std::unique_ptr<AST> ast = parse(tokenizer);
    if (!(bool)ast) return 1;
    TestVisitor visitor;
    ast->accept(visitor);
}
