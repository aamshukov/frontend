//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LR_VISUALIZATION_H__
#define __LR_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class lr_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using lr_item_type = lr_algorithm::lr_item_type;
        using lr_items_type = lr_algorithm::lr_items_type;

        using lr_state_type = lr_algorithm::lr_state_type;
        using lr_states_type = lr_algorithm::lr_states_type;

        using lr_transition_type = lr_algorithm::lr_transition_type;
        using lr_transitions_type = lr_algorithm::lr_transitions_type;

        using lr_goto_table_type = lr_algorithm::lr_goto_table_type;



        //using lr_collection_element_type = lr_algorithm::lr_collection_type;
        //using lr_canonical_collection_type = lr_algorithm::lr_canonical_collection_type;

        using lr_goto_function_type = lr_algorithm::lr_goto_table_type;

        using lr_action_table_row_type = lr_algorithm::lr_action_table_row_type;
        using lr_action_table_type = lr_algorithm::lr_action_table_type;

        using lr_goto_table_row_type = lr_algorithm::lr_goto_table_row_type;
        using lr_goto_table_type = lr_algorithm::lr_goto_table_type;

    public:
        static string_type decorate_lr_item(const lr_item_type& item);
        static string_type decorate_lr_items(const lr_items_type& items);

        static string_type decorate_lr_transition(const lr_transition_type& transition);
        static string_type decorate_lr_transitions(const lr_transitions_type& transitions);

        static string_type decorate_lr_state(const lr_state_type& state);
        static string_type decorate_lr_states(const lr_states_type& states);

        static string_type decorate_lr_goto_table(const grammar& gr, const lr_states_type& states, const lr_goto_table_type& goto_table);
};

END_NAMESPACE

#endif // __LR_VISUALIZATION_H__
