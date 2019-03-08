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

template <typename T>
void earley_visualization<T>::print_tree(const typename earley_visualization<T>::tree_type& tree, std::size_t level, std::wostream& stream)
{
    for(std::size_t i = 0; i < level; i++)
    {
        stream << "    ";
    }

    stream << (*(*tree).symbol).name() << std::endl;

    for(const auto& kid : (*tree).kids)
    {
        print_tree(std::dynamic_pointer_cast<earley_parser_type::earley_tree>(kid), level + 1, stream);
    }

    stream << std::endl;
}

template <typename T>
void earley_visualization<T>::print_tree(const typename earley_visualization<T>::trees_type& trees, std::wostream& stream)
{
    stream << std::endl;

    for(auto [k, tree] : enumerate(trees))
    {
        stream << L"[" << k << L"]:" << std::endl;
        print_tree(tree, 0, stream);
    }

    stream << std::endl;
}

template <typename T>
void earley_visualization<T>::collect_dot_labels(const typename earley_visualization<T>::tree_type& tree, std::size_t k, std::wostream& stream)
{
    stream << L"    " << k << L"[label=\"" << (*(*tree).symbol).name() << L"];" << std::endl;

    for(const auto& kid : (*tree).kids)
    {
        collect_dot_labels(std::dynamic_pointer_cast<earley_parser_type::earley_tree>(kid), k + 1, stream);
    }
}

template <typename T>
void earley_visualization<T>::decorate_tree(const typename earley_visualization<T>::tree_type& tree, std::wostream& stream)
{
    stream << (*(*tree).symbol).name();

    if(!(*tree).kids.empty())
    {
        stream << L" -> ";
    }

    for(const auto& kid : (*tree).kids)
    {
        decorate_tree(std::dynamic_pointer_cast<earley_parser_type::earley_tree>(kid), stream);
    }

    stream << L";" << std::endl;
}

template <typename T>
void earley_visualization<T>::decorate_trees(const typename earley_visualization<T>::trees_type& trees, const string_type& dot_file_name)
{
    // generate Graphviz dot file ...
    // for %i in (d:\tmp\earley.tree*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
    for(auto [k, tree] : enumerate(trees))
    {
        string_type file_name(dot_file_name + std::to_wstring(k) + L".dot");

        std::wofstream stream;

        try
        {
            stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

            if(!stream.is_open() || stream.bad() || stream.fail())
            {
                log_error(L"Failed to generate graphviz file %s: stream is either open or in a bad condition.", file_name.c_str());
            }

            stream << L"digraph EarleyTree" << std::endl;
            stream << L"{" << std::endl;

            collect_dot_labels(tree, 1, stream);
            decorate_tree(tree, stream);

            stream << L"}" << std::endl;

            stream.flush();
            stream.clear();
            stream.close();
        }
        catch(const std::exception& ex)
        {
            log_exception(ex, L"Failed to generate graphviz file %s: error occurred.", file_name.c_str());
        }
    }
}

END_NAMESPACE

#endif // __EARLEY_VISUALIZATION_INL__
