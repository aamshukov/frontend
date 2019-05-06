//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_TAC_VISITOR_H__
#define __TREE_TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


template <typename Token, typename TreeKind>
struct tree_tac_visitor : public visitor<parser_tree<Token, TreeKind>>
{
    using tree_type = parser_tree<Token, TreeKind>;

    void visit(tree_type& tree) override
    {
        tree;//??
    }
};

END_NAMESPACE

#endif // __TREE_TAC_VISITOR_H__
