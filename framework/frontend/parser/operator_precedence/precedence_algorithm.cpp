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

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\operator_precedence\precedence_algorithm.hpp>
#include <frontend\parser\operator_precedence\precedence_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪

void precedence_algorithm::build_indices(const std::vector<typename precedence_algorithm::symbol_type>& terminals,
                                         const std::vector<typename precedence_algorithm::symbol_type>& nonterminals,
                                         std::vector<symbol_type>& symbols,
                                         std::map<uint32_t, std::size_t>& indices)
{
    std::for_each(terminals.begin(),
                  terminals.end(),
                  [&symbols](const auto& terminal){ symbols.emplace_back(terminal); });

    std::for_each(nonterminals.begin(),
                  nonterminals.end(),
                  [&symbols](const auto& nonterminal){ symbols.emplace_back(nonterminal); });

    std::for_each(symbols.begin(),
                  symbols.end(),
                  [&indices](const auto& symb)
                  {
                      indices[(*symb).id()] = indices.size();
                  });
}

bool precedence_algorithm::is_operator(const typename precedence_algorithm::operators_type& operators,
                                       const typename precedence_algorithm::symbol_type& symbol,
                                       bool keep_operands)
{
    return (*symbol).terminal() && (keep_operands || operators.find(symbol) != operators.end());
}

void precedence_algorithm::collect_operators(const grammar& gr,
                                             const typename precedence_algorithm::operators_type& operators,
                                             bool keep_operands,
                                             std::vector<typename precedence_algorithm::symbol_type>& result)
{
    std::for_each(gr.pool().begin(),
                  gr.pool().end(),
                  [&operators, &result, keep_operands](const auto& pool_symbol_kvp)
                  {
                      const auto& pool_symbol(pool_symbol_kvp.second);

                      if(is_operator(operators, pool_symbol, keep_operands))
                      {
                          result.emplace_back(pool_symbol);
                      }
                  });

    std::sort(result.begin(),
              result.end(),
              [](const symbol_type& symb1, const symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });
}

void precedence_algorithm::make_vector_unique(typename precedence_algorithm::set_type& sequence)
{
    std::sort(sequence.begin(),
              sequence.end(),
              [](const typename precedence_algorithm::symbol_type& symb1, const typename precedence_algorithm::symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });

    sequence.erase(std::unique(sequence.begin(), sequence.end() ), sequence.end());
}

void precedence_algorithm::build_first_operator_precedence_set(const grammar& gr,
                                                               const typename precedence_algorithm::operators_type& operators,
                                                               bool keep_operands,
                                                               typename precedence_algorithm::first_precedence_set_type& result)
{
    // Grune, p. 270 
    log_info(L"Building FIRST operator precedence set ...");

    first_precedence_set_type first_op_set;

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    // 1.
    // For each non-terminal A, find all right-hand sides of all rules for A;
    // now for each right-hand side R we insert the first operator in R (if any) into FIRSTop(A).
    // This gives us the initial values of all FIRSTopS.
    for(const auto& nonterminal : nonterminals)
    {
        first_op_set[nonterminal] = set_type{};

        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            for(const auto& rhs_symbol : (*nonterminal_rule).rhs())
            {
                if(is_operator(operators, rhs_symbol, keep_operands))
                {
                    first_op_set[nonterminal].emplace_back(rhs_symbol);
                    break;
                }
            }
        }
    }

    first_precedence_set_type first_op_set_prime; // FIRSTop'(A) - previous FIRSTop(A)

    // 2.
    // For each non-terminal A, find all right-hand sides of all rules for A;
    // now for each right-hand side R that starts with a non-terminal, say B, we add the elements of FIRSTop(B) to FIRSTop(A).
    do
    {
        first_op_set_prime = first_op_set;

        for(const auto& nonterminal : nonterminals)
        {
            auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                if((*(*nonterminal_rule).rhs().front()).nonterminal() && (*(*nonterminal_rule).rhs().front()).id() != (*nonterminal).id())
                {
                    std::for_each(first_op_set[(*nonterminal_rule).rhs().front()].begin(),
                                  first_op_set[(*nonterminal_rule).rhs().front()].end(),
                                  [&first_op_set, &nonterminal](const auto& symb)
                                  {
                                      first_op_set[nonterminal].emplace_back(symb);
                                  });
                }
            }

            make_vector_unique(first_op_set[nonterminal]);
        }
    }
    // 3.
    // Repeat step 2 above until no FIRSTop changes any more ...
    while(first_op_set != first_op_set_prime);

    result.swap(first_op_set);

    log_info(L"FIRST operator precedence set:");

    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"    %s: %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_set(result[nonterminal]).c_str());
    }

    log_info(L"Built FIRST operator precedence set.");
}

