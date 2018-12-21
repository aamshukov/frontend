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
#include <core\counter.hpp>
#include <core\data_provider.hpp>
#include <core\factory.hpp>
#include <core\content.hpp>

#include <frontend\fsa\fsa_transition.hpp>
#include <frontend\fsa\fsa_state.hpp>
#include <frontend\fsa\fsa.hpp>
#include <frontend\fsa\fsa_re.hpp>

#include <frontend\fsa\fsa_visualization.hpp> //??

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

#define OPEN_PAREN_OP   (0x05000001)
#define CLOSE_PAREN_OP  (0x05000002)
#define CONCATENATE_OP  (0x05000003)
#define ALTERNATE_OP    (0x05000004)
#define ZERO_OR_MORE_OP (0x05000005)
#define ONE_OR_MORE_OP  (0x05000006)
#define ZERO_OR_ONE_OP  (0x05000007)

#define ERROR_DATUM     (text::bad_codepoint())

bool fsa_re::re_to_fsa(const std::shared_ptr<typename fsa_re::datum_type[]>& re,
                       size_type count,
                       typename fsa_re::token_type token,
                       typename fsa_re::token_type escape_token,
                       const string_type& escape_predicate,
                       typename fsa_re::fsa_type& result_fsa,
                       operation_status& status)
{
    log_info(L"Generating FSA from RE ...");

    bool result = false;

    try
    {
        std::shared_ptr<datum_type[]> processed_re;

        size_type new_count = preprocess(re, count, processed_re);

        std::shared_ptr<datum_type[]> postfix_re;

        if(infix_to_postfix(processed_re, new_count, postfix_re, status))
        {
            std::stack<fsa::fsa_type> fragments;

            const datum_type* p_src(postfix_re.get());

            result = true;

            while(*p_src && result)
            {
                switch(*p_src)
                {
                    case ALTERNATE_OP:
                        result = process_combine(fragments, status);
                        break;
                    case CONCATENATE_OP:
                        result = process_concatenate(fragments, status);
                        break;
                    case ZERO_OR_MORE_OP:
                        result = process_zero_or_more(fragments, status);
                        break;
                    case ONE_OR_MORE_OP:
                        result = process_one_or_more(fragments, status);
                        break;
                    case ZERO_OR_ONE_OP:
                        result = process_zero_or_one(fragments, status);
                        break;
                    default:
                        result = process_literal(p_src, fragments, status);
                        break;
                }

                *p_src++;
            }

            if(result)
            {
                if(!fragments.empty())
                {
                    auto fsa0(fragments.top());

                    fragments.pop();

                    for(auto& kvp : (*fsa0).final_states())
                    {
                        (*kvp.second).token() = token;
                    }

                    adjust_predicates(fsa0);
                    add_escape_state(fsa0, escape_token, escape_predicate);

                    result_fsa.swap(fsa0);

                    result = true;
                }
                else
                {
                    OPERATION_FAILED(status::custom_code::error, L"Generating FSA from RE: invlaid RE.")
                }
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Generating FSA from RE: error occurred.")
    }

    log_info(L"Generated FSA from RE.");

    return result;
}

fsa_re::datum_type fsa_re::get_codepoint(const typename fsa_re::datum_type*& p_src)
{
    datum_type result = ERROR_DATUM;

    switch(*p_src)
    {
        case L'(':
            result = OPEN_PAREN_OP;
            break;
        case L')':
            result = CLOSE_PAREN_OP;
            break;
        case L'|':
            result = ALTERNATE_OP;
            break;
        case L'.':
            result = CONCATENATE_OP;
            break;
        case L'*':
            result = ZERO_OR_MORE_OP;
            break;
        case L'+':
            result = ONE_OR_MORE_OP;
            break;
        case L'?':
            result = ZERO_OR_ONE_OP;
            break;
        case L'\n':
            result = 0x0D;
            break;
        case L'\r':
            result = 0x0A;
            break;
        case L'\t':
            result = 0x09;
            break;
        case L'\f':
            result = 0x0C;
            break;
        case L'\\':
            if(*(p_src + 1) == L'u')
            {
                // parse \uXXXXXXXX
            }
            else
            {
                result = *++p_src;
            }
            break;
        default:
            result = *p_src;
            break;
    }

    return result;
}

int8_t fsa_re::get_operator_precedence(typename fsa_re::datum_type op)
{
    static std::map<datum_type, int8_t> precendences
    {
        { OPEN_PAREN_OP,   static_cast<int8_t>(1) }, // '('
        { ALTERNATE_OP,    static_cast<int8_t>(2) }, // '|'
        { CONCATENATE_OP,  static_cast<int8_t>(3) }, // '.'
        { ZERO_OR_MORE_OP, static_cast<int8_t>(4) }, // '*'
        { ONE_OR_MORE_OP,  static_cast<int8_t>(4) }, // '+'
        { ZERO_OR_ONE_OP,  static_cast<int8_t>(4) }  // '?'
    };

    int8_t result;

    auto search(precendences.find(op));

    if(search != precendences.end())
    {
        result = (*search).second;
    }
    else
    {
        result = 5;
    }

    return result;
}

bool fsa_re::is_literal(typename fsa_re::datum_type ch)
{
    return ch != OPEN_PAREN_OP && ch != CLOSE_PAREN_OP && ch != ALTERNATE_OP && ch != CONCATENATE_OP && ch != ZERO_OR_MORE_OP && ch != ONE_OR_MORE_OP && ch != ZERO_OR_ONE_OP;
}

size_type fsa_re::preprocess(const std::shared_ptr<typename fsa_re::datum_type[]>& infix_re, size_type count, std::shared_ptr<typename fsa_re::datum_type[]>& processed_re)
{
    std::shared_ptr<datum_type[]> buffer(new datum_type[count * 2 + 1]);
    datum_type* p_dst(buffer.get());

    const datum_type* p_src(infix_re.get());

    size_type k = 0;

    datum_type ch = get_codepoint(p_src);

    while(ch)
    {
        *p_dst++ = ch, k++;

        if((is_literal(ch) || ch == CLOSE_PAREN_OP || ch == ZERO_OR_MORE_OP || ch == ONE_OR_MORE_OP || ch == ZERO_OR_ONE_OP) &&
            (*(p_src + 1) && *(p_src + 1) != L')' && *(p_src + 1) != L'|' && *(p_src + 1) != L'*' && *(p_src + 1) != L'?' && *(p_src + 1) != L'+'))
        {
            *p_dst++ = CONCATENATE_OP, k++;
        }

        ch = get_codepoint(++p_src);
    }

    buffer[k] = 0;

    processed_re.swap(buffer);

    return k + 1;
}

bool fsa_re::infix_to_postfix(const std::shared_ptr<typename fsa_re::datum_type[]>& infix_re,
                              size_type count,
                              std::shared_ptr<typename fsa_re::datum_type[]>& postfix_re,
                              operation_status& status)
{
    // a(bb)+a ==> abb.+.a.
    //
    // shunting-yard-algorithm
    //  1.  While there are tokens to be read:
    //  2.        Read a token
    //  3.        If it's a number add it to queue
    //  4.        If it's an operator
    //  5.               While there's an operator on the top of the stack with greater precedence:
    //  6.                       Pop operators from the stack onto the output queue
    //  7.               Push the current operator onto the stack
    //  8.        If it's a left bracket push it onto the stack
    //  9.        If it's a right bracket 
    // 10.            While there's not a left bracket at the top of the stack:
    // 11.                     Pop operators from the stack onto the output queue.
    // 12.             Pop the left bracket from the stack and discard it
    // 13. While there are operators on the stack, pop them to the queue
    // 14.
    log_info(L"Converting infix RE to postfix notation ...");

    bool result = false;

    std::stack<datum_type> operators;           // stack

    std::shared_ptr<datum_type[]> buffer(new datum_type[count + 1]);
    datum_type* p_dst(buffer.get());            // queue

    const datum_type* p_src(infix_re.get());    // tokens

    size_type k = 0;

    while(*p_src)
    {
        switch(*p_src)
        {
            case OPEN_PAREN_OP: // 8.
                operators.push(OPEN_PAREN_OP);
                break;
            case CLOSE_PAREN_OP: // 9. - 12.
                while(!operators.empty() && operators.top() != OPEN_PAREN_OP)
                {
                    *p_dst++ = operators.top(), k++;
                    operators.pop();
                }

                if(!operators.empty() && operators.top() == OPEN_PAREN_OP)
                {
                    operators.pop();
                }
                else
                {
                    OPERATION_FAILED(status::custom_code::error, L"Converting infix RE to postfix notation: parens mismatch, missing ')'.")
                    goto error;
                }

                break;
            case ALTERNATE_OP: // 4. - 7.
            case CONCATENATE_OP:
            case ZERO_OR_MORE_OP:
            case ONE_OR_MORE_OP:
            case ZERO_OR_ONE_OP:
            {
                if(*p_src == ALTERNATE_OP && *(p_src + 1) == 0)
                {
                    OPERATION_FAILED(status::custom_code::error, L"Converting infix RE to postfix notation: '|' cannot be the last term.")
                    goto error;
                }

                int8_t current_precedence = get_operator_precedence(*p_src);

                while(!operators.empty())
                {
                    int8_t precedence = get_operator_precedence(operators.top());

                    if(precedence >= current_precedence)
                    {
                        *p_dst++ = operators.top(), k++;
                        operators.pop();
                    }
                    else
                    {
                        break;
                    }
                }

                operators.push(*p_src);

                break;
            }
            default: // 3.
                *p_dst++ = *p_src, k++;
                break;
        }

        p_src++;
    }

    while(!operators.empty())
    {
        *p_dst++ = operators.top(), k++;
        operators.pop();
    }

    buffer[k] = 0;

    postfix_re.swap(buffer);

    result = true;

error: ;

    log_info(L"Converted infix RE to postfix notation.");

    return result;
}

bool fsa_re::process_combine(std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE combine ...");

    bool result = false;

    try
    {
        if(fragments.size() < 2)
        {
            OPERATION_FAILED(status::custom_code::error, L"Processing RE combine: invalid RE, two operands on the stack are required.")
        }
        else
        {
            auto fsa1(fragments.top());
            fragments.pop();

            auto fsa2(fragments.top());
            fragments.pop();

            fsa_type fsa0;

            fsa::combine(fsa1, fsa2, fsa0, status);

            auto final_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));

            (*fsa0).add_state(final_state, status);

            for(const auto& kvp : (*fsa0).final_states())
            {
                (*fsa0).add_transition(kvp.second, final_state, fsa_transition::epsilon_predicate(), status);
            }

            (*fsa0).final_states().clear();
            (*fsa0).add_final_state(final_state, status);

            fragments.push(fsa0);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE combine: error occurred.")
    }

    log_info(L"Processed RE combine.");

    return result;
}

bool fsa_re::process_concatenate(std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE concatenate ...");

    bool result = false;

    try
    {
        if(fragments.size() < 2)
        {
            OPERATION_FAILED(status::custom_code::error, L"Processing RE concatenate: invalid RE, two operands on the stack are required.")
        }
        else
        {
            auto fsa1(fragments.top());
            fragments.pop();

            auto fsa2(fragments.top());
            fragments.pop();

            fsa_type fsa0;

            // might be two cases,
            //  either merge FSA1-F with FSA2-S
            //  or combine FSA1 and FSA2 with an extra ε connection
            fsa::concatenate(fsa2, fsa1, fsa0, status); // FSA2 with FSA1 because FSAs are taken from stack

            fragments.push(fsa0);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE concatenate: error occurred.")
    }

    log_info(L"Processed RE concatenate.");

    return result;
}

bool fsa_re::process_zero_or_more(std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE zero or more ...");

    bool result = false;

    try
    {
        if(fragments.size() < 1)
        {
            OPERATION_FAILED(status::custom_code::error, L"Processing RE zero or more: invalid RE, one operands on the stack is required.")
        }
        else
        {
            auto fsa1(fragments.top());
            fragments.pop();

            auto fsa0(factory::create<fsa>());

            auto start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

            (*fsa0).add_state(start_state, status);
            (*fsa0).start_state() = start_state;

            using map_type = std::map<uint32_t, uint32_t>;

            map_type map;

            for(const auto& kvp : (*fsa1).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                (*fsa0).add_state(new_state, status);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));
            }

            for(const auto& state: (*fsa0).states())
            {
                for(const auto& kvp : (*state.second).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }

            auto final_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));

            (*fsa0).add_state(final_state, status);
            (*fsa0).add_final_state(final_state, status);

            // S --> F
            (*fsa0).add_transition(start_state, final_state, fsa_transition::epsilon_predicate(), status);

            // S --> Sm1
            auto fsa1_start_state((*fsa0).states().at(map.at((*(*fsa1).start_state()).id())));

            (*fsa0).add_transition(start_state, fsa1_start_state, fsa_transition::epsilon_predicate(), status);

            for(const auto& kvp : (*fsa1).final_states())
            {
                const auto& fsa1_final_state((*fsa0).states().at(map.at((*kvp.second).id())));

                // Fm1 --> Sm1
                (*fsa0).add_transition(fsa1_final_state, fsa1_start_state, fsa_transition::epsilon_predicate(), status);

                // Fm1 --> F
                (*fsa0).add_transition(fsa1_final_state, final_state, fsa_transition::epsilon_predicate(), status);
            }

            fragments.push(fsa0);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE zero or more: error occurred.")
    }

    log_info(L"Processed RE zero or more.");

    return result;
}

