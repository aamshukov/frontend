//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TEXT_H__
#define __TEXT_H__

#pragma once

BEGIN_NAMESPACE(core)

#define NUMERIC_BUFFER_PARSE_SIZE (64)

class text : private noncopyable
{
    public:
        using datum_type = cp_type;

    public:
        static constexpr datum_type ascii_numbers[128] =
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
            0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

    public:
        static const datum_type invalid_codepoint = 0x0000FFFD;

        // http://www.unicode.org/glossary/#supplementary_code_point
        static const datum_type kSupplementaryCodePointStart = 0x010000; //?? rename
        static const datum_type kSupplementaryCodePointEnd = 0x10FFFF;

        // http://www.unicode.org/glossary/#high_surrogate_code_unit
        static const datum_type kHighSurrogateStart = 0x0000D800;
        static const datum_type kHighSurrogateEnd = 0x0000DBFF;

        // http://www.unicode.org/glossary/#low_surrogate_code_unit
        static const datum_type kLowSurrogateStart = 0x0000DC00;
        static const datum_type kLowSurrogateEnd = 0x0000DFFF;

        static const datum_type kSurrogateStart = kHighSurrogateStart;
        static const datum_type kSurrogateEnd = kLowSurrogateEnd;

    public:
        static bool         string_to_codepoints0(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, size_type& count, operation_status& status);
        static bool         codepoints_to_string0(const datum_type* codepoints, size_type count, string_type& result_text, operation_status& status);

        static bool         string_to_codepoints(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, size_type& count, operation_status& status);
        static bool         codepoints_to_string(const datum_type* codepoints, size_type count, string_type& result_text, operation_status& status);

        static string_type  codepoint_to_string(datum_type codepoint);
        static bool         codepoint_to_string(datum_type codepoint, string_type& result_text, operation_status& status);

        static datum_type   epsilon_codepoint();
        static datum_type   bad_codepoint();

        static datum_type   make_codepoint(datum_type high_surrogate_codeunit, datum_type low_surrogate_codeunit);

        static bool         is_high_surrogate(datum_type codeunit);
        static bool         is_low_surrogate(datum_type codeunit);
        static bool         is_surrogate(datum_type codeunit);
        static bool         is_surrogate_pair(datum_type high_surrogate_codeunit, datum_type low_surrogate_codeunit);

        static bool         is_in_range(datum_type codepoint, datum_type r0, datum_type r1);

        static string_type  trim(const string_type& text, const string_type& delimiters);
        static void         trim(const string_type& text, const string_type& delimiters, string_type& result_text);

        static void         split(const string_type& text, const string_type& delimiters, std::vector<string_type>& parts);

        static bool         starts_with(const string_type& text, const char_type* prefix);
        static bool         starts_with(const string_type& text, const string_type& prefix);

        static bool         is_digit(datum_type codepoint);

        static bool         is_identifier_start(datum_type codepoint);
        static bool         is_identifier_part(datum_type codepoint);

        static bool         is_java_identifier_start(datum_type codepoint);
        static bool         is_java_identifier_part(datum_type codepoint);

        static bool         is_identifier_ignorable(datum_type codepoint);

