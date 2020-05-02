//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
//
// Andrew Durward:
//      https://stackoverflow.com/questions/11796121/implementing-the-visitor-pattern-using-c-templates
//
#ifndef __VISITOR_H__
#define __VISITOR_H__

#pragma once

BEGIN_NAMESPACE(core)

// template declaration
template <typename... Types>
class visitor;

// specialization for single type    
template <typename T>
class visitor<T>
{
    public:
        virtual void visit(T& visitable) = 0;
};

// specialization for multiple types
template <typename T, typename... Types>
class visitor<T, Types...> : public visitor<Types...>
{
    public:
        using visitor<Types...>::visit; // promote the function(s) from the base class

        virtual void visit(T& visitable) = 0;
};

END_NAMESPACE

#endif // __VISITOR_H__


/*??

// template declaration
template <typename Return, typename Param, typename... Types>
class visitor;

// specialization for single type    
template <typename TVisitable, typename TReturn, typename TParam>
class visitor<TVisitable, TReturn, TParam>
{
    public:
        virtual TReturn visit(TVisitable& visitable, const TParam& param) = 0;
};

// specialization for multiple types
template <typename TVisitable, typename TReturn, typename TParam, typename... Types>
class visitor<TVisitable, TReturn, TParam, Types...> : public visitor<TReturn, TParam, Types...>
{
    public:
        using visitor<TReturn, TParam, Types...>::visit; // promote the function(s) from the base class

        virtual Return visit(TVisitable& visitable, const TParam& param) = 0;
};

*/
