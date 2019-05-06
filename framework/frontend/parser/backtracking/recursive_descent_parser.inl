//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RECURSIVE_DESCENT_PARSER_INL__
#define __RECURSIVE_DESCENT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeKind>
recursive_descent_parser<Token, TreeKind>::recursive_descent_parser(const typename recursive_descent_parser<Token, TreeKind>::lexical_analyzer_type& lexical_analyzer)
                                         : parser<Token, TreeKind>(lexical_analyzer)
{
}

template <typename Token, typename TreeKind>
recursive_descent_parser<Token, TreeKind>::~recursive_descent_parser()
{
}

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_INL__
