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

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\ll\ll_algorithm.hpp>
#include <frontend\parser\ll\ll_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

void ll_visualization::decorate_strong_ll_table(const typename ll_visualization::strong_ll_table_type& table,
                                                uint8_t k,
                                                const typename ll_visualization::sets_type& table_la_sets,
                                                const std::vector<typename ll_visualization::symbol_type>& nonterminals)
{
    log_info(L"strong-LL(%d) table:", k);

    const std::size_t table_cell_width = 24;

    auto alignment = std::left;

    std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    for(const auto& la_set : table_la_sets)
    {
        std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L' ') << grammar_visualization::decorate_set(la_set, false).c_str();
    }

    std::cout << std::endl;

    for(std::size_t i = 0, m = table_la_sets.size(); i <= m; i++) // <=  for an extra column
    {
        std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L'-') << L'-';
    }

    std::cout << std::endl;

    for(std::size_t j = 0, n = nonterminals.size(); j < n; j++)
    {
        std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L' ') << (*nonterminals[j]).name().c_str();

        for(std::size_t i = 0, m = table_la_sets.size(); i < m; i++)
        {
            if(table[j][i].has_value())
            {
                const auto& rule(table[j][i]);

                std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L' ') << grammar_visualization::decorate_rule(rule.value(), false).c_str();
            }
            else
            {
                std::wcout << alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';
            }
        }

        std::cout << std::endl;
    }
}

void ll_visualization::decorate_ll_tal_tables(const typename ll_visualization::ll_tal_tables_type& tables, uint8_t k)
{
    log_info(L"LL(%d) T[A, L] TABLES:", k);

    for(const auto& table : tables)
    {
        decorate_ll_tal_table(table);
    }
}

string_type ll_visualization::decorate_ll_tal_table_key(const typename ll_visualization::ll_tal_table_key_type& key)
{
    std::wstringstream content;

    content <<
    std::endl <<
    format(L"T%d [ %s, %s]", std::get<0>(key), (*(std::get<1>(key))).name().c_str(), grammar_visualization::decorate_sets(std::get<2>(key)).c_str()).c_str() <<
    std::endl;

    return content.str();
}

void ll_visualization::decorate_ll_tal_table(const typename ll_visualization::ll_tal_table_type& table)
{
    const auto& key(table.first);
    const auto& rows(table.second);

    std::wstringstream content;

    content << decorate_ll_tal_table_key(key).c_str();

    for(const auto& row  : rows)
    {
        content << decorate_ll_tal_table_row(row).c_str();
    }

    log_info(L"%s", content.str().c_str());
}

string_type ll_visualization::decorate_ll_tal_table_row(const typename ll_visualization::ll_tal_table_row_type& row)
{
    std::wstringstream sets;

    for(const auto& kvp : row.follow)
    {
        sets << (((*kvp.first).name() + L":") + text::trim(grammar_visualization::decorate_sets(kvp.second), string_type(L" ")) + L", ").c_str();
    }

    if(row.follow.empty())
    {
        sets << L"---";
    }

    string_type sets_content(sets.str());
    sets_content = sets_content.substr(0, sets_content.size() - wcslen(L", "));

    std::wstringstream content;

    content <<
    std::left <<
    std::setfill(L' ') <<
    std::setw(12) <<
    grammar_visualization::decorate_set(row.lookahead).c_str() <<
    std::setw(32) <<
    grammar_visualization::decorate_rule(row.production, false).c_str() <<
    std::setw(128) <<
    sets_content.c_str() <<
    std::endl;

    return content.str();
}

string_type ll_visualization::decorate_ll_table_entry(const typename ll_visualization::ll_table_entry_type& entry)
{
    std::wstringstream content;

    for(const auto value : entry)
    {
        try
        {
            const auto& tal_table(std::get<ll_tal_table_type>(value));
            const auto& tal_table_key(tal_table.first);

            content <<
            std::left <<
            L"T" << std::get<0>(tal_table_key) << L" ";
            //L"T" << std::get<0>(tal_table_key) << L"[" << (*std::get<1>(tal_table_key)).name().c_str() << L"," <<
            //text::trim(decorate_sets(std::get<2>(tal_table_key), false), string_type(L" ")).c_str() << L"] ";
            continue;
        }
        catch(const std::bad_variant_access&)
        {
        }

        try
        {
            content <<
            std::left <<
            (*std::get<symbol_type>(value)).name().c_str() <<
            L" ";
        }
        catch(const std::bad_variant_access&)
        {
        }
    }

    return text::trim(content.str(), string_type(L" ")).c_str();
}

void ll_visualization::decorate_ll_table(const typename ll_visualization::ll_table_type& table,
                                              uint8_t k,
                                              const typename ll_visualization::sets_type& table_la_sets,
                                              const typename ll_visualization::ll_tal_tables_type& tal_tables)
{
    log_info(L"", k);
    log_info(L"L(%d) table:", k);

    const std::size_t table_cell_width = 56;

    auto alignment = std::left;

    std::wstringstream content;

    content <<
    std::endl <<
    alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    for(const auto& la_set : table_la_sets)
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << grammar_visualization::decorate_set(la_set, false).c_str();
    }

    content << std::endl;

    for(std::size_t i = 0, m = table_la_sets.size(); i <= m; i++) // <=  for an extra column
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L'-') << L'-';
    }

    content << std::endl;

    for(std::size_t j = 0, n = tal_tables.size(); j < n; j++)
    {
        std::wstringstream content0;

        content0 << L"T" << std::get<0>((tal_tables[j]).first);

        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << content0.str();

        for(std::size_t i = 0, m = table_la_sets.size(); i < m; i++)
        {
            if(table[j][i].has_value())
            {
                const auto& entry(table[j][i]);

                content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << decorate_ll_table_entry(entry.value()).c_str();
            }
            else
            {
                content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';
            }
        }

        content << std::endl;
    }

    log_info(L"%s", content.str().c_str());
}

END_NAMESPACE
