//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_INL__
#define __EARLEY_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪

template <typename T>
earley_parser<T>::earley_parser(const typename earley_parser<T>::lexical_analyzer_type& lexical_analyzer, grammar& gr)
                : parser<T>(lexical_analyzer), my_grammar(gr)
{
}

template <typename T>
earley_parser<T>::~earley_parser()
{
}

template <typename T>
void earley_parser<T>::build_charts()
{
    // Адаптированный для построения деревьев вывода алгоритм Эрли, Владимир Анатольевич Лапшин.
    log_info(L"Building charts ...");

    // cache variables
    grammar& gr(my_grammar);
    lexical_analyzer_type& lexer(my_lexical_analyzer);

    // aux charts
    charts_type charts;

    // add V[0]
    chart_type chart0(earley_algorithm::create_chart(static_cast<uint32_t>(charts.size()))); // V[0]

    charts.emplace_back(chart0);

    // Добавляем в состояние V[0] все ситуации, где слева в правиле грамматики стоит стартовый нетерминал S,
    // с точкой в начале правой части правила и нулем как вторым элементом ситуации. Полагаем l = null и p = ∅.
    auto nonterminal_it(gr.nt_rules().find((*gr.start_symbol()).name()));
    const auto& nonterminal_rules((*nonterminal_it).second);

    for(const auto& nonterminal_rule : nonterminal_rules)
    {
        item_type new_item(earley_algorithm::create_item(nonterminal_rule, chart0, nullptr, nullptr, earley_algorithm::flags::init));
        auto result = (*chart0).items.emplace(new_item); //??
    }

    log_info(earley_visualization::decorate_charts(charts).c_str()); //??

    // Для каждой ситуации Эрли в состоянии Эрли V[0] вызываем процедуры Completer и Predictor.
    for(const auto& item : (*chart0).items)
    {
        earley_algorithm::complete(item, chart0);
        earley_algorithm::predict(item, chart0);
    }

    log_info(earley_visualization::decorate_charts(charts).c_str()); //??

    chart_type chart(chart0); // V[i-1] chart

    // Для каждого символа входной строки заполняем соответствующие состояния Эрли.
    for(;;)
    {
        // advance lexer
        (*lexer).next_lexeme();

        if((*lexer).is_eos())
        {
            break;
        }

        // Вызываем процедуру Scanner для состояния V[i-1].
        chart_type new_chart;

        earley_algorithm::scan(chart, charts, static_cast<uint32_t>((*lexer).token().type), new_chart);
        log_info(earley_visualization::decorate_charts(charts).c_str()); //??

        // Если список ситуаций Эрли в состоянии Эрли V[i] пуст, возвращаем False, parsing has failed.
        if((*new_chart).items.empty())
        {
            status().custom_code() = status::custom_code::error;
            status().system_code() = ::GetLastError();
            status().text().assign(L"Invalid input, terminating parsing ...\n");
            status().text().append(status().get_system_error_message());

            break;
        }

        charts.emplace_back(new_chart);

        // Для каждой ситуации Эрли в состоянии Эрли V[i] вызываем процедуры Completer и Predictor.
        for(const auto& item : (*new_chart).items)
        {
            earley_algorithm::complete(item, new_chart);
            earley_algorithm::predict(item, new_chart);
        }

        log_info(earley_visualization::decorate_charts(charts).c_str()); //??

        chart = new_chart;
    }

    log_info(earley_visualization::decorate_charts(charts).c_str()); //??

    my_charts.swap(charts);

    log_info(L"Built charts.");
}

template <typename T>
void earley_parser<T>::build_trees()
{
    log_info(L"Building tree(s) ...");

    log_info(L"Built tree(s).");
}

template <typename T>
void earley_parser<T>::parse()
{
    log_info(L"Parsing ...");

    build_charts();
    build_trees();

    log_info(L"Parsed.");
}

END_NAMESPACE

#endif // __EARLEY_PARSER_INL__
