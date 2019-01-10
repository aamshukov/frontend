//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_ALGORITHM_H__
#define __EARLEY_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

#define PRECEDENCE_MATRIX_MAX_SIZE (64)

class earley_algorithm : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

        struct core_item
        {
            uint32_t  id;           // unique id
            rule_type rule;   // production (rule)
            uint32_t  dot_position; // dot - position in rhs,
                                    // if dot_position = rhs.size() it means it points to the end of the rhs
        };

        using core_item_type = std::shared_ptr<core_item>;
        using core_items_type = std::vector<core_item_type>;

        struct core_item_key_comparator
        {
            bool operator() (const core_item_type& lhs, const core_item_type& rhs) const
            {
                return (*lhs).id < (*rhs).id;
            }
        };

        using core_items_set_type = std::set<core_item_type, core_item_key_comparator>;

        struct chart;
        using chart_type = std::shared_ptr<chart>;

        struct item;
        using item_type = std::shared_ptr<item>;

        struct item_key_comparator
        {
            bool operator() (const item_type& lhs, const item_type& rhs) const
            {
                return (*lhs).id < (*rhs).id;
            }
        };

        using items_type = std::set<item_type, item_key_comparator>;

        struct item
        {
           uint32_t       id;           // unique id
           core_item_type core_item;    // pointer to static core item
           chart_type     origin_chart; // pointer to the orinal set/chart where recognition started
           chart_type     master_chart; // chart it belongs to
           item_type      lptr;         // pointer to left brother
           items_type     rptrs;        // rightmost child(s), if grammar is ambiguous
        };

        struct chart
        {
            uint32_t    id;    // unique id
            items_type  items; // list of items
        };

        using charts_type = std::vector<chart_type>;

    public:
        static void build_core_items_table(const grammar& gr, core_items_type& result);
        static void build_prediction_table(const grammar& gr, core_items_set_type& result);
};

END_NAMESPACE

#endif // __EARLEY_ALGORITHM_H__
