//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TREE_H__
#define __ARCTURUS_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token, typename TreeTraits>
struct arcturus_tree : public parse_tree<Token, TreeTraits>
{
    using token_type = typename parse_tree<Token, TreeTraits>::token_type;
    using tree_traits_type = typename parse_tree<Token, TreeTraits>::tree_traits_type;

    //using symbol_type = grammar::symbol_type;

    //using record_type = typename ir_symbol<token_type>::record_type;
    //using records_type = typename ir_symbol<token_type>::records_type;

    //using visitable_type = arcturus_tree<token_type, tree_traits_type>;
    //using visitor_type = visitor<visitable_type>;

    arcturus_tree()
    {
    }

    virtual ~arcturus_tree()
    {
    }

    template <typename TVisitor> // deficiency of C++ ... template methods cannot be virtual ...
    typename TVisitor::return_type accept(TVisitor& visitor, const typename TVisitor::param_type& param)
    {
        if constexpr (std::is_void_v<typename TVisitor::return_type>)
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                visitor.visit(*this);
            }
            else
            {
                visitor.visit(*this, param);
            }
        }
        else
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                return visitor.visit(*this);
            }
            else
            {
                return visitor.visit(*this, param);
            }
        }
    }
};

END_NAMESPACE

#endif // __ARCTURUS_TREE_H__
