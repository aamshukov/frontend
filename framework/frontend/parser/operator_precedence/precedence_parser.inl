//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_PARSER_INL__
#define __PRECEDENCE_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeKind>
precedence_parser<Token, TreeKind>::precedence_parser(const typename precedence_parser<Token, TreeKind>::lexical_analyzer_type& lexical_analyzer)
                                  : parser<Token, TreeKind>(lexical_analyzer)
{
}

template <typename Token, typename TreeKind>
op_parser<Token, TreeKind>::~op_parser()
{
}

END_NAMESPACE

#endif // __PRECEDENCE_PARSER_INL__
