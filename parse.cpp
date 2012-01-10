#include <memory>
#include "AST.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"

typedef std::unique_ptr<AST> ptr;

static std::unique_ptr<AST> parse_token(Token, Tokenizer &);

static std::unique_ptr<AST> parse_loop(Tokenizer & tokenizer) {
    std::unique_ptr<BlockAST> ast(new BlockAST);
    while (true) {
        Token token = tokenizer.next();
        if (token == Token::LoopE) return std::move(ast);
        ast->add(parse_token(token, tokenizer));
    }
}

static std::unique_ptr<AST> parse_token(Token token, Tokenizer & t) {
    switch (token) {
        case Token::Incr  : return ptr{new IncrAST};
        case Token::Decr  : return ptr{new DecrAST};
        case Token::Next  : return ptr{new NextAST};
        case Token::Prev  : return ptr{new PrevAST};
        case Token::LoopB : return parse_loop(t);
        case Token::LoopE : // TODO Syntax error -- ignore at the moment
        case Token::End   : // TODO Programming error -- ignore at the moment
        case Token::Input : return ptr{new InputAST};
        case Token::Output: return ptr{new OutputAST};
    }
    // TODO Programming error -- ignore at the moment
}

std::unique_ptr<AST> parse(Tokenizer & tokenizer) {
    std::unique_ptr<BlockAST> ast(new BlockAST);
    while (true) {
        Token token = tokenizer.next();
        if (token == Token::End) return std::move(ast);
        ast->add(parse_token(token, tokenizer));
    }
}
