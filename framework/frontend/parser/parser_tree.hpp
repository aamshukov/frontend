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

    enum class flags : uint64_t
    {
        clear = 0x0000,
        deleted = 0x0001
    };

    using symbol_type = grammar::symbol_type;
    using flags_type = tmpl_flags<flags>;

    symbol_type symbol;
    token_type token; // might be empty for non-terminals

    flags_type flags;

    virtual ~parser_tree()
    {
    }
};

END_NAMESPACE

#endif // __PARSER_TREE_H__
