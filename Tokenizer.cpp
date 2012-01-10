#include <istream>
#include "Tokenizer.hpp"

Tokenizer::Tokenizer(std::istream & is) : is_(&is) { }

Token Tokenizer::next() {
    while (true) {
        switch (is_->get()) {
            case '+': return Token::Incr;
            case '-': return Token::Decr;
            case '>': return Token::Next;
            case '<': return Token::Prev;
            case '[': return Token::LoopB;
            case ']': return Token::LoopE;
            case ',': return Token::Input;
            case '.': return Token::Output;
            default : ;// retry
        }
    }
}
