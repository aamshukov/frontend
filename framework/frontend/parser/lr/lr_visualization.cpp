//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\flags.hpp>
#include <core\text.hpp>
#include <core\enumerate.hpp>
#include <core\counter.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type lr_visualization::decorate_lr_item(const typename lr_visualization::lr_item_type& item)
{
    string_type result;

    result += L"[ ";

    for(const auto& sym : (*(*item).rule).lhs())
    {
        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += (*sym).name();

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += L" ";
    }

    result += L": ";

    for(const auto& [k, sym] : enumerate((*(*item).rule).rhs()))
    {
        if(k == (*item).dot)
        {
            result += L". ";
        }

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += (*sym).name();

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += L" ";
    }

    if((*item).dot == (*(*item).rule).rhs().size())
    {
        result += L". , ";
    }
    else
    {
        result += L", ";
    }

    result += grammar_visualization::decorate_sets((*item).la, false);

    result += L"]";
    result += L"\n";

    return result;
}

string_type lr_visualization::decorate_lr_items(const typename lr_visualization::lr_items_type& items)
{
    string_type result;

    std::wstringstream content;

    std::for_each(items.begin(),
                  items.end(),
                  [&content](const auto& item)
                  {
                      content << decorate_lr_item(item).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_transition(const typename lr_visualization::lr_transition_type& transition)
{
    string_type result;

    result += L"on " + (*(*transition).symbol).name() + L" -> ";

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%d]\n", (*(*transition).state).id);

    result += buffer;

    return result;
}

string_type lr_visualization::decorate_lr_transitions(const typename lr_visualization::lr_transitions_type& transitions)
{
    string_type result;

    std::wstringstream content;

    std::for_each(transitions.begin(),
                  transitions.end(),
                  [&content](const auto& transition_kvp)
                  {
                      content << decorate_lr_transition(transition_kvp.second).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_state(const lr_state_type& state)
{
    string_type result;

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%4d (%zd) %s] ", (*state).id, (*state).items.size(), (((*state).flags & flags::final) == flags::final) ? L"*" : L"");

    result += buffer;
    result += L"\n";

    result += decorate_lr_items((*state).items);
    result += decorate_lr_transitions((*state).transitions);

    result += L"\n";

    return result;
}

string_type lr_visualization::decorate_lr_states(const typename lr_visualization::lr_states_type& states)
{
    string_type result;

    std::wstringstream content;

    std::for_each(states.begin(),
                  states.end(),
                  [&content](const auto& state)
                  {
                      content << decorate_lr_state(state).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_action_table(const grammar& gr,
                                                       const typename lr_visualization::lr_states_type& states,
                                                       const typename lr_visualization::lr_action_table_type& action_table)
{
    string_type result;

    const std::size_t table_cell_width = 8;

    auto alignment = std::left;

    std::wstringstream content;

    content <<
    std::endl <<
    alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << (*pool_symb).name().c_str();
    }

    content << std::endl;

    for(const auto& _ : gr.pool())
    {
        _;
        content << alignment << std::setw(table_cell_width + 1) << std::setfill(L'-') << L"-";
    }

    content << std::endl;

    for(std::size_t i = 0, n = states.size(); i < n; i++) // i = state id
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << std::to_wstring(i);

        for(const auto& symb_kvp : gr.pool())
        {
            const auto& pool_symb(symb_kvp.second);

            string_type entry_text = L" ";

            const auto& key(std::make_pair((*pool_symb).id(), static_cast<uint32_t>(i)));

            const auto& it(action_table.find(key));
            
            if(it != action_table.end())
            {
                entry_text = std::to_wstring((*it).second);
            }

            content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << entry_text.c_str();
        }

        content << std::endl;
    }

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_goto_table(const grammar& gr,
                                                     const typename lr_visualization::lr_states_type& states,
                                                     const typename lr_visualization::lr_goto_table_type& goto_table)
{
    string_type result;

    const std::size_t table_cell_width = 8;

    auto alignment = std::left;

    std::wstringstream content;

    content <<
    std::endl <<
    alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << (*pool_symb).name().c_str();
    }

    content << std::endl;

    for(const auto& _ : gr.pool())
    {
        _;
        content << alignment << std::setw(table_cell_width + 1) << std::setfill(L'-') << L"-";
    }

    content << std::endl;

    for(std::size_t i = 0, n = states.size(); i < n; i++) // i = state id
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << std::to_wstring(i);

        for(const auto& symb_kvp : gr.pool())
        {
            const auto& pool_symb(symb_kvp.second);

            string_type entry_text = L" ";

            const auto& key(std::make_pair((*pool_symb).id(), static_cast<uint32_t>(i)));

            const auto& it(goto_table.find(key));
            
            if(it != goto_table.end())
            {
                entry_text = std::to_wstring((*it).second);
            }

            content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << entry_text.c_str();
        }

        content << std::endl;
    }

    result = content.str();

    return result;
}

END_NAMESPACE
