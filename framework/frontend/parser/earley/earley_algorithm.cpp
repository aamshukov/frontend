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

// α β ε λ ∅ ∈ Σ ∪

bool earley_algorithm::items_equal(const typename earley_algorithm::item_type& lhs, const typename earley_algorithm::item_type& rhs)
{
    return *lhs == *rhs;
}

bool earley_algorithm::has_item(const typename earley_algorithm::items_type& items, const typename earley_algorithm::item_type& item)
{
    return std::find_if(items.begin(),
                        items.end(),
                        [&item](const typename earley_algorithm::item_type& item0)
                        {
                            return *item0 == *item;
                        }) != items.end();
}

typename earley_algorithm::item_type earley_algorithm::create_item(const typename earley_algorithm::rule_type& rule,
                                                                   const typename earley_algorithm::chart_type& origin_chart,
                                                                   const typename earley_algorithm::chart_type& master_chart,
                                                                   const typename earley_algorithm::item_type& lptr,
                                                                   earley_algorithm::flags action)
{
    item_type result(factory::create<item>());

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

void earley_algorithm::set_rptr(const typename earley_algorithm::item_type& rptr_item, typename earley_algorithm::item_type& item)
{
    if(!std::any_of((*item).rptrs.begin(), (*item).rptrs.end(), [&rptr_item](const auto& rptr_item0){ return *rptr_item0 == *rptr_item; }))
    {
        (*item).rptrs.emplace(rptr_item);
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

void earley_algorithm::closure(typename earley_algorithm::chart_type& chart)
{
chart;//??
}

void earley_algorithm::predict(const typename earley_algorithm::item_type& item, typename earley_algorithm::chart_type& chart)
{
chart;item; //??

    // PREDICTOR:
    //  if [A -> ... • B ..., j] is in S(i), add [B -> • α, i] to S(i) for all rules B -> α
    log_info(L"Running predictor ...");

    log_info(L"Done running predictor.");
}

void earley_algorithm::complete(const typename earley_algorithm::item_type& item, typename earley_algorithm::chart_type& chart)
{
chart;item; //??
    log_info(L"Running completer ...");

    log_info(L"Done running completer.");
}

void earley_algorithm::scan(typename earley_algorithm::chart_type& chart,
                            typename earley_algorithm::charts_type& charts,
                            uint32_t terminal_id,
                            typename earley_algorithm::chart_type& result)
{
    // SCANNER:
    //  if [A -> ... • a ..., j] is in S(i) and a = x(i)+1, add [A -> ... a • ..., j] to S(i+1)
    log_info(L"Running scanning ...");

    chart_type new_chart(create_chart(static_cast<uint32_t>(charts.size())));

    for(const auto& item : (*chart).items)
    {
        const auto& rhs((*(*item).rule).rhs());
        const auto& symbol(rhs[(*item).dot]); // item.CoreItem.Dot is inside rhs.size

        if((*symbol).terminal() && (*symbol).id() == terminal_id)
        {
            item_type new_item(create_item((*item).rule, (*item).origin_chart, new_chart, item, flags::scanner));

            (*chart).items.emplace(new_item);
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