bool fsa_re::process_one_or_more(std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE one or more ...");

    bool result = false;

    try
    {
        if(fragments.size() < 1)
        {
            OPERATION_FAILED(status::custom_code::error, L"Processing RE one or more: invalid RE, one operands on the stack is required.")
        }
        else
        {
            auto fsa1(fragments.top());
            fragments.pop();

            auto fsa0(factory::create<fsa>());

            auto start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

            (*fsa0).add_state(start_state, status);
            (*fsa0).start_state() = start_state;

            using map_type = std::map<uint32_t, uint32_t>;

            map_type map;

            for(const auto& kvp : (*fsa1).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                (*fsa0).add_state(new_state, status);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));
            }

            for(const auto& state: (*fsa0).states())
            {
                for(const auto& kvp : (*state.second).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }

            auto final_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));

            (*fsa0).add_state(final_state, status);
            (*fsa0).add_final_state(final_state, status);

            // S --> Sm1
            auto fsa1_start_state((*fsa0).states().at(map.at((*(*fsa1).start_state()).id())));

            (*fsa0).add_transition(start_state, fsa1_start_state, fsa_transition::epsilon_predicate(), status);

            for(const auto& kvp : (*fsa1).final_states())
            {
                // Fm1 --> F
                const auto& fsa1_final_state((*fsa0).states().at(map.at((*kvp.second).id())));
                (*fsa0).add_transition(fsa1_final_state, final_state, fsa_transition::epsilon_predicate(), status);
            }

            // F --> Sm1
            (*fsa0).add_transition(final_state, fsa1_start_state, fsa_transition::epsilon_predicate(), status);

            fragments.push(fsa0);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE one or more: error occurred.")
    }

    log_info(L"Processed RE one or more.");

    return result;
}

