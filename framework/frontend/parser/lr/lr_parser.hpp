//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LR_PARSER_H__
#define __LR_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class lr_parser : private parser<T>
{
    public:
        using token_type = parser<T>::token_type;
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type;

    public:
                        lr_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~lr_parser();
};

END_NAMESPACE

#endif // __LR_PARSER_H__
