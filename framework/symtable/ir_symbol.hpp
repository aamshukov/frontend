//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_SYMBOL_H__
#define __IR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class ir_symbol : private noncopyable
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
                                        void*,
                                        datum_type,
                                        codepoints_type>;

        using symbol_type = std::shared_ptr<ir_symbol<token_type>>;
        using symbols_type = std::vector<symbol_type>;

    private:
        token_type          my_token;           // link with content
        value_type          my_value;           // inffered value if any

        std::size_t         my_ssa_id;          // 0 - unassigned, 1+

        //type                my_type;

    public:
                            ir_symbol() = default;
                           ~ir_symbol() = default;

        const token_type&   token() const;
        token_type&         token();

        const value_type&   value() const;
        value_type&         value();

        std::size_t         ssa_id() const;
        std::size_t&        ssa_id();
};

template <typename Token>
inline const typename ir_symbol<Token>::token_type& ir_symbol<Token>::token() const
{
    return my_token;
}

template <typename Token>
inline typename ir_symbol<Token>::token_type& ir_symbol<Token>::token()
{
    return my_token;
}

template <typename Token>
inline const typename ir_symbol<Token>::value_type& ir_symbol<Token>::value() const
{
    return my_value;
}

template <typename Token>
inline typename ir_symbol<Token>::value_type& ir_symbol<Token>::value()
{
    return my_value;
}

template <typename Token>
inline std::size_t ir_symbol<Token>::ssa_id() const
{
    return my_ssa_id;
}

template <typename Token>
inline std::size_t& ir_symbol<Token>::ssa_id()
{
    return my_ssa_id;
}

END_NAMESPACE

#endif // __IR_SYMBOL_H__
