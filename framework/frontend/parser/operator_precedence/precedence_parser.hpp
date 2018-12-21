//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_PARSER_H__
#define __PRECEDENCE_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class precedence_parser : private parser<T>
{
    public:
        using token_type = parser<T>::token_type;
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type;

    public:
                        precedence_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~precedence_parser();
};

END_NAMESPACE

#endif // __PRECEDENCE_PARSER_H__
