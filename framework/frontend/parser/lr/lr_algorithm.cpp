﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\factory.hpp>
#include <core\text.hpp>
#include <core\flags.hpp>
#include <core\counter.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪ δ γ

bool lr_algorithm::lr_items_equal(const typename lr_algorithm::lr_item_type& lhs, const typename lr_algorithm::lr_item_type& rhs)
{
    return lhs == rhs;
}

bool lr_algorithm::has_lr_item(const typename lr_algorithm::lr_items_type& items, const typename lr_algorithm::lr_item_type& item)
{
    return std::find_if(items.begin(),
                        items.end(),
                        [&item](const typename lr_algorithm::lr_item_type& item0)
                        {
                            return *item0 == *item;
                        }) != items.end();
}

bool lr_algorithm::lr_states_equal(const typename lr_algorithm::lr_state_type& lhs, const typename lr_algorithm::lr_state_type& rhs)
{
    return lhs == rhs;
}

bool lr_algorithm::has_lr_state(const typename lr_algorithm::lr_states_type& states, const typename lr_algorithm::lr_state_type& state)
{
    return std::find_if(states.begin(),
                        states.end(),
                        [&state](const typename lr_algorithm::lr_state_type& state0)
                        {
                            return *state0 == *state;
                        }) != states.end();
}

