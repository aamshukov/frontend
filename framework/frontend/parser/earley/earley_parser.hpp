//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_H__
#define __EARLEY_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class earley_parser : private parser<T>
{
    public:
        using token_type = parser<T>::token_type;
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type;

    public:
                        earley_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~earley_parser();
};

END_NAMESPACE

#endif // __EARLEY_PARSER_H__
