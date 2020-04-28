//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __QUADRUPLE_H__
#define __QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename Token>
struct quadruple
{
    using token_type = Token;

    using record_type = typename symbol_ir<token_type>::record_type;
    using records_type = typename symbol_ir<token_type>::records_type;

    record_type operation;
    record_type argument1;
    record_type argument2;
    record_type result;

    quadruple()
    {
    }

    quadruple(const record_type& operation,
              const record_type& argument1,
              const record_type& argument2,
              const record_type& result)
        : operation(operation), argument1(argument1), argument2(argument2), result(result)
    {
    }

    quadruple(const record_type& operation,
              const record_type& argument1,
              const record_type& result)
        : quadruple(operation, argument1, nullptr, result)
    {
    }

    quadruple(const record_type& operation,
              const record_type& result)
        : quadruple(operation, nullptr, nullptr, result)
    {
    }

    quadruple(const record_type& operation)
        : quadruple(operation, nullptr, nullptr, nullptr)
    {
    }

    quadruple(const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    quadruple(quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    const quadruple& operator = (const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }

    quadruple& operator = (quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }
};

END_NAMESPACE

#endif // __QUADRUPLE_H__
