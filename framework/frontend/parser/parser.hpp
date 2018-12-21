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

        using trees_type = std::vector<tree>;

    protected:
        lexical_analyzer_type   my_lexical_analyzer;
        trees_type              my_trees;

    protected:
        virtual void            parse() = 0;

    public:
                                parser(const lexical_analyzer_type& lexical_analyzer);
                               ~parser();

        const trees_type&       trees() const;

        void                    result() const; //?? result of parsing
};

template <typename T>
const typename parser<T>::trees_type& parser<T>::trees() const
{
    return my_trees;
}

template <typename T>
void parser<T>::result() const //??
{
}

END_NAMESPACE

#endif // __PARSER_H__
