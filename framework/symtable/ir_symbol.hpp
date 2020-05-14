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

        using index_type = std::size_t;
        using size_type = std::size_t;

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

        using type_type = type;

        enum class flags : uint64_t
        {
            clear = 0x0000
        };

        using flags_type = tmpl_flags<flags>;

    private:
        token_type          my_token;           // link with content
        value_type          my_value;           // inffered value if any, might be integer value, real value or identifier

        std::size_t         my_ssa_id;          // 0 - unassigned, 1+

        type_type           my_type;
        size_type           my_offset;          // runtime offset

        flags_type          my_flags;           // flags

    public:
                            ir_symbol() = default;
                           ~ir_symbol() = default;

        const token_type&   token() const;
        token_type&         token();

        const value_type&   value() const;
        value_type&         value();

        std::size_t         ssa_id() const;
        std::size_t&        ssa_id();

        const type_type&    type() const;
        type_type&          type();

        std::size_t         offset() const;
        std::size_t&        offset();

        flags_type          flags() const;
        flags_type&         flags();
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

template <typename Token>
inline const typename ir_symbol<Token>::type_type& ir_symbol<Token>::type() const
{
    return my_type;
}

template <typename Token>
inline typename ir_symbol<Token>::type_type& ir_symbol<Token>::type()
{
    return my_type;
}

template <typename Token>
inline std::size_t ir_symbol<Token>::offset() const
{
    return my_offset;
}

template <typename Token>
inline std::size_t& ir_symbol<Token>::offset()
{
    return my_offset;
}

template <typename Token>
inline typename ir_symbol<Token>::flags_type ir_symbol<Token>::flags() const
{
    return my_flags;
}

template <typename Token>
inline typename ir_symbol<Token>::flags_type& ir_symbol<Token>::flags()
{
    return my_flags;
}

END_NAMESPACE

#endif // __IR_SYMBOL_H__
