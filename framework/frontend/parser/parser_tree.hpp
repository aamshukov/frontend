//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_TREE_H__
#define __PARSER_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
struct parser_tree : public tree
{
    using token_type = T;
    using symbol_type = grammar::symbol_type;

    symbol_type symbol;
    token_type token; // might be empty for non-terminals

    virtual ~parser_tree()
    {
    }
};

END_NAMESPACE

#endif // __PARSER_TREE_H__
