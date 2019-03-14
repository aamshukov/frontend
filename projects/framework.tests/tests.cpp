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
#include <core\enumerate.hpp>

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

#include <frontend\parser\backtracking\recursive_descent_parser.hpp>
#include <frontend\parser\backtracking\recursive_descent_parser.inl>

#include <frontend\parser\ll\ll_algorithm.hpp>
#include <frontend\parser\ll\ll_visualization.hpp>
#include <frontend\parser\ll\ll_parser.hpp>
#include <frontend\parser\ll\ll_parser.inl>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>
#include <frontend\parser\lr\lr_parser.hpp>
#include <frontend\parser\lr\lr_parser.inl>

#include <frontend\semantics\symbol_attribute.hpp>
#include <frontend\semantics\symbol_table.hpp>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct my_token_traits : public token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        eol,
        eos,

        binary_integer_lit,
        octal_integer_lit,
        decimal_integer_lit,
        hexadecimal_integer_lit,
        decimal_floating_lit,
        hexadecimal_floating_lit,

        binary_integer8_lit,
        binary_integeru8_lit,
        binary_integer16_lit,
        binary_integeru16_lit,
        binary_integer32_lit,
        binary_integeru32_lit,
        binary_integer64_lit,
        binary_integeru64_lit,

        octal_integer8_lit,
        octal_integeru8_lit,
        octal_integer16_lit,
        octal_integeru16_lit,
        octal_integer32_lit,
        octal_integeru32_lit,
        octal_integer64_lit,
        octal_integeru64_lit,

        decimal_integer8_lit,
        decimal_integeru8_lit,
        decimal_integer16_lit,
        decimal_integeru16_lit,
        decimal_integer32_lit,
        decimal_integeru32_lit,
        decimal_integer64_lit,
        decimal_integeru64_lit,

        hexadecimal_integer8_lit,
        hexadecimal_integeru8_lit,
        hexadecimal_integer16_lit,
        hexadecimal_integeru16_lit,
        hexadecimal_integer32_lit,
        hexadecimal_integeru32_lit,
        hexadecimal_integer64_lit,
        hexadecimal_integeru64_lit,

        abstract_kw,
        assert_kw,
        boolean_kw,
        break_kw,
        byte_kw,
        case_kw,
        catch_kw,
        char_kw,
        class_kw,
        const_kw,
        continue_kw,
        default_kw,
        do_kw,
        double_kw,
        else_kw,
        enum_kw,
        extends_kw,
        final_kw,
        finally_kw,
        float_kw,
        for_kw,
        if_kw,
        goto_kw,
        implements_kw,
        import_kw,
        instanceof_kw,
        int_kw,
        interface_kw,
        long_kw,
        native_kw,
        new_kw,
        package_kw,
        private_kw,
        protected_kw,
        public_kw,
        return_kw,
        short_kw,
        static_kw,
        strictfp_kw,
        super_kw,
        switch_kw,
        synchronized_kw,
        this_kw,
        throw_kw,
        throws_kw,
        transient_kw,
        try_kw,
        void_kw,
        volatile_kw,
        while_kw,

        var_kw,

        true_lit,
        false_lit,
        null_lit,

        identifier,

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

typename my_token_traits::enum_map_type my_token_traits::mapping;

void test_re_to_nfa_to_dfa();
void test_nfa_to_dfa();
void test_minimize_dfa();
void test_remove_states();
void test_get_equivalence_class_combination();

void test_minimize_fsa();
void test_minimize_fsa1();
void test_minimize_fsa2();
void test_minimize_fsa3();
void test_minimize_fsa4();
void test_minimize_fsa5();
void test_minimize_fsa6();
void test_minimize_fsa7();
void test_minimize_fsa8();
void test_minimize_fsa9();
void test_minimize_fsa10();
void test_minimize_fsa11();

void test_binary_lit_cpp();
void test_octal_integer_lit();
void test_decimal_integer_lit();
void test_hexadecimal_integer_lit();
void test_decimal_floating_lit();
void test_hexadecimal_floating_lit();

void test_cpp_literals();

void test_java_literals();
void test_java_identifiers_and_keywords();

void test_load_grammar();

void test_recursive_descent_0();

void test_infix_op();
void test_infix_op_llk();

void test_cartesian_product();

void test_make_unique();

void test_algo_directory();

void test_operator_precedence();

void test_eff();
void test_lr_table();

void test_sets();

void test_earley_parser();

void test_re_dfa();

extern "C" double CombineA(int a, int b, int c, int d, int e, double f);
extern "C" void _start();

int main()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::cout << "CombineA: " << CombineA(1,2,3,4, 5, 6.1) << std::endl;

    auto id = static_cast<uint32_t>(-1);
    std::int32_t id0 = static_cast<int32_t>(id);
    id0;

    std::array<char, 10> str{"42"};
    int result;
    std::from_chars(str.data(), str.data()+str.size(), result);

    for(const auto& [i, a] : enumerate(str))
    {
        std::size_t k = i;
        char c = a;
        k, c;
    }

    operation_status status;

    logger::instance().initialize(LR"(d:\tmp\fe.log)", status);

    test_re_dfa();

    test_earley_parser();

    test_sets();

    test_lr_table();
    test_eff();
    test_recursive_descent_0();

    test_operator_precedence();

    test_algo_directory();

    //test_make_unique();
    //test_cartesian_product();
    //test_infix_op();
    //test_infix_op_llk();
    //test_load_grammar();

    std::vector<int> vec;

    vec.emplace_back(1);
    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);

    auto it(std::find(vec.begin(), vec.end(), 20));

    if(it != vec.end())
    {
        vec.erase(it);
    }

    std::map<int, int> iimm;

    iimm.emplace(std::map<int, int>::value_type(1, 5));
    iimm.erase(1);
    iimm.erase(2);
    iimm.emplace(std::map<int, int>::value_type(1, 50));

    std::multimap<int, string_type> mm;

    mm.emplace(std::multimap<int, string_type>::value_type(1, L"odin"));
    mm.emplace(std::multimap<int, string_type>::value_type(1, L"ODIN"));

    mm.emplace(std::multimap<int, string_type>::value_type(2, L"dva"));
    mm.emplace(std::multimap<int, string_type>::value_type(2, L"DVA"));

    auto it_mm(mm.find(1));
    it_mm;

    wchar_t* end;
    auto n = std::wcstol(L"101", &end, 2);
    n;

    text::datum_type ch = L'_';
    char_type chw = static_cast<char_type>(ch);
    chw;

    auto fd = std::wcstod(L"1.322_8243_2E-2_4", &end);
    fd;

    std::shared_ptr<text::datum_type[]> codepoints;
    size_type count;

    text::string_to_codepoints(L"123", codepoints, count, status);
    auto nn = text::parse<uint8_t>(codepoints.get(), (uint32_t)count, 10);
    nn;

    text::string_to_codepoints(L"6.02__2_1_3_7e+2___3f", codepoints, count, status);
    auto dn = text::parse<double>(codepoints.get(), (uint32_t)count, 10);

    text::string_to_codepoints(L"0xA.02_BC_2_1_3_7e+2___3f", codepoints, count, status);
    dn = text::parse<double>(codepoints.get(), (uint32_t)count, 16);

    text::string_to_codepoints(L"6.02__123456677757556757652_1_3_7e+2__57657657567_3f", codepoints, count, status);
    dn = text::parse<double>(codepoints.get(), (uint32_t)count, 10);
    dn;

    text::string_to_codepoints(L"1111", codepoints, count, status);
    auto bn = text::parse<int16_t>(codepoints.get(), (uint32_t)count, 2);
    bn;

    text::string_to_codepoints(L"0116", codepoints, count, status);
    auto on = text::parse<int16_t>(codepoints.get(), (uint32_t)count, 8);
    on;

    text::string_to_codepoints(L"0xABC116", codepoints, count, status);
    auto hn = text::parse<uint64_t>(codepoints.get(), (uint32_t)count, 16);
    hn;

    bool id_start = text::is_java_identifier_start(L'$');
    id_start = text::is_java_identifier_start(L'_');
    id_start = text::is_java_identifier_start(L'\u0000');
    id_start = text::is_java_identifier_start(L'\u0001');
    id_start = text::is_java_identifier_start(L'\u0002');
    id_start = text::is_java_identifier_start(L'\u0003');
    id_start = text::is_java_identifier_start(L'\u0004');
    id_start = text::is_java_identifier_start(L'\u0005');
    id_start = text::is_java_identifier_start(L'\u0006');
    id_start = text::is_java_identifier_start(L'\u0007');
    id_start = text::is_java_identifier_start(L'\u0008');
    id_start = text::is_java_identifier_start(L'\u000E');
    id_start = text::is_java_identifier_start(L'\u000F');
    id_start = text::is_java_identifier_start(L'\u0010');
    id_start = text::is_java_identifier_start(L'\u0011');
    id_start = text::is_java_identifier_start(L'\u0012');
    id_start = text::is_java_identifier_start(L'\u0013');
    id_start = text::is_java_identifier_start(L'\u0014');
    id_start = text::is_java_identifier_start(L'\u0015');
    id_start = text::is_java_identifier_start(L'\u0016');
    id_start = text::is_java_identifier_start(L'\u0017');
    id_start = text::is_java_identifier_start(L'\u0018');
    id_start = text::is_java_identifier_start(L'\u0019');
    id_start = text::is_java_identifier_start(L'\u001B');
    id_start = text::is_java_identifier_start(L'\u007F');

    bool id_part = text::is_java_identifier_part(L'$');
    id_part = text::is_java_identifier_part(L'_');
    id_part = text::is_java_identifier_part(L'\u001A');
    id_part = text::is_java_identifier_part(L'\u0000');
    id_part = text::is_java_identifier_part(L'\u0001');
    id_part = text::is_java_identifier_part(L'\u0002');
    id_part = text::is_java_identifier_part(L'\u0003');
    id_part = text::is_java_identifier_part(L'\u0004');
    id_part = text::is_java_identifier_part(L'\u0005');
    id_part = text::is_java_identifier_part(L'\u0006');
    id_part = text::is_java_identifier_part(L'\u0007');
    id_part = text::is_java_identifier_part(L'\u0008');
    id_part = text::is_java_identifier_part(L'\u000E');
    id_part = text::is_java_identifier_part(L'\u000F');
    id_part = text::is_java_identifier_part(L'\u0010');
    id_part = text::is_java_identifier_part(L'\u0011');
    id_part = text::is_java_identifier_part(L'\u0012');
    id_part = text::is_java_identifier_part(L'\u0013');
    id_part = text::is_java_identifier_part(L'\u0014');
    id_part = text::is_java_identifier_part(L'\u0015');
    id_part = text::is_java_identifier_part(L'\u0016');
    id_part = text::is_java_identifier_part(L'\u0017');
    id_part = text::is_java_identifier_part(L'\u0018');
    id_part = text::is_java_identifier_part(L'\u0019');
    id_part = text::is_java_identifier_part(L'\u001B');
    id_part = text::is_java_identifier_part(L'\u007F');

    const char_type* indent(get_indent(4));
    indent;

    std::set<int> ints;

    std::u32string str5 { U"hello" };

    std::vector<std::shared_ptr<string_type>> ss;

    auto s_1 = std::make_shared<string_type>(L"odin");
    auto s_2 = std::make_shared<string_type>(L"dwa");
    auto s_3 = s_2;
    
    ss.emplace_back(s_1);
    ss.emplace_back(s_2);

    auto ff = std::find(std::begin(ss), std::end(ss), s_2);

    //my_token_traits::initilize();

    //if(logger::instance().initialize(LR"(d:\tmp\fe.log)", status))
    {
        test_load_grammar();

        //std::exception ex("zopa");

        //log_exception(ex, L"test %ld", 5);
        //bool result;
        //OPERATION_FAILED_EX(ex, status::custom_code::error, L"Adding final FSA state: error occurred.")

        //string_type fmt(format(L"Test %d %s.dot", 5, L"kuku"));

        test_re_dfa();

        test_java_literals();
        test_java_identifiers_and_keywords();

        test_cpp_literals();

        test_binary_lit_cpp();
        test_octal_integer_lit();
        test_decimal_integer_lit();
        test_hexadecimal_integer_lit();
        test_decimal_floating_lit();
        test_hexadecimal_floating_lit();

        test_minimize_fsa();
        test_minimize_fsa1();
        test_minimize_fsa2();
        test_minimize_fsa3();
        test_minimize_fsa4();
        test_minimize_fsa5();
        test_minimize_fsa6();
        test_minimize_fsa7();
        test_minimize_fsa8();
        test_minimize_fsa9();
        test_minimize_fsa10();
        test_minimize_fsa11();

        test_get_equivalence_class_combination();
        test_minimize_dfa();
        test_remove_states();
        test_nfa_to_dfa();
        test_re_to_nfa_to_dfa();

        string_type file_name((char_type*)(LR"(d:\tmp\utf-8.txt)"));

        file_data_provider provider(file_name);

        content content;

        bool rc = content.load(provider, status);

        if(rc)
        {
            content.data();
            content.count();

            string_type message = operation_status::get_system_error_message();

            token<my_token_traits> t;
            //t.value;
            std::size_t token_size(sizeof(t));
            token_size;
            auto en(token<my_token_traits>::name(my_token_traits::type::eos));
            en = (token<my_token_traits>::name(my_token_traits::type::eos));
            en = (token<my_token_traits>::name(static_cast<my_token_traits::type>(-10)));

            std::queue<token<my_token_traits>> tokens;
            tokens.emplace(t);
            tokens.emplace(t);
            token<my_token_traits> t0(std::move(tokens.front()));
            t0 = std::move(tokens.front());
            tokens.pop();

            symbol& es(*symbol::epsilon);
            es.name();

            std::vector<symbol> symbols;
            symbol s0(0, string_type(L"S"), symbol::kind::nonterminal);
            symbols.emplace_back(std::move(s0));
            s0 = std::move(symbol(0, string_type(L"S"), symbol::kind::nonterminal));
            symbol s1(1, string_type(L"S"), symbol::kind::nonterminal);
            symbols.emplace_back(s1);
            s1 = symbol(1, string_type(L"S"), symbol::kind::nonterminal);
            s1.id();
        }

        fsa_transition transition0;
        fsa_transition transition(0, 0, empty_string());

        fsa::fsa_type fsa1(factory::create<fsa>());
        fsa::state_type stateA0(factory::create<fsa_state>(L"start", 0));
        fsa::state_type stateA1(factory::create<fsa_state>(L"S1", 0));
        fsa::state_type stateA2(factory::create<fsa_state>(L"S2", 0));
        fsa::state_type stateA3(factory::create<fsa_state>(L"end", 0));
        (*fsa1).start_state() = stateA0;
        (*fsa1).add_state(stateA0, status);
        (*fsa1).add_state(stateA1, status);
        (*fsa1).add_state(stateA2, status);
        (*fsa1).add_state(stateA3, status);
        (*fsa1).add_transition(stateA0, stateA0, L"A_A", status);
        (*fsa1).add_transition(stateA0, stateA1, L"A", status);
        (*fsa1).add_transition(stateA1, stateA2, L"B", status);
        (*fsa1).add_transition(stateA2, stateA3, L"C", status);
        (*fsa1).add_final_state(stateA2, status);
        (*fsa1).add_final_state(stateA3, status);
        fsa_visualization::generate_graphviz_file(fsa1, string_type(LR"(d:\tmp\fsa1.dot)"));

        fsa::fsa_type fsa2(factory::create<fsa>());
        fsa::state_type stateB0(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));
        fsa::state_type stateB1(factory::create<fsa_state>(L"S1", 0));
        fsa::state_type stateB2(factory::create<fsa_state>(L"S2", 0));
        fsa::state_type stateB3(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));
        (*fsa2).start_state() = stateB0;
        (*fsa2).add_state(stateB0, status);
        (*fsa2).add_state(stateB1, status);
        (*fsa2).add_state(stateB2, status);
        (*fsa2).add_state(stateB3, status);
        (*fsa1).add_transition(stateB0, stateB0, L"B_B", status);
        (*fsa2).add_transition(stateB0, stateB1, L"D", status);
        (*fsa2).add_transition(stateB1, stateB2, L"F", status);
        (*fsa2).add_transition(stateB2, stateB3, L"G", status);
        (*fsa2).add_transition(stateB1, stateB3, L"B1_B3", status);
        (*fsa2).add_transition(stateB3, stateB1, L"B3_B1", status);
        (*fsa2).add_final_state(stateB1, status);
        (*fsa2).add_final_state(stateB3, status);
        fsa_visualization::generate_graphviz_file(fsa2, string_type(LR"(d:\tmp\fsa2.dot)"));

        fsa::fsa_type fsa3;
        fsa::combine(fsa1, fsa2, fsa3, status);
        fsa_visualization::generate_graphviz_file(fsa3, string_type(LR"(d:\tmp\fsa3.dot)"));

        fsa_visualization::generate_graphviz_file(fsa1, string_type(LR"(d:\tmp\fsa1_0.dot)"));
        fsa_visualization::generate_graphviz_file(fsa2, string_type(LR"(d:\tmp\fsa2_0.dot)"));

        fsa::fsa_type fsa4;
        fsa::concatenate(fsa1, fsa2, fsa4, status);
        fsa_visualization::generate_graphviz_file(fsa4, string_type(LR"(d:\tmp\fsa4.dot)"));

        std::shared_ptr<fsa_transition> tr(factory::create<fsa_transition>(0, 0, empty_string()));
    }

    return 0;
}

