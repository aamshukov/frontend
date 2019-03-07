//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_INL__
#define __EARLEY_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪

template <typename T>
typename earley_parser<T>::item_type earley_parser<T>::create_item(const typename earley_parser<T>::rule_type& rule,
                                                                   const typename earley_parser<T>::chart_type& origin_chart,
                                                                   const typename earley_parser<T>::chart_type& master_chart,
                                                                   const typename earley_parser<T>::item_type& lptr,
                                                                   flags action)
{
    item_type result(factory::create<item>());

    (*result).id = static_cast<uint32_t>((*master_chart).items.size());

    (*result).rule = rule;
    (*result).dot = 0;

    (*result).origin_chart = origin_chart;
    (*result).master_chart = master_chart;

    (*result).lptr = lptr;

    (*result).flags = action;

    return result;
}

template <typename T>
void earley_parser<T>::set_rptr(typename earley_parser<T>::item_type& item, const typename earley_parser<T>::item_type& new_item)
{
    if(!std::any_of((*item).rptrs.begin(), (*item).rptrs.end(), [&new_item](const auto& item0){ return *item0 == *new_item; }))
    {
        (*item).rptrs.emplace(new_item);
    }
}

template <typename T>
bool earley_parser<T>::is_item_marked(typename const earley_parser<T>::item_type& item)
{
    return ((*item).flags & flags::marked) == flags::marked;
}

template <typename T>
void earley_parser<T>::mark_item(typename earley_parser<T>::item_type& item)
{
    (*item).flags |= flags::marked;
}

template <typename T>
void earley_parser<T>::unmark_item(typename earley_parser<T>::item_type& item)
{
    // crappy C++ syntax, need this hack for templates
    (*item).flags = ~static_cast<std::underlying_type_t<earley_parser<T>::flags>>(flags::marked);
}

template <typename T>
typename earley_parser<T>::chart_type earley_parser<T>::create_chart(uint32_t id)
{
    chart_type result(factory::create<chart>());

    (*result).id = id;

    return result;
}

template <typename T>
void earley_parser<T>::closure(const grammar& gr, typename earley_parser<T>::chart_type& chart)
{
    // 'λ case' - special case for empty rule, always assume the dot is at the end
    // Grune and Jacobs:
    //      The easiest way to handle this mare’s nest is to stay calm and keep running
    //      the Predictor and Completer in turn until neither has anything more to add.
    // for another method see predict()
    for(;;)
    {
        std::size_t count = (*chart).items.size();

        items_type items((*chart).items);

        for(auto item : items) // not const auto&
        {
            complete(item, chart);
            predict(gr, item, chart);
        }

        if((*chart).items.size() == count)
        {
            break;
        }
    }
}

