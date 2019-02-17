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
        using token_type = parser<T>::token_type; //??
        using lexical_analyzer_type = parser<T>::lexical_analyzer_type; //??

        using chart_type = earley_algorithm::chart_type;
        using charts_type = earley_algorithm::charts_type;

        using tree_type = earley_algorithm::tree_type;
        using trees_type = earley_algorithm::trees_type;

    private:
        charts_type             my_charts;   // list of sets/charts
        trees_type              my_trees;    // tree or list of trees

    public:
                                earley_parser(const lexical_analyzer_type& lexical_analyzer);
                               ~earley_parser();

        const charts_type&      charts() const;
        const trees_type&       trees() const;

        void                    build_charts();
        void                    build_trees();

        void                    parse() override;
};

template <typename T>
inline const typename earley_parser<T>::charts_type& earley_parser<T>::charts() const
{
    return my_charts;
}

template <typename T>
inline const typename earley_parser<T>::trees_type& earley_parser<T>::trees() const
{
    return my_trees;
}

END_NAMESPACE

#endif // __EARLEY_PARSER_H__