void precedence_algorithm::build_last_operator_precedence_set(const grammar& gr,
                                                              const typename precedence_algorithm::operators_type& operators,
                                                              bool keep_operands,
                                                              typename precedence_algorithm::last_precedence_set_type& result)
{
    // LASTop(A) is defined similarly, and a similar algorithm, using the last operator in R in step 1 and a B which ends A in step 2 provides it.
    log_info(L"Building LAST operator precedence set ...");

    last_precedence_set_type last_op_set;

    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    for(const auto& nonterminal : nonterminals)
    {
        last_op_set[nonterminal] = set_type{};

        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            for(std::size_t i = (*nonterminal_rule).rhs().size() - 1; static_cast<int>(i) >= 0; i--)
            {
                const auto& rhs_symbol((*nonterminal_rule).rhs()[i]);

                if(is_operator(operators, rhs_symbol, keep_operands))
                {
                    last_op_set[nonterminal].emplace_back(rhs_symbol);
                    break;
                }
            }
        }
    }

    last_precedence_set_type last_op_set_prime;

    do
    {
        last_op_set_prime = last_op_set;

        for(const auto& nonterminal : nonterminals)
        {
            auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                if((*(*nonterminal_rule).rhs().back()).nonterminal() && (*(*nonterminal_rule).rhs().back()).id() != (*nonterminal).id())
                {
                    std::for_each(last_op_set[(*nonterminal_rule).rhs().back()].begin(),
                                  last_op_set[(*nonterminal_rule).rhs().back()].end(),
                                  [&last_op_set, &nonterminal](const auto& symb)
                                  {
                                      last_op_set[nonterminal].emplace_back(symb);
                                  });
                }
            }

            make_vector_unique(last_op_set[nonterminal]);
        }
    }
    while(last_op_set != last_op_set_prime);

    result.swap(last_op_set);

    log_info(L"LAST operator precedence set:");

    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"    %s: %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_set(result[nonterminal]).c_str());
    }

    log_info(L"Built LAST operator precedence set.");
}

void precedence_algorithm::build_operator_precedence_table(const grammar& gr,
                                                           const typename precedence_algorithm::operators_type& operators,
                                                           bool keep_operands,
                                                           typename precedence_algorithm::precedence_table_type& result)
{
    log_info(L"Building operator precedence table ...");

    // collect terminals and nonterminals
    std::vector<symbol_type> terminals;

    collect_operators(gr, operators, keep_operands, terminals);

    std::map<uint32_t, std::size_t> indices;

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [&operators, &indices, keep_operands](const auto& terminal)
                  {
                      if(precedence_algorithm::is_operator(operators, terminal, keep_operands))
                          indices[(*terminal).id()] = indices.size();
                  });

    // initialize table
    precedence_table_type table;

    table.resize(terminals.size(), precedence_table_row_type(terminals.size(), std::nullopt));

    // initialize FIRSTop set
    first_precedence_set_type first_op_set;
    
    build_first_operator_precedence_set(gr, operators, keep_operands, first_op_set);

    // initialize LASTop set
    last_precedence_set_type last_op_set;

    build_last_operator_precedence_set(gr, operators, keep_operands, last_op_set);

    // calculate ≗
    for(const auto& rule : gr.rules())
    {
        // search for 'q1 q2' or 'q1 A q2'
        for(std::size_t i = 0, n = (*rule).rhs().size(); i < n - 1;)
        {
            if(!is_operator(operators, (*rule).rhs()[i], keep_operands))
            {
                i++;
                continue;
            }

            std::size_t j = i + 1;

            while(j < n) // skip A
            {
                if(is_operator(operators, (*rule).rhs()[j], keep_operands))
                {
                    break;
                }

                j++;
            }

            if(j < n && j - i <= 2) // must be at most one nonterminal, q1 A q2, 0 - 1 - 2
            {
                const auto& q1((*rule).rhs()[i]); // q1
                const auto& q2((*rule).rhs()[j]); // q2

                auto row = indices[(*q1).id()];
                auto col = indices[(*q2).id()];

                table[row][col] = precedence::equal_relation;

                i = j;
            }
            else
            {
                i++;
            }
        }
    }

    // calculate ⋖
    for(const auto& rule : gr.rules())
    {
        // search for 'q1 A'
        for(std::size_t i = 0, n = (*rule).rhs().size(); i < n - 1; i++)
        {
            if(is_operator(operators, (*rule).rhs()[i + 0], keep_operands) && !is_operator(operators, (*rule).rhs()[i + 1], keep_operands))
            {
                const auto& q1((*rule).rhs()[i + 0]); // q1

                const auto& nonterminal_first_op_set(first_op_set[(*rule).rhs()[i + 1]]);

                for(const auto& q2 : nonterminal_first_op_set) // q2
                {
                    auto row = indices[(*q1).id()];
                    auto col = indices[(*q2).id()];

                    if(table[row][col] == precedence::equal_relation)
                    {
                        log_info(L"Found =/> conflict.");
                    }

                    table[row][col] = precedence::less_relation;
                }
            }
        }
    }

    // calculate ⋗
    for(const auto& rule : gr.rules())
    {
        // search for 'A q1'
        for(std::size_t i = 0, n = (*rule).rhs().size(); i < n - 1; i++)
        {
            if(!is_operator(operators, (*rule).rhs()[i + 0], keep_operands) && is_operator(operators, (*rule).rhs()[i + 1], keep_operands))
            {
                const auto& q2((*rule).rhs()[i + 1]); // q2

                const auto& nonterminal_last_op_set(last_op_set[(*rule).rhs()[i + 0]]);

                for(const auto& q1 : nonterminal_last_op_set) // q1
                {
                    auto row = indices[(*q1).id()];
                    auto col = indices[(*q2).id()];

                    if(table[row][col] == precedence::equal_relation)
                    {
                        log_info(L"Found =/> conflict.");
                    }
                    else if(table[row][col] == precedence::less_relation)
                    {
                        log_info(L"Found </> conflict.");
                    }

                    table[row][col] = precedence::greater_relation;
                }
            }
        }
    }

    log_info(L"Operator Precedence Table:");
    log_info(L"%s", precedence_visualization::decorate_operator_precedence_table(gr, operators, keep_operands, table).c_str());

    result.swap(table);

    log_info(L"Built operator precedence table.");
}

