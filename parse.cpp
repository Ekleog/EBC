#include <memory>
#include <string>
#include "AST.hpp"
#include "helpers.hpp"
#include "parse.hpp"
#include "Tokenizer.hpp"

typedef std::unique_ptr<AST> ptr;

static ptr fail(std::string const & msg) {
    err(msg);
    return ptr{nullptr};
}

static std::unique_ptr<AST> parse_token(Token, Tokenizer &);

static std::unique_ptr<AST> parse_loop(Tokenizer & tokenizer) {
    std::unique_ptr<BlockAST> block(new BlockAST);
    Token token = tokenizer.next();
    while (token != Token::LoopE) {
        if (token == Token::End) return fail("Syntax error : non closed loop found");
        ptr child = parse_token(token, tokenizer);
        if (!child) return child;
        block->add(std::move(child));
        token = tokenizer.next();
    }
    return ptr{new LoopAST(std::move(block))};
}

static std::unique_ptr<AST> parse_token(Token token, Tokenizer & t) {
    switch (token) {
        case Token::Incr  : return ptr{new IncrAST};
        case Token::Decr  : return ptr{new DecrAST};
        case Token::Next  : return ptr{new NextAST};
        case Token::Prev  : return ptr{new PrevAST};
        case Token::LoopB : return parse_loop(t);
        case Token::LoopE : return fail("Syntax error : End of loop found while not in loop");
        case Token::End   : return fail("Internal error : Parsing an end-of-file token");
        case Token::Input : return ptr{new InputAST};
        case Token::Output: return ptr{new OutputAST};
    }
    return fail("Internal error : Encountered an unknown token");
}

std::unique_ptr<AST> parse(Tokenizer & tokenizer) {
    std::unique_ptr<BlockAST> ast(new BlockAST);
    while (true) {
        Token token = tokenizer.next();
        if (token == Token::End) return std::move(ast);
        ptr child = parse_token(token, tokenizer);
        if (!child) return child;
        ast->add(std::move(child));
    }
}
