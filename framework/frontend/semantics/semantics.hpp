//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class semantics : private noncopyable
{
    public:
        using token_type = Token;
};

END_NAMESPACE

#endif // __SEMANTICS_H__
