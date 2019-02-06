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
#include <core\enumerate.hpp>
#include <core\text.hpp>

#include <core\tree.hpp>
#include <core\visitor.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\earley\earley_algorithm.hpp>
#include <frontend\parser\earley\\earley_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type earley_visualization::decorate_item(const typename earley_visualization::item_type& item)
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

    //result += grammar_visualization::decorate_sets((*item).la, false);

    result += L"]";
    result += L"\n";

    return result;
}

string_type earley_visualization::decorate_chart(const typename earley_visualization::chart_type& chart)
{
    string_type result;

    std::wstringstream content;

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%d]", (*chart).id);

    content << buffer << std::endl;

    std::for_each((*chart).items.begin(),
                  (*chart).items.end(),
                  [&content](const auto& item)
                  {
                      content << decorate_item(item).c_str();
                  });

    result = content.str();

    return result;
}

string_type earley_visualization::decorate_charts(const typename earley_visualization::charts_type& charts)
{
    string_type result;

    std::wstringstream content;

    std::for_each(charts.begin(),
                  charts.end(),
                  [&content](const auto& item)
                  {
                      content << decorate_chart(item).c_str();
                  });

    result = content.str();

    return result;
}

string_type earley_visualization::decorate_trees(const typename earley_visualization::trees_type& trees)
{
trees;//??
    string_type result;

    return result;
}

END_NAMESPACE
