//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_DAG_H__
#define __PARSER_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token, typename TreeTraits>
struct parser_dag : public dag, public visitable<parser_dag<Token, TreeTraits>>
{
    using token_type = Token;
    using tree_traits_type = TreeTraits;

    using symbol_type = grammar::symbol_type;

    using record_type = typename symbol_table_record<token_type>::record_type;
    using records_type = typename symbol_table_record<token_type>::records_type;

    using visitor_type = visitor<parser_dag<token_type, tree_traits_type>>;

    symbol_type symbol;
    record_type record;

    virtual ~parser_dag()
    {
    }

    void accept(visitor_type& visitor) override
    {
        visitor.visit(static_cast<parser_dag<token_type, tree_traits_type>&>(*this));
    }
};

END_NAMESPACE

#endif // __PARSER_DAG_H__
