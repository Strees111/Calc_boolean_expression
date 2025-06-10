#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "boolexpr.h"
#include "Error.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace global
{
    std::unordered_map<int, std::vector<BooleanExpression>> s;
}

void add_expression(int id, const std::string& line)
{
    try
    {
        BooleanExpression a(line.c_str());
        global::s[id].push_back(a);
    }
    catch (const Error& e)
    {
        throw std::runtime_error(e.what());
    }
}

std::string delete_expression(int id, int index)
{
    if (global::s[id].size() <= index || index < 0) throw std::invalid_argument("Invalid expression index");
    std::string result = std::string(global::s[id][index]);
    global::s[id].erase(global::s[id].begin() + index);
    return result;
}

void clear_expressions(int id)
{
    global::s[id].clear();
}

bool empty_expressions(int id)
{
    return global::s[id].empty();
}

std::string print_expressions(int id)
{
    std::ostringstream output;
    int i = 1;
    for (auto const& expr : global::s[id])
    {
        output << i << ". " << std::string(expr) << std::endl;
        ++i;
    }
    return output.str();
}
std::string get_expression(int id, int index)
{
    std::ostringstream output;
    output << std::string(global::s[id][index]) << std::flush;
    return output.str();
}

std::string process_expression(const std::string& mode, int id)
{
    std::ostringstream output;
    if (mode == "-isfull")
    {
        output << (isFullSystem(global::s[id]) ? "yes" : "no");
    }
    else if (mode == "-zh")
    {
        for (auto& expr : global::s[id])
        {
            output << expr.GetZhegalkin() << std::endl;
        }
    }
    else if (mode == "-table")
    {
        for (auto& expr : global::s[id])
        {
            output << expr.table() << std::endl;
        }
    }
    else if (mode == "-FullTable")
    {
        for (auto& expr : global::s[id])
        {
            output << expr.GetTable() << '=' << std::endl;
        }
    }
    return output.str();
}

int get_user_count() {
    return global::s.size();
}

PYBIND11_MODULE(boolcalc, m)
{
    m.def("get_user_count", &get_user_count);
    m.def("process_expression", &process_expression);
    m.def("add_expression", &add_expression);
    m.def("delete_expression", &delete_expression);
    m.def("clear_expressions", &clear_expressions);
    m.def("empty_expressions", &empty_expressions);
    m.def("print_expressions", &print_expressions);
    m.def("get_expression", &get_expression);
}
