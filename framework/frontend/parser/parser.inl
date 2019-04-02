//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_INL__
#define __PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
parser<T>::parser(const typename parser<T>::lexical_analyzer_type& lexical_analyzer)
         : my_lexical_analyzer(lexical_analyzer)
{
}

template <typename T>
parser<T>::~parser()
{
}

END_NAMESPACE

#endif // __PARSER_INL__
