//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_PARSER_INL__
#define __LL_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeKind>
ll_parser<Token, TreeKind>::ll_parser(const typename ll_parser<Token, TreeKind>::lexical_analyzer_type& lexical_analyzer)
                          : parser<T>(lexical_analyzer)
{
}

template <typename Token, typename TreeKind>
ll_parser<Token, TreeKind>::~ll_parser()
{
}

END_NAMESPACE

#endif // __LL_PARSER_INL__
