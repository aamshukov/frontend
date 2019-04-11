//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_VISUALIZATION_INL__
#define __EARLEY_VISUALIZATION_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
string_type earley_visualization<T>::decorate_item(const typename earley_visualization<T>::item_type& item, bool recursive)
{
    string_type result;

    char_type buffer[64];

    snwprintf(buffer, array_size(buffer), TRUNCATE, L"%ld", (*item).id);

    result += L"[";
    result += buffer;
    result += L": ";

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

    result += L"-> ";

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

    result = text::trim(result, L" ");

    if((*item).dot == (*(*item).rule).rhs().size())
    {
        result += L" . , ";
    }
    else
    {
        result += L", ";
    }

    snwprintf(buffer, array_size(buffer), TRUNCATE, L"(%ld)", (*(*item).origin_chart).id);
    result += buffer;

    if(recursive)
    {
        result += L", ";
        if((*item).lptr)
        {
            //result += decorate_item((*item).lptr, false);
            snwprintf(buffer, array_size(buffer), TRUNCATE, L"%ld.%ld", (*(*(*item).lptr).master_chart).id, (*(*item).lptr).id);
            result += buffer;
        }
        else
        {
            result += L"null";
        }
        result += L", ";

        result += L"<";
        for(const auto& rptr : (*item).rptrs)
        {
            //result += decorate_item(rptr, false);
            snwprintf(buffer, array_size(buffer), TRUNCATE, L"%ld.%ld", (*(*rptr).master_chart).id, (*rptr).id);
            result += buffer;
            result += L", ";
        }
        result = text::trim(result, L" ");
        result = text::trim(result, L",");
        result += L">";
    }

    result += L", ";

    if(((*item).flags & earley_parser_type::flags::init) == earley_parser_type::flags::init)
    {
        result += L"i";
    }
    else if(((*item).flags & earley_parser_type::flags::completer) == earley_parser_type::flags::completer)
    {
        result += L"c";
    }
    else if(((*item).flags & earley_parser_type::flags::predictor) == earley_parser_type::flags::predictor)
    {
        result += L"p";
    }
    else if(((*item).flags & earley_parser_type::flags::scanner) == earley_parser_type::flags::scanner)
    {
        result += L"s";
    }

    result += L"]";

    if(recursive)
    {
        result += L"\n";
    }

    return result;
}

template <typename T>
string_type earley_visualization<T>::decorate_chart(const typename earley_visualization<T>::chart_type& chart)
{
    string_type result;

    std::wstringstream content;

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%d]", (*chart).id);

    content << buffer << std::endl;

    std::vector<item_type> items;

    std::for_each((*chart).items.begin(), (*chart).items.end(), [&items](const auto& item_kvp) { items.emplace_back(item_kvp); });
    std::sort(items.begin(), items.end(), [] (const auto& item1, const auto& item2) { return (*item1).id < (*item2).id; });

    std::for_each(items.begin(),
                  items.end(),
                  [&content](const auto& item)
                  {
                      content << decorate_item(item).c_str();
                  });

    result = content.str();
    result += L"\n";

    return result;
}

template <typename T>
string_type earley_visualization<T>::decorate_charts(const typename earley_visualization<T>::charts_type& charts)
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

END_NAMESPACE

#endif // __EARLEY_VISUALIZATION_INL__
