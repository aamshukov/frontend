//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_TREE_H__
#define __PARSER_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
struct parser_tree : public tree, public visitable<parser_tree<Token>>
{
    using token_type = Token;
    using symbol_type = grammar::symbol_type;

    using visitor_type = visitor<parser_tree<token_type>>;

    symbol_type symbol;
    token_type token; // might be empty for non-terminals

    virtual ~parser_tree()
    {
    }

    void accept(visitor_type& visitor) override
    {
        visitor.visit(static_cast<parser_tree<token_type>&>(*this));
    }
};

END_NAMESPACE

#endif // __PARSER_TREE_H__