void dump_equivalence_classes(const fsa::fsa_type& dfa)
{
    fsa_algorithm::equivalence_classes_type equivalence_classes;

    fsa_algorithm::build_k_equivalence_classes(dfa, equivalence_classes);

    for(const auto& equivalence_class : equivalence_classes)
    {
        std::wcout << L" { ";
        std::for_each(equivalence_class.begin(),
                      equivalence_class.end(),
                      [](const auto& state_kvp){ std::wcout << L"'" << (*state_kvp.second).label().c_str() << L"' "; });
        std::wcout << L"} ";
    }

    std::wcout << std::endl << std::endl;
}

void validate_dfa(const std::vector<string_type>& inputs, fsa::fsa_type& dfa)
{
    std::wcout << std::endl;

    for(const auto& input : inputs)
    {
        std::wcout << L"Validating " << input << L" ... ";

        uint32_t vrc = fsa_algorithm::validate_dfa(dfa, input);

        my_token_traits::type tkn = static_cast<my_token_traits::type>(vrc);

        if(tkn != my_token_traits::type::unknown)
        {
            std::wcout << L"SUCCESS, " <<  token<my_token_traits>::name(tkn).c_str() << std::endl;
        }
        else
        {
            std::wcout << L"ERROR" << std::endl;
        }
    }

    std::wcout << std::endl << std::endl;
}

void test_re_to_nfa_to_dfa()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    string_type res[] =
    {
        L"a+",

        L"(a|b)*a(a|b)(a|b)", // aho, ullman
        L"(a|b)*abb", // aho, ullman

        L"(a|b)*(abb|a+b)",
        L"(0|10*1)*",
        L"a\\.",
        L"a\\.(b\\.b)+\\.a",
        L"a\\.b|c",
        L"a\\.b+\\.c",
        //L"\t\r\n\f\\(\\)\\?\\*\\|\\.\\+a(bb)+a",
        L"a((bb))*a",
        L"a*",
        L"(a|b)*abb",
        L"(a|b)*ba",
        L"ab|a*",
        L"a*(a|b)ba*|a*",
        L"((ab)|c)*",
        //L"జ్ఞ‌ా*",
        L"a+(\\.a+)*@a+(\\.a+)+",
        L"(ab+(b+aa)(ba)*(a+bb))*",
        L"\\ban?\\b",
        L"\\b|(a(T|n)?)|\\b",
        L"\\b|(a(T|n)?)|\\b",
        L"((c|a)b*)*",
        L"(a(b|c))*a",
    };

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    fsa::fsa_type fsa0;

    uint16_t k = 0;

    operation_status status;

    for(const auto& re : res)
    {
        if(text::string_to_codepoints(re, re_data, count, status))
        {
            if(fsa_re::re_to_fsa(re_data, count, 0, 0, empty_string(), fsa0, status))
            {
                fsa_visualization::generate_graphviz_file(fsa0, format(LR"(d:\tmp\FSA\fsa_%d.dot)", ++k));
                fsa_visualization::generate_graphviz_file((*fsa0).clone(), format(LR"(d:\tmp\FSA\fsa_%d_cloned.dot)", ++k));

                fsa::fsa_type dfa;

                if(fsa_algorithm::nfa_to_dfa(fsa0, dfa, status))
                {
                    fsa_visualization::generate_graphviz_file(dfa, format(LR"(d:\tmp\FSA\fsa_%d.dot)", ++k));
                }
            }
        }
    }
}

void test_nfa_to_dfa()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::fsa_type nfa(factory::create<fsa>());

    fsa::state_type state0(factory::create<fsa_state>(L"0", 0));
    fsa::state_type state1(factory::create<fsa_state>(L"1", 0));
    fsa::state_type state2(factory::create<fsa_state>(L"2", 0));
    fsa::state_type state3(factory::create<fsa_state>(L"3", 0));
    fsa::state_type state4(factory::create<fsa_state>(L"4", 0));
    fsa::state_type state5(factory::create<fsa_state>(L"5", 0));
    fsa::state_type state6(factory::create<fsa_state>(L"6", 0));
    fsa::state_type state7(factory::create<fsa_state>(L"7", 0));
    fsa::state_type state8(factory::create<fsa_state>(L"8", 0));
    fsa::state_type state9(factory::create<fsa_state>(L"9", 0));
    fsa::state_type state10(factory::create<fsa_state>(L"10", 0));

    (*nfa).add_state(state0, status);
    (*nfa).add_state(state1, status);
    (*nfa).add_state(state2, status);
    (*nfa).add_state(state3, status);
    (*nfa).add_state(state4, status);
    (*nfa).add_state(state5, status);
    (*nfa).add_state(state6, status);
    (*nfa).add_state(state7, status);
    (*nfa).add_state(state8, status);
    (*nfa).add_state(state9, status);
    (*nfa).add_state(state10, status);

    // start
    (*nfa).start_state() = state0;

    // 0
    (*nfa).add_transition(state0, state1, fsa_transition::epsilon_predicate(), status);
    (*nfa).add_transition(state0, state7, fsa_transition::epsilon_predicate(), status);

    // 1
    (*nfa).add_transition(state1, state2, fsa_transition::epsilon_predicate(), status);
    (*nfa).add_transition(state1, state4, fsa_transition::epsilon_predicate(), status);

    // 2
    (*nfa).add_transition(state2, state3, L"a", status);

    // 3
    (*nfa).add_transition(state3, state6, fsa_transition::epsilon_predicate(), status);

    // 4
    (*nfa).add_transition(state4, state5, L"b", status);

    // 5
    (*nfa).add_transition(state5, state6, fsa_transition::epsilon_predicate(), status);

    // 6
    (*nfa).add_transition(state6, state1, fsa_transition::epsilon_predicate(), status);
    (*nfa).add_transition(state6, state7, fsa_transition::epsilon_predicate(), status);

    // 7
    (*nfa).add_transition(state7, state8, L"a", status);

    // 8
    (*nfa).add_transition(state8, state9, L"b", status);

    // 9
    (*nfa).add_transition(state9, state10, L"b", status);

    // final
    (*nfa).add_final_state(state10, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    for(const auto& state : (*dfa).states())
    {
        if((*state.second).label().compare(L" 0 1 2 4 7") == 0)
        {
            (*state.second).label() = L"A";
        }
        else if((*state.second).label().compare(L" 1 2 3 4 6 7 8") == 0)
        {
            (*state.second).label() = L"B";
        }
        else if((*state.second).label().compare(L" 1 2 4 5 6 7") == 0)
        {
            (*state.second).label() = L"C";
        }
        else if((*state.second).label().compare(L" 1 2 4 5 6 7 9") == 0)
        {
            (*state.second).label() = L"D";
        }
        else if((*state.second).label().compare(L" 1 2 4 5 6 7 10") == 0)
        {
            (*state.second).label() = L"E";
        }
    }

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa.dot)");
}

void test_minimize_dfa()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::fsa_type nfa(factory::create<fsa>());

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_F, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"0", status);
    (*nfa).add_transition(state_A, state_C, L"1", status);

    (*nfa).add_transition(state_B, state_A, L"0", status);
    (*nfa).add_transition(state_B, state_D, L"1", status);

    (*nfa).add_transition(state_C, state_E, L"0", status);
    (*nfa).add_transition(state_C, state_F, L"1", status);

    (*nfa).add_transition(state_D, state_E, L"0", status);
    (*nfa).add_transition(state_D, state_F, L"1", status);

    (*nfa).add_transition(state_E, state_E, L"0", status);
    (*nfa).add_transition(state_E, state_F, L"1", status);

    (*nfa).add_transition(state_F, state_F, L"0", status);
    (*nfa).add_transition(state_F, state_F, L"1", status);

    // final
    (*nfa).add_final_state(state_C, status);
    (*nfa).add_final_state(state_D, status);
    (*nfa).add_final_state(state_E, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa.dot)");
}

void test_remove_states()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::fsa_type nfa(factory::create<fsa>());

    fsa::state_type state0(factory::create<fsa_state>(L"0", 0));
    fsa::state_type state1(factory::create<fsa_state>(L"1", 0));
    fsa::state_type state2(factory::create<fsa_state>(L"2", 0));
    fsa::state_type state3(factory::create<fsa_state>(L"3", 0));
    fsa::state_type state4(factory::create<fsa_state>(L"4", 0));
    fsa::state_type state5(factory::create<fsa_state>(L"5", 0));
    fsa::state_type state6(factory::create<fsa_state>(L"6", 0));
    fsa::state_type state7(factory::create<fsa_state>(L"7", 0));
    fsa::state_type state8(factory::create<fsa_state>(L"8", 0));
    fsa::state_type state9(factory::create<fsa_state>(L"9", 0));
    fsa::state_type state10(factory::create<fsa_state>(L"10", 0));

    (*nfa).add_state(state0, status);
    (*nfa).add_state(state1, status);
    (*nfa).add_state(state2, status);
    (*nfa).add_state(state3, status);
    (*nfa).add_state(state4, status);
    (*nfa).add_state(state5, status);
    (*nfa).add_state(state6, status);
    (*nfa).add_state(state7, status);
    (*nfa).add_state(state8, status);
    (*nfa).add_state(state9, status);
    (*nfa).add_state(state10, status);

    // start
    (*nfa).start_state() = state0;

    // 0
    (*nfa).add_transition(state0, state4, L"a", status);

    // 1
    (*nfa).add_transition(state1, state2, L"a", status);

    // 2
    (*nfa).add_transition(state2, state3, L"a", status);
    (*nfa).add_transition(state3, state2, L"a", status);

    // 3
    (*nfa).add_transition(state3, state6, L"a", status);

    // 4
    (*nfa).add_transition(state4, state5, L"a", status);

    // 5
    (*nfa).add_transition(state5, state6, L"a", status);

    // 6
    (*nfa).add_transition(state6, state1, L"a", status);
    (*nfa).add_transition(state6, state7, L"a", status);

    // 7
    (*nfa).add_transition(state7, state8, L"a", status);

    // 8
    (*nfa).add_transition(state8, state8, L"b", status);

    // 9
    (*nfa).add_transition(state9, state8, L"b", status);
    (*nfa).add_transition(state9, state10, L"b", status);

    // final
    (*nfa).add_final_state(state5, status);
    (*nfa).add_final_state(state10, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa_rs.dot)");

    fsa::fsa_type cloned((*nfa).clone());

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cloned.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::remove_useless_states(nfa, dfa);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa_rs.dot)");
}

void test_get_equivalence_class_combination()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa_algorithm::equivalence_class_type equivalence_class;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    (*state_A).id() = 1;
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    (*state_B).id() = 2;
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    (*state_C).id() = 3;
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    (*state_D).id() = 4;
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    (*state_E).id() = 5;
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));
    (*state_F).id() = 6;

    equivalence_class.emplace((*state_A).id(), state_A);
    equivalence_class.emplace((*state_B).id(), state_B);
    equivalence_class.emplace((*state_C).id(), state_C);
    equivalence_class.emplace((*state_D).id(), state_D);
    equivalence_class.emplace((*state_E).id(), state_E);
    equivalence_class.emplace((*state_F).id(), state_F);

    std::vector<std::pair<std::pair<uint32_t, fsa_algorithm::state_type>, std::pair<uint32_t, fsa_algorithm::state_type>>> combinations;

    fsa_algorithm::build_equivalence_class_combinations(equivalence_class, combinations);

    for(const auto& combination : combinations)
    {
        std::wcout << (*combination.first.second).label().c_str() << L":" << (*combination.second.second).label().c_str() << std::endl;
    }
}

