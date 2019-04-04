//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\factory.hpp>
#include <core\flags.hpp>
#include <core\text.hpp>
#include <core\tree.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\operator_precedence\precedence_algorithm.hpp>
#include <frontend\parser\operator_precedence\\precedence_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type precedence_visualization::decorate_operator_precedence_matrix(const grammar& gr,
                                                                          const typename precedence_visualization::operators_type& operators,
                                                                          bool keep_operands,
                                                                          bool array[][PRECEDENCE_MATRIX_MAX_SIZE])
{
    std::wstringstream content;

    std::vector<symbol_type> terminals;

    precedence_algorithm::collect_operators(gr, operators, keep_operands, terminals);

    std::map<uint32_t, std::size_t> symbols_to_indices;

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [&operators, &symbols_to_indices, keep_operands](const auto& terminal)
                  {
                      if(precedence_algorithm::is_operator(operators, terminal, keep_operands))
                          symbols_to_indices[(*terminal).id()] = symbols_to_indices.size();
                  });

    auto alignment = std::left;

    const std::size_t column_width = 8;

    content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [column_width, &operators, &content, keep_operands](const auto& terminal)
                  {
                      if(precedence_algorithm::is_operator(operators, terminal, keep_operands))
                          content << std::left << std::setw(column_width) << std::setfill(L' ') << (*terminal).name().c_str();
                  });

    content << std::endl;

    for(std::size_t j = 0, n = terminals.size(); j < n; j++)
    {
        if(precedence_algorithm::is_operator(operators, terminals[j], keep_operands))
        {
            content << alignment << std::setw(column_width) << std::setfill(L' ') << (*terminals[j]).name().c_str();

            for(std::size_t i = 0, m = terminals.size(); i < m; i++)
            {
                if(precedence_algorithm::is_operator(operators, terminals[i], keep_operands))
                {
                    content << alignment << std::setw(column_width) << std::setfill(L' ') << array[j][i];
                }
            }

            content << std::endl;
        }
    }

    content << std::endl;

    return content.str();
}

string_type precedence_visualization::decorate_operator_precedence_table(const grammar& gr,
                                                                         const typename precedence_visualization::operators_type& operators,
                                                                         bool keep_operands,
                                                                         const typename precedence_visualization::precedence_table_type& table)
{
    std::wstringstream content;

    std::vector<symbol_type> terminals;

    precedence_algorithm::collect_operators(gr, operators, keep_operands, terminals);

    std::map<uint32_t, std::size_t> symbols_to_indices;

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [&operators, &symbols_to_indices, keep_operands](const auto& terminal)
                  {
                      if(precedence_algorithm::is_operator(operators, terminal, keep_operands))
                          symbols_to_indices[(*terminal).id()] = symbols_to_indices.size();
                  });

    auto alignment = std::left;

    const std::size_t column_width = 8;

    content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [column_width, &operators, &content, keep_operands](const auto& terminal)
                  {
                      if(precedence_algorithm::is_operator(operators, terminal, keep_operands))
                          content << std::left << std::setw(column_width) << std::setfill(L' ') << (*terminal).name().c_str();
                  });

    content << std::endl;

    for(std::size_t j = 0, n = terminals.size(); j < n; j++)
    {
        if(precedence_algorithm::is_operator(operators, terminals[j], keep_operands))
        {
            content << alignment << std::setw(column_width) << std::setfill(L' ') << (*terminals[j]).name().c_str();

            for(std::size_t i = 0, m = terminals.size(); i < m; i++)
            {
                if(precedence_algorithm::is_operator(operators, terminals[i], keep_operands))
                {
                    if(table[j][i].has_value())
                    {
                        char_type* value;

                        switch(table[j][i].value())
                        {
                            case precedence_algorithm::precedence::equal_relation:
                                value = L"=";
                                break;
                            case precedence_algorithm::precedence::less_relation:
                                value = L"<";
                                break;
                            case precedence_algorithm::precedence::greater_relation:
                                value = L">";
                                break;
                            case precedence_algorithm::precedence::unknown_relation:
                            default:
                                value = L"U";
                                break;
                        }

                        content << alignment << std::setw(column_width) << std::setfill(L' ') << value;
                    }
                    else
                    {
                        content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';
                    }
                }
            }

            content << std::endl;
        }
    }

    content << std::endl;

    return content.str();
}

