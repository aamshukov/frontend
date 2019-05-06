//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_PARSER_H__
#define __PRECEDENCE_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class precedence_parser : private parser<Token, TreeTraits>
{
    public:
        using token_type = typename parser<Token, TreeTraits>::token_type;
        using tree_traits_type = typename parser<Token, TreeTraits>::tree_traits_type;

        using lexical_analyzer_type = typename parser<token_type, tree_traits_type>::lexical_analyzer_type;

    public:
                        precedence_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~precedence_parser();
};

END_NAMESPACE

#endif // __PRECEDENCE_PARSER_H__
