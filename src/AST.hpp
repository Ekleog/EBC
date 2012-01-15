#ifndef bbc_AST_hpp
#define bbc_AST_hpp 1

#include <memory>
#include <vector>

class Visitor;

class AST {
    public:
        virtual ~AST();
        virtual void accept(Visitor & visitor) = 0;
};

class BlockAST : public AST {
        std::vector<std::unique_ptr<AST>> stmts_;
    public:
        void add(std::unique_ptr<AST> stmt);
        std::vector<AST *> stmts();
        void accept(Visitor & visitor);
};

class IncrAST : public AST {
    public:
        void accept(Visitor & visitor);
};
class DecrAST : public AST {
    public:
        void accept(Visitor & visitor);
};

class NextAST : public AST {
    public:
        void accept(Visitor & visitor);
};
class PrevAST : public AST {
    public:
        void accept(Visitor & visitor);
};

class LoopAST : public AST {
        std::unique_ptr<AST> content_;
    public:
        LoopAST(std::unique_ptr<AST> content);
        AST * content();
        void accept(Visitor & visitor);
};

class  InputAST : public AST {
    public:
        void accept(Visitor & visitor);
};
class OutputAST : public AST {
    public:
        void accept(Visitor & visitor);
};

#endif // bbc_AST_hpp
