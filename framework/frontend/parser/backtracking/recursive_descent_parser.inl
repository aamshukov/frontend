//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RECURSIVE_DESCENT_PARSER_INL__
#define __RECURSIVE_DESCENT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
recursive_descent_parser<T>::recursive_descent_parser(const typename recursive_descent_parser<T>::lexical_analyzer_type& lexical_analyzer)
                           : parser<T>(lexical_analyzer)
{
}

template <typename T>
recursive_descent_parser<T>::~recursive_descent_parser()
{
}

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_INL__
