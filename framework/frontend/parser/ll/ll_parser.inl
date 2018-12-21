//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_PARSER_INL__
#define __LL_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
ll_parser<T>::ll_parser(const typename ll_parser<T>::lexical_analyzer_type& lexical_analyzer)
            : parser<T>(lexical_analyzer)
{
}

template <typename T>
ll_parser<T>::~ll_parser()
{
}

END_NAMESPACE

#endif // __LL_PARSER_INL__
