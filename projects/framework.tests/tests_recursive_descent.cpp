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

#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/parser/parser_dag.hpp>
#include <frontend/parser/parser_tree.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/parser_algorithm.hpp>
#include <frontend/parser/parser_visualization.hpp>
#include <frontend/parser/parser_visualization.inl>

#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/parser/parser_dag.hpp>
#include <frontend/parser/parser_tree.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/parser_algorithm.hpp>

#include <frontend\parser\backtracking\recursive_descent_parser.hpp>
#include <frontend\parser\backtracking\recursive_descent_parser.inl>

#include <frontend/parser/ll/ll_algorithm.hpp>
#include <frontend/parser/ll/ll_parser.hpp>
#include <frontend/parser/ll/ll_parser.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

struct rd_token_traits : public token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        eol,
        eos,
        indent,
        dedent,

        a,
        b,
        c,

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

class rd_lexical_analyzer : public lexical_analyzer<token<rd_token_traits>>
{
    protected:
        virtual void next_lexeme_impl() override
        {
            my_token.type = token_type::traits::type::eos;
        }

    public:
        rd_lexical_analyzer(const content_type& content) : lexical_analyzer(content)
        {
        }
};

struct rd_tree_traits
{
    DECLARE_ENUM
    (
        kind,
        uint32_t,
        unknown = 0,

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

class rd_parser : recursive_descent_parser<token<rd_token_traits>, rd_tree_traits>
{
    public:
        rd_parser(const lexical_analyzer_type& lexical_analyzer) : recursive_descent_parser<token<rd_token_traits>, rd_tree_traits>(lexical_analyzer)
        {
        }

        void parse() override
        {
        }
};

void test_recursive_descent_0()
{
    grammar gr;

    gr.load(LR"(D:\Projects\fe\grammars\LL.G5.TAL.txt)");

    uint8_t k = 3;

    std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

    grammar_algorithm::build_nullability_set(gr);
    grammar_algorithm::build_first_set(gr, k);
    grammar_algorithm::build_follow_set(gr, k);
    grammar_algorithm::build_la_set(gr, k);
    
    ll_algorithm::strong_ll_table_type strong_ll_table;
    ll_algorithm::build_strong_ll_table(gr, k, strong_ll_table);

    ll_algorithm::ll_table_type ll_table;
    ll_algorithm::build_ll_table(gr, k, ll_table);

    std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

exit(0);    
    //std::wcout << grammar_visualization::decorate_symbols(gr) << std::endl;

    //string_type file_name((char_type*)(LR"(d:\tmp\utf-8.txt)"));

    //file_data_provider provider(file_name);

    //rd_lexical_analyzer::content_type content;

    //operation_status status;
    //(*content).load(provider, status);

    //auto le(factory::create<rd_lexical_analyzer>(content));
    //auto rd(factory::create<rd_parser>(le));

    //(*rd).parse();
}
