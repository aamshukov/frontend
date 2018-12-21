//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_H__
#define __TREE_H__

#pragma once

BEGIN_NAMESPACE(core)

struct tree
{
    using node_type = std::shared_ptr<tree>;
    using nodes_type = std::vector<node_type>;

    node_type papa;
    nodes_type kids;
};


END_NAMESPACE

#endif // __TREE_H__
