//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VISITOR_H__
#define __VISITOR_H__

#pragma once

BEGIN_NAMESPACE(core)

class visitor : private noncopyable
{
    protected:
        virtual void visit(const tree::tree_type& /*acceptor*/)
        {
        }
};

END_NAMESPACE

#endif // __VISITOR_H__
