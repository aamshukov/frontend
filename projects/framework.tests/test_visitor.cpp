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

#include <symtable/symbol_table_record.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <frontend/parser/parser_dag.hpp>

#include <backend/ir/dag_tac_visitor.hpp>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

struct A : private noncopyable
{
    virtual ~A() {}
};

template <typename T>
struct my_tree : public A, public visitable<my_tree<T>, my_tree<T>, int>
{
    T k;

    my_tree() : k(0)
    {
    }

    my_tree(T i) : k(i)
    {
    }

    virtual ~my_tree() {}

    using visitor_type = visitor<my_tree<T>, my_tree<T>, int>;

    void accept(visitor_type& visitor) override
    {
        visitor.visit(*this);
    }
};

template <typename T>
struct my_visitor : public visitor<my_tree<T>, my_tree<T>, int>
{
    void visit(my_tree<T>& tree) override
    {
        std::wcout << tree.k << std::endl;
    }

    void visit(int& integer) override
    {
        std::wcout << integer << std::endl;
    }
};

void test_visitor()
{
    std::vector<std::shared_ptr<my_tree<int>>> tree;
    my_tree<int> t;

    tree.emplace_back(std::make_shared<my_tree<int>>(1));
    tree.emplace_back(std::make_shared<my_tree<int>>(2));
    tree.emplace_back(std::make_shared<my_tree<int>>(3));
    tree.emplace_back(std::make_shared<my_tree<int>>(4));
    tree.emplace_back(std::make_shared<my_tree<int>>(5));

    std::vector<int> ints;

    ints.emplace_back(100);
    ints.emplace_back(200);
    ints.emplace_back(300);
    ints.emplace_back(400);
    ints.emplace_back(500);

    my_visitor<int> visitor;

    std::for_each(tree.begin(), tree.end(), [&visitor](auto& tree){visitor.visit(*tree);});
    std::for_each(ints.begin(), ints.end(), [&visitor](auto& intg){visitor.visit(intg);});
}
