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

template <typename T>
void parser<T>::cst_to_ast(typename parser<T>::tree_type& tree)
{
    struct queue_entry
    {
        tree_type node;
    };

    std::queue<queue_entry> queue;

    queue.emplace(queue_entry { tree });

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        for(std::size_t k = 0, n = (*entry.node).kids.size(); k < n; k++)
        {
            auto kid = (*entry.node).kids[k];
            queue.emplace(queue_entry { std::dynamic_pointer_cast<parser_tree<token_type>>(kid) });
        }
    }
}

END_NAMESPACE

#endif // __PARSER_INL__
