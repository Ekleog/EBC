#ifndef bbc_Visitor_hpp
#define bbc_Visitor_hpp 1

#include "AST.hpp"

class Visitor {
    public:
        virtual ~Visitor();
        virtual void visit( BlockAST &) = 0;
        virtual void visit(  IncrAST &) = 0;
        virtual void visit(  DecrAST &) = 0;
        virtual void visit(  NextAST &) = 0;
        virtual void visit(  PrevAST &) = 0;
        virtual void visit(  LoopAST &) = 0;
        virtual void visit( InputAST &) = 0;
        virtual void visit(OutputAST &) = 0;
};

#endif // bbc_Visitor_hpp
