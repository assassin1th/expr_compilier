#include "comp/cpp/include/parser.h"
#include <iostream>
#include "comp/cpp/include/compexcept.h"

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
        throw CompExept::UnexpectedSymbol(m_lex->src_str(),
                                          m_lex->start_curr_tok(),
                                          m_lex->end_curr_tok(),
                                          tag);
    }
}

const std::shared_ptr<const Inter::FuncDecl>
Parser::parse()
{
    return func();
}

const std::shared_ptr<const Inter::FuncDecl>
Parser::func()
{
    using CompLexer::Tag;
    const std::shared_ptr<const Token> id (m_look);
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
                    m_env->set(m_look,
                               std::shared_ptr<Id> (new Id(m_look, m_used)));
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
    return std::shared_ptr<const Inter::FuncDecl> (new Inter::FuncDecl(id, expr(), m_used / sizeof (double)));
}

const std::shared_ptr<const Expr>
Parser::expr()
{
    using Inter::Arith;
    std::shared_ptr<const Expr> x = term();
    while (m_look->tag() == '+' || m_look->tag() == '-')
    {
        const std::shared_ptr<const Token> tok = m_look;
        move();
        x = std::shared_ptr<Expr> (new Arith(tok, x, term()));
    }
    return x;
}

const std::shared_ptr<const Expr>
Parser::term()
{
    using Inter::Arith;
    std::shared_ptr<const Expr> x = unary();
    while (m_look->tag() == '*' || m_look->tag() == '/')
    {
        const std::shared_ptr<const Token> tok (m_look);
        move();
        x = std::shared_ptr<Expr> (new Arith(tok, x, unary()));
    }
    return x;
}

const std::shared_ptr<const Expr>
Parser::unary()
{
    using Inter::Unary;
    if (m_look->tag() == '-')
    {
        const std::shared_ptr<const Token> tok (m_look);
        move();
        return (std::shared_ptr<const Expr> (new Unary(tok, unary())));
    } else {
        return power();
    }
}

const std::shared_ptr<const Expr>
Parser::power()
{
    using Inter::Arith;
    std::shared_ptr<const Expr> x (factor());
    while (m_look->tag() == '^')
    {
        const std::shared_ptr<const Token> tok (m_look);
        move();
        x = std::shared_ptr<Expr> (new Arith(tok, x, power()));
    }
    return x;
}

const std::shared_ptr<const Expr>
Parser::factor()
{
    using CompLexer::Tag;
    using Inter::Constant;
    using Inter::Trig;
    using Inter::Arith;

    std::shared_ptr<const Expr> x (nullptr);
    std::shared_ptr<const Token> id (nullptr);
    switch (m_look->tag())
    {
        case Tag::REAL:
            x = std::shared_ptr<Expr> (new Constant(m_look));
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
            x = std::shared_ptr<Expr> (new Trig(id, expr()));
            match(')');
            break;
        case Tag::LOG:
            id = m_look;
            move();
            match('(');
            x = expr();
            match(',');
            x = std::shared_ptr<Expr> (new Arith(id, x, expr()));
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

const std::shared_ptr<const Expr>
Parser::call()
{
    using Inter::Call;
    std::shared_ptr<const Token> tmp = m_look;
    move();
    if (m_look->tag() == '(')
    {
        move();
        std::vector<std::shared_ptr<const Expr>> args;
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
        return std::shared_ptr<Expr> (new Call(tmp, args));
    } else {
        std::shared_ptr<const Expr> i = m_env->get(tmp);
        if (i == nullptr)
        {
            std::cerr << "undeclared variable: " << tmp->val() << std::endl;
        }
        return i;
    }
}

