//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class lexical_analyzer : private noncopyable
{
    public:
        using content_type = std::shared_ptr<content>;

        using datum_type = text::datum_type;
        using codepoints_type = std::basic_string<datum_type>;

        using token_type = T;
        using tokens_type = std::queue<token_type>;

        using snapshots_type = std::stack<const datum_type*>;

    protected:
        content_type                my_content;         // loaded content

        const datum_type*           my_start_content;   // begining of content
        const datum_type*           my_end_content;     // end of content

        token_type                  my_token;           // current lexeme
        tokens_type                 my_tokens;          // current and lookahead lexemes

        const datum_type*           my_ptr;             // current position in content
        const datum_type*           my_ptr_lexeme;      // begining position of lexeme in content

        snapshots_type              my_snapshots;       // backtracking's snapshots

    private:
        void                        prolog();
        void                        epilog();

    protected:
        virtual void                next_lexeme_impl() = 0;
                                    //{
                                    //    // !!! -- generated code -- !!!
                                    //    // !!! -- end of generated code -- !!!
                                    //}
    public:
                                    lexical_analyzer(const content_type& content);
                                   ~lexical_analyzer();

        const content_type&         content() const;
        content_type&               content();

        const token_type&           token();

        bool                        is_eol() const;
        bool                        is_eos() const;

        void                        next_lexeme();
        const token_type&           lookahead_lexeme();

        codepoints_type             lexeme_to_codepoints() const;
        codepoints_type             lexeme_to_codepoints(const token_type& token) const;

        string_type                 lexeme_to_string() const;
        string_type                 lexeme_to_string(const token_type& token) const;

        void                        take_snapshot();
        void                        rewind_to_snapshot(); // backtrack
};

template <typename T>
inline const typename lexical_analyzer<T>::content_type& lexical_analyzer<T>::content() const
{
    return my_content;
}

template <typename T>
inline typename lexical_analyzer<T>::content_type& lexical_analyzer<T>::content()
{
    return const_cast<content_type&>(static_cast<const lexical_analyzer&>(*this).content());
}

template <typename T>
inline const typename lexical_analyzer<T>::token_type& lexical_analyzer<T>::token()
{
    return my_token;
}

template <typename T>
inline bool lexical_analyzer<T>::is_eol() const
{
    return my_token.type == token_type::traits::type::eol;
}

template <typename T>
inline bool lexical_analyzer<T>::is_eos() const
{
    return my_token.type == token_type::traits::type::eos;
}

template <typename T>
inline typename lexical_analyzer<T>::codepoints_type lexical_analyzer<T>::lexeme_to_codepoints() const
{
    return my_token.codepoints(my_start_content);
}

template <typename T>
inline typename lexical_analyzer<T>::codepoints_type lexical_analyzer<T>::lexeme_to_codepoints(const typename lexical_analyzer<T>::token_type& token) const
{
    return token.codepoints(my_start_content);
}

template <typename T>
inline string_type lexical_analyzer<T>::lexeme_to_string() const
{
    return my_token.to_string(my_start_content);
}

template <typename T>
inline string_type lexical_analyzer<T>::lexeme_to_string(const typename lexical_analyzer<T>::token_type& token) const
{
    return token.to_string(my_start_content);
}

template <typename T>
inline void lexical_analyzer<T>::take_snapshot()
{
    my_snapshots.push(my_ptr);
}

template <typename T>
inline void lexical_analyzer<T>::rewind_to_snapshot()
{
    if(!my_snapshots.empty())
    {
        my_ptr = my_snapshots.top();
        my_snapshots.pop();

        my_tokens.clear();
    }
}

END_NAMESPACE

#define CURRENT_LOCATION get_line_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme))), get_column_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme))) //??

#include <frontend\lexical_analyzer\lexical_analyzer.inl>

#endif // __LEXICAL_ANALYZER_H__
