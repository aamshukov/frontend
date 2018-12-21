//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RECURSIVE_DESCENT_PARSER_H__
#define __RECURSIVE_DESCENT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class recursive_descent_parser : private parser<T>
{
    public:
        using token_type = parser<T>::token_type;
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type;

    public:
                        recursive_descent_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~recursive_descent_parser();
};

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_H__
