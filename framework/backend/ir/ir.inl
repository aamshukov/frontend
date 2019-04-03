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
void ir<T>::cst_to_ast(typename ir<T>::tree_type& cst)
{
    std::stack<tree_type> stack;
    std::queue<tree_type> queue;

    // collapse single parents
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

    while(!stack.empty())
    {
        auto node(stack.top());

        stack.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & parser_tree<token_type>::flags::deleted) == parser_tree<token_type>::flags::deleted)
        {
            continue;
        }

        // apply ! operator
        if(((*(*node).symbol).flags() & symbol::flags::not_in_ast) == symbol::flags::not_in_ast)
        {
            auto papa((*node).papa);

            if(papa != nullptr)
            {
                (*papa).kids.erase((std::remove((*papa).kids.begin(), (*papa).kids.end(), node)));
            }

            continue;
        }

        for(;;)
        {
            auto papa((*node).papa);

            if(papa == nullptr) // node is already root
            {
                break;
            }

            if((*papa).papa == nullptr) // papa is root, make node the root
            {
                break;
            }

            if((*papa).kids.size() == 1)
            {
                (*(*node).papa).flags |= parser_tree<token_type>::flags::deleted;

                (*(*papa).papa).kids.erase((std::remove((*(*papa).papa).kids.begin(), (*(*papa).papa).kids.end(), (*node).papa)));

                (*node).papa = (*papa).papa;
                (*(*papa).papa).kids.emplace_back(node);
            }
            else
            {
                break;
            }
        }
    }

    // apply ^ operator
    std::queue<tree_type> empty_queue;

    std::swap(queue, empty_queue);

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

    while(!stack.empty())
    {
        auto node(stack.top());

        stack.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & parser_tree<token_type>::flags::deleted) == parser_tree<token_type>::flags::deleted)
        {
            continue;
        }

        if(((*(*node).symbol).flags() & symbol::flags::root_in_ast) == symbol::flags::root_in_ast)
        {
            auto papa((*node).papa);

            if(papa == nullptr) // node is already root
            {
                continue;
            }

            (*(*node).papa).flags |= parser_tree<token_type>::flags::deleted;

            std::for_each((*(*node).papa).kids.begin(),
                          (*(*node).papa).kids.end(),
                          [&node](const auto& kid)
                          {
                              if(kid != node) (*node).kids.emplace_back(kid);
                          });

            std::for_each((*node).kids.begin(),
                          (*node).kids.end(),
                          [&node](const auto& kid)
                          {
                              (*kid).papa = node;;
                          });

            (*node).papa = (*papa).papa;

            if((*papa).papa != nullptr)
            {
                (*(*papa).papa).kids.erase((std::remove((*(*papa).papa).kids.begin(), (*(*papa).papa).kids.end(), papa)));
                (*(*papa).papa).kids.emplace_back(node);
            }
            else
            {
                cst = node;
            }
        }
    }
}

END_NAMESPACE

#endif // __IR_INL__
