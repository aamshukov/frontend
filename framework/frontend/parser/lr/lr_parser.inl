//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LR_PARSER_INL__
#define __LR_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
lr_parser<T>::lr_parser(const typename lr_parser<T>::lexical_analyzer_type& lexical_analyzer)
            : parser<T>(lexical_analyzer)
{
}

template <typename T>
lr_parser<T>::~lr_parser()
{
}

END_NAMESPACE

#endif // __LR_PARSER_INL__
