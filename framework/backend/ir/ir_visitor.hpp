//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_VISITOR_H__
#define __IR_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct ir_visitor : public visitor
{
    void visit(const tree::tree_type& /*acceptor*/) override
    {
    }
};

END_NAMESPACE

#endif // __IR_VISITOR_H__
