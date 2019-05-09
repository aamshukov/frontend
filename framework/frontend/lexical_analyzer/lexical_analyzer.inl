//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LEXICAL_ANALYZER_INL__
#define __LEXICAL_ANALYZER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
lexical_analyzer<Token>::lexical_analyzer(const typename lexical_analyzer<Token>::content_type& content)
                       : my_content(content),
                         my_start_content((*my_content).data()),
                         my_end_content(my_start_content + (*my_content).count()),
                         my_ptr(my_start_content),
                         my_ptr_lexeme(nullptr),
                         my_pending_indents(0),
                         my_indent(0),
                         my_boll(false), // not implemented by default
                         my_eoll(false)
{
    my_indents.reserve(our_max_indents);
}

template <typename Token>
lexical_analyzer<Token>::~lexical_analyzer()
{
}

template <typename Token>
void lexical_analyzer<Token>::next_lexeme()
{
    if(!is_eos())
    {
        if(!my_tokens.empty())
        {
            my_token = std::move(my_tokens.front());

            my_tokens.pop();

            my_ptr = my_start_content + my_token.offset;
        }
        else
        {
            prolog();
            next_lexeme_impl();
            epilog();
        }
    }
}

template <typename Token>
const typename lexical_analyzer<Token>::token_type& lexical_analyzer<Token>::lookahead_lexeme()
{
    token_type* result(nullptr);

    // check if anything need to be done
    if(is_eos())
    {
        result = &my_token;
    }
    else
    {
        // push state
        const datum_type* aux_ptr(my_ptr);

        token_type aux_token(my_token);

        // update state
        if(!my_tokens.empty())
        {
            const token_type& token(my_tokens.back());

            my_ptr = my_start_content + token.offset + token.length;
        }

        // get lexeme
        prolog();
        next_lexeme_impl();
        epilog();

        bool push_token = true;

        if(is_eos() && !my_tokens.empty())
        {
            const token_type& token(my_tokens.back());
            push_token = token.type != token_type::traits::type::eos;
        }

        if(push_token)
        {
            my_tokens.emplace(std::move(my_token));
        }

        // pop state
        my_token = std::move(aux_token);

        my_ptr = aux_ptr;

        // result
        result = &my_tokens.back();
    }

    return *result;
}

template <typename Token>
inline void lexical_analyzer<Token>::prolog()
{
    my_token.reset();

    my_ptr_lexeme = my_ptr;

    my_boll = false;
    my_eoll = false;
}

template <typename Token>
inline void lexical_analyzer<Token>::epilog()
{
    if(my_ptr > my_end_content)
    {
        my_ptr = my_end_content;
    }

    my_token.offset = std::ptrdiff_t(my_ptr_lexeme - my_start_content);
    my_token.length = static_cast<uint32_t>(std::ptrdiff_t(my_ptr - my_ptr_lexeme));

    my_token.literal.assign(my_ptr_lexeme, my_token.length);

    //??
    if(my_boll && my_token.type == token_type::traits::type::ws)
    {
        std::size_t length = my_token.literal.size(); //?? need to verify only spaces must be here, if tabs report tab-error


        if(length == my_indents[my_indent])
        {
            // same indent, continue
        }
        else if(length > my_indents[my_indent])
        {
            if(my_indent + 1 >= our_max_indents)
            {
                //?? error, too deep
                my_token.type = token_type::traits::type::unknown;
            }

            my_pending_indents++;
            my_indents[++my_indent] = length;
        }
        else // if(length > my_indents[my_indent])
        {
            while(my_indent > 0 && length < my_indents[my_indent])
            {
                my_pending_indents--;
                my_indent--;
            }

            if(length != my_indents[my_indent])
            {
                //?? error dedent
                my_token.type = token_type::traits::type::unknown;
            }
        }



        if(my_pending_indents != 0)
        {
            if(my_pending_indents < 0)
            {
                my_pending_indents++;
                my_token.type = token_type::traits::type::dedent;
            }
            else
            {
                my_pending_indents--;
                my_token.type = token_type::traits::type::indent;
            }
        }



        my_boll = true;
        my_eoll = true; // ... default implementation assumes each eol is logical eol
    }
}

END_NAMESPACE

#endif // __LEXICAL_ANALYZER_INL__
