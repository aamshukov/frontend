//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_PARSER_INL__
#define __LL_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
ll_parser<Token, TreeTraits>::ll_parser(const typename ll_parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                            : parser<T>(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
ll_parser<Token, TreeTraits>::~ll_parser()
{
}

END_NAMESPACE

#endif // __LL_PARSER_INL__
