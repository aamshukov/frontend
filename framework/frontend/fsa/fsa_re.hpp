//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_RE_H__
#define __FSA_RE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

#define IDENTIFIER_START (L"S") // should not be a part of any set of chars - keywords or any special symbols in language
#define IDENTIFIER_PART  (L"Q") // should not be a part of any set of chars - keywords or any special symbols in language


class fsa_re : private noncopyable
{
    public:
        using datum_type = content::datum_type;

        using token_type = fsa::token_type;
        
        using fsa_type = fsa::fsa_type;

    private:
        static datum_type   get_codepoint(const datum_type*& p_src);
        static int8_t       get_operator_precedence(datum_type op);

        static bool         is_literal(datum_type ch);

        static size_type    preprocess(const std::shared_ptr<datum_type[]>& infix_re, size_type count, std::shared_ptr<datum_type[]>& processed_re);
        static bool         infix_to_postfix(const std::shared_ptr<datum_type[]>& infix_re, size_type count, std::shared_ptr<datum_type[]>& postfix_refsa, operation_status& status);

        static bool         process_combine(std::stack<fsa::fsa_type>& fragments, operation_status& status);
        static bool         process_concatenate(std::stack<fsa::fsa_type>& fragments, operation_status& status);
        static bool         process_zero_or_more(std::stack<fsa::fsa_type>& fragments, operation_status& status);
        static bool         process_one_or_more(std::stack<fsa::fsa_type>& fragments, operation_status& status);
        static bool         process_zero_or_one(std::stack<fsa::fsa_type>& fragments, operation_status& status);
        static bool         process_literal(const datum_type*& p_src, std::stack<fsa::fsa_type>& fragments, operation_status& status);

        static void         adjust_predicates(typename fsa_re::fsa_type& fsa0);
        static void         add_escape_state(typename fsa_re::fsa_type& fsa0, token_type escape_token, const string_type& escape_predicate);

    public:
        static bool         re_to_fsa(const std::shared_ptr<datum_type[]>& re,
                                      size_type count,
                                      token_type token,
                                      token_type escape_token,
                                      const string_type& escape_predicate,
                                      fsa_type& result_fsa,
                                      operation_status& status);
};

END_NAMESPACE

#endif // __FSA_RE_H__
