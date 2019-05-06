//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LR_PARSER_INL__
#define __LR_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
lr_parser<Token, TreeTraits>::lr_parser(const typename lr_parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                            : parser<Token, TreeTraits>(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
lr_parser<Token, TreeTraits>::~lr_parser()
{
}

END_NAMESPACE

#endif // __LR_PARSER_INL__