void test_minimize_fsa()
{
    // { A B }  { F }  { C D E }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_F, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"0", status);
    (*nfa).add_transition(state_A, state_C, L"1", status);

    (*nfa).add_transition(state_B, state_A, L"0", status);
    (*nfa).add_transition(state_B, state_D, L"1", status);

    (*nfa).add_transition(state_C, state_E, L"0", status);
    (*nfa).add_transition(state_C, state_F, L"1", status);

    (*nfa).add_transition(state_D, state_E, L"0", status);
    (*nfa).add_transition(state_D, state_F, L"1", status);

    (*nfa).add_transition(state_E, state_E, L"0", status);
    (*nfa).add_transition(state_E, state_F, L"1", status);

    (*nfa).add_transition(state_F, state_F, L"0", status);
    (*nfa).add_transition(state_F, state_F, L"1", status);

    // final
    (*nfa).add_final_state(state_C, status);
    (*nfa).add_final_state(state_D, status);
    (*nfa).add_final_state(state_E, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa, false);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa.dot)");
}

void test_minimize_fsa1()
{
    // { q0 q4 }  { q6 }  { q1 q7 }  { q3 q5 }  { q2 }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state0(factory::create<fsa_state>(L"q0", 0));
    fsa::state_type state1(factory::create<fsa_state>(L"q1", 0));
    fsa::state_type state2(factory::create<fsa_state>(L"q2", 0));
    fsa::state_type state3(factory::create<fsa_state>(L"q3", 0));
    fsa::state_type state4(factory::create<fsa_state>(L"q4", 0));
    fsa::state_type state5(factory::create<fsa_state>(L"q5", 0));
    fsa::state_type state6(factory::create<fsa_state>(L"q6", 0));
    fsa::state_type state7(factory::create<fsa_state>(L"q7", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state0, status);
    (*nfa).add_state(state1, status);
    (*nfa).add_state(state2, status);
    (*nfa).add_state(state3, status);
    (*nfa).add_state(state4, status);
    (*nfa).add_state(state5, status);
    (*nfa).add_state(state6, status);
    (*nfa).add_state(state7, status);

    // start
    (*nfa).start_state() = state0;

    // states
    (*nfa).add_transition(state0, state1, L"0", status);
    (*nfa).add_transition(state0, state5, L"1", status);

    (*nfa).add_transition(state1, state6, L"0", status);
    (*nfa).add_transition(state1, state2, L"1", status);

    (*nfa).add_transition(state2, state0, L"0", status);
    (*nfa).add_transition(state2, state2, L"1", status);

    (*nfa).add_transition(state3, state2, L"0", status);
    (*nfa).add_transition(state3, state6, L"1", status);

    (*nfa).add_transition(state4, state7, L"0", status);
    (*nfa).add_transition(state4, state5, L"1", status);

    (*nfa).add_transition(state5, state2, L"0", status);
    (*nfa).add_transition(state5, state6, L"1", status);

    (*nfa).add_transition(state6, state6, L"0", status);
    (*nfa).add_transition(state6, state4, L"1", status);

    (*nfa).add_transition(state7, state6, L"0", status);
    (*nfa).add_transition(state7, state2, L"1", status);

    // final
    (*nfa).add_final_state(state2, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa1.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa1.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa1.dot)");
}

void test_minimize_fsa2()
{
    // { A }  { D E }  { B C }  { G }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_G(factory::create<fsa_state>(L"G", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_G, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"0", status);
    (*nfa).add_transition(state_A, state_C, L"1", status);

    (*nfa).add_transition(state_B, state_D, L"0", status);
    (*nfa).add_transition(state_B, state_E, L"1", status);

    (*nfa).add_transition(state_C, state_E, L"0", status);
    (*nfa).add_transition(state_C, state_D, L"1", status);

    (*nfa).add_transition(state_D, state_G, L"0", status);
    (*nfa).add_transition(state_D, state_G, L"1", status);

    (*nfa).add_transition(state_E, state_G, L"0", status);
    (*nfa).add_transition(state_E, state_G, L"1", status);

    (*nfa).add_transition(state_G, state_G, L"0", status);
    (*nfa).add_transition(state_G, state_G, L"1", status);

    // final
    (*nfa).add_final_state(state_B, status);
    (*nfa).add_final_state(state_C, status);
    (*nfa).add_final_state(state_G, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa2.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa2.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa2.dot)");
}

void test_minimize_fsa3()
{
    // { C A }  { B }  { D }  { E }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"a", status);
    (*nfa).add_transition(state_A, state_C, L"b", status);

    (*nfa).add_transition(state_B, state_B, L"a", status);
    (*nfa).add_transition(state_B, state_D, L"b", status);

    (*nfa).add_transition(state_C, state_B, L"a", status);
    (*nfa).add_transition(state_C, state_C, L"b", status);

    (*nfa).add_transition(state_D, state_B, L"a", status);
    (*nfa).add_transition(state_D, state_E, L"b", status);

    (*nfa).add_transition(state_E, state_B, L"a", status);
    (*nfa).add_transition(state_E, state_C, L"b", status);

    // final
    (*nfa).add_final_state(state_E, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa3.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa3.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa3.dot)");
}

void test_minimize_fsa4()
{
    // { q0 q1 }  { q2 q3 }  { q7 }  { q4 }  { q5 q6 }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state0(factory::create<fsa_state>(L"q0", 0));
    fsa::state_type state1(factory::create<fsa_state>(L"q1", 0));
    fsa::state_type state2(factory::create<fsa_state>(L"q2", 0));
    fsa::state_type state3(factory::create<fsa_state>(L"q3", 0));
    fsa::state_type state4(factory::create<fsa_state>(L"q4", 0));
    fsa::state_type state5(factory::create<fsa_state>(L"q5", 0));
    fsa::state_type state6(factory::create<fsa_state>(L"q6", 0));
    fsa::state_type state7(factory::create<fsa_state>(L"q7", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state0, status);
    (*nfa).add_state(state1, status);
    (*nfa).add_state(state2, status);
    (*nfa).add_state(state3, status);
    (*nfa).add_state(state4, status);
    (*nfa).add_state(state5, status);
    (*nfa).add_state(state6, status);
    (*nfa).add_state(state7, status);

    // start
    (*nfa).start_state() = state3;

    // states
    (*nfa).add_transition(state0, state7, L"0", status);
    (*nfa).add_transition(state0, state1, L"1", status);

    (*nfa).add_transition(state1, state7, L"0", status);
    (*nfa).add_transition(state1, state0, L"1", status);

    (*nfa).add_transition(state2, state4, L"0", status);
    (*nfa).add_transition(state2, state5, L"1", status);

    (*nfa).add_transition(state3, state4, L"0", status);
    (*nfa).add_transition(state3, state5, L"1", status);

    (*nfa).add_transition(state4, state6, L"0", status);
    (*nfa).add_transition(state4, state6, L"1", status);

    (*nfa).add_transition(state5, state5, L"0", status);
    (*nfa).add_transition(state5, state5, L"1", status);

    (*nfa).add_transition(state6, state6, L"0", status);
    (*nfa).add_transition(state6, state5, L"1", status);

    (*nfa).add_transition(state7, state2, L"0", status);
    (*nfa).add_transition(state7, state2, L"1", status);

    // final
    (*nfa).add_final_state(state5, status);
    (*nfa).add_final_state(state6, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa5.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa5.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa5.dot)");
}

void test_minimize_fsa5()
{
    // https://www.cs.wcupa.edu/rkline/fcs/dfa-min.html
    // { 0 4 }  { 3 }  { 5 }  { 1 2 }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_0(factory::create<fsa_state>(L"0", 0));
    fsa::state_type state_1(factory::create<fsa_state>(L"1", 0));
    fsa::state_type state_2(factory::create<fsa_state>(L"2", 0));
    fsa::state_type state_3(factory::create<fsa_state>(L"3", 0));
    fsa::state_type state_4(factory::create<fsa_state>(L"4", 0));
    fsa::state_type state_5(factory::create<fsa_state>(L"5", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_0, status);
    (*nfa).add_state(state_1, status);
    (*nfa).add_state(state_2, status);
    (*nfa).add_state(state_3, status);
    (*nfa).add_state(state_4, status);
    (*nfa).add_state(state_5, status);

    // start
    (*nfa).start_state() = state_0;

    // states
    (*nfa).add_transition(state_0, state_3, L"a", status);
    (*nfa).add_transition(state_0, state_1, L"b", status);

    (*nfa).add_transition(state_1, state_4, L"a", status);
    (*nfa).add_transition(state_1, state_1, L"b", status);

    (*nfa).add_transition(state_2, state_0, L"a", status);
    (*nfa).add_transition(state_2, state_2, L"b", status);

    (*nfa).add_transition(state_3, state_5, L"a", status);
    (*nfa).add_transition(state_3, state_2, L"b", status);

    (*nfa).add_transition(state_4, state_3, L"a", status);
    (*nfa).add_transition(state_4, state_1, L"b", status);

    (*nfa).add_transition(state_5, state_5, L"a", status);
    (*nfa).add_transition(state_5, state_5, L"b", status);

    // final
    (*nfa).add_final_state(state_1, status);
    (*nfa).add_final_state(state_2, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa5.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa5.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa, false);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa5.dot)");
}

void test_minimize_fsa6()
{
    // Minimization+of+DFA’s+Example_+The+DFA+has+equivalence+classes.jpg
    // { A E }  { G }  { B H }  { D F }  { C }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));
    fsa::state_type state_G(factory::create<fsa_state>(L"G", 0));
    fsa::state_type state_H(factory::create<fsa_state>(L"H", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_F, status);
    (*nfa).add_state(state_G, status);
    (*nfa).add_state(state_H, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"0", status);
    (*nfa).add_transition(state_A, state_F, L"1", status);

    (*nfa).add_transition(state_B, state_G, L"0", status);
    (*nfa).add_transition(state_B, state_C, L"1", status);

    (*nfa).add_transition(state_C, state_E, L"0", status);
    (*nfa).add_transition(state_C, state_D, L"1", status);

    (*nfa).add_transition(state_D, state_C, L"0", status);
    (*nfa).add_transition(state_D, state_G, L"1", status);

    (*nfa).add_transition(state_E, state_H, L"0", status);
    (*nfa).add_transition(state_E, state_F, L"1", status);

    (*nfa).add_transition(state_F, state_C, L"0", status);
    (*nfa).add_transition(state_F, state_G, L"1", status);

    (*nfa).add_transition(state_G, state_G, L"0", status);
    (*nfa).add_transition(state_G, state_E, L"1", status);

    (*nfa).add_transition(state_H, state_G, L"0", status);
    (*nfa).add_transition(state_H, state_C, L"1", status);

    // final
    (*nfa).add_final_state(state_C, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa6.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa6.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa6.dot)");
}

void test_minimize_fsa7()
{
    // { P }  { Q }  { R }  { S T }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_P(factory::create<fsa_state>(L"P", 0));
    fsa::state_type state_Q(factory::create<fsa_state>(L"Q", 0));
    fsa::state_type state_R(factory::create<fsa_state>(L"R", 0));
    fsa::state_type state_S(factory::create<fsa_state>(L"S", 0));
    fsa::state_type state_T(factory::create<fsa_state>(L"T", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_P, status);
    (*nfa).add_state(state_Q, status);
    (*nfa).add_state(state_R, status);
    (*nfa).add_state(state_S, status);
    (*nfa).add_state(state_T, status);

    // start
    (*nfa).start_state() = state_P;

    // states
    (*nfa).add_transition(state_P, state_S, L"a", status);
    (*nfa).add_transition(state_P, state_Q, L"b", status);

    (*nfa).add_transition(state_Q, state_T, L"a", status);
    (*nfa).add_transition(state_Q, state_R, L"b", status);

    (*nfa).add_transition(state_R, state_R, L"a", status);
    (*nfa).add_transition(state_R, state_R, L"b", status);

    (*nfa).add_transition(state_S, state_S, L"a", status);
    (*nfa).add_transition(state_S, state_S, L"b", status);

    (*nfa).add_transition(state_T, state_T, L"a", status);
    (*nfa).add_transition(state_T, state_T, L"b", status);

    // final
    (*nfa).add_final_state(state_S, status);
    (*nfa).add_final_state(state_T, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa7.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa7.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa7.dot)");
}

void test_minimize_fsa8()
{
    // https://studfiles.net/preview/4199559/page:6/
    // { q0 }  { q1 q2 q5 }  { q3 q4 }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state0(factory::create<fsa_state>(L"q0", 0));
    fsa::state_type state1(factory::create<fsa_state>(L"q1", 0));
    fsa::state_type state2(factory::create<fsa_state>(L"q2", 0));
    fsa::state_type state3(factory::create<fsa_state>(L"q3", 0));
    fsa::state_type state4(factory::create<fsa_state>(L"q4", 0));
    fsa::state_type state5(factory::create<fsa_state>(L"q5", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state0, status);
    (*nfa).add_state(state1, status);
    (*nfa).add_state(state2, status);
    (*nfa).add_state(state3, status);
    (*nfa).add_state(state4, status);
    (*nfa).add_state(state5, status);

    // start
    (*nfa).start_state() = state0;

    // states
    (*nfa).add_transition(state0, state1, L"a", status);
    (*nfa).add_transition(state0, state2, L"b", status);

    (*nfa).add_transition(state1, state2, L"a", status);
    (*nfa).add_transition(state1, state3, L"b", status);

    (*nfa).add_transition(state2, state2, L"a", status);
    (*nfa).add_transition(state2, state4, L"b", status);

    (*nfa).add_transition(state3, state3, L"a", status);
    (*nfa).add_transition(state3, state3, L"b", status);

    (*nfa).add_transition(state4, state4, L"a", status);
    (*nfa).add_transition(state4, state4, L"b", status);

    (*nfa).add_transition(state5, state5, L"a", status);
    (*nfa).add_transition(state5, state4, L"b", status);

    // final
    (*nfa).add_final_state(state3, status);
    (*nfa).add_final_state(state4, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa8.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa8.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa8.dot)");
}

void test_minimize_fsa9()
{
    // https://www.intuit.ru/studies/courses/26/26/lecture/801?page=4
    // { A C }  { B }  { D }  { F }  { E }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_F, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"a", status);
    (*nfa).add_transition(state_A, state_C, L"b", status);

    (*nfa).add_transition(state_B, state_B, L"a", status);
    (*nfa).add_transition(state_B, state_D, L"b", status);

    (*nfa).add_transition(state_C, state_B, L"a", status);
    (*nfa).add_transition(state_C, state_C, L"b", status);

    (*nfa).add_transition(state_D, state_B, L"a", status);
    (*nfa).add_transition(state_D, state_E, L"b", status);

    (*nfa).add_transition(state_E, state_B, L"a", status);
    (*nfa).add_transition(state_E, state_C, L"b", status);

    (*nfa).add_transition(state_F, state_E, L"a", status);

    // final
    (*nfa).add_final_state(state_E, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa9.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa9.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa, false);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa9.dot)");
}

void test_minimize_fsa10()
{
    // https://lektsia.com/3x141b.html
    // { A }  { B C }  { D  E }, after remove useless states
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_A(factory::create<fsa_state>(L"A", 0));
    fsa::state_type state_B(factory::create<fsa_state>(L"B", 0));
    fsa::state_type state_C(factory::create<fsa_state>(L"C", 0));
    fsa::state_type state_D(factory::create<fsa_state>(L"D", 0));
    fsa::state_type state_E(factory::create<fsa_state>(L"E", 0));
    fsa::state_type state_F(factory::create<fsa_state>(L"F", 0));
    fsa::state_type state_G(factory::create<fsa_state>(L"G", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_A, status);
    (*nfa).add_state(state_B, status);
    (*nfa).add_state(state_C, status);
    (*nfa).add_state(state_D, status);
    (*nfa).add_state(state_E, status);
    (*nfa).add_state(state_F, status);
    (*nfa).add_state(state_G, status);

    // start
    (*nfa).start_state() = state_A;

    // states
    (*nfa).add_transition(state_A, state_B, L"a", status);
    (*nfa).add_transition(state_A, state_C, L"b", status);

    (*nfa).add_transition(state_B, state_D, L"a", status);

    (*nfa).add_transition(state_C, state_E, L"a", status);

    (*nfa).add_transition(state_D, state_C, L"a", status);
    (*nfa).add_transition(state_D, state_E, L"b", status);

    (*nfa).add_transition(state_E, state_B, L"a", status);
    (*nfa).add_transition(state_E, state_D, L"b", status);

    (*nfa).add_transition(state_F, state_D, L"a", status);
    (*nfa).add_transition(state_F, state_G, L"b", status);

    (*nfa).add_transition(state_G, state_F, L"a", status);
    (*nfa).add_transition(state_G, state_E, L"b", status);

    // final
    (*nfa).add_final_state(state_D, status);
    (*nfa).add_final_state(state_E, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa10.dot)");

    fsa::fsa_type dfa1;

    fsa_algorithm::remove_useless_states(nfa, dfa1);

    fsa_visualization::generate_graphviz_file(dfa1, LR"(d:\tmp\FSA\dfa10.rmd.dot)");

    dump_equivalence_classes(dfa1);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa10.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa, false);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa10.dot)");
}

void test_minimize_fsa11()
{
    // http://sovietov.com/txt/minfa/minfa.html
    // { 0 }  { 1 2 }  { 3 }
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status;

    fsa::state_type state_0(factory::create<fsa_state>(L"0", 0));
    fsa::state_type state_1(factory::create<fsa_state>(L"1", 0));
    fsa::state_type state_2(factory::create<fsa_state>(L"2", 0));
    fsa::state_type state_3(factory::create<fsa_state>(L"3", 0));

    fsa::fsa_type nfa(factory::create<fsa>());

    (*nfa).add_state(state_0, status);
    (*nfa).add_state(state_1, status);
    (*nfa).add_state(state_2, status);
    (*nfa).add_state(state_3, status);

    // start
    (*nfa).start_state() = state_0;

    // states
    (*nfa).add_transition(state_0, state_1, L"a", status);
    (*nfa).add_transition(state_0, state_2, L"a", status);
    (*nfa).add_transition(state_0, state_2, L"b", status);

    (*nfa).add_transition(state_1, state_2, L"a", status);
    (*nfa).add_transition(state_1, state_3, L"b", status);

    (*nfa).add_transition(state_2, state_1, L"a", status);
    (*nfa).add_transition(state_2, state_2, L"a", status);
    (*nfa).add_transition(state_2, state_3, L"b", status);

    // final
    (*nfa).add_final_state(state_3, status);

    fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\nfa11.dot)");

    dump_equivalence_classes(nfa);

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\dfa11.dot)");

    fsa::fsa_type mindfa;

    fsa_algorithm::minimize_dfa(dfa, mindfa, false);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\mindfa11.dot)");
}

