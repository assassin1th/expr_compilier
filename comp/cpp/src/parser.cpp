#include "comp/cpp/include/parser.h"
#include <iostream>


Parser::Parser(Lexer *lex) :
    lex(lex), env(new Env)
{
    move();
}

void Parser::move()
{
    look = lex->scan();
}

void Parser::match(int tag)
{
    if (look->tag() == tag)
    {
        move();
    } else {
        std::cerr << "error: expeted" << tag << std::endl;
    }
}

Expr *Parser::parse()
{
    func();
    match('=');
    return expr();
}

void Parser::func()
{
    match(Tag::ID);
    match('(');
    if (look->tag() != ')')
    {
        do {
            if (look->tag() == Tag::ID)
            {
                if (env->get(look))
                {
                    std::cerr << "used: " << look->val() << std::endl;
                } else {
                    env->set(look, new Id(look, used));
                    used += sizeof(double);
                }
                move();
            }
            if (look->tag() == ')')
            {
                break;
            }
            std::cout << look->tag() << std::endl;
            match(',');
        } while (true);
    }
    move();
}

Expr *Parser::expr()
{
    Expr *x = term();
    while (look->tag() == '+' || look->tag() == '-')
    {
        Token *tok = look;
        move();
        x = new Arith(tok, x, term());
    }
    return x;
}

Expr *Parser::term()
{
    Expr *x = unary();
    while (look->tag() == '*' || look->tag() == '/')
    {
        Token *tok = look;
        move();
        x = new Arith(tok, x, unary());
    }
    return x;
}

Expr *Parser::unary()
{
    if (look->tag() == '-')
    {
        Token *tok = look;
        move();
        return (new Unary(tok, unary()));
    } else {
        return factor();
    }
}

Expr *Parser::factor()
{
    Expr *x = nullptr;
    switch (look->tag())
    {
        case Tag::REAL:
            x = new Constant(look);
            move();
            return x;
        case '(':
            move();
            x = expr();
            match(')');
            return x;
        case Tag::ID:{
            Id *i = env->get(look);
            if (i == nullptr)
            {
                std::cerr << "undeclared variable: " << look->val() << std::endl;
            }
            move();
            return i;
        }
        default:
            std::cerr << "syntax error1" << std::endl;
            std::cerr << look->val() << std::endl;
            std::cerr << look->tag() << std::endl;
            return x;
    }
}
