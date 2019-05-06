//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RECURSIVE_DESCENT_PARSER_H__
#define __RECURSIVE_DESCENT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeKind>
class recursive_descent_parser : private parser<Token, TreeKind>
{
    public:
        using token_type = typename parser<Token, TreeKind>::token_type;
        using tree_kind_type = typename parser<Token, TreeKind>::tree_kind_type;

        using lexical_analyzer_type = typename parser<token_type, tree_kind_type>::lexical_analyzer_type;

    public:
                        recursive_descent_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~recursive_descent_parser();
};

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_H__
