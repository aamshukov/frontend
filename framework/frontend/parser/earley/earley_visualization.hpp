//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_VISUALIZATION_H__
#define __EARLEY_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class earley_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

    public:
};

END_NAMESPACE

#endif // __EARLEY_VISUALIZATION_H__
