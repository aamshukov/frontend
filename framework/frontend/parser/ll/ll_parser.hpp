//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LL_PARSER_H__
#define __LL_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class ll_parser : private parser<T>
{
    public:
        using token_type = parser<T>::token_type;
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type;

    public:
                        ll_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~ll_parser();
};

END_NAMESPACE

#endif // __LL_PARSER_H__
