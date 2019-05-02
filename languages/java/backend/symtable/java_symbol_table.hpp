//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __JAVA_SYMBOL_TABLE_H__
#define __JAVA_SYMBOL_TABLE_H__

#pragma once

BEGIN_NAMESPACE(java)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

class java_symbol_table : public symbol_table<token<java_token_traits>>
{
    // hashtable and stack, LeBlanc/Cook

    //public:
    //    using symbol_attribute_type = std::shared_ptr<symbol_attribute<T>>;
    //    using symbol_attributes_type = std::map<std::size_t, symbol_attribute_type>;
    //    using stack_type = std::stack<symbol_attribute_type>;

    //private:
    //    symbol_attributes_type  table;
    //    stack_type              stack; // display

    //public:
    //                            symbol_table();
    //                           ~symbol_table();
    //    // enter scope
    //    // leave scope

    //    // add
    //    // get/lookup
};

END_NAMESPACE

#endif // __JAVA_SYMBOL_TABLE_H__
