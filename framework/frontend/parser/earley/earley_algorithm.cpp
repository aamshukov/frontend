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

#include <frontend\parser\earley\earley_algorithm.hpp>
#include <frontend\parser\earley\earley_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪

void earley_algorithm::build_core_items_table(const grammar& gr, typename earley_algorithm::core_items_type& result)
{
gr;//??
result; //??
    log_info(L"Building core items table ...");

    log_info(L"Built core items table.");
}

void earley_algorithm::build_prediction_table(const grammar& gr, typename earley_algorithm::core_items_set_type& result)
{
gr;//??
result; //??
    log_info(L"Building prediction table ...");

    log_info(L"Built prediction table.");
}

END_NAMESPACE
