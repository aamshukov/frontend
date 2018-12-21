//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_INL__
#define __EARLEY_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
earley_parser<T>::earley_parser(const typename earley_parser<T>::lexical_analyzer_type& lexical_analyzer)
                : parser<T>(lexical_analyzer)
{
}

template <typename T>
op_parser<T>::~op_parser()
{
}

END_NAMESPACE

#endif // __EARLEY_PARSER_INL__
