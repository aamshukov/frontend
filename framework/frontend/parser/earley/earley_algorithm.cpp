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

typename earley_algorithm::item_type earley_algorithm::add_item(const typename earley_algorithm::rule_type& rule,
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
    if(!std::any_of((*item).rptrs.begin(), (*item).rptrs.end(), [&rptr_item](const auto& rptr_item0){ return rptr_item0 == rptr_item; }))
    {
        (*item).rptrs.emplace_back(rptr_item);
    }
}

typename earley_algorithm::chart_type earley_algorithm::add_chart(uint32_t id)
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

void earley_algorithm::predict(typename earley_algorithm::chart_type& chart, typename earley_algorithm::charts_type& charts)
{
chart;charts; //??
    log_info(L"Running predictor ...");

    log_info(L"Done running predictor.");
}

void earley_algorithm::complete(typename earley_algorithm::chart_type& chart, typename earley_algorithm::charts_type& charts)
{
chart;charts; //??
    log_info(L"Running completer ...");

    log_info(L"Done running completer.");
}

void earley_algorithm::scan(typename earley_algorithm::chart_type& chart, typename earley_algorithm::charts_type& charts, typename earley_algorithm::chart_type& result)
{
chart;charts; //??
result;//??
    log_info(L"Running scanning ...");

    log_info(L"Done running scanning.");
}

void earley_algorithm::build_charts(typename earley_algorithm::charts_type& result)
{
result; //??
    log_info(L"Building charts ...");

    log_info(L"Built charts.");
}

void earley_algorithm::build_trees(typename earley_algorithm::charts_type& charts, typename earley_algorithm::trees_type& result)
{
result;charts; //??
    log_info(L"Building tree(s) ...");

    log_info(L"Built tree(s).");
}

END_NAMESPACE
