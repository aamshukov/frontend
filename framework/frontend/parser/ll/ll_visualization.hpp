//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_VISUALIZATION_H__
#define __LL_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class ll_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using strong_ll_table_type = ll_algorithm::strong_ll_table_type;

        using ll_tal_table_row_type = ll_algorithm::ll_tal_table_row_type;
        using ll_tal_table_key_type = ll_algorithm::ll_tal_table_key_type;
        using ll_tal_table_type = ll_algorithm::ll_tal_table_type;
        using ll_tal_tables_type = ll_algorithm::ll_tal_tables_type;

        using ll_table_entry_type = ll_algorithm::ll_table_entry_type;
        using ll_table_entry_symbol_type = ll_algorithm::ll_table_entry_symbol_type;
        using ll_table_entry_type = ll_algorithm::ll_table_entry_type;
        using ll_table_row_type = ll_algorithm::ll_table_row_type;
        using ll_table_type = ll_algorithm::ll_table_type;

    public:
        static void         decorate_strong_ll_table(const strong_ll_table_type& table,
                                                     uint8_t k,
                                                     const sets_type& table_la_sets,
                                                     const std::vector<symbol_type>& nonterminals);

        static void         decorate_ll_tal_tables(const ll_tal_tables_type& tables, uint8_t k);
        static string_type  decorate_ll_tal_table_key(const ll_tal_table_key_type& key);
        static void         decorate_ll_tal_table(const ll_tal_table_type& table);
        static string_type  decorate_ll_tal_table_row(const ll_tal_table_row_type& row);

        static string_type  decorate_ll_table_entry(const ll_table_entry_type& entry);
        static void         decorate_ll_table(const ll_table_type& table,
                                              uint8_t k,
                                              const sets_type& table_la_sets,
                                              const ll_tal_tables_type& tal_tables);
};

END_NAMESPACE

#endif // __LL_VISUALIZATION_H__
