#ifndef lbf_AST_hpp
#define lbf_AST_hpp 1

#include <memory>
#include <vector>

class AST {
    public:
        virtual ~AST();
};

class BlockAST : public AST {
        std::vector<std::unique_ptr<AST>> stmts_;
    public:
        void add(std::unique_ptr<AST> stmt);
};

class IncrAST : public AST { };
class DecrAST : public AST { };

class NextAST : public AST { };
class PrevAST : public AST { };

class LoopAST : public AST {
        std::unique_ptr<AST> content_;
    public:
        LoopAST(std::unique_ptr<AST> content);
};

class  InputAST : public AST { };
class OutputAST : public AST { };

#endif // lbf_AST_hpp
