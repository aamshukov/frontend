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

        using lr_collection_element_type = lr_algorithm::lr_collection_element_type;
        using lr_canonical_collection_type = lr_algorithm::lr_canonical_collection_type;

        using lr_goto_function_type = lr_algorithm::lr_goto_function_type;

        using lr_action_table_row_type = lr_algorithm::lr_action_table_row_type;
        using lr_action_table_type = lr_algorithm::lr_action_table_type;

        using lr_goto_table_row_type = lr_algorithm::lr_goto_table_row_type;
        using lr_goto_table_type = lr_algorithm::lr_goto_table_type;

    public:
        static string_type decorate_lr_item(const lr_item_type& item);
        static string_type decorate_lr_items(const lr_items_type& items);
};

END_NAMESPACE

#endif // __LR_VISUALIZATION_H__
