//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_H__
#define __IR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename T>
class ir : private noncopyable
{
    public:
        using token_type = T;

        using tree_type = typename parser<token_type>::tree_type;
        using trees_type = typename parser<token_type>::trees_type;

    public:
        static void cst_to_ast(tree_type& cst);
};

END_NAMESPACE

#endif // __IR_H__
