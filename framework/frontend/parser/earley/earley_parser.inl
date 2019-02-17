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
earley_parser<T>::earley_parser(const typename earley_parser<T>::lexical_analyzer_type& lexical_analyzer)
                : parser<T>(lexical_analyzer)
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

    // advance lexer
    (*my_lexical_analyzer).next_lexeme();

    // Добавляем в состояние V[0] все ситуации, где слева в правиле грамматики стоит стартовый нетерминал S,
    // с точкой в начале правой части правила и нулем как вторым элементом ситуации. Полагаем l = null и p = ∅.
    chart_type chart0(earley_algorithm::create_chart(static_cast<uint32_t>(my_charts.size())));
    chart_type chart1;

    earley_algorithm::scan(chart0, my_charts, static_cast<uint32_t>((*my_lexical_analyzer).token().type), chart1);


    // Для каждой ситуации Эрли в состоянии Эрли V[0] вызываем процедуры Completer и Predictor.


    // Для каждого символа входной строки заполняем соответствующие состояния Эрли.
    for(;;)
    {
        (*my_lexical_analyzer).next_lexeme();

        if((*my_lexical_analyzer).is_eos())
        {
            break;
        }

        // Вызываем процедуру Scanner для состояния i - 1.


        // Если список ситуаций Эрли в состоянии Эрли V[i] пуст, возвращаем False, parsing has failed.
        //if()
        {
            status().custom_code() = status::custom_code::error;
            status().system_code() = ::GetLastError();
            status().text().assign(L"");
            status().text().append(L"\n");
            status().text().append(status().get_system_error_message());
        }


        // Для каждой ситуации Эрли в состоянии Эрли V[i] вызываем процедуры Completer и Predictor.

    }

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
