#ifndef lbf_Tokenizer_hpp
#define lbf_Tokenizer_hpp 1

#include <iosfwd>

enum class Token {
    Incr , Decr ,
    Next , Prev ,
    LoopB, LoopE,
    Input, Output
};

class Tokenizer {
    public:
        Tokenizer(std::istream & is);
        Token next();

    private:
        std::istream * is_;
};

#endif // lbf_Tokenizer_hpp
