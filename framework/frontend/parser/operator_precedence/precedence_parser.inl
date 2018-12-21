//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PRECEDENCE_PARSER_INL__
#define __PRECEDENCE_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
precedence_parser<T>::precedence_parser(const typename precedence_parser<T>::lexical_analyzer_type& lexical_analyzer)
                    : parser<T>(lexical_analyzer)
{
}

template <typename T>
op_parser<T>::~op_parser()
{
}

END_NAMESPACE

#endif // __PRECEDENCE_PARSER_INL__