    public:
        template <typename T>
        static T parse(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static int8_t parse<int8_t>(const text::datum_type* lexeme, uint32_t  length, int base);

        template <>
        static uint8_t parse<uint8_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static int16_t parse<int16_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint16_t parse<uint16_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static int32_t parse<int32_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint32_t parse<uint32_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static int64_t parse<int64_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint64_t parse<uint64_t>(const text::datum_type* lexeme, uint32_t length, int base);

        template <>
        static float parse<float>(const text::datum_type* lexeme, uint32_t length, int);

        template <>
        static double parse<double>(const text::datum_type* lexeme, uint32_t length, int);
};

inline typename text::datum_type text::epsilon_codepoint()
{
    static datum_type result(0x000003B5);
    return result;
}

inline typename text::datum_type text::bad_codepoint()
{
    static datum_type result(0x0F000002);
    return result;
}

inline typename text::datum_type text::make_codepoint(typename text::datum_type high_surrogate_codeunit, typename text::datum_type low_surrogate_codeunit)
{
    return ((high_surrogate_codeunit - kHighSurrogateStart) << 10/* divide by 0x400*/) +
            (low_surrogate_codeunit - kLowSurrogateStart) + kSupplementaryCodePointStart;
}

inline bool text::is_high_surrogate(typename text::datum_type codeunit)
{
    return codeunit >= kHighSurrogateStart && codeunit <= kHighSurrogateEnd;
}

inline bool text::is_low_surrogate(typename text::datum_type codeunit)
{
    return codeunit >= kLowSurrogateStart && codeunit <= kLowSurrogateEnd;
}

inline bool text::is_surrogate(typename text::datum_type codeunit)
{
    return codeunit >= kSurrogateStart && codeunit <= kSurrogateEnd;
}

inline bool text::is_surrogate_pair(typename text::datum_type high_surrogate_codeunit, typename text::datum_type low_surrogate_codeunit)
{
    return is_high_surrogate(high_surrogate_codeunit) && is_low_surrogate(low_surrogate_codeunit);
}

inline bool text::is_in_range(datum_type codepoint, datum_type r0, datum_type r1)
{
    return codepoint >= r0 && codepoint <= r1;
}

inline bool text::is_digit(typename text::datum_type codepoint)
{
    return u_isdigit(codepoint);
}

inline bool text::is_identifier_start(typename text::datum_type codepoint)
{
    return u_isIDStart(codepoint);
}

inline bool text::is_identifier_part(typename text::datum_type codepoint)
{
    return u_isIDPart(codepoint);
}

inline bool text::is_java_identifier_start(typename text::datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
            result = true;
        default:
            result = u_isJavaIDStart(codepoint);
    }

    return result;
    //return u_isJavaIDStart(codepoint);
}

inline bool text::is_java_identifier_part(typename text::datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            result = true;
        default:
            result = u_isJavaIDPart(codepoint);
    }

    return result;
    //return u_isJavaIDPart(codepoint);
}

inline bool text::is_identifier_ignorable(datum_type codepoint)
{
    return u_isIDIgnorable(codepoint);
}

#define PARSE_PROLOG                                                    \
    char_type buffer[NUMERIC_BUFFER_PARSE_SIZE];                        \
                                                                        \
    const datum_type* ptr(lexeme);                                      \
                                                                        \
    uint32_t k = 0;                                                     \
    uint32_t n = std::min<uint32_t>(length, NUMERIC_BUFFER_PARSE_SIZE); \
                                                                        \
    for(uint32_t i = 0; i < n; i++, ptr++)                              \
    {                                                                   \
        if(*ptr != L'_')                                                \
        {                                                               \
            *(buffer + k++) = static_cast<char_type>(*ptr);             \
        }                                                               \
    }                                                                   \
                                                                        \
    *(buffer + k) = 0;                                                  \
                                                                        \
    char_type* end;


template <typename T>
inline T text::parse(const text::datum_type* lexeme, uint32_t length, int base)
{
    return T {};
}

template <>
inline int8_t text::parse<int8_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int8_t result = static_cast<int8_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint8_t text::parse<uint8_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint8_t result = static_cast<uint8_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int16_t text::parse<int16_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int16_t result = static_cast<int16_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint16_t text::parse<uint16_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint16_t result = static_cast<uint16_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int32_t text::parse<int32_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int32_t result = static_cast<int32_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint32_t text::parse<uint32_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint32_t result = static_cast<uint32_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int64_t text::parse<int64_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int64_t result = static_cast<int64_t>(std::wcstoll(buffer, &end, base));

    return result;
}

template <>
inline uint64_t text::parse<uint64_t>(const text::datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint64_t result = static_cast<uint64_t>(std::wcstoll(buffer, &end, base));

    return result;
}

template <>
inline float text::parse<float>(const text::datum_type* lexeme, uint32_t length, int)
{
    PARSE_PROLOG

    float result = std::wcstof(buffer, &end);

    return result;
}

template <>
inline double text::parse<double>(const text::datum_type* lexeme, uint32_t length, int)
{
    PARSE_PROLOG

    double result = std::wcstod(buffer, &end);

    return result;
}

END_NAMESPACE

#endif // __TEXT_H__
