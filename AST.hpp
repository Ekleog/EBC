#ifndef lbf_AST_hpp
#define lbf_AST_hpp 1

#include <memory>
#include <vector>

class AST {
    public:
        virtual ~AST();
};

class Block : public AST {
        std::vector<std::unique_ptr<AST>> stmts_;
    public:
        void add(std::unique_ptr<AST> stmt);
};

class Incr : public AST { };
class Decr : public AST { };

class Next : public AST { };
class Prev : public AST { };

class Loop : public AST {
        std::unique_ptr<AST> content_;
    public:
        Loop(std::unique_ptr<AST> content);
};

class  Input : public AST { };
class Output : public AST { };

#endif // lbf_AST_hpp
