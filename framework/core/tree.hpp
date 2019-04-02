//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_H__
#define __TREE_H__

#pragma once

BEGIN_NAMESPACE(core)

struct tree : private noncopyable
{
    using tree_type = std::shared_ptr<tree>;
    using trees_type = std::vector<tree_type>;

    tree_type papa; // parent
    trees_type kids;

    virtual ~tree()
    {
    }
};

END_NAMESPACE

#endif // __TREE_H__
