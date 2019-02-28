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
#include <core\tree.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\earley\earley_algorithm.hpp>
#include <frontend\parser\earley\earley_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ ∉ Σ ∪ ⊕ γ

typename earley_algorithm::item_type earley_algorithm::create_item(const typename earley_algorithm::rule_type& rule,
                                                                   const typename earley_algorithm::chart_type& origin_chart,
                                                                   const typename earley_algorithm::chart_type& master_chart,
                                                                   const typename earley_algorithm::item_type& lptr,
                                                                   earley_algorithm::flags action)
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

bool earley_algorithm::is_final_item(const grammar& gr, const typename earley_algorithm::item_type& item)
{
    const auto& symbol((*(*item).rule).lhs()[0]);

    bool result = (*(*item).origin_chart).id == 0 &&
                  (*item).dot == (*(*item).rule).rhs().size() &&
                  (*symbol).id() == (*gr.start_symbol()).id();

    return result;
}

void earley_algorithm::set_rptr(typename earley_algorithm::item_type& item, const typename earley_algorithm::item_type& new_item)
{
    if(!std::any_of((*item).rptrs.begin(), (*item).rptrs.end(), [&new_item](const auto& item0){ return *item0 == *new_item; }))
    {
        (*item).rptrs.emplace(new_item);
    }
}

typename earley_algorithm::chart_type earley_algorithm::create_chart(uint32_t id)
{
    chart_type result(factory::create<chart>());

    (*result).id = id;

    return result;
}

bool earley_algorithm::is_final_chart(const grammar& gr, const typename earley_algorithm::chart_type& chart)
{
    bool result = std::any_of((*chart).items.begin(), (*chart).items.end(), [&gr](const auto& item){ return is_final_item(gr, item); });
    return result;
}

void earley_algorithm::closure(const grammar& gr, typename earley_algorithm::chart_type& chart)
{
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

void earley_algorithm::predict(const grammar& gr,
                               const typename earley_algorithm::item_type& item,
                               typename earley_algorithm::chart_type& chart)
{
    log_info(L"Running predictor ...");

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
                item_type new_item(earley_algorithm::create_item(nonterminal_rule,                     // production (rule)
                                                                 chart,                                // original chart recognition started
                                                                 chart,                                // chart to add to
                                                                 nullptr,                              // l-ptr
                                                                 earley_algorithm::flags::predictor)); // action introduced this item
                (*chart).items.emplace(new_item);
            }
        }
    }

    log_info(L"Done running predictor.");
}

void earley_algorithm::complete(typename earley_algorithm::item_type& item, typename earley_algorithm::chart_type& chart)
{
    log_info(L"Running completer ...");

    const auto& rhs((*(*item).rule).rhs());

    if((*item).dot == rhs.size()) // is completed
    {
        const auto& lhs((*(*item).rule).lhs());
        const auto& lhs_symbol(lhs[0]);

        const auto& origin_chart((*item).origin_chart); // j

        for(const auto& origin_item : (*origin_chart).items)
        {
            const auto& rhs_origin((*(*origin_item).rule).rhs());

            if((*origin_item).dot == rhs.size()) // dot must be within rhs.size
            {
                continue;
            }

            const auto& rhs_symbol(rhs_origin[(*origin_item).dot]);

            if((*lhs_symbol).id() != (*rhs_symbol).id())
            {
                continue;
            }

            item_type new_item(earley_algorithm::create_item((*origin_item).rule,                  // production (rule)
                                                             (*origin_item).origin_chart,          // original chart recognition started
                                                             chart,                                // chart to add to
                                                             origin_item,                          // l-ptr
                                                             earley_algorithm::flags::completer)); // action introduced this item
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

    log_info(L"Done running completer.");
}

void earley_algorithm::scan(typename earley_algorithm::chart_type& chart,
                            typename earley_algorithm::charts_type& charts,
                            uint32_t terminal,
                            typename earley_algorithm::chart_type& result)
{
    log_info(L"Running scanning ...");

    chart_type new_chart(create_chart(static_cast<uint32_t>(charts.size())));

    for(const auto& item : (*chart).items)
    {
        const auto& rhs((*(*item).rule).rhs());

        if((*item).dot < rhs.size()) // dot is within rhs.size
        {
            const auto& symbol(rhs[(*item).dot]);

            if((*symbol).terminal() && (*symbol).id() == terminal)
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
        result.swap(new_chart);
        charts.emplace_back(result);
    }

    log_info(L"Done running scanning.");
}

END_NAMESPACE
