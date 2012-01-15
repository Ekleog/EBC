#ifndef bbc_Tokenizer_hpp
#define bbc_Tokenizer_hpp 1

#include <iosfwd>

enum class Token {
    Incr , Decr ,
    Next , Prev ,
    LoopB, LoopE,
    Input, Output,
    End
};

class Tokenizer {
    public:
        Tokenizer(std::istream & is);
        Token next();
        bool end() const;

    private:
        std::istream * is_;
};

#endif // bbc_Tokenizer_hpp