void lr_algorithm::calculate_lr_closure(const grammar& gr, uint8_t k, const typename lr_algorithm::lr_state_type& state)
{
    // Based on Aho.A.V Lam.M.S Sethi.R Ullman.J.D Compilers Principles Techniques And Tools (2ed), p.261
    // SetOfltems CLOSURE(I)
    //     repeat
    //         for(each item [A -> α • B β, u] in I)
    //             for(each production B -> γ in G')
    //                 for(each x in FIRSTk(βu))
    //                     add [B -> • γ, x] to set I
    //     until no more items are added to I
    //     return I

    // ... repeat ...
    for(;;)
    {
        lr_items_type new_items;

        // ... for(each item [A -> α • B β, u] in I) ...
        for(const auto& item : (*state).items)
        {
            const auto& production((*item).rule);

            if(!((*item).dot < (*production).rhs().size())) // consider only non-completed items ... α • B β ...
            {
                continue;
            }

            const auto& current_symbol((*production).rhs()[(*item).dot]);

            // ... • B ...
            if(!(*current_symbol).nonterminal())
            {
                continue;
            }

            // calculate FIRSTk(βu)
            symbols_type first_set_symbols;

            std::for_each((*production).rhs().begin() + (*item).dot + 1, (*production).rhs().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });
            //                                              •         B, +1 for 'over B'

            if(!(*item).la.empty())
            {
                std::for_each((*item).la.front().begin(), (*item).la.front().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });
            }

            sets_type first_set;

            grammar_algorithm::build_first_set(first_set_symbols, k, first_set);

            // ... add [B -> • γ, x] to set I ...
            auto nonterminal_it(gr.nt_rules().find((*current_symbol).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                for(const auto& set : first_set)
                {
                    lr_item_type new_item(factory::create<lr_item>());

                    (*new_item).rule = nonterminal_rule;
                    (*new_item).dot = 0;

                    (*new_item).la.emplace_back(set);

                    if(!has_lr_item(new_items, new_item)) // ... provided it is not already there ...
                    {
                        new_items.emplace_back(new_item);
                    }
                }
            }
        }

        // ... until no more items are added to I ...
        if(!new_items.empty())
        {
            std::size_t count = (*state).items.size();

            std::copy_if(new_items.begin(),
                         new_items.end(),
                         std::back_inserter((*state).items),
                         [&state](const auto& new_item)
                         {
                             return !has_lr_item((*state).items, new_item);
                         });

            if(count == (*state).items.size())
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

typename lr_algorithm::lr_state_type lr_algorithm::calculate_lr_goto(const grammar& gr,
                                                                     uint8_t k,
                                                                     const typename lr_algorithm::lr_state_type& state, // I
                                                                     const typename lr_algorithm::symbol_type& symb)    // X
{
    // Based on Aho.A.V Lam.M.S Sethi.R Ullman.J.D Compilers Principles Techniques And Tools (2ed), p.261
    // SetOfltems GOTO(I, X)
    //     initialize  J to be the empty set
    //     for(each item [A -> α • X β, u] in I)
    //         add [A -> α X • β, u] to set J
    //     return CLOSURE(J)

    // ... initialize  J to be the empty set ...
    lr_state_type new_state(factory::create<lr_state>());

    (*new_state).id = 0;
    (*new_state).flags = flags::nothing;

    lr_items_type new_items;

    // ... for(each item [A -> α • X β, u] in I) ...
    for(const auto& item : (*state).items)
    {
        const auto& production((*item).rule);

        if(!((*item).dot < (*production).rhs().size())) // consider only non-completed items ... α • X β ...
        {
            continue;
        }

        if((*(*production).rhs()[(*item).dot]).id() != (*symb).id()) // ... and Xi must be equal to X
        {
            continue;
        }

        lr_item_type new_item(factory::create<lr_item>());

        (*new_item).rule = (*item).rule;
        (*new_item).dot = (*item).dot + 1;

        (*new_item).la = ((*item).la);

        if(!has_lr_item(new_items, new_item)) // ... provided it is not already there ...
        {
            new_items.emplace_back(new_item);
        }
    }

    if(!new_items.empty())
    {
        std::copy_if(new_items.begin(),
                     new_items.end(),
                     std::back_inserter((*new_state).items),
                     [&new_state](const auto& new_item)
                     {
                         return !has_lr_item((*new_state).items, new_item);
                     });
    }

    // ... return CLOSURE(J) ...
    calculate_lr_closure(gr, k, new_state);

    // get result
    lr_state_type result;

    result.swap(new_state);

    return result;
}

void lr_algorithm::build_lr_automaton(const grammar& gr, uint8_t k, typename lr_algorithm::lr_states_type& result)
{
    // Based on Aho.A.V Lam.M.S Sethi.R Ullman.J.D Compilers Principles Techniques And Tools (2ed), p.261
    // initialize C to CLOSURE( { [S' -> • S, $ ] } )
    // repeat
    //     for(each set of items I in C)
    //         for(each grammar symbol X)
    //             if(GOTO(I, X) is not empty and not in C)
    //                 add GOTO(I, X) to C
    // until no new sets of items are added to C
    log_info(L"Building LR(%d) automaton ...", k);

    lr_states_type states;

    // ... initialize C to CLOSURE( { [S' -> • S, $ ] } ) ...
    {
        lr_state_type state(factory::create<lr_state>());

        (*state).id = 0;
        (*state).flags = flags::nothing;

        auto nonterminal_it(gr.nt_rules().find((*gr.start_symbol()).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            nonterminal_rule;

            lr_item_type item(factory::create<lr_item>());

            (*item).rule = nonterminal_rule;
            (*item).dot = 0;

            (*item).la.emplace_back(set_type { symbol::epsilon });

            if(!has_lr_item((*state).items, item)) // could be optimized by checking prior factory::create ...
            {
                (*state).items.emplace_back(item);
            }
        }

        calculate_lr_closure(gr, k, state);

        states.emplace_back(state);
    }

    // build automaton
    for(;;)
    {
        lr_states_type new_states;

        for(const auto& state : states)
        {
            if(((*state).flags & flags::marked) == flags::marked)
            {
                continue;
            }

            (*state).flags = modify_flags<flags>((*state).flags, flags::marked, flags::nothing);

            for(const auto& symb_kvp : gr.pool())
            {
                const auto& symb(symb_kvp.second);

                if((*symb).id() == (*symbol::epsilon).id())
                {
                    continue; // λ is not counting
                }

                // add state
                auto new_state(calculate_lr_goto(gr, k, state, symb));

                if((*new_state).items.empty())
                {
                    continue;
                }

                const auto& it(std::find_if(states.begin(),
                                            states.end(),
                                            [&new_state](const typename lr_algorithm::lr_state_type& state0)
                                            {
                                                return *state0 == *new_state;
                                            }));
                if(it != states.end())
                {
                    new_state = *it;
                }
                else
                {
                    (*new_state).id = static_cast<uint32_t>(states.size());
                    new_states.emplace_back(new_state);
                }

                // add transition
                lr_transition_type transition(factory::create<lr_transition>());

                (*transition).state = new_state;
                (*transition).symbol = symb;

                (*state).transitions.insert(lr_transitions_type::value_type(symb, transition));
            }
        }

        if(!new_states.empty())
        {
            std::size_t count = states.size();

            std::copy_if(new_states.begin(),
                         new_states.end(),
                         std::back_inserter(states),
                         [&states](const auto& new_state)
                         {
                             return !has_lr_state(states, new_state);
                         });

            if(count == states.size())
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    // set final sets
    for(const auto& state : states)
    {
        for(const auto& item : (*state).items)
        {
            const auto& production((*item).rule);

            if((*item).dot == (*production).rhs().size()) // consider only completed items ... α B β • ...
            {
                // marked as final state
                (*state).flags |= flags::final;
                break;
            }
        }
    }

    // renumber
    for(uint32_t i = 0; i < static_cast<uint32_t>(states.size()); i++)
    {
        (*states[i]).id = i;
    }

    // get result
    result.swap(states);

    // visualize
    log_info(L"LR(%d) automaton:", k);
    log_info(L"%s", lr_visualization::decorate_lr_states(result).c_str());

    log_info(L"Built LR(%d) automaton.", k);
}

void lr_algorithm::build_goto_table(const grammar& gr, const typename lr_algorithm::lr_states_type& states, typename lr_algorithm::lr_goto_table_type& result)
{
    log_info(L"Building GOTO table ...");

    lr_goto_table_type goto_table;

    // build table
    for(const auto& state : states)
    {
        for(const auto& symb_kvp : gr.pool())
        {
            const auto& symb(symb_kvp.second);

            const auto& transitions((*state).transitions);
            const auto& transition_it(transitions.find(symb));

            if(transition_it != (*state).transitions.end())
            {
                auto key(std::make_pair((*symb).id(), (*state).id));

                if(goto_table.find(key) == goto_table.end())
                {
                    goto_table.emplace(lr_goto_table_type::value_type(key, (*(*(*transition_it).second).state).id));
                }
            }
        }
    }

    // get result
    result.swap(goto_table);

    // visualize
    log_info(L"GOTO table:");
    log_info(L"%s", lr_visualization::decorate_lr_goto_table(gr, states, result).c_str());

    log_info(L"Built GOTO table .");
}

void lr_algorithm::build_lr_table(const grammar& gr,
                                  uint8_t k,
                                  typename lr_algorithm::lr_action_table_type& action_table,
                                  typename lr_algorithm::lr_goto_table_type& goto_table)
{
action_table;
    log_info(L"Building LR(k) table for k = %d ...", k);

    lr_states_type automaton;

    build_lr_automaton(gr, k, automaton);

    build_goto_table(gr, automaton, goto_table);


    lr_canonical_collection_type canonical_collection;

    build_lr_canonical_collection(gr, k, canonical_collection);

    log_info(L"Built LR(k) table for k = %d.", k);
}
































void lr_algorithm::build_lr_items_set_for_viable_prefix(const grammar& gr,
                                                        const typename lr_algorithm::symbols_type& symbols, // viable prefix
                                                        uint8_t k,
                                                        typename lr_algorithm::lr_items_type& result)
{
    // Based on AU, Russian edition, page 437, algorithm 5.8
    log_info(L"Building LR(k) items set for k = %d ...", k);

    // I. calculate Vk(λ):
    lr_items_type closure_items; // // V(X1...Xi-1) and eventually holds result

    // a) if S -> α is in P, add [S -> • α, λ] to Vk(λ)
    {
        auto nonterminal_it(gr.nt_rules().find((*gr.start_symbol()).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            nonterminal_rule;

            lr_item_type item(factory::create<lr_item>());

            (*item).rule = nonterminal_rule;
            (*item).dot = 0;

            (*item).la.emplace_back(set_type { symbol::epsilon });

            if(!has_lr_item(closure_items, item)) // could be optimized by checking prior factory::create ...
            {
                closure_items.emplace_back(item);
            }
        }
    }

    // b) if [A -> • B α, u] is in Vk(λ) and B -> β is in P, then for each x in FIRSTk(αu)
    //    add [B -> • β, x] to Vk(λ), provided it is not already there
    for(;;)
    {
        lr_items_type new_items;

        // .. if [A -> • B α, u] is in Vk(λ) ...
        for(const auto& item : closure_items)
        {
            if((*item).dot == 0) // consider only items where dot is in front ... A -> • B α, u ...
            {
                const auto& production((*item).rule);
                const auto& first_symbol((*production).rhs()[0]);

                // ... • B ...
                if((*first_symbol).nonterminal())
                {
                    // calculate FIRSTk(αu)
                    symbols_type first_set_symbols;

                    std::for_each((*production).rhs().begin() + 1, (*production).rhs().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });

                    if(!(*item).la.empty())
                    {
                        std::for_each((*item).la.front().begin(), (*item).la.front().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });
                    }

                    sets_type first_set;

                    grammar_algorithm::build_first_set(first_set_symbols, k, first_set);

                    // ... then for each x in FIRSTk(αu) add [B -> • β, x] to Vk(λ) ...
                    auto nonterminal_it(gr.nt_rules().find((*first_symbol).name()));
                    const auto& nonterminal_rules((*nonterminal_it).second);

                    for(const auto& nonterminal_rule : nonterminal_rules)
                    {
                        for(const auto& set : first_set)
                        {
                            lr_item_type new_item(factory::create<lr_item>());

                            (*new_item).rule = nonterminal_rule;
                            (*new_item).dot = 0;

                            (*new_item).la.emplace_back(set);

                            if(!has_lr_item(new_items, new_item)) // ... provided it is not already there ...
                            {
                                new_items.emplace_back(new_item);
                            }
                        }
                    }
                }
            }
        }

        // c) repeat step b) until no more new items can be added to Vk(λ)
        if(!new_items.empty())
        {
            std::size_t count = closure_items.size();

            std::copy_if(new_items.begin(),
                         new_items.end(),
                         std::back_inserter(closure_items),
                         [&closure_items](const auto& new_item)
                         {
                             return !has_lr_item(closure_items, new_item);
                         });

            if(count == closure_items.size())
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    // II. calculate Vk(X1 X2 ... Xi):
    for(const auto& pre_dot_symbol : symbols) // iterate over viable prefix, this step is really independent of X1 ... Xi-1 depending only on the Vk(X1 ... Xi-1)
    {
        lr_items_type new_closure_items; // V(X1...Xi)

        // a) if [A -> α • Xi β, u] is in Vk(X1 ... Xi-1), add [A -> α Xi • β, u] to Vk(X1 ... Xi)
        for(const auto& item : closure_items)
        {
            if((*item).dot < (*(*item).rule).rhs().size() &&
               (*pre_dot_symbol).id() == (*(*(*item).rule).rhs()[(*item).dot]).id()) // ... only consider dot at • Xi β ... and Xi must be equal to pre_dot_symbol
            {
                lr_item_type new_item(factory::create<lr_item>());

                (*new_item).rule = (*item).rule;
                (*new_item).dot = (*item).dot + 1;

                (*new_item).la = ((*item).la);

                if(!has_lr_item(new_closure_items, new_item)) // ... provided it is not already there ...
                {
                    new_closure_items.emplace_back(new_item);
                }
            }
        }

        // b) if [A -> α • B β, u] has been placed in Vk(X1 ... Xi) and B -> δ is in P,
        //    then add [B -> • δ, x] to Vk(X1 ... Xi) for each x in FIRSTk(βu), provided if not already there
        for(;;)
        {
            lr_items_type new_items;

            for(const auto& item : new_closure_items)
            {
                const auto& production((*item).rule);

                if((*item).dot < (*production).rhs().size()) // consider only non-completed items ... α • B β ...
                {
                    const auto& current_symbol((*production).rhs()[(*item).dot]);

                    // ... • B ...
                    if((*current_symbol).nonterminal())
                    {
                        // calculate FIRSTk(βu)
                        symbols_type first_set_symbols;

                        std::for_each((*production).rhs().begin() + (*item).dot + 1, (*production).rhs().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });
                        //                                              •         B, +1 for 'over B'

                        if(!(*item).la.empty())
                        {
                            std::for_each((*item).la.front().begin(), (*item).la.front().end(), [&first_set_symbols](const auto& symb){ first_set_symbols.emplace_back(symb); });
                        }

                        sets_type first_set;

                        grammar_algorithm::build_first_set(first_set_symbols, k, first_set);

                        // ... then for each x in FIRSTk(βu) add [B -> • δ, x] to Vk(X1...Xi) ...
                        auto nonterminal_it(gr.nt_rules().find((*current_symbol).name()));
                        const auto& nonterminal_rules((*nonterminal_it).second);

                        for(const auto& nonterminal_rule : nonterminal_rules)
                        {
                            for(const auto& set : first_set)
                            {
                                lr_item_type new_item(factory::create<lr_item>());

                                (*new_item).rule = nonterminal_rule;
                                (*new_item).dot = 0;

                                (*new_item).la.emplace_back(set);

                                if(!has_lr_item(new_items, new_item)) // ... provided it is not already there ...
                                {
                                    new_items.emplace_back(new_item);
                                }
                            }
                        }
                    }
                }
            }

            // c) repeat step 2b) until no more new items can be added to Vk(X1 ... Xi)
            if(!new_items.empty())
            {
                std::size_t count = new_closure_items.size();

                std::copy_if(new_items.begin(),
                             new_items.end(),
                             std::back_inserter(new_closure_items),
                             [&new_closure_items](const auto& new_item)
                             {
                                 return !has_lr_item(new_closure_items, new_item);
                             });

                if(count == new_closure_items.size())
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        closure_items = new_closure_items;
    }

    // get result
    result.swap(closure_items);

    // visualize
    string_type symbols_decorated;

    for(const auto& symbol : symbols)
    {
        symbols_decorated += (*symbol).name() + L" ";
    }

    symbols_decorated = text::trim(symbols_decorated, L" ");

    log_info(L"LR(%d) items for viable prefix '%s':", k, symbols_decorated.c_str());
    log_info(L"%s", lr_visualization::decorate_lr_items(result).c_str());

    log_info(L"Built LR(k) items set for k = %d.", k);
}


void lr_algorithm::build_lr_canonical_collection(const grammar& gr, uint8_t k, typename lr_algorithm::lr_canonical_collection_type& result)
{
gr; //??
result;
    // Based on Aho.A.V Lam.M.S Sethi.R Ullman.J.D Compilers Principles Techniques And Tools (2ed), p.261
    // initialize C to CLOSURE( { [S' -> • S, $ ] } )
    // repeat
    //     for(each set of items I in C)
    //         for(each grammar symbol X)
    //             if(GOTO(I, X) is not empty and not in C)
    //                 add GOTO(I, X) to C
    // until no new sets of items are added to C
    log_info(L"Building LR(k) canonical collection for k = %d ...", k);


    // ... add GOTO(I, X) to C ... also cache in goto_table


    log_info(L"Built LR(k) canonical collection for k = %d.", k);
}

END_NAMESPACE




                //log_info(L"%s", lr_visualization::decorate_lr_items(new_closure_items).c_str()); //??
                //log_info(L"%s", lr_visualization::decorate_lr_item(new_item).c_str());


                    //??string_type first_set_str;
                    //??std::for_each(first_set_symbols.begin(), first_set_symbols.end(), [&first_set_str](const auto& symb){ first_set_str += (*symb).name(); });
                    //??log_info(L"FIRST(%s) = %s", first_set_str.c_str(), grammar_visualization::decorate_sets(first_set).c_str());
