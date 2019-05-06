//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_PARSER_INL__
#define __PRECEDENCE_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
precedence_parser<Token, TreeTraits>::precedence_parser(const typename precedence_parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                                    : parser<Token, TreeTraits>(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
op_parser<Token, TreeTraits>::~op_parser()
{
}

END_NAMESPACE

#endif // __PRECEDENCE_PARSER_INL__