string_type cpp_binary_re(L"0(b|B)(0|1)('?(0|1))*(((u|U)((l|L)(l|L)?)?)|(((l|L)(l|L)?)(u|U)?))?");
string_type cpp_octal_re(L"0('?(0|1|2|3|4|5|6|7))*(((u|U)((l|L)(l|L)?)?)|(((l|L)(l|L)?)(u|U)?))?");
string_type cpp_decimal_re(L"(1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*(((u|U)((l|L)(l|L)?)?)|(((l|L)(l|L)?)(u|U)?))?");
string_type cpp_hexadecimal_re(L"0(x|X)(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)('?(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*(((u|U)((l|L)(l|L)?)?)|(((l|L)(l|L)?)(u|U)?))?");
string_type cpp_float_re(L"(((((((0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)?)\\.((0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*))|(((0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)\\.))((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)?)|(((0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)))(f|l|F|L)?");
string_type cpp_hexfloat_re(L"0(x|X)(((0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)('?(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)*\\.(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)('?(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)('?(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*\\.|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)('?(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)((p|P)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)('?(0|1|2|3|4|5|6|7|8|9))*)(f|l|F|L)?");


string_type java_binary_re(L"0(b|B)(0|1)(_*(0|1))*(l|L)?");
string_type java_octal_re(L"0(((0|1|2|3|4|5|6|7)*)_*(0|1|2|3|4|5|6|7))+(l|L)?");
string_type java_decimal_re(L"((0)|(((1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)))(l|L)?");
string_type java_hexadecimal_re(L"0(x|X)(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*(l|L)?");
string_type java_float_re(L"(((((((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)?)\\.((0|1|2|3|4|5|6|7|8|9)(_*?(0|1|2|3|4|5|6|7|8|9))*))|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)\\.))((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)?)|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)))(f|d|F|D)?|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)(f|d|F|D))");
string_type java_hexfloat_re(L"0(x|X)(((0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)*\\.(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*\\.|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)((p|P)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)(f|d|F|D)?");


