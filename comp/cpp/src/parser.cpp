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
        case Tag::ID:
            return call();
        default:
            std::cerr << "syntax error1" << std::endl;
            std::cerr << look->val() << std::endl;
            std::cerr << look->tag() << std::endl;
            return x;
    }
}

Expr *Parser::call()
{
    Token *tmp = look;
    move();
    if (look->tag() == '(')
    {
        move();
        std::vector<Expr *> args;
        if (look->tag() != ')')
        {
            do {
                args.push_back(expr());
                if (look->tag() == ')')
                {
                    break;
                }
                match(',');
            } while (true);
        }
        return new Call(tmp, args);
    } else {
        Id *i = env->get(tmp);
        if (i == nullptr)
        {
            std::cerr << "undeclared variable: " << tmp->val() << std::endl;
        }
        return i;
    }
}

