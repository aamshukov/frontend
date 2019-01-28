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
