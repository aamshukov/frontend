//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_VISUALIZATION_H__
#define __EARLEY_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class earley_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using item_type = earley_algorithm::item_type;
        using chart_type = earley_algorithm::chart_type;
        using charts_type = earley_algorithm::charts_type;

        using tree_type = earley_algorithm::tree_type;
        using trees_type = earley_algorithm::trees_type;

    public:
        static string_type decorate_item(const item_type& item, bool recursive = true);
        static string_type decorate_chart(const chart_type& chart);
        static string_type decorate_charts(const charts_type& charts);

        static string_type decorate_trees(const trees_type& trees);
};

END_NAMESPACE

#endif // __EARLEY_VISUALIZATION_H__
