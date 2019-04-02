//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_H__
#define __PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class parser : private noncopyable
{
    public:
        using token_type = T;
        using lexical_analyzer_type = std::shared_ptr<lexical_analyzer<token_type>>;

        using tree_type = std::shared_ptr<parser_tree<token_type>>;
        using trees_type = std::vector<tree_type>;

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

        static void             cst_to_ast(tree_type& tree); // convert cst to ast
};

template <typename T>
const typename parser<T>::trees_type& parser<T>::trees() const
{
    return my_trees;
}

template <typename T>
const operation_status& parser<T>::status() const
{
    return my_status;
}

template <typename T>
operation_status& parser<T>::status()
{
    return my_status;
}

END_NAMESPACE

#endif // __PARSER_H__