template <typename T>
void earley_parser<T>::predict(const grammar& gr,
                               const typename earley_parser<T>::item_type& item,
                               typename earley_parser<T>::chart_type& chart)
{
    // Practical Earley Parsing, JOHN AYCOCK1 AND R. NIGEL HORSPOOL, 2001
    //      if [A -> ... • B ... , j] is in Si, add [B -> • α, i] to Si for all rules B -> α
    //      if B is nullable, also add [A -> ... B • ... , j] to Si
    const auto& rhs((*(*item).rule).rhs());

    if((*item).dot < rhs.size()) // dot is within rhs.size, not completed
    {
        const auto& symbol(rhs[(*item).dot]); // dot is within rhs.size

        if((*symbol).nonterminal())
        {
            auto nonterminal_it(gr.nt_rules().find((*symbol).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                item_type new_item(create_item(nonterminal_rule,   // production (rule)
                                               chart,              // original chart recognition started
                                               chart,              // chart to add to
                                               nullptr,            // l-ptr
                                               flags::predictor)); // action introduced this item
                (*chart).items.emplace(new_item);
            }

            // looks like in this implementation does not help
            //if((*symbol).nullable())
            //{
            //    item_type new_item(create_item((*item).rule,  // production (rule)
            //                       chart,                     // original chart recognition started
            //                       chart,                     // chart to add to
            //                       nullptr,                   // l-ptr
            //                       flags::predictor));        // action introduced this item
            //    (*chart).items.emplace(new_item);
            //}
        }
    }
}

template <typename T>
void earley_parser<T>::complete(typename earley_parser<T>::item_type& item, typename earley_parser<T>::chart_type& chart)
{
    const auto& rhs((*(*item).rule).rhs());

    if((*item).dot == rhs.size() || // is completed
      (*(*item).rule).empty())      // 'λ case' - special case for empty rule, always assume the dot is at the end
    {
        const auto& lhs((*(*item).rule).lhs());
        const auto& lhs_symbol(lhs[0]);

        const auto& origin_chart((*item).origin_chart); // j

        for(const auto& origin_item : (*origin_chart).items)
        {
            const auto& rhs_origin((*(*origin_item).rule).rhs());

            if((*origin_item).dot == rhs_origin.size()) // dot must be within rhs_origin.size
            {
                continue;
            }

            const auto& rhs_symbol(rhs_origin[(*origin_item).dot]);

            if(((*lhs_symbol).id() != (*rhs_symbol).id()))
            {
                continue;
            }

            item_type new_item(create_item((*origin_item).rule,           // production (rule)
                                           (*origin_item).origin_chart,   // original chart recognition started
                                           chart,                         // chart to add to
                                           origin_item,                   // l-ptr
                                           flags::completer));            // action introduced this item
            (*new_item).dot = (*origin_item).dot + 1; // move • over ... • B ... ==> ... B • ...

            auto result = (*chart).items.emplace(new_item);

            if(!result.second)
            {
                // exists, update <p>
                // <p>' = <p>' + [B -> γ •, j, l, <p>]
                set_rptr(const_cast<item_type&>(*result.first), item);
            }
            else
            {
                // does not exist, update new item
                // V[i] = V[i] + [new item, <[B -> γ •, j, l, <p>]>]
                set_rptr(new_item, item);
            }
        }
    }
}

template <typename T>
void earley_parser<T>::scan(typename earley_parser<T>::chart_type& chart,
                            typename earley_parser<T>::charts_type& charts,
                            typename const earley_parser<T>::token_type& token,
                            typename earley_parser<T>::chart_type& result)
{
    chart_type new_chart(create_chart(static_cast<uint32_t>(charts.size())));

    for(const auto& item : (*chart).items)
    {
        const auto& rhs((*(*item).rule).rhs());

        if((*item).dot < rhs.size()) // dot is within rhs.size
        {
            const auto& symbol(rhs[(*item).dot]);

            if((*symbol).terminal() && (*symbol).id() == static_cast<uint32_t>(token.type))
            {
                item_type new_item(create_item((*item).rule,         // production (rule)
                                               (*item).origin_chart, // original chart recognition started
                                               new_chart,            // chart to add to
                                               item,                 // l-ptr
                                               flags::scanner));     // action introduced this item

                (*new_item).dot = (*item).dot + 1; // move • over ... • a ... ==> ... a • ...

                (*new_chart).items.emplace(new_item);
            }
        }
    }

    if(!(*new_chart).items.empty())
    {
        (*new_chart).token = token;

        result.swap(new_chart);
        charts.emplace_back(result);
    }
}

template <typename T>
earley_parser<T>::earley_parser(const typename earley_parser<T>::lexical_analyzer_type& lexical_analyzer, grammar& gr, tree_kind kind)
                : parser<T>(lexical_analyzer), my_grammar(gr), my_tree_kind(kind)
{
}

template <typename T>
earley_parser<T>::~earley_parser()
{
}

template <typename T>
void earley_parser<T>::build_charts()
{
    // Адаптированный для построения деревьев вывода алгоритм Эрли, Владимир Анатольевич Лапшин.
    log_info(L"Building charts ...");

    // cache variables
    grammar& gr(my_grammar);
    lexical_analyzer_type& lexer(my_lexical_analyzer);

    // aux charts
    charts_type charts;

    // add V[0]
    chart_type chart0(create_chart(static_cast<uint32_t>(charts.size()))); // V[0]

    charts.emplace_back(chart0);

    // Добавляем в состояние V[0] все ситуации, где слева в правиле грамматики стоит стартовый нетерминал S,
    // с точкой в начале правой части правила и нулем как вторым элементом ситуации. Полагаем l = null и p = ∅.
    auto nonterminal_it(gr.nt_rules().find((*gr.start_symbol()).name()));
    const auto& nonterminal_rules((*nonterminal_it).second);

    for(const auto& nonterminal_rule : nonterminal_rules)
    {
        item_type new_item(create_item(nonterminal_rule, // production (rule)
                                       chart0,           // original chart recognition started
                                       chart0,           // chart to add to
                                       nullptr,          // l-ptr
                                       flags::init));    // action introduced this item
        (*chart0).items.emplace(new_item);
    }

    // Для каждой ситуации Эрли в состоянии Эрли V[0] вызываем процедуры Completer и Predictor.
    closure(gr, chart0);

    chart_type chart(chart0); // V[i-1] chart

    // Для каждого символа входной строки заполняем соответствующие состояния Эрли.
    for(;;)
    {
        // advance lexer
        (*lexer).next_lexeme();

        if((*lexer).is_eos())
        {
            if(validate_charts(chart))
            {
                status().custom_code() = status::custom_code::success;
                status().text().assign(L"Built charts successfully.\n");
            }
            else
            {
                status().custom_code() = status::custom_code::error;
                status().text().assign(L"Build charts failed.\n");
            }

            status().system_code() = ::GetLastError();
            status().text().append(status().get_system_error_message());

            break;
        }

        // Вызываем процедуру Scanner для состояния V[i-1].
        chart_type new_chart;

        scan(chart, charts, (*lexer).token(), new_chart);

        // Если список ситуаций Эрли в состоянии Эрли V[i] пуст, возвращаем False, parsing has failed.
        if(new_chart == nullptr || (*new_chart).items.empty())
        {
            status().custom_code() = status::custom_code::error;
            status().system_code() = ::GetLastError();
            status().text().assign(L"Invalid input, terminating parsing ...\n");
            status().text().append(status().get_system_error_message());

            break;
        }
                                                
        // Для каждой ситуации Эрли в состоянии Эрли V[i] вызываем процедуры Completer и Predictor.
        closure(gr, new_chart);

        chart = new_chart; // remember V[i-1] chart
    }

    if(status().custom_code() == status::custom_code::success)
    {
        my_charts.swap(charts);
        log_info(L"Succeeded.");
    }
    else if(status().custom_code() == status::custom_code::error)
    {
        log_info(L"Failed.");
    }

    log_info(L"Built charts.");
}

template <typename T>
void earley_parser<T>::build_parse_trees()
{
    log_info(L"Building parse tree(s) ...");

    trees_type trees;

    // the last built chart should have completed items
    const chart_type& chart(charts().back());

    // collect all completed items kind of [S -> α •, 0, ...] ...
    for(auto current_item : (*chart).items)
    {
        const auto& lhs((*(*current_item).rule).lhs());
        const auto& rhs((*(*current_item).rule).rhs());

        const auto& lhs_symbol(lhs[0]);

        // ... [S -> α •, 0, ...] and build tree(s)
        if((*lhs_symbol).id() == (*my_grammar.start_symbol()).id() && (*current_item).dot == rhs.size() && (*(*current_item).origin_chart).id == 0)
        {
            tree_type root(factory::create<earley_tree>());

            (*root).item = current_item;
            (*root).token = (*(*current_item).master_chart).token;

            trees.emplace_back(root);

            mark_item(current_item);
            build_parse_trees(current_item, root, root, trees);
            unmark_item(current_item);
        }
    }

    my_trees.swap(trees);

    log_info(L"Built parse tree(s).");
}

template <typename T>
void earley_parser<T>::populate_rhs_stack(const typename earley_parser<T>::item_type& item,
                                          typename earley_parser<T>::rhs_stack_type& stack)
{
    // Заполнение магазина ссылками на ситуации текущего уровня.
    for(auto current_item = item; (*current_item).dot > 0; current_item = (*current_item).lptr)
    {
log_info(earley_visualization<earley_parser<T>>::decorate_item(current_item).c_str()); //??

        rhs_stack_element element;

        const auto& rhs((*(*current_item).rule).rhs());
        const auto& symbol(rhs[(*current_item).dot - 1]);

        if((*symbol).nonterminal())
        {
            if((*current_item).rptrs.size() > 1)
            {
                element.data = (*current_item).rptrs;
                element.type = rhs_stack_element::element_type::rptrs;
            }
            else
            {
                element.data = *(*current_item).rptrs.begin();
                element.type = rhs_stack_element::element_type::item;
            }
        }
        else
        {
            element.data = symbol;
            element.type = rhs_stack_element::element_type::symbol;
        }

        stack.push(element);
    }
}

template <typename T>
void earley_parser<T>::clone_tree(const typename earley_parser<T>::tree_type& tree, typename earley_parser<T>::tree_type& result)
{
    struct queue_entry
    {
        tree_type org_node;
        tree_type new_papa;
    };

    tree_type new_tree(factory::create<earley_tree>());

    (*new_tree).papa = nullptr;
    (*new_tree).item = (*tree).item;
    (*new_tree).token = (*tree).token;

    std::queue<queue_entry> queue;

    queue.emplace(queue_entry { tree, nullptr });

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        auto new_node(factory::create<earley_tree>());

        (*new_node).papa = entry.new_papa;
        (*entry.new_papa).kids.emplace_back(new_node);
        (*new_node).item = (*entry.org_node).item;
        (*new_node).token = (*entry.org_node).token;

        for(auto kid : (*entry.org_node).kids)
        {
            queue.emplace(queue_entry { std::dynamic_pointer_cast<earley_tree>(kid), new_node });
        }
    }

    result.swap(new_tree);
}