void precedence_algorithm::build_first_wirth_weber_precedence_set(const grammar& gr, typename precedence_algorithm::first_precedence_set_type& result)
{
    log_info(L"Building FIRST operator precedence set Wirth-Weber ...");

    first_precedence_set_type first_op_set;

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    for(const auto& nonterminal : nonterminals)
    {
        first_op_set[nonterminal] = set_type{};

        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            first_op_set[nonterminal].emplace_back((*nonterminal_rule).rhs().front());
        }
    }

    first_precedence_set_type first_op_set_prime; // FIRSTop'(A) - previous FIRSTop(A)

    do
    {
        first_op_set_prime = first_op_set;

        for(const auto& nonterminal : nonterminals)
        {
            auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                if((*(*nonterminal_rule).rhs().front()).nonterminal() && (*(*nonterminal_rule).rhs().front()).id() != (*nonterminal).id())
                {
                    std::for_each(first_op_set[(*nonterminal_rule).rhs().front()].begin(),
                                  first_op_set[(*nonterminal_rule).rhs().front()].end(),
                                  [&first_op_set, &nonterminal](const auto& symb)
                                  {
                                      first_op_set[nonterminal].emplace_back(symb);
                                  });
                }
            }

            make_vector_unique(first_op_set[nonterminal]);
        }
    }
    while(first_op_set != first_op_set_prime);

    result.swap(first_op_set);

    log_info(L"FIRST operator precedence set:");

    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"    %s: %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_set(result[nonterminal]).c_str());
    }

    log_info(L"Built FIRST operator precedence set Wirth-Weber.");
}

void precedence_algorithm::build_last_wirth_weber_precedence_set(const grammar& gr, typename precedence_algorithm::last_precedence_set_type& result)
{
    log_info(L"Building LAST operator precedence set Wirth-Weber ...");

    last_precedence_set_type last_op_set;

    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    for(const auto& nonterminal : nonterminals)
    {
        last_op_set[nonterminal] = set_type{};

        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            last_op_set[nonterminal].emplace_back((*nonterminal_rule).rhs().back());
        }
    }

    last_precedence_set_type last_op_set_prime;

    do
    {
        last_op_set_prime = last_op_set;

        for(const auto& nonterminal : nonterminals)
        {
            auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                if((*(*nonterminal_rule).rhs().back()).nonterminal() && (*(*nonterminal_rule).rhs().back()).id() != (*nonterminal).id())
                {
                    std::for_each(last_op_set[(*nonterminal_rule).rhs().back()].begin(),
                                  last_op_set[(*nonterminal_rule).rhs().back()].end(),
                                  [&last_op_set, &nonterminal](const auto& symb)
                                  {
                                      last_op_set[nonterminal].emplace_back(symb);
                                  });
                }
            }

            make_vector_unique(last_op_set[nonterminal]);
        }
    }
    while(last_op_set != last_op_set_prime);

    result.swap(last_op_set);

    log_info(L"LAST operator precedence set:");

    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"    %s: %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_set(result[nonterminal]).c_str());
    }

    log_info(L"Built LAST operator precedence set Wirth-Weber.");
}

