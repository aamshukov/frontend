//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_INL__
#define __IR_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename T>
void ir<T>::cst_to_ast(const typename ir<T>::tree_type& cst, typename ir<T>::tree_type& ast_result)
{
    tree_type ast(factory::create<parser_tree<token_type>>());

    std::stack<tree_type> stack;
    std::queue<tree_type> queue;

    // collect
    queue.emplace(cst);

    while(!queue.empty())
    {
        auto root(queue.front());

        queue.pop();

        stack.push(root);

        for(std::size_t k = 0, n = (*root).kids.size(); k < n; k++)
        {
            auto kid = (*root).kids[k];
            queue.emplace(std::dynamic_pointer_cast<parser_tree<token_type>>(kid));
        }
    }

    // process
    while(!stack.empty())
    {
        auto node(stack.top());

        stack.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & parser_tree<token_type>::flags::deleted) == parser_tree<token_type>::flags::deleted)
        {
            continue;
        }

        //?? auto new_node = factory::create<parser_tree<token_type>>();

        // collapse single parents


        if(((*(*node).symbol).flags() & symbol::flags::not_in_ast) == symbol::flags::not_in_ast)
        {
            // process '!' operator
        }
        else if(((*(*node).symbol).flags() & symbol::flags::root_in_ast) == symbol::flags::root_in_ast)
        {
            // process '^' operator
        }
    }

    ast_result.swap(ast);
}

END_NAMESPACE

#endif // __IR_INL__
