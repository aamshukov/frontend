//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_VISUALIZATION_H__
#define __PRECEDENCE_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class precedence_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using precedence_table_row_type = precedence_algorithm::precedence_table_row_type;
        using precedence_table_type = precedence_algorithm::precedence_table_type;

        using operators_type = precedence_algorithm::operators_type;

    public:
        static string_type  decorate_operator_precedence_matrix(const grammar& gr,
                                                                const operators_type& operators,
                                                                bool keep_operands,
                                                                bool array[][PRECEDENCE_MATRIX_MAX_SIZE]);
        static string_type  decorate_operator_precedence_table(const grammar& gr,
                                                               const operators_type& operators,
                                                               bool keep_operands,
                                                               const precedence_table_type& table);

        static string_type  decorate_wirth_weber_precedence_matrix(const grammar& gr, bool array[][PRECEDENCE_MATRIX_MAX_SIZE]);
        static string_type  decorate_wirth_weber_precedence_table(const grammar& gr, const precedence_table_type& table);
};

END_NAMESPACE

#endif // __PRECEDENCE_VISUALIZATION_H__
