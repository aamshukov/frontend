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

        enum class flags : uint32_t
        {
            init          = 0x00000001, // .
            scanner       = 0x00000002, //  .
            predictor     = 0x00000004, //   . which action introduced item
            completer     = 0x00000008, //  .
            error_scanner = 0x00000010, // .
            error_mask    = 0x07000000,
            marked        = 0x00000080
        };

        struct chart;
        using chart_type = std::shared_ptr<chart>;

        struct item;
        using item_type = std::shared_ptr<item>;

        struct item_hash
        {
            std::size_t operator()(const item_type&) const
            {
                return 0; // returning 0 causes item_key_comparator to be called
            }
        };

        struct item_key_comparator
        {
            bool operator() (const item_type& lhs, const item_type& rhs) const
            {
                return *lhs == *rhs;
            }
        };

        using items_type = std::unordered_set<item_type, item_hash, item_key_comparator>;

        struct item
        {
            uint32_t    id;              // unique id

            rule_type   rule;           // production (rule)
            uint32_t    dot;            // dot - position in rhs, if dot_position = rhs.size() it means it points to the end of the rhs

            chart_type  origin_chart;   // pointer to the orinal set/chart where recognition started
            chart_type  master_chart;   // chart it belongs to

            item_type   lptr;           // pointer to left brother
            items_type  rptrs;          // rightmost child(s), if grammar is ambiguous

            flags       flags;          // action (predictor, completer, scanner, errorscanner), error cost

            bool operator == (const item& other)
            {
                return rule != nullptr && other.rule != nullptr && (*rule).id() == (*other.rule).id() &&
                       dot == other.dot &&
                       origin_chart != nullptr && other.origin_chart != nullptr && (*origin_chart).id == (*other.origin_chart).id &&
                       lptr == other.lptr;
            }
        };

        struct chart // state or chart
        {
            uint32_t    id;              // unique id
            items_type  items;           // list of items
        };

        using charts_type = std::vector<chart_type>;

        using tree_type = std::shared_ptr<tree>;
        using trees_type = std::vector<tree_type>;

    public:
        static item_type    create_item(const rule_type& rule,            // production (rule)
                                        const chart_type& origin_chart,   // original chart recognition started
                                        const chart_type& master_chart,   // chart to add to
                                        const item_type& lptr,            // l-ptr
                                        flags action);                    // action introduced this item
        static bool         is_final_item(const grammar& gr, const item_type& item);
        static void         set_rptr(item_type& item, const item_type& new_item);

        static chart_type   create_chart(uint32_t id);
        static bool         is_final_chart(const grammar& gr, const chart_type& chart);

    public:
        static void         closure(const grammar& gr, chart_type& chart);

        static void         predict(const grammar& gr, const item_type& item, chart_type& chart);
        static void         complete(item_type& item, chart_type& chart);

        static void         scan(chart_type& chart, charts_type& charts, uint32_t terminal, chart_type& result);
};

END_NAMESPACE

#endif // __EARLEY_ALGORITHM_H__
