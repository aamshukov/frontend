//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_H__
#define __PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class parser : private noncopyable
{
    public:
        using token_type = Token;
        using tree_traits_type = TreeTraits;

        using lexical_analyzer_type = std::shared_ptr<lexical_analyzer<token_type>>;

        using tree_type = std::shared_ptr<parser_tree<token_type, tree_traits_type>>;
        using trees_type = std::vector<tree_type>;

        using dag_type = std::shared_ptr<parser_dag<token_type, tree_traits_type>>;
        using dags_type = std::vector<dag_type>;

    protected:
        lexical_analyzer_type   my_lexical_analyzer;
        trees_type              my_trees;

        operation_status        my_status;

    protected:
        virtual void            parse() = 0;

    public:
                                parser(const lexical_analyzer_type& lexical_analyzer);
                               ~parser();

        const trees_type&       trees() const;

        const operation_status& status() const;
        operation_status&       status();
};

template <typename Token, typename TreeTraits>
const typename parser<Token, TreeTraits>::trees_type& parser<Token, TreeTraits>::trees() const
{
    return my_trees;
}

template <typename Token, typename TreeTraits>
const operation_status& parser<Token, TreeTraits>::status() const
{
    return my_status;
}

template <typename Token, typename TreeTraits>
operation_status& parser<Token, TreeTraits>::status()
{
    return my_status;
}

END_NAMESPACE

#endif // __PARSER_H__
