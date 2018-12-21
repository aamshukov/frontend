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

    char_type buffer[64];

    for(const auto& sym : (*(*item).production).lhs())
    {
        snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%4zd] ", (*item).id);

        result += buffer;

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

    for(const auto& [k, sym] : enumerate((*(*item).production).rhs()))
    {
        if(k == (*item).dot_position)
        {
            result += L" . ";
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

    if((*item).dot_position == (*(*item).production).rhs().size())
    {
        result += L" .";
    }

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

    content << std::endl;

    result = content.str();

    return result;
}

END_NAMESPACE
