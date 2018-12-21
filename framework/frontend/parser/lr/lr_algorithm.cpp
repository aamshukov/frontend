//..............................
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

// α β ε λ ∅ ∈ Σ ∪ δ

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

void lr_algorithm::build_lr_items_set(const grammar& gr,
                                      const typename lr_algorithm::symbols_type& symbols, // viable eprefix
                                      uint8_t k,
                                      typename lr_algorithm::lr_items_type& result)
{
    log_info(L"Building LR(k) items set for k = %d ...", k);

    // collect nonterminals
    std::vector<symbol_type> nonterminals;
    grammar_algorithm::collect_nonterminals(gr, nonterminals);

    // I. Vk(λ):
    lr_items_type items;

    // a) if S -> α is in P, add [S -> • α, λ] to Vk(λ)
    auto nonterminal_it(gr.nt_rules().find((*gr.start_symbol()).name()));
    const auto& nonterminal_rules((*nonterminal_it).second);

    for(const auto& nonterminal_rule : nonterminal_rules)
    {
        nonterminal_rule;

        lr_item_type item(factory::create<lr_item>());

        (*item).id = items.size();
        //(*item).production = ;
        (*item).dot_position = 0;
        //(*item).la = ;

        items.push_back(item);
    }

    // b) if [A -> • B α, u] is in Vk(λ) and B -> β is in P, then for each x in FIRSTk(αu)
    //    add [B -> • β, x] to Vk(λ), provided it is not already there

    // c) repeat step b) until no more new items can be added to Vk(λ)

    // II. Vk(X1 X2 ... Xi):
    // a) if [A -> α • Xi β, u] is in Vk(X1 ... Xi-1), add [A -> α Xi • β, u] to Vk(X1 ... Xi)

    // b) if [A -> α • B β, u] has been placed in Vk(X1 ... Xi) and B -> δ is in P,
    //    then add [B -> • δ, x] to Vk(X1 ... Xi) for each x in FIRSTk(βu), provided if not already there

    // c) repeat step 2b) until no more new items can be added to Vk(X1 ... Xi)



    lr_item_type item(factory::create<lr_item>());

    (*item).id = items.size();
    //(*item).production = ;
    (*item).dot_position = 0;
    //(*item).la = ;

    items.push_back(item);

    string_type symbols_decorated;

    for(const auto& symbol : symbols)
    {
        symbols_decorated += (*symbol).name() + L" ";
    }

    result.swap(items);

    log_info(L"LR(%s) items for (%d):", symbols_decorated.c_str(), k);
    log_info(L"%s", lr_visualization::decorate_lr_items(result).c_str());

    log_info(L"Built LR(k) items set for k = %d.", k);
}

void lr_algorithm::build_lr_items_canonical_collection(const grammar& gr,
                                                       uint8_t k,
                                                       typename lr_algorithm::lr_canonical_collection_type& result)
{
gr; //??
result;

    log_info(L"Building LR(k) canonical collection for k = %d ...", k);

    log_info(L"Built LR(k) canonical collection for k = %d.", k);
}

void lr_algorithm::build_lr_table(const grammar& gr,
                                  uint8_t k,
                                  typename lr_algorithm::lr_action_table_type& action_table,
                                  typename lr_algorithm::lr_goto_table_type& goto_table)
{
action_table;
goto_table;
    log_info(L"Building LR(k) table for k = %d ...", k);

    lr_canonical_collection_type canonical_collection;

    build_lr_items_canonical_collection(gr, k, canonical_collection);

    log_info(L"Built LR(k) table for k = %d.", k);
}

void lr_algorithm::build_lalr_table_yacc(const grammar& gr,
                                         uint8_t k,
                                         typename lr_algorithm::lr_action_table_type& action_table,
                                         typename lr_algorithm::lr_goto_table_type& goto_table)
{
gr;//??
k;
action_table;
goto_table;
}

void lr_algorithm::build_lalr_table_deremer(const grammar& gr,
                                            uint8_t k,
                                            typename lr_algorithm::lr_action_table_type& action_table,
                                            typename lr_algorithm::lr_goto_table_type& goto_table)
{
gr;//??
k;
action_table;
goto_table;
}

END_NAMESPACE
