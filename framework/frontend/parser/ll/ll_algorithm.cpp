//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\factory.hpp>
#include <core\text.hpp>
#include <core\flags.hpp>
#include <core\tree.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\ll\ll_algorithm.hpp>
#include <frontend\parser\ll\ll_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪

bool ll_algorithm::is_strong_ll_grammar(const grammar& gr, uint8_t k)
{
    strong_ll_table_type table;

    ll_algorithm::build_strong_ll_table(gr, k, table);

    return !table.empty();
}

void ll_algorithm::build_strong_ll_table(const grammar& gr, uint8_t k, typename ll_algorithm::strong_ll_table_type& result)
{
    log_info(L"Building strong-LL(k) table for k = %d ...", k);

    // collect lookahead sets
    sets_type table_la_sets;

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        std::for_each((*pool_symb).la_sets().begin(), (*pool_symb).la_sets().end(), [&table_la_sets](const auto& set){ table_la_sets.emplace_back(set); });
    }

    grammar_algorithm::make_vector_unique(table_la_sets);

    log_info(L"Collected LA sets: %s", grammar_visualization::decorate_sets(table_la_sets).c_str());

    // collect nonterminals/terminals
    std::vector<symbol_type> nonterminals;
    std::map<uint32_t, std::size_t> nonterminals_to_indices;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    std::for_each(nonterminals.begin(),
                  nonterminals.end(),
                  [&nonterminals_to_indices](const auto& nonterminal){ nonterminals_to_indices[(*nonterminal).id()] = nonterminals_to_indices.size(); });

    // build table
    strong_ll_table_type table;

    table.resize(nonterminals.size(), strong_ll_table_row_type(table_la_sets.size(), std::nullopt));

    bool is_strong_ll_grammar = true;

    for(const auto& nonterminal : nonterminals)
    {
        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            sets_type la_sets;

            grammar_algorithm::build_la_set(nonterminal_rule, k, la_sets);

            for(std::size_t i = 0, m = table_la_sets.size(); i < m; i++)
            {
                if(std::find(la_sets.begin(), la_sets.end(), table_la_sets[i]) != la_sets.end())
                {
                    auto row = nonterminals_to_indices[(*nonterminal).id()];
                    auto col = i;

                    if(table[row][col].has_value())
                    {
                        is_strong_ll_grammar = false;

                        log_info(L"The grammar is not strong-LL(k) for k = %d, found conflict on %s:",
                                 k,
                                 grammar_visualization::decorate_set(table_la_sets[i]).c_str());
                        log_info(L"    %s   ---------   %s",
                                 grammar_visualization::decorate_rule(table[row][col].value()).c_str(),
                                 grammar_visualization::decorate_rule(nonterminal_rule).c_str());
                    }
                    else
                    {
                        table[row][col] = nonterminal_rule; // nonterminal id is the row index
                    }
                }
            }
        }
    }

    ll_visualization::decorate_strong_ll_table(table, k, table_la_sets, nonterminals);

    if(is_strong_ll_grammar)
    {
        result.swap(table);
    }

    log_info(L"Built strong-LL(k) table for k = %d.", k);
}

bool ll_algorithm::is_ll_grammar(const grammar& gr, uint8_t k)
{
    ll_table_type table;

    ll_algorithm::build_ll_table(gr, k, table);

    return !table.empty();
}

