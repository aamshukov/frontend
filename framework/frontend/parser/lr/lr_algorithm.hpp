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

        using counter_type = counter;

        struct lr_item
        {
            rule_type   rule;   // production (rule)
            uint32_t    dot;    // dot - position in rhs, if dot_position = rhs.size() it means it points to the end of the rhs

            sets_type   la;     // lookahead for LR(k)/LALR(k) where k > 0, for LALR(k) might be set

            bool operator == (const lr_item& other)
            {
                return (*rule).id() == (*other.rule).id() && dot == other.dot && la == other.la;
            }
        };

        using lr_item_type = std::shared_ptr<lr_item>;
        using lr_items_type = std::vector<lr_item_type>;

        struct lr_transition;

        using lr_transition_type = std::shared_ptr<lr_transition>;
        using lr_transitions_type = std::map<symbol_type, lr_transition_type, symbol::symbol_key_comparator>;

        using flags_type = flags;

        enum class lr_action : uint32_t
        {
            shift = std::numeric_limits<uint32_t>::max() - 5,
            reduce, // -4
            accept, // -3
            error   // -2
        };

        struct lr_state // automaton state with set of items
        {
            uint32_t            id;            // unique id

            lr_items_type       items;         // list of items
            lr_transitions_type transitions;   // outcomming transitions

            flags_type          flags;         // flags

            bool operator == (const lr_state& other)
            {
                bool result = items.size() == other.items.size();

                if(result)
                {
                    for(std::size_t i = 0, n = items.size(); i < n; i++)
                    {
                        if(!(*items[i] == *other.items[i]))
                        {
                            result = false;
                            break;
                        }
                    }
                }

                return result;
            }
        };

        using lr_state_type = std::shared_ptr<lr_state>;
        using lr_states_type = std::vector<lr_state_type>;

        struct lr_transition
        {
            lr_state_type state;  // to state
            symbol_type   symbol; // predicate

            bool operator == (const lr_transition& other)
            {
                return (*symbol).id() == (*other.symbol).id() && (*state).id == (*other.state).id;
            }
        };

        using lr_goto_table_type = std::map<std::pair<uint32_t, uint32_t>, uint32_t>;
        using lr_action_table_type = std::map<std::pair<std::vector<uint32_t>, uint32_t>, std::vector<uint32_t>>;




        //??
        using lr_canonical_collection_type = std::vector<lr_items_type>;

    private:
        static bool             lr_items_equal(const lr_item_type& lhs, const lr_item_type& rhs);
        static bool             has_lr_item(const lr_items_type& items, const lr_item_type& item);

        static bool             lr_states_equal(const lr_state_type& lhs, const lr_state_type& rhs);
        static bool             has_lr_state(const lr_states_type& states, const lr_state_type& state);

    public:
        static void             collect_grammar_la(const grammar& gr, uint8_t k, sets_type& result);

        static void             calculate_lr_closure(const grammar& gr, uint8_t k, const lr_state_type& state);
        static lr_state_type    calculate_lr_goto(const grammar& gr,
                                                  uint8_t k,
                                                  const lr_state_type& state, // I
                                                  const symbol_type& symb);   // X

        static void             build_lr_automaton(const grammar& gr, uint8_t k, lr_states_type& result);

        static void             build_goto_table(const grammar& gr, const lr_states_type& states, lr_algorithm::lr_goto_table_type& result);
        static void             build_action_table(const grammar& gr,
                                                   uint8_t k,
                                                   const lr_states_type& states,
                                                   const lr_goto_table_type& goto_table,
                                                   lr_algorithm::lr_action_table_type& result);

        static void             build_lr_table(const grammar& gr,
                                               uint8_t k,
                                               lr_goto_table_type& goto_table_result,
                                               lr_action_table_type& action_table_result);

    public:
        static void             build_lr_canonical_collection(const grammar& gr, uint8_t k, lr_canonical_collection_type& result);

    public:
        static void             build_lr_items_set_for_viable_prefix(const grammar& gr,
                                                                     const symbols_type& symbols, // symbols = viable prefix
                                                                     uint8_t k,
                                                                     lr_items_type& result);
};

END_NAMESPACE

#endif // __LR_ALGORITHM_H__
