//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class symbol_table : private noncopyable
{
    // hashtable and stack, LeBlanc/Cook

    //public:
    //    using symbol_attribute_type = std::shared_ptr<symbol_attribute<T>>;
    //    using symbol_attributes_type = std::map<std::size_t, symbol_attribute_type>;
    //    using stack_type = std::stack<symbol_attribute_type>;

    //private:
    //    symbol_attributes_type  table;
    //    stack_type              stack; // display

    public:
                                symbol_table();
                               ~symbol_table();
    //    // enter scope
    //    // leave scope

    //    // add
    //    // get/lookup
};

END_NAMESPACE

#endif // __SYMBOL_TABLE_H__
