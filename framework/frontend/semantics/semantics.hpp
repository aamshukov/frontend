//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeKind>
class semantics : private noncopyable
{
    public:
        using token_type = typename parser<Token, TreeKind>::token_type;
        using tree_kind_type = typename parser<Token, TreeKind>::tree_kind_type;
};

END_NAMESPACE

#endif // __SEMANTICS_H__
