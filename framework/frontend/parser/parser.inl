//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_INL__
#define __PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
parser<Token, TreeTraits>::parser(const typename parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                         : my_lexical_analyzer(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
parser<Token, TreeTraits>::~parser()
{
}

END_NAMESPACE

#endif // __PARSER_INL__
