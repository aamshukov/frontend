//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PACKRAT_PARSER_INL__
#define __PACKRAT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
packrat_parser<Token, TreeTraits>::packrat_parser(const typename packrat_parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                                 : parser<T>(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
packrat_parser<Token, TreeTraits>::~packrat_parser()
{
}

END_NAMESPACE

#endif // __PACKRAT_PARSER_INL__
