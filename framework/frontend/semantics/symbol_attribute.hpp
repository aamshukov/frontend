//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SYMBOL_ATTRIBUTE_H__
#define __SYMBOL_ATTRIBUTE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class symbol_attribute : private noncopyable
{
    public:
        using token_type = T;

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
    private:
        token_type          my_token;
	    value_type          my_value;

    public:
                            symbol_attribute();
                           ~symbol_attribute();

        const token_type&   token() const;
        token_type&         token();

        const value_type&   value() const;
        value_type&         value();
};

template <typename T>
inline const typename symbol_attribute<T>::token_type& symbol_attribute<T>::token() const
{
    return my_token;
}

template <typename T>
inline typename symbol_attribute<T>::token_type& symbol_attribute<T>::token()
{
    return my_token;
}

template <typename T>
inline const typename symbol_attribute<T>::value_type& symbol_attribute<T>::value() const
{
    return my_value;
}

template <typename T>
inline typename symbol_attribute<T>::value_type& symbol_attribute<T>::value()
{
    return my_value;
}

END_NAMESPACE

#endif // __SYMBOL_ATTRIBUTE_H__
