//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PACKRAT_PARSER_H__
#define __PACKRAT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class packrat_parser : private parser<Token, TreeTraits>
{
    public:
        using token_type = typename parser<Token, TreeTraits>::token_type;
        using tree_traits_type = typename parser<Token, TreeTraits>::tree_traits_type;

        using lexical_analyzer_type = typename parser<token_type, tree_traits_type>::lexical_analyzer_type;

    public:
                        packrat_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~packrat_parser();
};

END_NAMESPACE

#endif // __PACKRAT_PARSER_H__