void test_binary_lit_cpp()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_binary_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_binary_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::binary_integer_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_binary_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_binary_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    std::wcout << (*(*mindfa).start_state()).label() << L":" << (*(*mindfa).start_state()).id() << std::endl;

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_binary_lit.dot)");

    std::vector<string_type> inputs
    (
        {
            L"0b1'1",
            L"0b101uL",
            L"0B10'1uL",
            L"0B000'111'010110'1lLu",
            L"0b100'000000'000000'000000",
            L"0B01101001",
            L"0B0101'0111'1100",
            L"0b1011'0010",
            L"0b1u",
            L"0b1ul",
            L"0b1ull",
            L"0b1uL",
            L"0b1uLl",
            L"0b1ulL",
            L"0b1lu",
            L"0b1llu",
            L"0b1LLu",
            L"0b1lLu",
            L"0b1Llu",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_octal_integer_lit()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_octal_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_octal_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::octal_integer_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_octal_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_octal_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    std::wcout << (*(*mindfa).start_state()).label() << L":" << (*(*mindfa).start_state()).id() << std::endl;

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_octal_lit.dot)");

    std::vector<string_type> inputs
    (
        {
            L"0",
            L"00",
            L"0012345671u",
            L"01'2",
            L"0101uL",
            L"010'1uL",
            L"0000'111'010110'1lLu",
            L"0100'000000'000000'000000",
            L"001101001",
            L"00101'0111'1100",
            L"01011'0010",
            L"01u",
            L"01ul",
            L"01ull",
            L"01uL",
            L"01uLl",
            L"01ulL",
            L"01lu",
            L"01llu",
            L"01LLu",
            L"01lLu",
            L"01Llu",
            L"052",
            L"052ULl",
            L"052Ul",
            L"052uLl",
            L"052Llu",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_decimal_integer_lit()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_decimal_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_decimal_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::decimal_integer_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_decimal_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_decimal_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    std::wcout << (*(*mindfa).start_state()).label() << L":" << (*(*mindfa).start_state()).id() << std::endl;

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_decimal_lit.dot)");

    std::vector<string_type> inputs
    (
        {
            L"1",
            L"1023456789",
            L"1023456789'1023456789'1023456789'1023456789'1023456789",
            L"101uL",
            L"10'1uL",
            L"100'000000'000000'000000",
            L"1011'0010",
            L"1u",
            L"1ul",
            L"1ull",
            L"1uL",
            L"1uLl",
            L"1ulL",
            L"1lu",
            L"1llu",
            L"1LLu",
            L"1lLu",
            L"1Llu",
            L"52",
            L"52ULl",
            L"52Ul",
            L"52uLl",
            L"52Llu",
            L"5'02Llu",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_hexadecimal_integer_lit()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_hexadecimal_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_hexadecimal_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_integer_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_hexadecimal_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_hexadecimal_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_hexadecimal_lit.dot)");

    std::vector<string_type> inputs
    (
        {
            L"0x1",
            L"0x10'1uL",
            L"0x100'000000'000000'000000",
            L"0x1011'0010",
            L"0x1u",
            L"0x1ul",
            L"0x1ull",
            L"0x1uL",
            L"0x1uLl",
            L"0x1ulL",
            L"0x1lu",
            L"0x1llu",
            L"0xABCDEF1LLu",
            L"0xabcdef1lLu",
            L"0x75368431Llu",
            L"0xa'e'ddDDABA'D52",
            L"0X52ULl",
            L"0x52Ul",
            L"0x52uLl",
            L"0x52Llu",
            L"0x5'02Llu",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_decimal_floating_lit()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_decimal_float_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_float_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::decimal_floating_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_decimal_floating_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_decimal_floating_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_decimal_floating_lit.dot)");

    std::vector<string_type> inputs
    (
        {
            L".322'243'2E-2'4",
            L"0.5",
            L"1.322'243'2E-2'4",
            L"1e10",
            L"1e-5L",
            L"1.",
            L"1.e-2",
            L"3.14",
            L".1f",
            L"0.1e-1L",
            L"1.23456e-65",
            L"123.456e-67",
            L".1E4f",
            L"58.",
            L"5'8.",
            L"4e2",
            L"4e-2",
            L"4e+2'3'4L",
            L"1e0",
            L"1e+0",
            L"1e-0",
            L".1e0",
            L"5.1e0",
            L"1e1f",
            L"2.f",
            L".3f",
            //L"0f", illegal
            L"3.14f",
            L"6.022137e+23f",
            L"1e1",
            L"2.",
            L".3",
            L"0.0",
            L"0.",
            L"3.14",
            L"1e-9l",
            L"1e137",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_hexadecimal_floating_lit()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    std::wcout << L"test_hexadecimal_float_lit_cpp" << std::endl;

    operation_status status; //??

    string_type re(cpp_hexfloat_re);

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;

    text::string_to_codepoints(re, re_data, count, status);

    fsa::fsa_type nfa;

    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_floating_lit), 0, empty_string(), nfa, status);

    //fsa_visualization::generate_graphviz_file(nfa, LR"(d:\tmp\FSA\cpp_nfa_hexadecimal_floating_lit.dot)");

    fsa::fsa_type dfa;

    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    //fsa_visualization::generate_graphviz_file(dfa, LR"(d:\tmp\FSA\cpp_dfa_hexadecimal_floating_lit.dot)");

    fsa::fsa_type mindfa;

    //dump_equivalence_classes(dfa);

    fsa_algorithm::minimize_dfa(dfa, mindfa);

    //fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\cpp_mindfa_hexadecimal_floating_lit.dot)");

    double v1 = 0x1.2p3; v1;
    double v8 = 0x1.2p3f; v8;
    double v2 = 0x1ffp10; v2;
    double v3 = 0X0p-1; v3;
    double v4 = 0x1.p0; v4;
    double v5 = 0xf.p-1; v5;
    double v6 = 0x0.123p-1; v6;
    double v7 = 0xa.bp10l; v7;
    double v10 = 0x0.12345p+012345f; v10;
    double v11 = 0x0.12345p+01'23'45F; v11;
    double v12 = 0x0.12345p-012345l; v12;
    double v15 = 0x0.12345p-01'23'45L; v15;

    std::vector<string_type> inputs
    (
        {
            L"0x1.2p3",
            L"0x1.2p3f",
            L"0x1.2p3F",
            L"0x1.2p3L",
            L"0x1.2p3l",
            L"0x1ffp10",
            L"0X0p-1",
            L"0x1.p0",
            L"0xf.p-1",
            L"0x0.123p-1",
            L"0xa.bp10l",
            L"0x0.12345p+012345f",
            L"0x0.12345p+01'23'45F",
            L"0x0.12345p-012345l",
            L"0x0.12345p-01'23'45L",
            L"0x056874356357329290.17598574375875984352345p+0123983758437598439845L",
            L"0x05'68'7'4'356'357'329'290.1'7'59857'4'3'7587'5984'3'5'234'5p+0'12'39837'584'375'984'398'4'5L",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_cpp_literals()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status; //??

    std::shared_ptr<fsa_re::datum_type[]> binary_re_data;
    size_type binary_count = 0;
    text::string_to_codepoints(cpp_binary_re, binary_re_data, binary_count, status);
    fsa::fsa_type binary_nfa;
    fsa_re::re_to_fsa(binary_re_data, binary_count, static_cast<uint32_t>(my_token_traits::type::binary_integer_lit), 0, empty_string(), binary_nfa, status);
    fsa::fsa_type binary_dfa;
    fsa_algorithm::nfa_to_dfa(binary_nfa, binary_dfa, status);
    fsa::fsa_type binary_mindfa;
    fsa_algorithm::minimize_dfa(binary_dfa, binary_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> octal_re_data;
    size_type octal_count = 0;
    text::string_to_codepoints(cpp_octal_re, octal_re_data, octal_count, status);
    fsa::fsa_type octal_nfa;
    fsa_re::re_to_fsa(octal_re_data, octal_count, static_cast<uint32_t>(my_token_traits::type::octal_integer_lit), 0, empty_string(), octal_nfa, status);
    fsa::fsa_type octal_dfa;
    fsa_algorithm::nfa_to_dfa(octal_nfa, octal_dfa, status);
    fsa::fsa_type octal_mindfa;
    fsa_algorithm::minimize_dfa(octal_dfa, octal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> decimal_re_data;
    size_type decimal_count = 0;
    text::string_to_codepoints(cpp_decimal_re, decimal_re_data, decimal_count, status);
    fsa::fsa_type decimal_nfa;
    fsa_re::re_to_fsa(decimal_re_data, decimal_count, static_cast<uint32_t>(my_token_traits::type::decimal_integer_lit), 0, empty_string(), decimal_nfa, status);
    fsa::fsa_type decimal_dfa;
    fsa_algorithm::nfa_to_dfa(decimal_nfa, decimal_dfa, status);
    fsa::fsa_type decimal_mindfa;
    fsa_algorithm::minimize_dfa(decimal_dfa, decimal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> hexadecimal_re_data;
    size_type hexadecimal_count = 0;
    text::string_to_codepoints(cpp_hexadecimal_re, hexadecimal_re_data, hexadecimal_count, status);
    fsa::fsa_type hexadecimal_nfa;
    fsa_re::re_to_fsa(hexadecimal_re_data, hexadecimal_count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_integer_lit), 0, empty_string(), hexadecimal_nfa, status);
    fsa::fsa_type hexadecimal_dfa;
    fsa_algorithm::nfa_to_dfa(hexadecimal_nfa, hexadecimal_dfa, status);
    fsa::fsa_type hexadecimal_mindfa;
    fsa_algorithm::minimize_dfa(hexadecimal_dfa, hexadecimal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> decimal_float_re_data;
    size_type decimal_float_count = 0;
    text::string_to_codepoints(cpp_float_re, decimal_float_re_data, decimal_float_count, status);
    fsa::fsa_type decimal_float_nfa;
    fsa_re::re_to_fsa(decimal_float_re_data, decimal_float_count, static_cast<uint32_t>(my_token_traits::type::decimal_floating_lit), 0, empty_string(), decimal_float_nfa, status);
    fsa::fsa_type decimal_float_dfa;
    fsa_algorithm::nfa_to_dfa(decimal_float_nfa, decimal_float_dfa, status);
    fsa::fsa_type decimal_float_mindfa;
    fsa_algorithm::minimize_dfa(decimal_float_dfa, decimal_float_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> hexadecimal_float_re_data;
    size_type hexadecimal_float_count = 0;
    text::string_to_codepoints(cpp_hexfloat_re, hexadecimal_float_re_data, hexadecimal_float_count, status);
    fsa::fsa_type hexadecimal_float_nfa;
    fsa_re::re_to_fsa(hexadecimal_float_re_data, hexadecimal_float_count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_floating_lit), 0, empty_string(), hexadecimal_float_nfa, status);
    fsa::fsa_type hexadecimal_float_dfa;
    fsa_algorithm::nfa_to_dfa(hexadecimal_float_nfa, hexadecimal_float_dfa, status);
    fsa::fsa_type hexadecimal_float_mindfa;
    fsa_algorithm::minimize_dfa(hexadecimal_float_dfa, hexadecimal_float_mindfa);

    fsa::fsa_type binary_octal_nfa;
    fsa::combine(binary_mindfa, octal_mindfa, binary_octal_nfa, status);

    fsa::fsa_type decimal_hexadecimal_nfa;
    fsa::combine(decimal_mindfa, hexadecimal_mindfa, decimal_hexadecimal_nfa, status);

    fsa::fsa_type decimal_hexadecimal_float_nfa;
    fsa::combine(decimal_float_mindfa, hexadecimal_float_mindfa, decimal_hexadecimal_float_nfa, status);

    fsa::fsa_type integer_nfa;
    fsa::combine(binary_octal_nfa, decimal_hexadecimal_nfa, integer_nfa, status);

    fsa::fsa_type literals_nfa;
    fsa::combine(integer_nfa, decimal_hexadecimal_float_nfa, literals_nfa, status);

    fsa::fsa_type literals_dfa;
    fsa_algorithm::nfa_to_dfa(literals_nfa, literals_dfa, status);

    fsa::fsa_type literals_mindfa;
    fsa_algorithm::minimize_dfa(literals_dfa, literals_mindfa);

    fsa_visualization::generate_graphviz_file(literals_mindfa, LR"(d:\tmp\FSA\cpp_lits.dot)");

    std::vector<string_type> inputs
    (
        {
            L"0b01",        // binary
            L"0",           // octal
            L"00",          // octal
            L"1",           // decimal       
            L"10",          // decimal       
            L"0x0A",        // hexadecimal
            L"0.5E+5",      // decimal floating
            L"0xABC.EFP+5", // hexadecimal floating

            L"0b1'1",
            L"0b101uL",
            L"0B10'1uL",
            L"0B000'111'010110'1lLu",
            L"0b100'000000'000000'000000",
            L"0B01101001",
            L"0B0101'0111'1100",
            L"0b1011'0010",
            L"0b1u",
            L"0b1ul",
            L"0b1ull",
            L"0b1uL",
            L"0b1uLl",
            L"0b1ulL",
            L"0b1lu",
            L"0b1llu",
            L"0b1LLu",
            L"0b1lLu",
            L"0b1Llu",

            L"0012345671u",
            L"01'2",
            L"0101uL",
            L"010'1uL",
            L"0000'111'010110'1lLu",
            L"0100'000000'000000'000000",
            L"001101001",
            L"00101'0111'1100",
            L"01011'0010",
            L"01u",
            L"01ul",
            L"01ull",
            L"01uL",
            L"01uLl",
            L"01ulL",
            L"01lu",
            L"01llu",
            L"01LLu",
            L"01lLu",
            L"01Llu",
            L"052",
            L"052ULl",
            L"052Ul",
            L"052uLl",
            L"052Llu",

            L"1023456789",
            L"1023456789'1023456789'1023456789'1023456789'1023456789",
            L"101uL",
            L"10'1uL",
            L"100'000000'000000'000000",
            L"1011'0010",
            L"1u",
            L"1ul",
            L"1ull",
            L"1uL",
            L"1uLl",
            L"1ulL",
            L"1lu",
            L"1llu",
            L"1LLu",
            L"1lLu",
            L"1Llu",
            L"52",
            L"52ULl",
            L"52Ul",
            L"52uLl",
            L"52Llu",
            L"5'02Llu",

            L"0x1",
            L"0x10'1uL",
            L"0x100'000000'000000'000000",
            L"0x1011'0010",
            L"0x1u",
            L"0x1ul",
            L"0x1ull",
            L"0x1uL",
            L"0x1uLl",
            L"0x1ulL",
            L"0x1lu",
            L"0x1llu",
            L"0xABCDEF1LLu",
            L"0xabcdef1lLu",
            L"0x75368431Llu",
            L"0xa'e'ddDDABA'D52",
            L"0X52ULl",
            L"0x52Ul",
            L"0x52uLl",
            L"0x52Llu",
            L"0x5'02Llu",

            L".322'243'2E-2'4",
            L"0.5",
            L"1.322'243'2E-2'4",
            L"1e10",
            L"1e-5L",
            L"1.",
            L"1.e-2",
            L"3.14",
            L".1f",
            L"0.1e-1L",
            L"1.23456e-65",
            L"123.456e-67",
            L".1E4f",
            L"58.",
            L"5'8.",
            L"4e2",
            L"4e-2",
            L"4e+2'3'4L",
            L"1e0",
            L"1e+0",
            L"1e-0",
            L".1e0",
            L"5.1e0",

            L"0x1.2p3",
            L"0x1.2p3f",
            L"0x1.2p3F",
            L"0x1.2p3L",
            L"0x1.2p3l",
            L"0x1ffp10",
            L"0X0p-1",
            L"0x1.p0",
            L"0xf.p-1",
            L"0x0.123p-1",
            L"0xa.bp10l",
            L"0x0.12345p+012345f",
            L"0x0.12345p+01'23'45F",
            L"0x0.12345p-012345l",
            L"0x0.12345p-01'23'45L",
            L"0x056874356357329290.17598574375875984352345p+0123983758437598439845L",
            L"0x05'68'7'4'356'357'329'290.1'7'59857'4'3'7587'5984'3'5'234'5p+0'12'39837'584'375'984'398'4'5L",
        }
    );

    validate_dfa(inputs, literals_mindfa);
}

void test_java_literals()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status; //??

    std::shared_ptr<fsa_re::datum_type[]> binary_re_data;
    size_type binary_count = 0;
    text::string_to_codepoints(java_binary_re, binary_re_data, binary_count, status);
    fsa::fsa_type binary_nfa;
    fsa_re::re_to_fsa(binary_re_data, binary_count, static_cast<uint32_t>(my_token_traits::type::binary_integer_lit), 0, empty_string(), binary_nfa, status);
    fsa::fsa_type binary_dfa;
    fsa_algorithm::nfa_to_dfa(binary_nfa, binary_dfa, status);
    fsa::fsa_type binary_mindfa;
    fsa_algorithm::minimize_dfa(binary_dfa, binary_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> octal_re_data;
    size_type octal_count = 0;
    text::string_to_codepoints(java_octal_re, octal_re_data, octal_count, status);
    fsa::fsa_type octal_nfa;
    fsa_re::re_to_fsa(octal_re_data, octal_count, static_cast<uint32_t>(my_token_traits::type::octal_integer_lit), 0, empty_string(), octal_nfa, status);
    fsa::fsa_type octal_dfa;
    fsa_algorithm::nfa_to_dfa(octal_nfa, octal_dfa, status);
    fsa::fsa_type octal_mindfa;
    fsa_algorithm::minimize_dfa(octal_dfa, octal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> decimal_re_data;
    size_type decimal_count = 0;
    text::string_to_codepoints(java_decimal_re, decimal_re_data, decimal_count, status);
    fsa::fsa_type decimal_nfa;
    fsa_re::re_to_fsa(decimal_re_data, decimal_count, static_cast<uint32_t>(my_token_traits::type::decimal_integer_lit), 0, empty_string(), decimal_nfa, status);
    fsa::fsa_type decimal_dfa;
    fsa_algorithm::nfa_to_dfa(decimal_nfa, decimal_dfa, status);
    fsa::fsa_type decimal_mindfa;
    fsa_algorithm::minimize_dfa(decimal_dfa, decimal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> hexadecimal_re_data;
    size_type hexadecimal_count = 0;
    text::string_to_codepoints(java_hexadecimal_re, hexadecimal_re_data, hexadecimal_count, status);
    fsa::fsa_type hexadecimal_nfa;
    fsa_re::re_to_fsa(hexadecimal_re_data, hexadecimal_count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_integer_lit), 0, empty_string(), hexadecimal_nfa, status);
    fsa::fsa_type hexadecimal_dfa;
    fsa_algorithm::nfa_to_dfa(hexadecimal_nfa, hexadecimal_dfa, status);
    fsa::fsa_type hexadecimal_mindfa;
    fsa_algorithm::minimize_dfa(hexadecimal_dfa, hexadecimal_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> decimal_float_re_data;
    size_type decimal_float_count = 0;
    text::string_to_codepoints(java_float_re, decimal_float_re_data, decimal_float_count, status);
    fsa::fsa_type decimal_float_nfa;
    fsa_re::re_to_fsa(decimal_float_re_data, decimal_float_count, static_cast<uint32_t>(my_token_traits::type::decimal_floating_lit), 0, empty_string(), decimal_float_nfa, status);
    fsa::fsa_type decimal_float_dfa;
    fsa_algorithm::nfa_to_dfa(decimal_float_nfa, decimal_float_dfa, status);
    fsa::fsa_type decimal_float_mindfa;
    fsa_algorithm::minimize_dfa(decimal_float_dfa, decimal_float_mindfa);

    std::shared_ptr<fsa_re::datum_type[]> hexadecimal_float_re_data;
    size_type hexadecimal_float_count = 0;
    text::string_to_codepoints(java_hexfloat_re, hexadecimal_float_re_data, hexadecimal_float_count, status);
    fsa::fsa_type hexadecimal_float_nfa;
    fsa_re::re_to_fsa(hexadecimal_float_re_data, hexadecimal_float_count, static_cast<uint32_t>(my_token_traits::type::hexadecimal_floating_lit), 0, empty_string(), hexadecimal_float_nfa, status);
    fsa::fsa_type hexadecimal_float_dfa;
    fsa_algorithm::nfa_to_dfa(hexadecimal_float_nfa, hexadecimal_float_dfa, status);
    fsa::fsa_type hexadecimal_float_mindfa;
    fsa_algorithm::minimize_dfa(hexadecimal_float_dfa, hexadecimal_float_mindfa);

    fsa::fsa_type binary_octal_nfa;
    fsa::combine(binary_mindfa, octal_mindfa, binary_octal_nfa, status);

    fsa::fsa_type hexadecimal_decimal_nfa;
    fsa::combine(decimal_mindfa, hexadecimal_mindfa, hexadecimal_decimal_nfa, status);

    fsa::fsa_type decimal_hexadecimal_float_nfa;
    fsa::combine(decimal_float_mindfa, hexadecimal_float_mindfa, decimal_hexadecimal_float_nfa, status);

    fsa::fsa_type integer_nfa;
    fsa::combine(binary_octal_nfa, hexadecimal_decimal_nfa, integer_nfa, status);

    fsa::fsa_type literals_nfa;
    fsa::combine(integer_nfa, decimal_hexadecimal_float_nfa, literals_nfa, status);

    fsa::fsa_type literals_dfa;
    fsa_algorithm::nfa_to_dfa(literals_nfa, literals_dfa, status);

    fsa::fsa_type literals_mindfa;
    fsa_algorithm::minimize_dfa(literals_dfa, literals_mindfa);

    fsa_visualization::generate_graphviz_file(literals_mindfa, LR"(d:\tmp\FSA\java_lits.dot)");

    fsa_codegen::generate<token<my_token_traits>>(literals_mindfa, LR"(d:\tmp\fsa.gen.txt)"); //??

    std::vector<string_type> inputs
    (
        {
            L"0b01",        // binary
            L"0",           // decimal
            L"00",          // octal
            L"1",           // decimal       
            L"10",          // decimal       
            L"0x0A",        // hexadecimal
            L"0.5E+5",      // decimal floating
            L"0x0.5P+5",    // hexadecimal floating
            L"0xABC.EFP+5", // hexadecimal floating

            L"0b1_1",
            L"0b101l",
            L"0B10_1l",
            L"0B000_111_010110_1l",
            L"0b100_000000_000000_000000",
            L"0B01101001",
            L"0B0101_0111_1100",
            L"0b1011_0010",
            L"0b1",
            L"0b1l",

            L"0",  // decimal
            L"00", // octal
            L"0012345671",
            L"01_2",
            L"0101l",
            L"010_1l",
            L"0000_111_010110_1l",
            L"0100_000000_000000_000000",
            L"001101001",
            L"00101_0111_1100",
            L"01011_0010",
            L"01",
            L"01l",
            L"052",
            L"052l",
            L"052l",
            L"052l",
            L"052l",

            L"1023456789",
            L"1023456789_1023456789_1023456789_1023456789_1023456789",
            L"101l",
            L"10_1l",
            L"100_000000_000000_000000",
            L"1011_0010",
            L"1",
            L"1l",
            L"52",
            L"52l",
            L"52l",
            L"52l",
            L"52l",
            L"5_02l",

            L"0x1",
            L"0x10_1l",
            L"0x100_000000_000000_000000",
            L"0x1011_0010",
            L"0x1",
            L"0x1l",
            L"0x1L",
            L"0xABCDEF1l",
            L"0xabcdef1l",
            L"0x75368431l",
            L"0xa_e_ddDDABA_D52",
            L"0X52l",
            L"0x52l",
            L"0x52l",
            L"0x52l",
            L"0x5_02l",

            L".322_243_2E-2_4",
            L"0.5",
            L"1.322_243_2E-2_4",
            L"1e10",
            L"1e-5D",
            L"1.",
            L"1.e-2",
            L"3.14",
            L".1f",
            L"0.1e-1D",
            L"1.23456e-65",
            L"123.456e-67",
            L".1E4f",
            L"58.",
            L"5_8.d",
            L"4e2d",
            L"4e-2f",
            L"4e+2_3_4D",
            L"1e0d",
            L"1e+0d",
            L"1e-0F",
            L".1e0",
            L"5.1e0",

            L"0x1.2p3",
            L"0x1.2p3f",
            L"0x1.2p3F",
            L"0x1.2p3D",
            L"0x1.2p3d",
            L"0x1ffp10",
            L"0X0p-1",
            L"0x1.p0",
            L"0xf.p-1",
            L"0x0.123p-1",
            L"0xa.bp10d",
            L"0x0.12345p+012345f",
            L"0x0.12345p+01_23_45F",
            L"0x0.12345p-012345d",
            L"0x0.12345p-01_23_45D",
            L"0x056874________356357329290.175985_____74375875984352345p+012398375843_____7598439845D",
            L"0x05_68_7_4_356_357_329_290.1_7_59857_4_3_7587_5984_3_5_234_5p+0_12_39837_584_375_984_398_4_5D",

            L"0",
            L"2",
            L"0372",
            L"0xDada_Cafe",
            L"1996",
            L"0x00_FF__00_FF",
            L"0l",
            L"0777L",
            L"0x100000000L",
            L"2_147_483_648L",
            L"0xC0B0L",
            L"1e1f",
            L"2.f",
            L".3f",
            L"0f",
            L"3.14f",
            L"6.022137e+23f",
            L"1e1",
            L"2.",
            L".3",
            L"0.0",
            L"0.",
            L"3.14",
            L"1e-9d",
            L"1e137",
            L"1.0E1",
            L"1E-1D",
            L"1.0e10f",
            L"0x1.2p3",
            L"0x1.2p3f",
            L"0x1.2p3F",
            L"0x1.2p3D",
            L"0x1.2p3d",
            L"0x1ffp10",
            L"0X0p-1",
            L"0x1.p0",
            L"0xf.p-1",
            L"0x0.123p-1",
            L"0xa.bp10d",
            L"0x0.12345p+012345f",
            L"0x0.12345p+01_23_45F",
            L"0x0.12345p-012345d",
            L"0x0.12345p-01_23_45D",
            L"0x056874356357329290.17598574375875984352345p+0123983758437598439845D",
            L"0x05_68_7_4_356_357_329_290.1_7_59857_4_3_7587_5984_3_5_234_5p+0_12_39837_584_375_984_398_4_5D",
            L"0x12.2P2",
            L"0x1.fffffffffffffp1023",
            L"0x0.DAB789Bp0",
            L"0x0.0p0f",
            L"0xff.0p19",
            L"0X1.921FB4D12D84AP+1",
            L"0x1.999_9_99999999ap-4",
            L"0x1.000000_0_0_00001p-1",
            L"0x1.0000000000002p-1",
            L"0x1.f44abd5aa7ca3p+5",
            L"0xFp3",
            L"0XF.1P3",
            L"0x1p1",
            L"0x1p3",
            L"0x0.0p0f",
            L"0xff.0p19",
        }
    );

    validate_dfa(inputs, literals_mindfa);
}

void build_fsa_from_re(const string_type& re, my_token_traits::type token, fsa::fsa_type& result_fsa)
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status; //??

    std::shared_ptr<fsa_re::datum_type[]> re_data;
    size_type count = 0;
    text::string_to_codepoints(re, re_data, count, status);
    fsa::fsa_type nfa;
    fsa_re::re_to_fsa(re_data, count, static_cast<uint32_t>(token), 0, empty_string(), nfa, status);
    fsa::fsa_type dfa;
    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);
    fsa::fsa_type mindfa;
    fsa_algorithm::minimize_dfa(dfa, mindfa);

    result_fsa.swap(mindfa);
}

void test_java_identifiers_and_keywords()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    operation_status status; //??

    fsa::fsa_type abstract_fsa;
    build_fsa_from_re(L"abstract", my_token_traits::type::abstract_kw, abstract_fsa);

    fsa::fsa_type assert_fsa;
    build_fsa_from_re(L"assert", my_token_traits::type::assert_kw, assert_fsa);

    fsa::fsa_type boolean_fsa;
    build_fsa_from_re(L"boolean", my_token_traits::type::boolean_kw, boolean_fsa);

    fsa::fsa_type break_fsa;
    build_fsa_from_re(L"break", my_token_traits::type::break_kw, break_fsa);

    fsa::fsa_type byte_fsa;
    build_fsa_from_re(L"byte", my_token_traits::type::byte_kw, byte_fsa);

    fsa::fsa_type case_fsa;
    build_fsa_from_re(L"case", my_token_traits::type::case_kw, case_fsa);

    fsa::fsa_type catch_fsa;
    build_fsa_from_re(L"catch", my_token_traits::type::catch_kw, catch_fsa);

    fsa::fsa_type char_fsa;
    build_fsa_from_re(L"char", my_token_traits::type::char_kw, char_fsa);

    fsa::fsa_type class_fsa;
    build_fsa_from_re(L"class", my_token_traits::type::class_kw, class_fsa);

    fsa::fsa_type const_fsa;
    build_fsa_from_re(L"const", my_token_traits::type::const_kw, const_fsa);

    fsa::fsa_type continue_fsa;
    build_fsa_from_re(L"continue", my_token_traits::type::continue_kw, continue_fsa);

    fsa::fsa_type default_fsa;
    build_fsa_from_re(L"default", my_token_traits::type::default_kw, default_fsa);

    fsa::fsa_type do_fsa;
    build_fsa_from_re(L"do", my_token_traits::type::do_kw, do_fsa);

    fsa::fsa_type double_fsa;
    build_fsa_from_re(L"double", my_token_traits::type::double_kw, double_fsa);

    fsa::fsa_type else_fsa;
    build_fsa_from_re(L"else", my_token_traits::type::else_kw, else_fsa);

    fsa::fsa_type enum_fsa;
    build_fsa_from_re(L"enum", my_token_traits::type::enum_kw, enum_fsa);

    fsa::fsa_type extends_fsa;
    build_fsa_from_re(L"extends", my_token_traits::type::extends_kw, extends_fsa);

    fsa::fsa_type final_fsa;
    build_fsa_from_re(L"final", my_token_traits::type::final_kw, final_fsa);

    fsa::fsa_type finally_fsa;
    build_fsa_from_re(L"finally", my_token_traits::type::finally_kw, finally_fsa);

    fsa::fsa_type float_fsa;
    build_fsa_from_re(L"float", my_token_traits::type::float_kw, float_fsa);

    fsa::fsa_type for_fsa;
    build_fsa_from_re(L"for", my_token_traits::type::for_kw, for_fsa);

    fsa::fsa_type if_fsa;
    build_fsa_from_re(L"if", my_token_traits::type::if_kw, if_fsa);

    fsa::fsa_type goto_fsa;
    build_fsa_from_re(L"goto", my_token_traits::type::goto_kw, goto_fsa);

    fsa::fsa_type implements_fsa;
    build_fsa_from_re(L"implements", my_token_traits::type::implements_kw, implements_fsa);

    fsa::fsa_type import_fsa;
    build_fsa_from_re(L"import", my_token_traits::type::import_kw, import_fsa);

    fsa::fsa_type instanceof_fsa;
    build_fsa_from_re(L"instanceof", my_token_traits::type::instanceof_kw, instanceof_fsa);

    fsa::fsa_type int_fsa;
    build_fsa_from_re(L"int", my_token_traits::type::int_kw, int_fsa);

    fsa::fsa_type interface_fsa;
    build_fsa_from_re(L"interface", my_token_traits::type::interface_kw, interface_fsa);

    fsa::fsa_type long_fsa;
    build_fsa_from_re(L"long", my_token_traits::type::long_kw, long_fsa);

    fsa::fsa_type native_fsa;
    build_fsa_from_re(L"native", my_token_traits::type::native_kw, native_fsa);

    fsa::fsa_type new_fsa;
    build_fsa_from_re(L"new", my_token_traits::type::new_kw, new_fsa);

    fsa::fsa_type package_fsa;
    build_fsa_from_re(L"package", my_token_traits::type::package_kw, package_fsa);

    fsa::fsa_type private_fsa;
    build_fsa_from_re(L"private", my_token_traits::type::private_kw, private_fsa);

    fsa::fsa_type protected_fsa;
    build_fsa_from_re(L"protected", my_token_traits::type::protected_kw, protected_fsa);

    fsa::fsa_type public_fsa;
    build_fsa_from_re(L"public", my_token_traits::type::public_kw, public_fsa);

    fsa::fsa_type return_fsa;
    build_fsa_from_re(L"return", my_token_traits::type::return_kw, return_fsa);

    fsa::fsa_type short_fsa;
    build_fsa_from_re(L"short", my_token_traits::type::short_kw, short_fsa);

    fsa::fsa_type static_fsa;
    build_fsa_from_re(L"static", my_token_traits::type::static_kw, static_fsa);

    fsa::fsa_type strictfp_fsa;
    build_fsa_from_re(L"strictfp", my_token_traits::type::strictfp_kw, strictfp_fsa);

    fsa::fsa_type super_fsa;
    build_fsa_from_re(L"super", my_token_traits::type::super_kw, super_fsa);

    fsa::fsa_type switch_fsa;
    build_fsa_from_re(L"switch", my_token_traits::type::switch_kw, switch_fsa);

    fsa::fsa_type synchronized_fsa;
    build_fsa_from_re(L"synchronized", my_token_traits::type::synchronized_kw, synchronized_fsa);

    fsa::fsa_type this_fsa;
    build_fsa_from_re(L"this", my_token_traits::type::this_kw, this_fsa);

    fsa::fsa_type throw_fsa;
    build_fsa_from_re(L"throw", my_token_traits::type::throw_kw, throw_fsa);

    fsa::fsa_type transient_fsa;
    build_fsa_from_re(L"transient", my_token_traits::type::transient_kw, transient_fsa);

    fsa::fsa_type try_fsa;
    build_fsa_from_re(L"try", my_token_traits::type::try_kw, try_fsa);

    fsa::fsa_type void_fsa;
    build_fsa_from_re(L"void", my_token_traits::type::void_kw, void_fsa);

    fsa::fsa_type volatile_fsa;
    build_fsa_from_re(L"volatile", my_token_traits::type::volatile_kw, volatile_fsa);

    fsa::fsa_type while_fsa;
    build_fsa_from_re(L"while", my_token_traits::type::while_kw, while_fsa);

    fsa::fsa_type true_fsa;
    build_fsa_from_re(L"true", my_token_traits::type::true_lit, true_fsa);

    fsa::fsa_type false_fsa;
    build_fsa_from_re(L"false", my_token_traits::type::false_lit, false_fsa);

    fsa::fsa_type null_fsa;
    build_fsa_from_re(L"null", my_token_traits::type::null_lit, null_fsa);

    fsa::fsa_type var_fsa;
    build_fsa_from_re(L"var", my_token_traits::type::var_kw, var_fsa);

    fsa::fsa_type identifier_fsa;
    //build_fsa_from_re(L"(S|_|\\$)(S|0|_|\\$)*", my_token_traits::type::identifier, identifier_fsa);
    build_fsa_from_re(L"(A|a|B|b|C|c|D|d|E|e|F|f|G|g|H|h|I|i|J|j|K|k|L|l|M|m|N|n|O|o|P|p|Q|q|R|r|S|s|T|t|U|u|V|v|W|w|X|x|Y|y|Z|z|_|\\$)(A|a|B|b|C|c|D|d|E|e|F|f|G|g|H|h|I|i|J|j|K|k|L|l|M|m|N|n|O|o|P|p|Q|q|R|r|S|s|T|t|U|u|V|v|W|w|X|x|Y|y|Z|z|_|\\$|0|1|2|3|4|5|6|7|8|9)*", my_token_traits::type::identifier, identifier_fsa);
    //build_fsa_from_re(L"(b|e|_|t|y)(b|e|_|t|y)*", my_token_traits::type::identifier, identifier_fsa);

    std::vector<fsa::fsa_type> fsas;

    fsas.emplace_back(abstract_fsa);
    fsas.emplace_back(assert_fsa);
    fsas.emplace_back(boolean_fsa);
    fsas.emplace_back(break_fsa);
    fsas.emplace_back(byte_fsa);
    fsas.emplace_back(case_fsa);
    fsas.emplace_back(catch_fsa);
    fsas.emplace_back(char_fsa);
    fsas.emplace_back(class_fsa);
    fsas.emplace_back(const_fsa);
    fsas.emplace_back(continue_fsa);
    fsas.emplace_back(default_fsa);
    fsas.emplace_back(do_fsa);
    fsas.emplace_back(double_fsa);
    fsas.emplace_back(else_fsa);
    fsas.emplace_back(enum_fsa);
    fsas.emplace_back(extends_fsa);
    fsas.emplace_back(final_fsa);
    fsas.emplace_back(finally_fsa);
    fsas.emplace_back(float_fsa);
    fsas.emplace_back(for_fsa);
    fsas.emplace_back(if_fsa);
    fsas.emplace_back(goto_fsa);
    fsas.emplace_back(implements_fsa);
    fsas.emplace_back(import_fsa);
    fsas.emplace_back(instanceof_fsa);
    fsas.emplace_back(int_fsa);
    fsas.emplace_back(interface_fsa);
    fsas.emplace_back(long_fsa);
    fsas.emplace_back(native_fsa);
    fsas.emplace_back(new_fsa);
    fsas.emplace_back(package_fsa);
    fsas.emplace_back(private_fsa);
    fsas.emplace_back(protected_fsa);
    fsas.emplace_back(public_fsa);
    fsas.emplace_back(return_fsa);
    fsas.emplace_back(short_fsa);
    fsas.emplace_back(static_fsa);
    fsas.emplace_back(strictfp_fsa);
    fsas.emplace_back(super_fsa);
    fsas.emplace_back(switch_fsa);
    fsas.emplace_back(synchronized_fsa);
    fsas.emplace_back(this_fsa);
    fsas.emplace_back(throw_fsa);
    fsas.emplace_back(transient_fsa);
    fsas.emplace_back(try_fsa);
    fsas.emplace_back(void_fsa);
    fsas.emplace_back(volatile_fsa);
    fsas.emplace_back(while_fsa);
    fsas.emplace_back(true_fsa);
    fsas.emplace_back(false_fsa);
    fsas.emplace_back(null_fsa);
    fsas.emplace_back(var_fsa);
    fsas.emplace_back(identifier_fsa);

    fsa::fsa_type nfa;
    fsa::combine(fsas, nfa, status);

    fsa::fsa_type dfa;
    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);

    fsa::fsa_type mindfa;
    fsa_algorithm::minimize_dfa(dfa, mindfa);

    fsa_visualization::generate_graphviz_file(mindfa, LR"(d:\tmp\FSA\java_ids_kws.dot)");

    std::vector<string_type> inputs
    (
        {
            L"byte",
            L"byte_",

            L"abstract",
            L"assert",
            L"boolean",
            L"break",
            L"byte",
            L"case",
            L"catch",
            L"char",
            L"class",
            L"const",
            L"continue",
            L"default",
            L"do",
            L"double",
            L"else",
            L"enum",
            L"extends",
            L"final",
            L"finally",
            L"float",
            L"for",
            L"if",
            L"goto",
            L"implements",
            L"import",
            L"instanceof",
            L"int",
            L"interface",
            L"long",
            L"native",
            L"new",
            L"package",
            L"private",
            L"protected",
            L"public",
            L"return",
            L"short",
            L"static",
            L"strictfp",
            L"super",
            L"switch",
            L"synchronized",
            L"this",
            L"throw",
            L"throws",
            L"transient",
            L"try",
            L"void",
            L"volatile",
            L"while",
            L"var",
            L"true",
            L"false",
            L"null",

            L"abstract_id",
            L"assert_id",
            L"boolean_id",
            L"break_id",
            L"byte_id",
            L"case_id",
            L"catch_id",
            L"char_id",
            L"class_id",
            L"const_id",
            L"continue_id",
            L"default_id",
            L"do_id",
            L"double_id",
            L"else_id",
            L"enum_id",
            L"extends_id",
            L"final_id",
            L"finally_id",
            L"float_id",
            L"for_id",
            L"if_id",
            L"goto_id",
            L"implements_id",
            L"import_id",
            L"instanceof_id",
            L"int_id",
            L"interface_id",
            L"long_id",
            L"native_id",
            L"new_id",
            L"package_id",
            L"private_id",
            L"protected_id",
            L"public_id",
            L"return_id",
            L"short_id",
            L"static_id",
            L"strictfp_id",
            L"super_id",
            L"switch_id",
            L"synchronized_id",
            L"this_id",
            L"throw_id",
            L"throws_id",
            L"transient_id",
            L"try_id",
            L"void_id",
            L"volatile_id",
            L"while_id",
            L"var_id",
            L"true_id",
            L"false"
            L"null_id",

            L"_",
            L"$",
            L"a",
            L"a___",
            L"a$$$",
            L"a0000",
        }
    );

    validate_dfa(inputs, mindfa);
}

void test_load_grammar()
{
    const char_type* grammars[] =
    {
        //LR"(D:\Projects\fe\grammars\Sudkamp.3rd.4.4.1.txt)",
        //LR"(D:\Projects\fe\grammars\Sudkamp.3rd.4.2.1.G.txt)",
        //LR"(D:\Projects\fe\grammars\Sudkamp.3rd.4.2.3.G.txt)",

        //LR"(D:\Projects\fe\grammars\Bendersky.G.null.txt)",
        //LR"(D:\Projects\fe\grammars\Grune.Jacobs.hyg.txt)",
        //LR"(D:\Projects\fe\grammars\Molchanov.159.txt)",
        //LR"(D:\Projects\fe\grammars\cpp_grammar.txt)",

        //LR"(D:\Projects\fe\grammars\LeftFactoring.G0.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G1.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G2.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G4.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G5.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G6.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G7.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G8.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G9.txt)",
        //LR"(D:\Projects\fe\grammars\LeftFactoring.G10.txt)",

        //LR"(D:\Projects\fe\grammars\LeftRecursive.G0.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G1.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G2.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G3.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G4.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G5.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G6.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G7.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G8.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G9.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G10.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G11.txt)",
        //LR"(D:\Projects\fe\grammars\LeftRecursive.G12.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G0.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G1.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G2.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G3.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G4.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G5.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G6.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G7.txt)",
        //LR"(D:\Projects\fe\grammars\EFFirstSet.Aho.G0.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Parsons.G0.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Parsons.G1.txt)",
        //LR"(D:\Projects\fe\grammars\FirstSet.Parsons.G3.txt)",
        LR"(D:\Projects\fe\grammars\Java10.txt)",
    };

    for(auto grammar_path : grammars)
    {
        grammar gr;

        gr.load(grammar_path);

        grammar::symbol_type sym((*gr.rules()[0]).lhs()[0]);

        //std::set<grammar::symbol_type, symbol::symbol_key_comparator> set0;
        //set0.emplace(sym);
        //set0.emplace(sym);
        //(*sym).first_sets().emplace(set0);

        //std::set<grammar::symbol_type, symbol::symbol_key_comparator> set1;
        //set1.emplace(sym);
        //set1.emplace(sym);
        //(*sym).first_sets().emplace(set1);

        std::wcout << grammar_visualization::decorate_symbols(gr) << std::endl;
        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

        uint8_t k = 2;

        grammar_algorithm::build_nullability_set(gr);
        grammar_algorithm::build_first_set(gr, k);
        grammar_algorithm::build_follow_set(gr, k);
        grammar_algorithm::build_la_set(gr, k);

        grammar_algorithm::symbols_type symbols;

        //symbols.emplace_back(gr.pool()[L"A"]);
        //symbols.emplace_back(gr.pool()[L"B"]);
        //symbols.emplace_back(gr.pool()[L"C"]);
        //symbols.emplace_back(gr.pool()[L"d"]);

        grammar_algorithm::sets_type alpha_first_sets;
        grammar_algorithm::build_first_set(symbols, k, alpha_first_sets);

        grammar_algorithm::sets_type la_result;

        for(std::size_t i = 0; i < gr.rules().size(); i++)
        {
            grammar_algorithm::build_la_set(gr.rules()[i], k, la_result);
        }

        // Sudkamp.G0
        //symbols.emplace_back(gr.pool()[L"S"]);
        //grammar_algorithm::build_la_set(symbols, gr.rules()[0], 0, k, la_result); // S, S -> Aabd
        //grammar_algorithm::build_la_set(symbols, gr.rules()[1], 0, k, la_result); // S, S -> cAbcd
        //symbols.clear();

        //symbols.emplace_back(gr.pool()[L"A"]);
        //symbols.emplace_back(gr.pool()[L"a"]);
        //symbols.emplace_back(gr.pool()[L"b"]);
        //symbols.emplace_back(gr.pool()[L"d"]);
        //grammar_algorithm::build_la_set(symbols, gr.rules()[2], 0, k, la_result); // Aabd, A -> a
        //grammar_algorithm::build_la_set(symbols, gr.rules()[3], 0, k, la_result); // Aabd, A -> b
        //grammar_algorithm::build_la_set(symbols, gr.rules()[4], 0, k, la_result); // Aabd, A -> λ
        //symbols.clear();

        //symbols.emplace_back(gr.pool()[L"c"]);
        //symbols.emplace_back(gr.pool()[L"A"]);
        //symbols.emplace_back(gr.pool()[L"b"]);
        //symbols.emplace_back(gr.pool()[L"c"]);
        //symbols.emplace_back(gr.pool()[L"d"]);
        //grammar_algorithm::build_la_set(symbols, gr.rules()[2], 1, k, la_result); // cAbcd, A -> a
        //grammar_algorithm::build_la_set(symbols, gr.rules()[3], 1, k, la_result); // cAbcd, A -> b
        //grammar_algorithm::build_la_set(symbols, gr.rules()[4], 1, k, la_result); // cAbcd, A -> λ
        //symbols.clear();

        // Sudkamp.G7
        //symbols.emplace_back(gr.pool()[L"a"]);
        //symbols.emplace_back(gr.pool()[L"B"]);
        //symbols.emplace_back(gr.pool()[L"A"]);
        //symbols.emplace_back(gr.pool()[L"d"]);
        //grammar_algorithm::build_la_set(symbols, gr.rules()[4], 1, k, la_result); // aBAd, B -> ab
        //grammar_algorithm::build_la_set(symbols, gr.rules()[5], 1, k, la_result); // aBAd, B -> a
        //symbols.clear();

        //symbols.emplace_back(gr.pool()[L"b"]);
        //symbols.emplace_back(gr.pool()[L"B"]);
        //symbols.emplace_back(gr.pool()[L"b"]);
        //symbols.emplace_back(gr.pool()[L"A"]);
        //symbols.emplace_back(gr.pool()[L"d"]);
        //grammar_algorithm::build_la_set(symbols, gr.rules()[4], 1, k, la_result); // bBbAd, B -> ab
        //grammar_algorithm::build_la_set(symbols, gr.rules()[5], 1, k, la_result); // bBbAd, B -> a
        //symbols.clear();


        grammar_algorithm::remove_left_recursion(gr);
        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

        grammar_algorithm::cleanup_grammar(gr);
        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;
    }
}

void test_infix_op()
{
    grammar::symbol_type a(factory::create<symbol>(0, L"a", symbol::kind::terminal));
    grammar::symbol_type b(factory::create<symbol>(1, L"b", symbol::kind::terminal));
    grammar::symbol_type c(factory::create<symbol>(2, L"c", symbol::kind::terminal));
    grammar::symbol_type d(factory::create<symbol>(3, L"d", symbol::kind::terminal));
    grammar::symbol_type f(factory::create<symbol>(4, L"f", symbol::kind::terminal));

    grammar::symbol_type A(factory::create<symbol>(5, L"A", symbol::kind::terminal));
    grammar::symbol_type B(factory::create<symbol>(6, L"B", symbol::kind::terminal));
    grammar::symbol_type C(factory::create<symbol>(7, L"C", symbol::kind::terminal));
    grammar::symbol_type D(factory::create<symbol>(8, L"D", symbol::kind::terminal));

    grammar::symbol_type _0(factory::create<symbol>(9, L"0", symbol::kind::terminal));
    grammar::symbol_type _1(factory::create<symbol>(10, L"1", symbol::kind::terminal));
    grammar::symbol_type _2(factory::create<symbol>(11, L"2", symbol::kind::terminal));
    grammar::symbol_type _3(factory::create<symbol>(12, L"3", symbol::kind::terminal));
    grammar::symbol_type _4(factory::create<symbol>(13, L"4", symbol::kind::terminal));
    grammar::symbol_type _5(factory::create<symbol>(14, L"5", symbol::kind::terminal));

    grammar::symbol_type g(factory::create<symbol>(15, L"g", symbol::kind::terminal));
    grammar::symbol_type h(factory::create<symbol>(16, L"h", symbol::kind::terminal));
    grammar::symbol_type q(factory::create<symbol>(17, L"q", symbol::kind::terminal));

    std::vector<grammar_algorithm::sets_type> sets;
    grammar_algorithm::sets_type result;

    // L1 = {e, abb}
    // L2 = { b, bab }
    // L1 (+)2 L2 = { b, ba, ab }
    sets.clear();
    result.clear();

    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{symbol::epsilon}, grammar_algorithm::set_type{a,b,b}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{b}, grammar_algorithm::set_type{b,a,b}});

    grammar_algorithm::infix_operator(sets, 2, result);
    std::wcout << grammar_visualization::decorate_sets(result) << std::endl;

    // TRUNC3( {a, b, λ} {a} {b} {d} {λ} ) = TRUNC3( { aabd, babd, abd} ) = { aab, bab, abd }
    sets.clear();
    result.clear();

    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{a}, grammar_algorithm::set_type{b}, grammar_algorithm::set_type{symbol::epsilon}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{a}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{b}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{d}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{symbol::epsilon}});

    grammar_algorithm::infix_operator(sets, 3, result);
    std::wcout << grammar_visualization::decorate_sets(result) << std::endl;

    // { a b c } { A B C } { 0 1 2 3 4 5 }
    sets.clear();
    result.clear();

    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{a}, grammar_algorithm::set_type{b}, grammar_algorithm::set_type{c}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{A}, grammar_algorithm::set_type{B}, grammar_algorithm::set_type{C}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{_0},
                                                     grammar_algorithm::set_type{_1},
                                                     grammar_algorithm::set_type{_2},
                                                     grammar_algorithm::set_type{_3},
                                                     grammar_algorithm::set_type{_4},
                                                     grammar_algorithm::set_type{_5}});
    grammar_algorithm::infix_operator(sets, 3, result);
    std::wcout << grammar_visualization::decorate_sets(result) << std::endl;

    // { λ abb } { b bab } { cd df gh }
    sets.clear();
    result.clear();

    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{symbol::epsilon}, grammar_algorithm::set_type{a,b,b}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{b}, grammar_algorithm::set_type{b,a,b}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{c,d}, grammar_algorithm::set_type{d,f}, grammar_algorithm::set_type{g, h}});

    grammar_algorithm::infix_operator(sets, 3, result);
    std::wcout << grammar_visualization::decorate_sets(result) << std::endl;

    // { c cc } { cc cc } { cc cc cc }
    sets.clear();
    result.clear();

    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{c}, grammar_algorithm::set_type{c,c}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{c,c}, grammar_algorithm::set_type{c,c}});
    sets.emplace_back(grammar_algorithm::sets_type { grammar_algorithm::set_type{c,c}, grammar_algorithm::set_type{c,c}, grammar_algorithm::set_type{c,c}});

    grammar_algorithm::infix_operator(sets, 3, result);
    std::wcout << grammar_visualization::decorate_sets(result) << std::endl;
}

