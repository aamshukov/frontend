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

#include <frontend\parser\parser.hpp>
#include <frontend\parser\parser.inl>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>
#include <frontend\parser\lr\lr_parser.hpp>
#include <frontend\parser\lr\lr_parser.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

void test_lr_table()
{
    std::vector<string_type> inputs  =
    {
        LR"(D:\Projects\fe\grammars\LR.Aho.G5.29.txt)",
        //LR"(D:\Projects\fe\grammars\LR.Aho.G.4.55.txt)",
        //LR"(D:\Projects\fe\grammars\LR.GruneJacobsBook.9.32.txt)",
        //LR"(D:\Projects\fe\grammars\LR.2.Nigel.G5.txt)",
        //LR"(D:\Projects\fe\grammars\LR.2.Nigel.G1.txt)",
        //LR"(D:\Projects\fe\grammars\LR.2.Nigel.G3.txt)",
        //LR"(D:\Projects\fe\grammars\LR.Sippu.6.7.txt)",
        //LR"(D:\Projects\fe\grammars\LR.2+.Sippu.6.14.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.Sippu.6.24.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.Sippu.7.25.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.Incremental.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.Expr.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.Chapter02Compilers6December.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.bottom(2).txt)",

        //LR"(D:\Projects\fe\grammars\LR.1.shift.reduce.conflict.G1.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.shift.reduce.conflict.G2.txt)",
        //LR"(D:\Projects\fe\grammars\LR.1.reduce.reduce.conflict.G1.txt)"
    };

    uint8_t k = 1;

    for(const auto& input : inputs)
    {
        grammar gr;

        gr.load(input);

        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

        grammar_algorithm::build_nullability_set(gr);
        grammar_algorithm::build_first_set(gr, k);
        grammar_algorithm::build_first_set(gr, k, true);
        grammar_algorithm::build_follow_set(gr, k);
        grammar_algorithm::build_la_set(gr, k);

        lr_algorithm::lr_goto_table_type goto_table;
        lr_algorithm::lr_action_table_type action_table;

        lr_algorithm::build_lr_table(gr, k, goto_table, action_table);
    }
}

    //{
    //    lr_algorithm::lr_items_type items;

    //    grammar_algorithm::symbols_type symbols;
    //    symbols.emplace_back(symbol::epsilon);

    //    lr_algorithm::build_lr_items_set_for_viable_prefix(gr, symbols, k, items);
    //}
    //{
    //    lr_algorithm::lr_items_type items;

    //    grammar_algorithm::symbols_type symbols;
    //    symbols.emplace_back(gr.pool()[L"S"]);

    //    lr_algorithm::build_lr_items_set_for_viable_prefix(gr, symbols, k, items);
    //}
    //{
    //    lr_algorithm::lr_items_type items;

    //    grammar_algorithm::symbols_type symbols;
    //    symbols.emplace_back(gr.pool()[L"S"]);
    //    symbols.emplace_back(gr.pool()[L"a"]);

    //    lr_algorithm::build_lr_items_set_for_viable_prefix(gr, symbols, k, items);
    //}
