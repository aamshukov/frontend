//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_INL__
#define __EARLEY_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
earley_parser<T>::earley_parser(const typename earley_parser<T>::lexical_analyzer_type& lexical_analyzer)
                : parser<T>(lexical_analyzer)
{
}

template <typename T>
earley_parser<T>::~earley_parser()
{
}

template <typename T>
void earley_parser<T>::build_charts(typename earley_parser<T>::charts_type& result)
{
result;//??
    log_info(L"Building charts ...");

    log_info(L"Built charts.");
}

template <typename T>
void earley_parser<T>::build_trees(typename earley_parser<T>::charts_type& charts, typename earley_parser<T>::trees_type& result)
{
charts;//??
result;//??
    log_info(L"Building tree(s) ...");

    log_info(L"Built tree(s).");
}

template <typename T>
void earley_parser<T>::parse()
{
    build_charts(my_charts);
    build_trees(my_charts, my_trees);
}

END_NAMESPACE

#endif // __EARLEY_PARSER_INL__
