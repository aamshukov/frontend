//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TOKEN_H__
#define __TOKEN_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

struct token_traits
{
};

template <typename Traits = token_traits>
struct token
{
    using datum_type = text::datum_type;
    using codepoints_type = std::basic_string<datum_type>;

    enum class flags : uint64_t
    {
        clear     = 0x00,
        genuine   = 0x02,
        synthetic = 0x04  // additional (artificial) tokens which are inserted into the token stream ...
    };

    using flags_type = tmpl_flags<flags>;

    using traits = Traits;
    using token_type = typename traits::type;

    token_type      type;    // type of lexeme

    loc_t           offset;  // offset in context (absolute address)
    uint32_t        length;  // length of lexeme

    codepoints_type literal; // string or char literal

    flags_type      flags;

    private:
        void accept(const token& other)
        {
            if(this != &other)
            {
                type = other.type;

                offset = other.offset;
                length = other.length;

                literal = other.literal;

                flags = other.flags;
            }
        }

    public:
        token()
        {
            reset();
        }

        token(const token& other)
        {
            accept(other);
        }
        
        token(token&& other)
        {
            accept(other);
        }

        const token& operator = (const token& other)
        {
            accept(other);
            return *this;
        }

        token& operator = (token&& other)
        {
            accept(other);
            return *this;
        }

        void reset()
        {
            type = token_type::unknown;

            offset = INVALID_VALUE;
            length = INVALID_VALUE;

            flags = flags::clear;
            flags |= flags::genuine;
        }

        codepoints_type codepoints(const datum_type* content) const
        {
            codepoints_type result;
            result.assign(content + offset, length);
            return result;
        }

        string_type to_string(const datum_type* content) const
        {
            string_type result;
            
            operation_status status; //??
            text::codepoints_to_string(content + offset, length, result, status);

            result += L" [" + name() + L"]:" + std::to_wstring(length);
            
            return result;
        }

        string_type name() const
        {
            return name(type);
        }

        static string_type name(const typename traits::type& token_type)
        {
            if(traits::mapping.empty())
            {
                traits::initialize();
            }

            return traits::name(token_type);
        }
};

END_NAMESPACE

#endif // __TOKEN_H__