string_type precedence_visualization::decorate_wirth_weber_precedence_matrix(const grammar& gr, bool array[][PRECEDENCE_MATRIX_MAX_SIZE])
{
    std::wstringstream content;

    std::vector<symbol_type> terminals;
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_terminals(gr, terminals);
    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    std::vector<symbol_type> symbols;

    std::for_each(terminals.begin(),
                  terminals.end(),
                  [&symbols](const auto& terminal){ symbols.emplace_back(terminal); });

    std::for_each(nonterminals.begin(),
                  nonterminals.end(),
                  [&symbols](const auto& nonterminal){ symbols.emplace_back(nonterminal); });

    auto alignment = std::left;

    const std::size_t column_width = 8;

    content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';

    std::for_each(symbols.begin(),
                  symbols.end(),
                  [column_width, &content](const auto& symb)
                  {
                      content << std::left << std::setw(column_width) << std::setfill(L' ') << (*symb).name().c_str();
                  });

    content << std::endl;

    for(std::size_t j = 0, n = symbols.size(); j < n; j++)
    {
        content << alignment << std::setw(column_width) << std::setfill(L' ') << (*symbols[j]).name().c_str();

        for(std::size_t i = 0, m = symbols.size(); i < m; i++)
        {
            content << alignment << std::setw(column_width) << std::setfill(L' ') << array[j][i];
        }

        content << std::endl;
    }

    content << std::endl;

    return content.str();
}

string_type precedence_visualization::decorate_wirth_weber_precedence_table(const grammar& gr, const typename precedence_visualization::precedence_table_type& table)
{
    std::wstringstream content;

    std::vector<symbol_type> terminals;
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_terminals(gr, terminals);
    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    std::vector<symbol_type> symbols;
    std::map<uint32_t, std::size_t> indices;

    precedence_algorithm::build_indices(terminals, nonterminals, symbols, indices);

    auto alignment = std::left;

    const std::size_t column_width = 8;

    content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';

    std::for_each(symbols.begin(),
                  symbols.end(),
                  [column_width, &content](const auto& symb)
                  {
                      content << std::left << std::setw(column_width) << std::setfill(L' ') << (*symb).name().c_str();
                  });

    content << std::endl;

    for(std::size_t i = 0, m = symbols.size(); i <= m; i++) // <=  for an extra column
    {
        content << alignment << std::setw(column_width) << std::setfill(L'-') << L'-';
    }

    content << std::endl;

    for(std::size_t j = 0, n = symbols.size(); j < n; j++)
    {
        content << alignment << std::setw(column_width) << std::setfill(L' ') << (*symbols[j]).name().c_str();

        for(std::size_t i = 0, m = symbols.size(); i < m; i++)
        {
            if(table[j][i].has_value())
            {
                char_type* value;

                switch(table[j][i].value())
                {
                    case precedence_algorithm::precedence::equal_relation:
                        value = L"=";
                        break;
                    case precedence_algorithm::precedence::less_relation:
                        value = L"<";
                        break;
                    case precedence_algorithm::precedence::greater_relation:
                        value = L">";
                        break;
                    case precedence_algorithm::precedence::unknown_relation:
                    default:
                        value = L"C"; // conflict
                        break;
                }

                content << alignment << std::setw(column_width) << std::setfill(L' ') << value;
            }
            else
            {
                content << alignment << std::setw(column_width) << std::setfill(L' ') << L' ';
            }
        }

        content << std::endl;
    }

    content << std::endl;

    return content.str();
}

END_NAMESPACE