void ll_algorithm::build_ll_tal_tables(const grammar& gr, uint8_t k, typename ll_algorithm::ll_tal_tables_type& result)
{
    log_info(L"Building LL(%d) T[A, L] tables ...", k);

    // intermidiate results
    ll_tal_tables_type tables;

    // T[A, L] tables (keys) to be constructed, T[S, {ε}], etc.
    std::deque<ll_tal_table_key_type> keys;

    // processed keys, might be infinite looping due to S -> a A b and A -> c S rules, see Grune's grammars ...
    std::vector<ll_tal_table_key_type> processed_keys;

    std::size_t table_index = 0;

    // initialize with T[S, {ε}]
    ll_tal_table_key_type initial_key(std::make_tuple(table_index++, gr.start_symbol(), sets_type{ set_type{ symbol::epsilon } }));

    keys.emplace_back(initial_key);
    processed_keys.emplace_back(initial_key);

    do
    {
        // pop T[A, L] to be constructed
        auto& key(keys.front());

        // create new T[A, L] table
        std::vector<ll_tal_table_row_type> rows;

        ll_tal_table_type table(std::make_pair(key, rows));

        // get L
        const auto& follow(std::get<2>(key));

        // iterate over rules of nonterminal
        const auto& nonterminal_rules((*gr.nt_rules().find((*(std::get<1>(key))).name())).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            // calculate lookahead sets, FIRSTk(α) (+)k L
            // calculate FIRSTk(α)
            sets_type first_alpha;

            if((*nonterminal_rule).empty()) // B -> ε
            {
                first_alpha = sets_type { set_type { symbol::epsilon } };
            }
            else
            {
                grammar_algorithm::build_first_set((*nonterminal_rule).rhs(), k, first_alpha);
            }

            // calculate FIRSTk(α) (+)k L
            sets_type lookaheads;

            if(first_alpha == sets_type { set_type { symbol::epsilon } } && follow == sets_type { set_type { symbol::epsilon } })
            {
                lookaheads = sets_type { set_type { symbol::epsilon } };
            }
            else
            {
                std::vector<sets_type> infix_op_input;

                infix_op_input.emplace_back(first_alpha);
                infix_op_input.emplace_back(follow);

                grammar_algorithm::infix_operator(infix_op_input, k, lookaheads);
            }

            for(const auto& lookahead : lookaheads)
            {
                // add row to table
                ll_tal_table_row_type row;

                row.lookahead = lookahead;
                row.production = nonterminal_rule;

                // calculate L: for each A in RHS calculate FIRSTk(whatever-is-after A) (+)k L
                // S -> 'B a A a a' gives FIRSTk(a A a a) and FIRSTk(a a)
                for(std::size_t i = 0, n = (*nonterminal_rule).rhs().size(); i < n; i++)
                {
                    const auto& current_nonterminal((*nonterminal_rule).rhs()[i]);

                    if((*current_nonterminal).nonterminal())
                    {
                        // construct input for FIRSTk(whatever-is-after A)
                        set_type symbols;

                        std::for_each((*nonterminal_rule).rhs().begin() + i + 1, // skip A itself
                                      (*nonterminal_rule).rhs().end(),
                                      [&symbols](const auto& symb){ symbols.emplace_back(symb); });

                        // calculate FIRSTk(whatever-is-after A)
                        sets_type first_sets;

                        grammar_algorithm::build_first_set(symbols, k, first_sets);

                        // FIRSTk(whatever-is-after A) (+)k L
                        sets_type follow_sets;

                        if(first_sets == sets_type { set_type { symbol::epsilon } } && follow == sets_type { set_type { symbol::epsilon } })
                        {
                            follow_sets = sets_type { set_type { symbol::epsilon } };
                        }
                        else
                        {
                            std::vector<sets_type> infix_op_input;

                            infix_op_input.emplace_back(first_sets);
                            infix_op_input.emplace_back(follow);

                            grammar_algorithm::infix_operator(infix_op_input, k, follow_sets);
                        }

                        // add to local follow set
                        row.follow.emplace_back(std::make_pair(current_nonterminal, follow_sets));

                        // insert new T[A, L] in the set of tables to be constructed
                        ll_tal_table_key_type new_key(std::make_tuple(0, current_nonterminal, follow_sets));

                        if(std::find_if(processed_keys.begin(),
                                        processed_keys.end(),
                                        [&new_key](const auto& key)
                                        {
                                            return (*(std::get<1>(key))).id() == (*std::get<1>(new_key)).id() && std::get<2>(key) == std::get<2>(new_key);
                                        }) == processed_keys.end())
                        {
                            std::get<0>(new_key) = table_index++; // update table index

                            keys.emplace_back(new_key);
                            processed_keys.emplace_back(new_key);
                        }
                    }
                }

                // add new row
                table.second.emplace_back(row);
            }
        }

        // add new table
        tables.emplace_back(table);

        keys.pop_front();
    }
    while(!keys.empty());

    ll_visualization::decorate_ll_tal_tables(tables, k);

    result.swap(tables);

    log_info(L"Built LL(%d) T[A, L] tables.", k);
}

