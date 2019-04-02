//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_DAG_H__
#define __PARSER_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

struct parser_dag : public dag
{
    enum class flags : uint64_t
    {
        clear = 0x0000
    };

    using symbol_type = grammar::symbol_type;
    using flags_type = tmpl_flags<flags>;

    symbol_type symbol;
    flags_type flags;

    virtual ~parser_dag()
    {
    }
};

END_NAMESPACE

#endif // __PARSER_DAG_H__
