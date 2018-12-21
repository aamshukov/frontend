//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LR_ALGORITHM_H__
#define __LR_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class lr_algorithm : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

        struct lr_item
        {
            std::size_t id;            // unique id
            rule_type   production;    // production (rule)
            uint32_t    dot_position;  // dot - position in rhs,
                                       // if dot_position = rhs.size() it means it points to the end of the rhs
            set_type    la;            // lookahead for LR(k)/LALR(k) where k > 0

            bool operator == (const lr_item& other)
            {
                return (*production).id() == (*other.production).id() && dot_position == other.dot_position && la == other.la;
            }
        };

        using lr_item_type = std::shared_ptr<lr_item>;
        using lr_items_type = std::vector<lr_item_type>;

        struct lr_collection_element
        {
            std::size_t   id;
            flags         flags;
            lr_items_type items;

        };

        using lr_collection_element_type = std::shared_ptr<lr_collection_element>;
        using lr_canonical_collection_type = std::map<uint32_t, lr_collection_element_type>;

        using lr_goto_function_type = std::map<uint32_t, symbol_type>;

        using lr_action_table_row_type = std::vector<std::optional<rule_type>>;
        using lr_action_table_type = std::vector<lr_action_table_row_type>;

        using lr_goto_table_row_type = std::vector<std::optional<uint32_t>>;
        using lr_goto_table_type = std::vector<lr_goto_table_row_type>;

        static bool lr_items_equal(const lr_item_type& lhs, const lr_item_type& rhs);
        static bool has_lr_item(const lr_items_type& items, const lr_item_type& item);

    public:
        static void build_lr_items_set(const grammar& gr, const symbols_type& symbols, uint8_t k, lr_items_type& result); // symbols = viable prefix
        static void build_lr_items_canonical_collection(const grammar& gr, uint8_t k, lr_canonical_collection_type& result);

    public:
        static void build_lr_table(const grammar& gr, uint8_t k, lr_action_table_type& action_table, lr_goto_table_type& goto_table);

        static void build_lalr_table_yacc(const grammar& gr, uint8_t k, lr_action_table_type& action_table, lr_goto_table_type& goto_table);
        static void build_lalr_table_deremer(const grammar& gr, uint8_t k, lr_action_table_type& action_table, lr_goto_table_type& goto_table);
};

END_NAMESPACE

#endif // __LR_ALGORITHM_H__