bool fsa_re::process_zero_or_one(std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE zero or one ...");

    bool result = false;

    try
    {
        if(fragments.size() < 1)
        {
            OPERATION_FAILED(status::custom_code::error, L"Processing RE zero or one: invalid RE, one operands on the stack is required.")
        }
        else
        {
            auto fsa1(fragments.top());
            fragments.pop();

            auto fsa0(factory::create<fsa>());

            auto start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

            (*fsa0).add_state(start_state, status);
            (*fsa0).start_state() = start_state;

            using map_type = std::map<uint32_t, uint32_t>;

            map_type map;

            for(const auto& kvp : (*fsa1).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                (*fsa0).add_state(new_state, status);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));
            }

            for(const auto& state: (*fsa0).states())
            {
                for(const auto& kvp : (*state.second).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }

            auto final_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));

            (*fsa0).add_state(final_state, status);
            (*fsa0).add_final_state(final_state, status);

            // S --> F
            (*fsa0).add_transition(start_state, final_state, fsa_transition::epsilon_predicate(), status);

            // S --> Sm1
            auto fsa1_start_state((*fsa0).states().at(map.at((*(*fsa1).start_state()).id())));

            (*fsa0).add_transition(start_state, fsa1_start_state, fsa_transition::epsilon_predicate(), status);

            for(const auto& kvp : (*fsa1).final_states())
            {
                auto fsa1_final_state((*fsa0).states().at(map.at((*kvp.second).id())));

                // Fm1 --> F
                (*fsa0).add_transition(fsa1_final_state, final_state, fsa_transition::epsilon_predicate(), status);
            }

            fragments.push(fsa0);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE zero or one: error occurred.")
    }

    log_info(L"Processed RE zero or one.");

    return result;
}

