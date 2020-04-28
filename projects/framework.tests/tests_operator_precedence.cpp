//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>
#include <core/flags.hpp>
#include <core/counter.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/domain_helper.hpp>

#include <core/status.hpp>
#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/text.hpp>
#include <core/unicode.hpp>

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

#include <symtable/symbol_ir.hpp>
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

#include <frontend/parser/operator_precedence/precedence_algorithm.hpp>
#include <frontend/parser/operator_precedence/precedence_visualization.hpp>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

void test_operator_precedence()
{
    grammar gr;

    //gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.Opaleva.G0.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.Grune.9.2.txt)");
    gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.Barret.G0.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.G2.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.Aho.G0.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\OperatorPrecedence.Watson.G1.txt)");

    std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

    precedence_algorithm::operators_type operators;

    operators.emplace(symbol::op_mark);
    operators.emplace((*gr.pool().find(L"+")).second);
    operators.emplace((*gr.pool().find(L"*")).second);
    operators.emplace((*gr.pool().find(L"(")).second);
    operators.emplace((*gr.pool().find(L")")).second);

    precedence_algorithm::precedence_table_type table;

    precedence_algorithm::build_operator_precedence_table(gr, operators, false, table);
    precedence_algorithm::build_operator_precedence_table(gr, operators, true, table);

    precedence_algorithm::build_wirth_weber_precedence_table(gr, table);
}