void ll_algorithm::build_ll_table(const grammar& gr, uint8_t k, typename ll_algorithm::ll_table_type& result)
{
    log_info(L"Building LL(%d) table ...", k);

    // build T[A, L] tables
    ll_tal_tables_type tal_tables;

    build_ll_tal_tables(gr, k, tal_tables);

    // collect lookahead sets
    sets_type table_la_sets;

    for(const auto& tal_table : tal_tables)
    {
        const auto& tal_table_rows(tal_table.second);

        for(const auto& tal_table_row : tal_table_rows)
        {
            table_la_sets.emplace_back(tal_table_row.lookahead);
        }
    }

    grammar_algorithm::make_vector_unique(table_la_sets);

    // move {ε} to the end for visibility (if exist)
    auto it(std::remove_if(table_la_sets.begin(), table_la_sets.end(), [](const auto& set){ return set == set_type{ symbol::epsilon }; }));
    
    if(it != table_la_sets.end())
    {
        table_la_sets.erase(it);
        table_la_sets.emplace_back(set_type { symbol::epsilon });
    }

    log_info(L"Collected LA sets: %s", grammar_visualization::decorate_sets(table_la_sets).c_str());

    bool is_ll_grammar = true;

    // build table
    ll_table_type table;

    table.resize(tal_tables.size(), ll_table_row_type(table_la_sets.size(), std::nullopt));

    std::size_t col = 0;
    std::size_t row = 0;

    // loop over T[A, L] tables, each T[A, L] table constitutes row
    for(const auto& tal_table : tal_tables)
    {
        const auto& tal_table_key(tal_table.first);
        const auto& tal_table_rows(tal_table.second);

        // assemble row
        ll_table_row_type table_row;

        for(const auto& tal_table_row : tal_table_rows) // each iteration builds a new row's entry
        {
            const auto& lookahead(tal_table_row.lookahead);     // lookahead of the current T[A, L]
            const auto& production(tal_table_row.production);   // production we deal with in the current T[A, L]
            const auto& follow(tal_table_row.follow);           // set of local FOLLOWk sets in the current T[A, L]

            // calculate corresponding lookahead column: T[A, { b a }] = { b a }
            for(col = 0; col < table_la_sets.size(); col++)
            {
                if(table_la_sets[col] == lookahead)
                {
                    break;
                }
            }

            // assemble LL(k) table entry
            ll_table_entry_type table_entry;

            std::size_t i = 0; // index in follow sets,  { a a }     S : B a C A a C c a A   B:{ { a a } { a c } { a b } }, C:{ { a c } { b a } }, A:{ { a c } }, C:{ { c a } }, A:{ { ε ) } }
                               //                                                            0                              1                      2              3              4
            for(const auto& current_production_nonterminal : (*production).rhs()) // for each symbol in the RHS of the production, S -> B a A a a           ^
            {                                                                                                                                           //  |
                ll_table_entry_symbol_type table_entry_symbol; // new table entry                                                                       //  |
                                                                                                                                                        //  |
                if((*current_production_nonterminal).nonterminal())                                                                                     //  |
                {                                                                                                                                       //  |
                    const auto& local_follow(follow[i++]); // get entry from -------------------------------------------------------------------------------|

                    // looking for a corresponding T[B, L] table in all T[A, L] tables
                    for(const auto& tal_table0 : tal_tables)
                    {
                        // T1[B, { { a a } { a b } } ]
                        const auto& current_tal_table_key(tal_table0.first);

                        const auto& current_tal_table_key_symbol(std::get<1>(current_tal_table_key)); // B
                        const auto& current_tal_table_key_follow(std::get<2>(current_tal_table_key)); // { { a a } { a b } }

                        // compare nonterminal and follow set: T1[B, { { a a } { a c } { a b } } ] and B:{ { a a } { a c } { a b } }, index = 0
                        if((*current_tal_table_key_symbol).id() == (*current_production_nonterminal).id() && current_tal_table_key_follow == local_follow.second)
                        {
                            table_entry_symbol = tal_table0;
                            break;
                        }
                    }
                }
                else
                {
                    // terminal symbols are added as is: T1 a T2 b c
                    table_entry_symbol = current_production_nonterminal;
                }

                table_entry.emplace_back(table_entry_symbol);
            }

            row = std::get<0>(tal_table_key); // T[A, L} table index is the row index

            if(table[row][col].has_value())
            {
                is_ll_grammar = false;

                log_info(L"The grammar is not LL(k) for k = %d, found conflict on %s:",
                         k,
                         grammar_visualization::decorate_set(table_la_sets[col]).c_str());
                log_info(L"    %s   ---------   %s",
                         ll_visualization::decorate_ll_table_entry(table[row][col].value()).c_str(),
                         ll_visualization::decorate_ll_table_entry(table_entry).c_str());
            }
            else
            {
                table[row][col] = table_entry;
            }

            table_row.emplace_back(table_entry);
        }

        table.emplace_back(table_row);
    }

    ll_visualization::decorate_ll_table(table, k, table_la_sets, tal_tables);

    if(is_ll_grammar)
    {
        result.swap(table);
    }

    log_info(L"Built LL(%d) table.", k);
}

END_NAMESPACE