bool fsa_re::process_literal(const typename fsa_re::datum_type*& p_src, std::stack<fsa::fsa_type>& fragments, operation_status& status)
{
    log_info(L"Processing RE literal ...");

    bool result = false;

    try
    {
        auto fsa0(factory::create<fsa>());

        auto start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

        (*fsa0).add_state(start_state, status);
        (*fsa0).start_state() = start_state;

        auto final_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, 0));

        (*fsa0).add_state(final_state, status);
        (*fsa0).add_final_state(final_state, status);

        (*fsa0).add_transition(start_state,
                               final_state,
                               *p_src, // including fsa_transition::epsilon_predicate() and text::epsilon_codepoint()
                               status);

        fragments.push(fsa0);

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Processing RE literal: error occurred.")
    }

    log_info(L"Processed RE literal.");

    return result;
}

void fsa_re::adjust_predicates(typename fsa_re::fsa_type& fsa0)
{
    // assign switch-char to predicate
    for(const auto& state_kvp: (*fsa0).states())
    {
        const auto& state(state_kvp.second);

        for(const auto& kvp : (*state).transitions())
        {
            const auto& transition(kvp.second);

            if(!(*transition).is_epsilon_transition() && (*transition).predicate().empty())
            {
                string_type predicate;
                operation_status status; //??

                text::codepoint_to_string((*transition).switch_char(), predicate, status);

                (*transition).predicate() = predicate;
            }
        }
    }
}

void fsa_re::add_escape_state(typename fsa_re::fsa_type& fsa0,
                              typename fsa_re::token_type escape_token,
                              const string_type& escape_predicate)
{
    if(escape_token != 0) // unknown
    {
        operation_status status; //??

        auto escape_state(factory::create<fsa_state>(DUMMY_END_STATE_NAME, escape_token));

        (*fsa0).add_state(escape_state, status);

        for(const auto& state_kvp: (*fsa0).states())
        {
            const auto& state(state_kvp.second);

            if(!(*fsa0).is_start_state(state))
            {
                (*fsa0).add_transition(state, escape_state, escape_predicate, status);

                if(!(*fsa0).is_final_state(state))
                {
                    (*state).token() = escape_token;
                    (*fsa0).add_final_state(state, status);
                }
            }
        }

        (*fsa0).add_transition(escape_state, escape_state, escape_predicate, status);
        (*fsa0).add_final_state(escape_state, status);
    }
}

END_NAMESPACE
