//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
//
// Andrew Durward:
//      https://stackoverflow.com/questions/11796121/implementing-the-visitor-pattern-using-c-templates
//
#ifndef __VISITABLE_H__
#define __VISITABLE_H__

#pragma once

BEGIN_NAMESPACE(core)

template<typename... Types>
class visitable
{
    public:
        using visitor_type = visitor<Types...>;

    public:
        virtual void accept(visitor_type& /*visitor*/) = 0;
};

END_NAMESPACE

#endif // __VISITABLE_H__
