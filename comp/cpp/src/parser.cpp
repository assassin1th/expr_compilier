#include "comp/cpp/include/parser.h"
#include <iostream>

using namespace CompParser;
using CompLexer::Lexer;
using CompLexer::Token;
using Symbols::Env;
using Inter::Expr;
using Inter::Id;

Parser::Parser(Lexer *lex) :
    m_lex(lex), m_env(new Env)
{
    using CompLexer::Word;
    using CompLexer::Tag;
    m_lex->reserve(new Word(Tag::COS, "cos"), "cos");
    m_lex->reserve(new Word(Tag::SIN, "sin"), "sin");
    m_lex->reserve(new Word(Tag::TAN, "tan"), "tan");
    m_lex->reserve(new Word(Tag::CTAN, "ctan"), "ctan");
    m_lex->reserve(new Word(Tag::ACOS, "acos"), "acos");
    m_lex->reserve(new Word(Tag::ASIN, "asin"), "asin");
    m_lex->reserve(new Word(Tag::ATAN, "atan"), "atan");
    m_lex->reserve(new Word(Tag::ACTAN, "actan"), "actan");
    m_lex->reserve(new Word(Tag::LOG, "log"), "log");
    move();
}

Parser::~Parser()
{
    delete m_env;
    delete m_lex;
}

void
Parser::move()
{
    m_look = m_lex->scan();
}

void
Parser::match(int tag)
{
    if (m_look->tag() == tag)
    {
        move();
    } else {
        std::cerr << "error: expeted" << tag << std::endl;
    }
}

Expr *
Parser::parse()
{
    func();
    match('=');
    return expr();
}

void
Parser::func()
{
    using CompLexer::Tag;
    match(Tag::ID);
    match('(');
    if (m_look->tag() != ')')
    {
        do {
            if (m_look->tag() == Tag::ID)
            {
                if (m_env->get(m_look))
                {
                    std::cerr << "used: " << m_look->val() << std::endl;
                } else {
                    m_env->set(m_look, new Id(m_look, used));
                    used += sizeof(double);
                }
                move();
            }
            if (m_look->tag() == ')')
            {
                break;
            }
            std::cout << m_look->tag() << std::endl;
            match(',');
        } while (true);
    }
    move();
}

Expr *
Parser::expr()
{
    using Inter::Arith;
    Expr *x = term();
    while (m_look->tag() == '+' || m_look->tag() == '-')
    {
        Token *tok = m_look;
        move();
        x = new Arith(tok, x, term());
    }
    return x;
}

Expr *
Parser::term()
{
    using Inter::Arith;
    Expr *x = unary();
    while (m_look->tag() == '*' || m_look->tag() == '/')
    {
        Token *tok = m_look;
        move();
        x = new Arith(tok, x, unary());
    }
    return x;
}

Expr *
Parser::unary()
{
    using Inter::Unary;
    if (m_look->tag() == '-')
    {
        Token *tok = m_look;
        move();
        return (new Unary(tok, unary()));
    } else {
        return factor();
    }
}

Expr *
Parser::factor()
{
    using CompLexer::Tag;
    using Inter::Constant;
    Expr *x = nullptr;
    switch (m_look->tag())
    {
        case Tag::REAL:
            x = new Constant(m_look);
            move();
            return x;
        case '(':
            move();
            x = expr();
            match(')');
            return x;
        case Tag::ID:
            return call();
        default:
            std::cerr << "syntax error1" << std::endl;
            std::cerr << m_look->val() << std::endl;
            std::cerr << m_look->tag() << std::endl;
            return x;
    }
}

Expr *
Parser::call()
{
    using Inter::Call;
    Token *tmp = m_look;
    move();
    if (m_look->tag() == '(')
    {
        move();
        std::vector<Expr *> args;
        if (m_look->tag() != ')')
        {
            do {
                args.push_back(expr());
                if (m_look->tag() == ')')
                {
                    break;
                }
                match(',');
            } while (true);
        }
        return new Call(tmp, args);
    } else {
        Id *i = m_env->get(tmp);
        if (i == nullptr)
        {
            std::cerr << "undeclared variable: " << tmp->val() << std::endl;
        }
        return i;
    }
}