// BuildTreeLevel([A -> X1...Xm •, i, l, <s>], P, Tr)
template <typename T>
void earley_parser<T>::build_parse_trees(typename earley_parser<T>::item_type& item,
                                         typename earley_parser<T>::tree_type& papa,
                                         typename earley_parser<T>::tree_type& tree,
                                         typename earley_parser<T>::trees_type& trees)
{
    // Заполнение магазина ссылками на ситуации текущего уровня.
    rhs_stack_type rhs_stack;

    tree_type src_tree;
    tree_type result;

    populate_rhs_stack(item, rhs_stack);

    // Положить в список pr пару (Tr, P) ...
    parse_tree_elements_type parse_roots; // список pr (parse roots), в котором содержатся деревья, созданные во время исполнения данного вызова процедуры

    parse_roots.push_back(parse_tree_element{ tree, papa });

    // ... и для каждого элемента магазина rhs выполнить шаг
    for(; !rhs_stack.empty(); rhs_stack.pop())
    {
        rhs_stack_element current_rhs_stack_element(rhs_stack.top());

        switch(current_rhs_stack_element.type)
        {
            // Если Xk - это терминальный символ ...
            case rhs_stack_element::element_type::symbol:
            {
                auto current_item(std::get<item_type>(current_rhs_stack_element.data));
                auto token((*(*current_item).master_chart).token);

                for(const auto& parse_root : parse_roots) // parse_tree_element = (Tr, P)
                {
                    auto new_node(factory::create<earley_tree>());

                    (*new_node).papa = parse_root.node;
                    (*parse_root.node).kids.emplace_back(new_node);
                    (*new_node).item = current_item;
                    (*new_node).token = token;
                }

                break;
            }
            // Если Xk - это нетерминальный символ и список <s> имеет единственный элемент ...
            case rhs_stack_element::element_type::item:
            {
                auto current_item(std::get<item_type>(current_rhs_stack_element.data));

                for(const auto& parse_root : parse_roots) // parse_tree_element = (Tr, P)
                {
                    auto new_node(factory::create<earley_tree>());

                    (*new_node).papa = parse_root.node;
                    (*parse_root.node).kids.emplace_back(new_node);
                    (*new_node).item = current_item;

                    mark_item(current_item);
                    build_parse_trees(current_item, new_node, tree, trees);
                    unmark_item(current_item);
                }

                break;
            }
            // Если Xk - это нетерминальный символ и список <s> имеет более одного элемента ...
            case rhs_stack_element::element_type::rptrs:
            {
                volatile bool handle_first_rptr = true; // если первый элемент списка <s> ...

                parse_tree_elements_type new_parse_roots; // список nn новых узлов, созданных на этапе обработки списка s данной ситуации

                const auto& rptr_items(std::get<items_type>(current_rhs_stack_element.data));

                for(const auto& rptr_item : rptr_items)
                {
                    if(!is_item_marked(rptr_item))
                    {
                        parse_tree_elements_type current_parse_roots; // список cn (current nodes) текущих нетерминальных узлов, подлежащих обработке с помощью процедуры build_parse_trees()

                        if(handle_first_rptr)
                        {
                            for(const auto& parse_root : parse_roots) // parse_tree_element = (Tr, P)
                            {
                                current_parse_roots.emplace_back(parse_root);
                            }

                            handle_first_rptr = false;
                        }
                        else
                        {
                            for(const auto& parse_root : parse_roots) // parse_tree_element = (Tr, P)
                            {
								parse_tree_element new_parse_root;

                                clone_tree(parse_root.tree, new_parse_root.tree);

								trees.emplace_back(new_parse_root.tree);

								current_parse_roots.push_back(new_parse_root);
								new_parse_roots.push_back(new_parse_root);
                            }
                        }

                        auto current_item(std::get<item_type>(current_rhs_stack_element.data));

                        for(const auto& parse_root : current_parse_roots) // parse_tree_element = (Tr, P)
                        {
                            auto new_node(factory::create<earley_tree>());

                            (*new_node).papa = parse_root.node;
                            (*parse_root.node).kids.emplace_back(new_node);
                            (*new_node).item = current_item;

                            mark_item(current_item);
                            build_parse_trees(current_item, new_node, tree, trees);
                            unmark_item(current_item);
                        }
                    }
                }

                for(const auto& parse_root : new_parse_roots)
                {
                    parse_roots.emplace_back(parse_root);
                }

                break;
            }
        }
    }
}

