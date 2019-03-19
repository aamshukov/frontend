//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_VISITOR_H__
#define __IR_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
struct ir_visitor : public visitor<T>
{
    public:
        using concrete_acceptor_type = visitor<T>::concrete_acceptor_type;
};

END_NAMESPACE

#endif // __IR_VISITOR_H__
