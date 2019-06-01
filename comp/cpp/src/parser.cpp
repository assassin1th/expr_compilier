#include "comp/cpp/include/parser.h"
#include <iostream>

using namespace CompParser;
using CompLexer::Lexer;
using CompLexer::Token;
using Symbols::Env;
using Inter::Expr;
using Inter::Id;

Parser::Parser(Lexer *lex) :
    m_lex(lex), m_env(new Env), m_used(0)
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
        std::cerr << "recieved - " << m_look->tag() << std::endl;
        std::cerr << m_look->val() << std::endl;
    }
}

Inter::Stmt *
Parser::parse()
{
    return func();
}

Inter::Stmt *
Parser::func()
{
    using CompLexer::Tag;
    Token *id = m_look;
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
                    m_env->set(m_look, new Id(m_look, m_used));
                    m_used += sizeof(double);
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
    match('=');
    return new Inter::FuncDecl(id, expr());
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
        return power();
    }
}

Expr *
Parser::power()
{
    using Inter::Arith;
    Expr *x = factor();
    while (m_look->tag() == '^')
    {
        Token *tok = m_look;
        move();
        x = new Arith(tok, x, power());
    }
    return x;
}

Expr *
Parser::factor()
{
    using CompLexer::Tag;
    using Inter::Constant;
    using Inter::Trig;
    using Inter::Arith;

    Expr *x = nullptr;
    Token *id = nullptr;
    switch (m_look->tag())
    {
        case Tag::REAL:
            x = new Constant(m_look);
            move();
            break;
        case '(':
            move();
            x = expr();
            match(')');
            break;
        case Tag::COS: case Tag::SIN: case Tag::TAN:
        case Tag::CTAN: case Tag::ASIN: case Tag::ACOS:
        case Tag::ATAN: case Tag::ACTAN:
            id = m_look;
            move();
            match('(');
            x = new Trig(id, expr());
            match(')');
            break;
        case Tag::LOG:
            id = m_look;
            match('(');
            x = expr();
            match(',');
            x = new Arith(id, x, expr());
            match(')');
            break;
        case Tag::ID:
            x = call();
            break;
        default:
            std::cerr << "syntax error1" << std::endl;
            std::cerr << m_look->val() << std::endl;
            std::cerr << m_look->tag() << std::endl;
            break;
    }
    return x;
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

