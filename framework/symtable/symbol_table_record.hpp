//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __record_H__
#define __record_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class symbol_table_record : private noncopyable
{
    public:
        using token_type = Token;

        using datum_type = text::datum_type;
        using codepoints_type = std::basic_string<datum_type>;

        using value_type = std::variant<int8_t,
                                        uint8_t,
                                        int16_t,
                                        uint16_t,
                                        int32_t,
                                        uint32_t,
                                        int64_t,
                                        uint64_t,
                                        float,
                                        double,
                                        codepoints_type>;

        using record_type = std::shared_ptr<symbol_table_record<token_type>>;
        using records_type = std::vector<record_type>;

    private:
        token_type          my_token;
	    value_type          my_value;

        std::size_t         my_ssa_id;          // 0 - unassigned, 1+

    public:
                            symbol_table_record() = default;
                           ~symbol_table_record() = default;

        const token_type&   token() const;
        token_type&         token();

        const value_type&   value() const;
        value_type&         value();

        std::size_t         ssa_id() const;
        std::size_t&        ssa_id();
};

template <typename Token>
inline const typename symbol_table_record<Token>::token_type& symbol_table_record<Token>::token() const
{
    return my_token;
}

template <typename Token>
inline typename symbol_table_record<Token>::token_type& symbol_table_record<Token>::token()
{
    return my_token;
}

template <typename Token>
inline const typename symbol_table_record<Token>::value_type& symbol_table_record<Token>::value() const
{
    return my_value;
}

template <typename Token>
inline typename symbol_table_record<Token>::value_type& symbol_table_record<Token>::value()
{
    return my_value;
}

template <typename Token>
inline std::size_t symbol_table_record<Token>::ssa_id() const
{
    return my_ssa_id;
}

template <typename Token>
inline std::size_t& symbol_table_record<Token>::ssa_id()
{
    return my_ssa_id;
}

END_NAMESPACE

#endif // __record_H__
