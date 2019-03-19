//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VISITOR_H__
#define __VISITOR_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename T>
class visitor : private noncopyable
{
    public:
        using concrete_acceptor_type = T;

    protected:
        virtual void visit(const concrete_acceptor_type& concrete_acceptor) = 0;
};

END_NAMESPACE

#endif // __VISITOR_H__
