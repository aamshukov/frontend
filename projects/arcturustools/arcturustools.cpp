//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/status.hpp>
#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/text.hpp>
#include <core/unicode.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>
#include <core/flags.hpp>
#include <core/counter.hpp>

#include <core/domain_helper.hpp>

#include <core/data_provider.hpp>
#include <core/file_data_provider.hpp>
#include <core/string_data_provider.hpp>

#include <core/content.hpp>

#include <core/command_line.hpp>
#include <core/configurator.hpp>

#include <core/dag.hpp>
#include <core/tree.hpp>

#include <core/visitor.hpp>
#include <core/visitable.hpp>

#include <core/logger.hpp>

#include <frontend/grammar/token.hpp>
#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>
#include <frontend/grammar/grammar_visualization.hpp>

#include <symtable/scope/scope.hpp>
#include <symtable/scope/scope_namespace.hpp>
#include <symtable/scope/scope_structure.hpp>
#include <symtable/scope/scope_function.hpp>
#include <symtable/scope/scope_parameter.hpp>
#include <symtable/scope/scope_block.hpp>

#include <symtable/symbol_table_record.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>
#include <frontend/fsa/fsa_re.hpp>
#include <frontend/fsa/fsa_codegen.hpp>
#include <frontend/fsa/fsa_visualization.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/parser/parser_dag.hpp>
#include <frontend/parser/parser_tree.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/parser_algorithm.hpp>
#include <frontend/parser/parser_visualization.hpp>
#include <frontend/parser/parser_visualization.inl>

#include <frontend/semantics/semantics.hpp>
#include <frontend/semantics/semantics.inl>

#include <backend/ir/quadruple.hpp>

#include <backend/ir/ir_visitor.hpp>
#include <backend/ir/dag_tac_visitor.hpp>
#include <backend/ir/tree_tac_visitor.hpp>

#include <backend/ir/ssa.hpp>

#include <backend/ir/ir.hpp>
#include <backend/ir/ir.inl>
#include <backend/ir/ir_visualization.hpp>
#include <backend/ir/ir_visualization.inl>

#include <controller/controller.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(controller)

END_NAMESPACE

int main()
{
    return 0;
}
