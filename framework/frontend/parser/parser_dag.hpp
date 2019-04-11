//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_DAG_H__
#define __PARSER_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
struct parser_dag : public dag
{
    using token_type = T;
    using symbol_type = grammar::symbol_type;

    symbol_type symbol;
    token_type token; // might be empty for non-terminals

    virtual ~parser_dag()
    {
    }
};

END_NAMESPACE

#endif // __PARSER_DAG_H__