template <typename T>
void earley_parser<T>::build_ast()
{
    log_info(L"Building AST tree(s) ...");
    log_info(L"Built AST tree(s).");
}

template <typename T>
void earley_parser<T>::parse()
{
    log_info(L"Parsing ...");

    build_charts();

    if(status().custom_code() == status::custom_code::success)
    {
        if(my_tree_kind == tree_kind::build_trees)
        {
            build_parse_trees();
        }
        else if(my_tree_kind == tree_kind::build_ast)
        {
            build_ast();
        }

        if(status().custom_code() == status::custom_code::success)
        {
            log_info(L"Parsed.");
        }
    }
    else
    {
        log_info(L"Parse failed.");
    }
}

template <typename T>
bool earley_parser<T>::validate_charts(const typename earley_parser<T>::chart_type& chart)
{
    bool result = false;

    for(const auto& item : (*chart).items)
    {
        const auto& lhs((*(*item).rule).lhs());
        const auto& rhs((*(*item).rule).rhs());

        const auto& lhs_symbol(lhs[0]);

        result = (*lhs_symbol).id() == (*my_grammar.start_symbol()).id() && (*item).dot == rhs.size() && (*(*item).origin_chart).id == 0;

        if(result)
        {
            break;
        }
    }

    return result;
}

END_NAMESPACE

#endif // __EARLEY_PARSER_INL__


//log_info(earley_visualization<earley_parser<T>>::decorate_charts(charts).c_str()); //??
//earley_visualization<earley_parser<T>>::print_tree(trees, std::wcout); //??
