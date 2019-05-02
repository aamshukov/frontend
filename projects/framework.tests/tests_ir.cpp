//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>
#include <core\counter.hpp>
#include <core\factory.hpp>
#include <core\singleton.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>
#include <core\command_line.hpp>
#include <core\configurator.hpp>

#include <core\tree.hpp>
#include <core\dag.hpp>

#include <core\visitor.hpp>
#include <core\visitable.hpp>

#include <frontend\grammar\token.hpp>
#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\fsa\fsa_transition.hpp>
#include <frontend\fsa\fsa_state.hpp>
#include <frontend\fsa\fsa.hpp>
#include <frontend\fsa\fsa_re.hpp>
#include <frontend\fsa\fsa_state_set.hpp>
#include <frontend\fsa\fsa_algorithm.hpp>

#include <frontend\fsa\fsa_visualization.hpp>
#include <frontend\fsa\fsa_codegen.hpp>

#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>

#include <core\diagnostics.hpp>
#include <core\statistics.hpp>

#include <frontend\lexical_analyzer\lexical_analyzer.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\parser_tree.hpp>
#include <frontend\parser\parser_dag.hpp>

#include <frontend\parser\parser.hpp>
#include <frontend\parser\parser.inl>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>
#include <frontend\parser\lr\lr_parser.hpp>
#include <frontend\parser\lr\lr_parser.inl>

#include <backend\symtable\symtab_symbol.hpp>
#include <backend\symtable\symbol_table.hpp>
#include <backend\symtable\symbol_table.hpp>

#include <backend\ir\quadruple.hpp>
#include <backend\ir\ssa.hpp>
#include <backend\ir\ir_visitor.hpp>

#include <backend\ir\ir.hpp>
#include <backend\ir\ir.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

void test_ir()
{
}
