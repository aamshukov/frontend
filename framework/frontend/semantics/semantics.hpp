//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class semantics : private noncopyable
{
    public:
        using token_type = typename parser<Token, TreeTraits>::token_type;
        using tree_traits_type = typename parser<Token, TreeTraits>::tree_traits_type;
};

END_NAMESPACE

#endif // __SEMANTICS_H__