void precedence_algorithm::build_wirth_weber_precedence_table(const grammar& gr, typename precedence_algorithm::precedence_table_type& result)
{
    log_info(L"Building operator precedence table ...");

    // collect terminals and nonterminals
    std::vector<symbol_type> terminals;
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_terminals(gr, terminals);
    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    std::vector<symbol_type> symbols;
    std::map<uint32_t, std::size_t> indices;

    build_indices(terminals, nonterminals, symbols, indices);

    // calculate ≗
    bool equal_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(equal_matrix, 0, sizeof(equal_matrix));

    for(const auto& rule : gr.rules())
    {
        // search for 'q1 q2'
        for(int i = 0, n = static_cast<int>((*rule).rhs().size()); i < n - 1; i++)
        {
            const auto& q1((*rule).rhs()[i + 0]); // q1
            const auto& q2((*rule).rhs()[i + 1]); // q2

            auto row = indices[(*q1).id()];
            auto col = indices[(*q2).id()];

            equal_matrix[row][col] = true;
        }
    }

    log_info(L"EQUAL matrix:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, equal_matrix).c_str());

    // initialize FIRSTop set
    first_precedence_set_type first_op_set;
    
    build_first_wirth_weber_precedence_set(gr, first_op_set);

    // initialize LASTop set
    last_precedence_set_type last_op_set;

    build_last_wirth_weber_precedence_set(gr, last_op_set);

    // compute FIRST matrices
    bool first_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(first_matrix, 0, sizeof(first_matrix));

    for(const auto& nonterminal : nonterminals)
    {
        auto row = indices[(*nonterminal).id()];

        const auto& noterminal_first_op_set(first_op_set[nonterminal]);

        for(const auto& symb : noterminal_first_op_set)
        {
            auto col = indices[(*symb).id()];

            first_matrix[row][col] = true;
        }
    }

    log_info(L"FIRST matrix:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, first_matrix).c_str());

    bool first_transitive_closure_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memcpy(first_transitive_closure_matrix, first_matrix, sizeof(first_matrix));
    calculate_transitive_closure<PRECEDENCE_MATRIX_MAX_SIZE>(first_transitive_closure_matrix);

    log_info(L"FIRST+, transitive closure:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, first_transitive_closure_matrix).c_str());

    // calculate ⋖, product of EQUAL and FIRST+ (transitive completion)
    bool less_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(less_matrix, 0, sizeof(less_matrix));
    multiply<PRECEDENCE_MATRIX_MAX_SIZE>(equal_matrix, first_transitive_closure_matrix, less_matrix);

    log_info(L"LESS ((=) * FIRST+) matrix:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, less_matrix).c_str());

    bool first_reflexive_transitive_completion_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memcpy(first_reflexive_transitive_completion_matrix, first_transitive_closure_matrix, sizeof(first_transitive_closure_matrix));
    build_reflexive_transitive_completion<PRECEDENCE_MATRIX_MAX_SIZE>(first_reflexive_transitive_completion_matrix);

    log_info(L"FIRST*, reflexive transitive completion:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, first_reflexive_transitive_completion_matrix).c_str());

    bool first_terminals_reflexive_transitive_completion_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memcpy(first_terminals_reflexive_transitive_completion_matrix, first_reflexive_transitive_completion_matrix, sizeof(first_reflexive_transitive_completion_matrix));

    for(std::size_t i = 0, n = symbols.size(); i < n; i++) // remove all entries where the second (top) symbol is not terminal
    {
        for(std::size_t j = 0, m = n; j < m; j++)
        {
            if((*symbols[j]).nonterminal())
            {
                first_terminals_reflexive_transitive_completion_matrix[i][j] = false;
            }
        }
    }

    log_info(L"TFIRST*, terminals reflexive transitive completion:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, first_terminals_reflexive_transitive_completion_matrix).c_str());

    // compute LAST matrices
    bool last_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(last_matrix, 0, sizeof(last_matrix));

    for(const auto& nonterminal : nonterminals)
    {
        auto row = indices[(*nonterminal).id()];

        const auto& noterminal_last_op_set(last_op_set[nonterminal]);

        for(const auto& symb : noterminal_last_op_set)
        {
            auto col = indices[(*symb).id()];

            last_matrix[row][col] = true;
        }
    }

    log_info(L"LAST matrix:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, last_matrix).c_str());

    bool last_transitive_closure_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memcpy(last_transitive_closure_matrix, last_matrix, sizeof(last_matrix));
    calculate_transitive_closure<PRECEDENCE_MATRIX_MAX_SIZE>(last_transitive_closure_matrix);

    log_info(L"LAST+, transitive closure:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, last_transitive_closure_matrix).c_str());

    bool last_transitive_closure_transposed_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(last_transitive_closure_transposed_matrix, 0, sizeof(last_transitive_closure_transposed_matrix));
    transpose<PRECEDENCE_MATRIX_MAX_SIZE>(last_transitive_closure_matrix, last_transitive_closure_transposed_matrix);

    log_info(L"TRANSPOSE(LAST+), transposed transitive closure:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, last_transitive_closure_transposed_matrix).c_str());

    bool last_transitive_closure_transposed_equal_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(last_transitive_closure_transposed_equal_matrix, 0, sizeof(last_transitive_closure_transposed_equal_matrix));
    multiply<PRECEDENCE_MATRIX_MAX_SIZE>(last_transitive_closure_transposed_matrix, equal_matrix, last_transitive_closure_transposed_equal_matrix);

    log_info(L"TRANSPOSE(LAST+) * EQUAL, transposed transitive closure with equal:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, last_transitive_closure_transposed_equal_matrix).c_str());

    // calculate ⋗, product of TRANSPOSE(LAST+) * EQUAL * TFIRST* (terminals reflexive transitive completion)
    bool greater_matrix[PRECEDENCE_MATRIX_MAX_SIZE][PRECEDENCE_MATRIX_MAX_SIZE];

    memset(greater_matrix, 0, sizeof(greater_matrix));
    multiply<PRECEDENCE_MATRIX_MAX_SIZE>(last_transitive_closure_transposed_equal_matrix, first_terminals_reflexive_transitive_completion_matrix, greater_matrix);

    log_info(L"GREATER ((TRANSPOSE(LAST+)) * (=) * (TFIRST*)) matrix:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_matrix(gr, greater_matrix).c_str());

    // build table
    precedence_table_type table;

    table.resize(symbols.size(), precedence_table_row_type(symbols.size(), std::nullopt));

    for(std::size_t j = 0, n = symbols.size(); j < n; j++)
    {
        for(std::size_t i = 0, m = n; i < m; i++)
        {
            if(equal_matrix[j][i])
            {
                table[j][i] = precedence::equal_relation;
            }
        }
    }

    for(std::size_t j = 0, n = symbols.size(); j < n; j++)
    {
        for(std::size_t i = 0, m = n; i < m; i++)
        {
            if(less_matrix[j][i])
            {
                if(table[j][i].has_value() && table[j][i].value() == precedence::equal_relation)
                {
                    table[j][i] = precedence::less_relation;
                    log_info(L"Found =/< spurious conflict (%d, %d), ignoring and replacing with <.", j, i);
                }
                else
                {
                    table[j][i] = precedence::less_relation;
                }
            }
        }
    }

    for(std::size_t j = 0, n = symbols.size(); j < n; j++)
    {
        for(std::size_t i = 0, m = n; i < m; i++)
        {
            if(greater_matrix[j][i])
            {
                if(table[j][i].has_value())
                {
                    if(table[j][i].value() == precedence::equal_relation)
                    {
                        table[j][i] = precedence::greater_relation;
                        log_info(L"Found =/> spurious conflict (%d, %d), ignoring and replacing with >.", j, i);
                    }
                    else if(table[j][i].value() == precedence::less_relation)
                    {
                        table[j][i] = precedence::unknown_relation;
                        log_info(L"Found </> conflict (%d, %d).", j, i);
                    }
                }
                else
                {
                    table[j][i] = precedence::greater_relation;
                }
            }
        }
    }

    log_info(L"Operator Precedence Table:");
    log_info(L"%s", precedence_visualization::decorate_wirth_weber_precedence_table(gr, table).c_str());

    result.swap(table);

    log_info(L"Built operator precedence table.");
}

END_NAMESPACE