void test_infix_op_llk()
{
//firstPlusOp: function(set1, set2)
//{
//    var ip, jp, fel;
//    var result = [];
//    var resultcheck = [];
//    
//    for (var i = 0; i < set1.length; i++)
//    {
//      for (var j = 0; j < set2.length; j++)
//      {
//        
//        ip = 0; jp = 0; fel = new FirstKEl(this.k);
//
//        for (var k = 0; k < this.k; k++)
//        {
//          if (ip < set1[i].str.length)
//          {
//            fel.addGEl(set1[i].str[ip]);
//            ip++;
//            continue;
//          }
//          
//          if (jp < set2[j].str.length)
//          {
//            fel.addGEl(set2[j].str[jp]);
//            jp++;
//            continue;
//          }
//          
//          break;
//        }
//
//        addToArrayFlat(fel, fel.toFlat(), result, resultcheck);
//        
//      }
//    }
//    
//    return result;
//  },


    std::vector<string_type> set1;
    std::vector<string_type> set2;

    set1.push_back(L"j");
    set1.push_back(L"f");
    set1.push_back(L"g");
    set1.push_back(L"h");

    set2.push_back(L"p");
    set2.push_back(L"q");

    std::size_t ip, jp;

    for(std::size_t i = 0; i < set1.size(); i++)
    {
        for(std::size_t j = 0; j < set2.size(); j++)
        {
            ip = 0; jp = 0;

            std::vector<char_type> fel;

            for(int k = 0; k < 1; k++)
            {
                if(ip < set1[i].size())
                {
                    fel.emplace_back(set1[i][ip]);
                    ip++;
                    continue;
                  }
          
                if(jp < set2[j].size())
                {
                    fel.emplace_back(set2[j][jp]);
                    jp++;
                    continue;
                }

                break;
            }

            int kk = 123;
            kk;
        }
    }
}

