﻿//..............................
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
#include <core\string_data_provider.hpp>
#include <core\content.hpp>
#include <core\counter.hpp>
#include <core\factory.hpp>
#include <core\singleton.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>
#include <core\command_line.hpp>
#include <core\configurator.hpp>

#include <core\tree.hpp>
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

#include <frontend\tree\ast.hpp>
#include <frontend\tree\cst.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\parser.hpp>
#include <frontend\parser\parser.inl>

#include <frontend\parser\earley\earley_algorithm.hpp>
#include <frontend\parser\earley\earley_visualization.hpp>
#include <frontend\parser\earley\earley_parser.hpp>
#include <frontend\parser\earley\earley_parser.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct earley_token_traits : public token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        eol,
        eos,

        a,
        b,
        c,
        n,
        p, // +

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

earley_token_traits::enum_map_type earley_token_traits::mapping;


class earley_lexical_analyzer : public lexical_analyzer<token<earley_token_traits>>
{
    private:
        std::size_t k = 0;

    protected:
        virtual void next_lexeme_impl() override
        {
            if(k == (*content()).count())
            {
                my_token.type = token_type::traits::type::eos;
            }
            else
            {
                string_type name(text::codepoint_to_string((*content()).data()[k]));
                my_token.type = token_type::traits::value(name);
            }

            k++;
        }

    public:
        earley_lexical_analyzer(const content_type& content) : lexical_analyzer(content)
        {
            earley_token_traits::initialize();
        }
};

class my_earley_parser : public earley_parser<token<earley_token_traits>>
{
    public:
        my_earley_parser(const lexical_analyzer_type& lexical_analyzer, grammar& gr) : earley_parser(lexical_analyzer, gr)
        {
        }

        tree_type handle_start(const item_type& item) override
        {
            item;//??
            tree_type result;
            return result;
        }

        tree_type handle_terminal(const token_type& token, const tree_type& node) override
        {
            token, node;//??
            tree_type result;
            return result;
        }

        tree_type handle_before_terminal(const item_type& item, const tree_type& node) override
        {
            item, node;//??
            tree_type result;
            return result;
        }

        tree_type handle_after_terminal(const item_type& item, const tree_type& node) override
        {
            item, node;//??
            tree_type result;
            return result;
        }
};

void test_earley_parser()
{
    std::vector<string_type> inputs  =
    {
        LR"(D:\Projects\fe\grammars\Earley.G0.txt)",
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

        earley_algorithm::items_type items;

        earley_algorithm::item_type item1(factory::create<earley_algorithm::item>());
        earley_algorithm::item_type item2(factory::create<earley_algorithm::item>());

        items.emplace(item1);
        items.emplace(item2);

        using content_type = lexical_analyzer<token<earley_token_traits>>::content_type;

        content_type content(factory::create<content>());

        string_data_provider provider(L"n+n");

        operation_status status; //??

        bool result = (*content).load(provider, status);

        if(result)
        {
            auto lexical_analyzer(factory::create<earley_lexical_analyzer>(content));

            my_earley_parser parser(lexical_analyzer, gr);

            parser.parse();

            std::wcout << earley_visualization::decorate_charts(parser.charts()).c_str() << std::endl;
            std::wcout << earley_visualization::decorate_trees(parser.trees()).c_str() << std::endl;
        }
    }
}
