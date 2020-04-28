//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TREE_H__
#define __ARCTURUS_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token, typename TreeTraits>
struct arcturus_tree : public parser_tree<Token, TreeTraits>
{
    using token_type = Token;
    using tree_traits_type = TreeTraits;

    using symbol_type = grammar::symbol_type;

    using record_type = typename symbol_ir<token_type>::record_type;
    using records_type = typename symbol_ir<token_type>::records_type;

    using visitor_type = visitor<parser_tree<token_type, tree_traits_type>>;

    virtual ~arcturus_tree()
    {
    }

    void accept(visitor_type& visitor) override
    {
        visitor.visit(static_cast<arcturus_tree<token_type, tree_traits_type>&>(*this));
    }
};

END_NAMESPACE

#endif // __ARCTURUS_TREE_H__
