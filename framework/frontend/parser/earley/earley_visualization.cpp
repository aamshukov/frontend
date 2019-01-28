//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\factory.hpp>
#include <core\flags.hpp>
#include <core\text.hpp>

#include <core\tree.hpp>
#include <core\visitor.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\earley\earley_algorithm.hpp>
#include <frontend\parser\earley\\earley_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type earley_visualization::decorate_charts(const typename earley_visualization::charts_type& charts)
{
charts;//??
    string_type result;

    return result;
}

string_type earley_visualization::decorate_trees(const typename earley_visualization::trees_type& trees)
{
trees;//??
    string_type result;

    return result;
}

END_NAMESPACE