std::vector<std::vector<int>> cart_product (const std::vector<std::vector<int>>& v)
{
    std::vector<std::vector<int>> s = {{}};

    for(auto& u : v)
    {
        std::vector<std::vector<int>> r;

        for(auto& x : s)
        {
            for (auto y : u)
            {
                r.push_back(x);
                r.back().push_back(y);
            }
        }

        s.swap(r);
    }

    return s;
}

void test_cartesian_product()
{
    std::vector<std::vector<int>> test{{1}, {4,5,6}, {8,9}};
    std::vector<std::vector<int>> res = cart_product(test);

    for(size_t i = 0; i < res.size();i++)
    {
        for(size_t j = 0; j < res[i].size(); j++)
        {
            std::cout << res[i][j] << "\t";
        }

        std::cout << std::endl;
    }
}

static bool is_less(const typename grammar_algorithm::set_type& seq1, const typename grammar_algorithm::set_type& seq2)
{
    // see _Lex_compare_unchecked, <xutility>
    auto it1(seq1.begin());
    auto it1_end(seq1.end());

    auto it2(seq2.begin());
    auto it2_end(seq2.end());

    for(; it1 != it1_end && it2 != it2_end; ++it1, ++it2)
    {
        std::int32_t id1 = static_cast<std::int32_t>((*(*it1)).id());
        std::int32_t id2 = static_cast<std::int32_t>((*(*it2)).id());

        if(id1 < id2)
        {
            return true;
        }
        else if(id1 > id2)
        {
            return false;
        }
    }

	return (it1 == it1_end && it2 != it2_end);
}

