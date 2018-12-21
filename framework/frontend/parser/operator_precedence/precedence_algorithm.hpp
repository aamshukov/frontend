//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_ALGORITHM_H__
#define __PRECEDENCE_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

#define PRECEDENCE_MATRIX_MAX_SIZE (64)

class precedence_algorithm : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

        // precedence relations
        enum class precedence
        {
            unknown_relation = 0,
            less_relation    = 1,   // ⋖, op1 has higher precedence than op2
            equal_relation   = 2,   // ≗, op1 an op2 have the same precedence
            greater_relation = 3    // ⋗, op1 has lower precedence than op2
        };

        using operators_type = std::set<symbol_type, symbol::symbol_key_comparator>;

        using first_precedence_set_type = std::map<symbol_type, set_type, symbol::symbol_key_comparator>;
        using last_precedence_set_type = std::map<symbol_type, set_type, symbol::symbol_key_comparator>;

        using precedence_table_row_type = std::vector<std::optional<precedence>>;
        using precedence_table_type = std::vector<precedence_table_row_type>;

    private:
        static void make_vector_unique(set_type& sequence);

    public:
        static void build_indices(const std::vector<symbol_type>& terminals,
                                  const std::vector<symbol_type>& nonterminals,
                                  std::vector<symbol_type>& symbols,
                                  std::map<uint32_t, std::size_t>& indices);

        // Operator precedence table by Grune, only 'operators' are considered
        static bool is_operator(const operators_type& operators, const symbol_type& symbol, bool keep_operands);
        static void collect_operators(const grammar& gr, const operators_type& operators, bool keep_operands, std::vector<symbol_type>& result);

        static void build_first_operator_precedence_set(const grammar& gr, const operators_type& operators, bool keep_operands, first_precedence_set_type& result);
        static void build_last_operator_precedence_set(const grammar& gr, const operators_type& operators, bool keep_operands, last_precedence_set_type& result);
        static void build_operator_precedence_table(const grammar& gr, const operators_type& operators, bool keep_operands, precedence_table_type& result);

        // Wirth-Weber relations, simple-precedence table
        static void build_first_wirth_weber_precedence_set(const grammar& gr, first_precedence_set_type& result);
        static void build_last_wirth_weber_precedence_set(const grammar& gr, last_precedence_set_type& result);
        static void build_wirth_weber_precedence_table(const grammar& gr, precedence_table_type& result);
};

END_NAMESPACE

#endif // __PRECEDENCE_ALGORITHM_H__
