//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>
#include <core/flags.hpp>
#include <core/counter.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/domain_helper.hpp>

#include <core/status.hpp>
#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/text.hpp>
#include <core/unicode.hpp>

#include <core/data_provider.hpp>
#include <core/file_data_provider.hpp>
#include <core/string_data_provider.hpp>

#include <core/content.hpp>

#include <core/command_line.hpp>
#include <core/configurator.hpp>

#include <core/dag.hpp>
#include <core/tree.hpp>

#include <core/visitor.hpp>
#include <core/visitable.hpp>

#include <core/logger.hpp>

#include <frontend/grammar/token.hpp>
#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>
#include <symtable/symbol_table_builder.hpp>
#include <symtable/symbol_table_builder.inl>

#include <frontend/parser/parse_dag.hpp>

#include <backend/ir/dag_tac_visitor.hpp>


USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

struct A : private noncopyable
{
    virtual ~A() {}
};

//template <typename V>
//struct my_visitor;
//struct my_visitor0;

template <typename T, typename U>
struct my_tree0 : public A, public visitable//<my_tree0<T, U>>
{
    T k;
    U u;

    my_tree0() : k(0), u(0)
    {
    }

    my_tree0(T i, U u) : k(i), u(u)
    {
    }

    virtual ~my_tree0() {}

    //int accept(my_visitor& visitor, const char& param) override
    //template <typename TReturn, typename TParam, typename TVisitor>
    template <typename TVisitor>
    typename TVisitor::return_type accept(TVisitor& visitor, const typename TVisitor::param_type& param)
    {
        if constexpr (std::is_void_v<typename TVisitor::return_type>)
        {
            visitor.visit(*this, param);
        }
        else
        {
            return visitor.visit(*this, param);
        }
    }


    //using visitor_type = visitor<int, int, my_tree<T, U>, int>;

    //int accept(visitor_type& visitor, const int& k) override
    //{
    //    return visitor.visit(*this, k);
    //}

    //template <typename TReturn, typename TParam> //??
    //TReturn accept(visitor<my_tree<T, U>, int>& visitor, const TParam& param)
    //{
    //    return visitor.visit(*this, param);
    //}
};

template <typename T, typename U>
struct my_tree : public my_tree0<T, U>
{
    my_tree() : my_tree0(0, 0)
    {
    }

    my_tree(T i, U u) : my_tree0(i, u)
    {
    }

    virtual ~my_tree() {}

    template <typename TVisitor>
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

//template <typename V>
struct my_visitor : public visitor<int, char, my_tree<int, char>, my_tree<int, char>, my_tree<int, char>>, private noncopyable
{
    int visit(my_tree<int, char>& tree, const char& param) override
    {
        std::wcout << tree.k + param << tree.u << std::endl;
        return 0;
    }

    //int visit(int& integer, const int& k) override
    //{
    //    std::wcout << integer << std::endl;
    //    return 0;
    //}
};

struct my_visitor0 : public visitor<void, long, my_tree<int, char>>
{
    void visit(my_tree<int, char>& tree, const long& k) override
    {
        std::cout << tree.k + k << " - 0 - " << tree.u << std::endl;
    }
};

void test_visitor()
{
    std::vector<std::shared_ptr<my_tree<int, char>>> tree;

    my_tree<int, char> t;

    tree.emplace_back(std::make_shared<my_tree<int, char>>(1, 'O'));
    tree.emplace_back(std::make_shared<my_tree<int, char>>(2, 'D'));
    tree.emplace_back(std::make_shared<my_tree<int, char>>(3, 'T'));
    tree.emplace_back(std::make_shared<my_tree<int, char>>(4, 'C'));
    tree.emplace_back(std::make_shared<my_tree<int, char>>(5, 'P'));

    std::vector<int> ints;

    ints.emplace_back(100);
    ints.emplace_back(200);
    ints.emplace_back(300);
    ints.emplace_back(400);
    ints.emplace_back(500);

    //my_visitor<my_tree<int, char>> visitor;
    my_visitor visitor;

    //std::for_each(tree.begin(), tree.end(), [&visitor](auto& tree){(*tree).accept(visitor, 5);});
    //std::for_each(tree.begin(), tree.end(), [&visitor](auto& tree){(*tree).accept<int, char, my_visitor>(visitor, '4');});
    std::for_each(tree.begin(), tree.end(), [&visitor](auto& tree){(*tree).accept<my_visitor>(visitor, '4');});

    my_visitor0 visitor0;
    std::for_each(tree.begin(), tree.end(), [&visitor0](auto& tree){(*tree).accept<my_visitor0>(visitor0, 15);});
    //std::for_each(tree.begin(), tree.end(), [&visitor0](auto& tree){(*tree).accept<void, long, my_visitor0>(visitor0, 4);});

    //std::for_each(ints.begin(), ints.end(), [&visitor](auto& intg){visitor.visit(intg, 6);});
}
