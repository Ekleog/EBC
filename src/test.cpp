#include <iostream>
#include <memory>
#include "AST.hpp"
#include "helpers.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"
#include "Visitor.hpp"

struct CoutVisitor : public Visitor {
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

struct StringVisitor : public Visitor {
    std::string s;

    virtual void visit(BlockAST & b) {
        s += "{";
        for (AST * ast : b.stmts()) {
            ast->accept(*this);
        }
        s += "}";
    }
    virtual void visit(IncrAST &) {
        s += "+";
    }
    virtual void visit(DecrAST &) {
        s += "-";
    }
    virtual void visit(NextAST &) {
        s += ">";
    }
    virtual void visit(PrevAST &) {
        s += "<";
    }
    virtual void visit(LoopAST & l) {
        s += "[";
        l.content()->accept(*this);
        s += "]";
    }
    virtual void visit(InputAST &) {
        s += ",";
    }
    virtual void visit(OutputAST &) {
        s += ".";
    }
};

int main(int, char**) {
    Tokenizer tokenizer{std::cin};

    std::unique_ptr<AST> ast = parse(tokenizer);
    if (!(bool)ast) return 1;

    CoutVisitor cout_visitor;
    std::cout << "Cout Visitor   : ";
    ast->accept(cout_visitor);
    std::cout << std::endl;

    StringVisitor string_visitor;
    ast->accept(string_visitor);
    std::cout << "String Visitor : " << string_visitor.s << std::endl;
}
