//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_ALGORITHM_H__
#define __LL_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class ll_algorithm : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

    public:
        // strong-LL(k)
        using strong_ll_table_row_type = std::vector<std::optional<rule_type>>;
        using strong_ll_table_type = std::vector<strong_ll_table_row_type>;

        // LL(k)
        // http://www.fit.vutbr.cz/~ikocman/llkptg/
        // %token a b c
        // %% /* LL(2) */
        // S : B a A a a
        //   | b A b a ;
        // A : /*eps*/
        //   | b ;
        // B : b ;
        // Table T0(T(S,{ε})
        // .................
        //
        //  u     production       follow
        // ---------------------------------------------
        // b a    S -> B a A a a   B:{a a, a b}, A:{a a}
        // b b    S -> b A b a     A:{b a}
        //
        using ll_tal_table_key_type = std::tuple<std::size_t, symbol_type, sets_type>; // T[A, L] and number of the table

        using ll_tal_table_follow_type = std::vector<std::pair<symbol_type, sets_type>>;

        struct ll_tal_table_row
        {
            using follow_type = ll_tal_table_follow_type; // for brevity

            set_type lookahead;     // u aka L, FIRSTk(α) (+)k L, { ba } or { bb }
            rule_type production;   // production, S -> B a A a a or S -> b A b a
            follow_type follow;     // local FOLLOWk sets, B:{a a, a b}, A:{a a} or A:{b a}
        };

        using ll_tal_table_row_type = ll_tal_table_row;
        using ll_tal_table_type = std::pair<ll_tal_table_key_type, std::vector<ll_tal_table_row_type>>;
        using ll_tal_tables_type = std::vector<ll_tal_table_type>;

        using ll_table_entry_symbol_type = std::variant<symbol_type, ll_tal_table_type>;
        using ll_table_entry_type = std::vector<ll_table_entry_symbol_type>;
        using ll_table_row_type = std::vector<std::optional<ll_table_entry_type>>;
        using ll_table_type = std::vector<ll_table_row_type>;

        static void     infix_operator(const sets_type& sets1, const sets_type& sets2, std::size_t k, sets_type& result);
        static void     infix_operator(const std::vector<sets_type>& sets, std::size_t k, sets_type& result);

    public:
        static bool     is_strong_ll_grammar(const grammar& gr, uint8_t k);
        static void     build_strong_ll_table(const grammar& gr, uint8_t k, strong_ll_table_type& result);

        static bool     is_ll_grammar(const grammar& gr, uint8_t k);
        static void     build_ll_tal_tables(const grammar& gr, uint8_t k, ll_tal_tables_type& result);
        static void     build_ll_table(const grammar& gr, uint8_t k, ll_table_type& result);
};

END_NAMESPACE

#endif // __LL_ALGORITHM_H__
