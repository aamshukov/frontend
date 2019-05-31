//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SCOPE_H__
#define __SCOPE_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class scope : private noncopyable
{
    public:
        using scope_type = std::shared_ptr<scope>;

    public:
        enum class kind
        {
            scope_namespace = 1,
            scope_structure = 2,
            scope_function  = 3,
            scope_parameter = 4
        };

    private:
        scope::kind     my_kind;
        scope_type      my_papa; // parent scope

    public:
                       // scope();
                       //~scope();
};

END_NAMESPACE

#endif // __SCOPE_H__
