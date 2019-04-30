//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_DAG_H__
#define __PARSER_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
struct parser_dag : public dag, public visitable<parser_dag<Token>>
{
    using token_type = Token;
    using symbol_type = grammar::symbol_type;

    using visitor_type = visitor<parser_dag<token_type>>;

    symbol_type symbol;
    token_type token; // might be empty for non-terminals

    virtual ~parser_dag()
    {
    }

    void accept(visitor_type& visitor) override
    {
        visitor.visit(static_cast<parser_dag<token_type>&>(*this));
    }
};

END_NAMESPACE

#endif // __PARSER_DAG_H__