static void make_vector_unique2(typename grammar_algorithm::sets_type& sequence)
{
    std::sort(sequence.begin(),
              sequence.end(),
              [](const typename grammar_algorithm::set_type& seq1, const typename grammar_algorithm::set_type& seq2)
              {
                  return is_less(seq1, seq2);
              });

    //std::sort(sequence.begin(), sequence.end());
    sequence.erase(std::unique(sequence.begin(), sequence.end() ), sequence.end());
}

void test_make_unique()
{
    grammar::symbol_type a(factory::create<symbol>(0, L"a", symbol::kind::terminal));
    grammar::symbol_type b(factory::create<symbol>(1, L"b", symbol::kind::terminal));
    grammar::symbol_type c(factory::create<symbol>(2, L"c", symbol::kind::terminal));
    grammar::symbol_type d(factory::create<symbol>(3, L"d", symbol::kind::terminal));
    grammar::symbol_type f(factory::create<symbol>(4, L"f", symbol::kind::terminal));

    // { cc c cc c }
    auto seq0(grammar_algorithm::sets_type { grammar_algorithm::set_type{c,c},
                                             grammar_algorithm::set_type{c},
                                             grammar_algorithm::set_type{c,c},
                                             grammar_algorithm::set_type{c}});
    make_vector_unique2(seq0);
    std::wcout << grammar_visualization::decorate_sets(seq0).c_str() << std::endl;

    auto seq(grammar_algorithm::sets_type { grammar_algorithm::set_type{b},
                                            grammar_algorithm::set_type{b,a},
                                            grammar_algorithm::set_type{a,b},
                                            grammar_algorithm::set_type{a,b}});
    make_vector_unique2(seq);
    std::wcout << grammar_visualization::decorate_sets(seq).c_str() << std::endl;
}

void test_algo_directory()
{
    std::filesystem::path path(LR"(D:\Projects\fe\grammars\test)");

    for(uint8_t k = 1; k <= 3; k++)
    {
        log_info(L"\n --------------------------------------------- K = %d ---------------------------------------", k);

        for(std::filesystem::directory_iterator it(path), it_end; it != it_end; ++it)
        {
            const std::filesystem::directory_entry& entry(*it);

            string_type file_path = (char_type*)(entry.path().u16string().c_str());

            grammar gr;

            gr.load(file_path);

            std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

            grammar_algorithm::build_nullability_set(gr);
            grammar_algorithm::build_first_set(gr, k);
            grammar_algorithm::build_follow_set(gr, k);
            grammar_algorithm::build_la_set(gr, k);
    
            ll_algorithm::strong_ll_table_type strong_ll_table;
            ll_algorithm::build_strong_ll_table(gr, k, strong_ll_table);

            ll_algorithm::ll_table_type ll_table;
            ll_algorithm::build_ll_table(gr, k, ll_table);
        }
    }

    exit(0);
}

void test_eff()
{
    grammar gr;

    //gr.load(LR"(D:\Projects\fe\grammars\EFF.G0.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\EFF.G1.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\EFF.Aho.G0.txt)");
    gr.load(LR"(D:\Projects\fe\grammars\EFF.Aho.G1.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\EFF.Seidl.G0.txt)");
    //gr.load(LR"(D:\Projects\fe\grammars\FirstSet.Aho.G0.txt)");

    uint8_t k = 1;

    std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

    grammar_algorithm::build_nullability_set(gr);
    grammar_algorithm::build_first_set(gr, k);
    grammar_algorithm::build_first_set(gr, k, true);
    grammar_algorithm::build_follow_set(gr, k);
    grammar_algorithm::build_la_set(gr, k);

    grammar_algorithm::sets_type first;
    grammar_algorithm::sets_type eff;

    grammar_algorithm::build_first_set(grammar_algorithm::set_type { gr.start_symbol() }, k, first);

    // EFF(Ac)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"A"], gr.pool()[L"c"] }, k, eff);
    eff.clear();

    // EFF(Bc)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"B"], gr.pool()[L"c"] }, k, eff);
    eff.clear();




    // EFF(S)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"S"] }, k, eff);
    eff.clear();

    // EFF(aSb)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"a"], gr.pool()[L"S"], gr.pool()[L"b"] }, k, eff);
    eff.clear();

    // EFF(aSba)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"a"], gr.pool()[L"S"], gr.pool()[L"b"], gr.pool()[L"a"] }, k, eff);
    eff.clear();

    // EFF(Sb)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"S"], gr.pool()[L"b"] }, k, eff);
    eff.clear();

    // EFF(SaSb)
    grammar_algorithm::build_eff_set(gr, grammar_algorithm::set_type { gr.pool()[L"S"], gr.pool()[L"a"], gr.pool()[L"S"], gr.pool()[L"b"] }, k, eff);
    eff.clear();
}

void test_sets()
{
    std::vector<string_type> inputs  =
    {
        LR"(D:\Projects\fe\grammars\LR.G.No-First.txt)",
        LR"(D:\Projects\fe\grammars\First.Cycle.parr.phd.txt)",
        LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G6.txt)",
        LR"(D:\Projects\fe\grammars\Backhouse.G.3.2.3.txt)",
        LR"(D:\Projects\fe\grammars\FirstSet.Sudkamp.G3.txt)",
    };

    uint8_t k = 1;

    for(const auto& input : inputs)
    {
        grammar gr;

        gr.load(input);

        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

        //grammar::symbol_type a(factory::create<symbol>(0, L"a", symbol::kind::terminal));
        //grammar::symbol_type b(factory::create<symbol>(1, L"b", symbol::kind::terminal));
        //grammar::symbol_type c(factory::create<symbol>(2, L"c", symbol::kind::terminal));

        //grammar_algorithm::sets_type new_fl_prime_a;

        //std::vector<grammar_algorithm::sets_type> infix_op_input;

        //infix_op_input.emplace_back(grammar_algorithm::sets_type{ grammar_algorithm::set_type{b}, grammar_algorithm::set_type{a,c} });
        //infix_op_input.emplace_back(grammar_algorithm::sets_type{ grammar_algorithm::set_type{c}});
        //infix_op_input.emplace_back(grammar_algorithm::sets_type{ grammar_algorithm::set_type{symbol::epsilon}});
        //infix_op_input.emplace_back(grammar_algorithm::sets_type{ grammar_algorithm::set_type{a}});

        //grammar_algorithm::infix_operator(infix_op_input, k, new_fl_prime_a); // TRUNCk(FIRSTk(ui+1) L)

        grammar_algorithm::build_nullability_set(gr);

        grammar_algorithm::build_first_set(gr, k);
        grammar_algorithm::build_first_set(gr, k, true);
        grammar_algorithm::build_follow_set(gr, k);

        gr.cleanup();

        grammar_algorithm::build_first1_set(gr);
        grammar_algorithm::build_follow1_set(gr);

        grammar_algorithm::build_la_set(gr, k);

        lr_algorithm::lr_goto_table_type goto_table;
        lr_algorithm::lr_action_table_type action_table;

        lr_algorithm::build_lr_table(gr, k, goto_table, action_table);
    }
}

void test_re_dfa()
{
    USINGNAMESPACE(core)
    USINGNAMESPACE(frontend)

    string_type res[] =
    {
        L"(a|b)*abb",

        L"a+",

        L"(a|b)*a(a|b)(a|b)", // aho, ullman
        L"(a|b)*abb", // aho, ullman

        L"(a|b)*(abb|a+b)",
        L"(0|10*1)*",
        L"a\\.",
        L"a\\.(b\\.b)+\\.a",
        L"a\\.b|c",
        L"a\\.b+\\.c",
        //L"\t\r\n\f\\(\\)\\?\\*\\|\\.\\+a(bb)+a",
        L"a((bb))*a",
        L"a*",
        L"(a|b)*abb",
        L"(a|b)*ba",
        L"ab|a*",
        L"a*(a|b)ba*|a*",
        L"((ab)|c)*",
        //L"జ్ఞ‌ా*",
        L"a+(\\.a+)*@a+(\\.a+)+",
        L"(ab+(b+aa)(ba)*(a+bb))*",
        L"\\ban?\\b",
        L"\\b|(a(T|n)?)|\\b",
        L"\\b|(a(T|n)?)|\\b",
        L"((c|a)b*)*",
        L"(a(b|c))*a",
    };

    uint16_t k = 0;

    operation_status status;

    for(const auto& re : res)
    {
        fsa::fsa_type dfa;

        if(fsa_re::re_to_dfa(re, 0, 0, empty_string(), dfa, status))
        {
            fsa_visualization::generate_graphviz_file(dfa, format(LR"(d:\tmp\FSA\dfa_%d.dot)", ++k));
        }
    }
}


// for %i in (d:\tmp\FSA\*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
// D:\Soft\graphviz\2.38\release\bin\dot -Tpng d:\tmp\FSA\nfa.dot -o d:\tmp\FSA\nfa.dot.png
// for /r "D:\Tmp\Books\Parsing & Syntactic Theory" %f in (*) do @copy /Y "%f" .
