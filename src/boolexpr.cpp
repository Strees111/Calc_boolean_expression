#include "boolexpr.h"
#include "Error.h"

#include <memory>
#include <set>
#include <stack>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstring>

BooleanExpressionNode* BooleanExpression::Postfix2Tree(const char* str)
{
    int index = 0;
    std::stack<BooleanExpressionNode*> S{};
    std::string str1;
    BooleanExpressionNode* result = nullptr;
    BooleanExpressionNode* right = nullptr;
    BooleanExpressionNode* left = nullptr;

    try
    {
        char ch;
        while ((ch = str[index]) != '\0')
        {
            if (ch == 'x')
            {
                str1 += ch;
                ++index;
                continue;
            }
            left = right = nullptr;
            switch (ch)
            {
            case '&':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new ConjunctionNode(left, right);
                break;
            case 'v':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new DisjunctionNode(left, right);
                break;
            case '>':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new ImplicationNode(left, right);
                break;
            case '<':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new ConverseNode(left, right);
                break;
            case '=':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new EquivalentNode(left, right);
                break;
            case '|':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new NANDNode(left, right);
                break;
            case '^':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new NORNode(left, right);
                break;
            case '+':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                left = S.top();
                S.pop();
                result = new XORNode(left, right);
                break;
            case '~':
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new NegationNode(left);
                break;
            default:
                if (ch >= '0' && ch <= '9' && !str1.empty())
                {
                    str1 += ch;
                    result = new VarNode(str1, 0);
                }
                else if (ch >= '0' && ch <= '1' && str1.empty())
                {
                    result = new ConstNode(ch - '0');
                }
            }
            S.push(result);
            ++index;
            str1.clear();
        }
        if (S.size() != 1)
        {
            left = right = nullptr;
            throw 2;
        }
        return S.top();
    }
    catch (...)
    {
        if (left) delete left;
        if (right) delete right;
        while (!S.empty())
        {
            left = S.top();
            S.pop();
            delete left;
        }
        throw ErrorPostfix(str, index);
    }
}


const unsigned char ActionsTable[][14] = {
    // 0  ~  &  v  +  >  <  =  |  ^  (  )  P
    {7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 1}, // 0 пусто
    {3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // ~ отрицание
    {3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // & конъюнкция
    {3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // v дизъюнкция
    {3, 2, 2, 2, 3, 3, 3, 3, 2, 3, 2, 3, 1}, // + XOR
    {3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 3, 1}, // > импликация
    {3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 3, 1}, // < обратная имп.
    {3, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 3, 1}, // = эквивалентн.
    {3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // | дизъюнкция
    {3, 2, 2, 2, 3, 3, 3, 3, 2, 3, 2, 3, 1}, // ^ XOR
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1}, // ( скобка
};

int actionsRowNumber(char ch)
{
    switch (ch)
    {
    case 0: return 0;
    case '~': return 1;
    case '&': return 2;
    case 'v': return 3;
    case '+': return 4;
    case '>': return 5;
    case '<': return 6;
    case '=': return 7;
    case '|': return 8;
    case '^': return 9;
    case '(': return 10;
    }
    return 11;
}

int actionsColNumber(const char ch)
{
    switch (ch)
    {
    case 0: return 0;
    case '~': return 1;
    case '&': return 2;
    case 'v': return 3;
    case '+': return 4;
    case '>': return 5;
    case '<': return 6;
    case '=': return 7;
    case '|': return 8;
    case '^': return 9;
    case '(': return 10;
    case ')': return 11;
    }
    if (ch == 'x' ||
        (ch >= '0' && ch <= '9'))
        return 12;
    return 13;
}

void BooleanExpression::Infix2Postfix(const char* instr, char* outstr)
{
    int i = 0;
    int j = 0;
    int row, col;
    unsigned char action;
    std::string current_operand_name;
    std::stack<char> S;
    do
    {
        if (instr[i] == 'x')
        {
            current_operand_name += instr[i++];
            while ((instr[i] >= '0' && instr[i] <= '9'))
            {
                current_operand_name += instr[i++];
            }
            for (const char c : current_operand_name)
            {
                outstr[j++] = c;
            }
            current_operand_name.clear();
            continue;
        }
        col = actionsColNumber(instr[i]);
        row = S.empty() ? 0 : actionsRowNumber(S.top());
        action = ActionsTable[row][col];
        switch (action)
        {
        case 1: outstr[j++] = instr[i];
            ++i;
            break;
        case 2: S.push(instr[i]);
            ++i;
            break;
        case 3: outstr[j++] = S.top();
            S.pop();
            break;
        case 4: S.pop();
            ++i;
            break;
        case 5: throw ErrorBracketsClose(instr, i);
            break;
        case 6: throw ErrorBracketsOpen(instr, i);
            break;
        case 7: outstr[j] = '\0';
            break;
        }
    }
    while (action != 7);
}

void BooleanExpression::InfixFilter(const char* instr, char* outstr)
{
    int i = 0;
    int j = 0;
    int k = 1;
    char ch;
    std::string buf;
    try
    {
        while ((ch = instr[i++]) != '\0')
        {
            if (!buf.empty() && buf[0] == 'x' && buf.size() < 2 && ch == ' ') throw ErrorLackOfIndexing(buf.c_str(), k);
            if (!buf.empty() && ch == 'x') throw ErrorDuplicationOfVariables(instr, k);
            if (ch == ' ' || ch == '\t' || ch == '\r') continue;
            if (ch == 'x' || (ch >= '0' && ch <= '9'))
            {
                buf += ch;
            }
            else
            {
                if (!buf.empty())
                {
                    for (const char c : buf)
                    {
                        outstr[j++] = c;
                    }
                    k++;
                    buf.clear();
                }
                if (ch == '!' || ch == '~')
                {
                    outstr[j++] = '~';
                }
                else if (ch == 'v')
                {
                    outstr[j++] = 'v';
                }
                else if (ch == '&')
                {
                    outstr[j++] = '&';
                }
                else if (ch == '+')
                {
                    outstr[j++] = '+';
                }
                else if (ch == '<')
                {
                    outstr[j++] = '<';
                }
                else if (ch == '>')
                {
                    outstr[j++] = '>';
                }
                else if (ch == '=')
                {
                    outstr[j++] = '=';
                }
                else if (ch == '|')
                {
                    outstr[j++] = '|';
                }
                else if (ch == '^')
                {
                    outstr[j++] = '^';
                }
                else if (ch == '(' || ch == ')')
                {
                    outstr[j++] = ch;
                }
                else
                {
                    throw ErrorUnknownSymbol(instr, k);
                }
                ++k;
            }
        }
        if (!buf.empty())
        {
            for (char c : buf)
            {
                outstr[j++] = c;
            }
        }
        outstr[j] = '\0';
    }
    catch (...)
    {
        throw;
    }
}

BooleanExpression::BooleanExpression(const char* str): root(nullptr)
{
    size_t len = strlen(str);
    len = (len < 10) ? 20 : 2 * len;
    auto str_infix = std::make_unique<char[]>(len);
    auto str_postfix = std::make_unique<char[]>(len);
    try
    {
        InfixFilter(str, str_infix.get());
        Infix2Postfix(str_infix.get(), str_postfix.get());
        root = Postfix2Tree(str_postfix.get());
    }
    catch (...)
    {
        throw;
    }
}

BooleanExpression::BooleanExpression(const BooleanExpression& a): root(nullptr), zh(a.zh)
{
    if (a.root)
    {
        root = a.root->clone();
    }
}

BooleanExpression::BooleanExpression(BooleanExpression&& a) noexcept: root(a.root), zh(std::move(a.zh))
{
    a.root = nullptr;
}

BooleanExpression& BooleanExpression::operator=(const BooleanExpression& a)
{
    if (this != &a)
    {
        BooleanExpression tmp(a);
        std::swap(root, tmp.root);
        std::swap(zh, tmp.zh);
    }
    return *this;
}

BooleanExpression& BooleanExpression::operator=(BooleanExpression&& a) noexcept
{
    if (this != &a)
    {
        delete root;
        root = a.root;
        zh = std::move(a.zh);
        a.root = nullptr;
    }
    return *this;
}

BooleanExpression BooleanExpression::zhegalkin() const
{
    if (root == nullptr)
    {
        return BooleanExpression();
    }
    std::string q = table();
    int varcount = 0;
    int size = q.size();
    std::string root_string = root->str();
    std::set<char> set;
    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    std::string temp;
    for (const auto& elem : set) {
        temp += elem;
    }
    while (size >>= 1)
    {
        varcount++;
    }
    int combination = 1 << varcount;
    std::vector<std::vector<bool>> pascal(combination, std::vector<bool>(q.size(), false));
    for (size_t i = 0; i < q.size(); i++)
    {
        pascal[0][i] = (q[i] == '1');
    }
    for (int i = 1; i < combination; i++)
    {
        for (int j = 0; j < combination - i; j++)
        {
            pascal[i][j] = pascal[i - 1][j] ^ pascal[i - 1][j + 1];
        }
    }
    std::vector<bool> left_column(q.size(), false);
    for (int i = 0; i < combination; i++)
    {
        left_column[i] = pascal[i][0];
    }
    std::vector<std::string> terms;
    for (int i = 0; i < combination; i++)
    {
        if (!left_column[i])
        {
            continue;
        }
        if (i == 0)
        {
            terms.push_back("1");
            continue;
        }

        std::string term;
        for (int j = 0; j < varcount; ++j)
        {
            if ((i & (1 << (varcount - 1 - j))))
            {
                if (term.size() >= 2)
                {
                    term += '&';
                }
                term += std::string("x") + temp[j];
            }
        }
        terms.push_back(term);
    }
    std::string result;
    if (terms.empty())
    {
        result = "0";
    }
    else
    {
        for (size_t i = 0; i < terms.size(); i++)
        {
            if (i != 0)
            {
                result += '+';
            }
            result += terms[i];
        }
    }
    zh = result;
    return BooleanExpression(result.c_str());
}

std::string BooleanExpression::table() const
{
    if (!root)
    {
        return "";
    }
    char variables[10]{};
    bool value[10]{};
    int varcount = 0;
    std::function<void(BooleanExpressionNode*)> findVars = [&](BooleanExpressionNode* node)
    {
        if (!node) return;
        auto varNode = dynamic_cast<VarNode*>(node);
        if (varNode)
        {
            std::string varName = varNode->str();
            if (varName.length() == 2)
            {
                char variable = varName[1];
                bool found = false;
                for (int i = 0; i < varcount; i++)
                {
                    if (variable == variables[i])
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    variables[varcount++] = variable;
                }
            }
            return;
        }
        auto binNode = dynamic_cast<BinNode*>(node);
        if (binNode)
        {
            findVars(binNode->getLeft());
            findVars(binNode->getRight());
            return;
        }
        auto unarNode = dynamic_cast<UnarNode*>(node);
        if (unarNode)
        {
            findVars(unarNode->getNext());
            return;
        }
    };
    findVars(root);

    for (int i = 0; i < varcount; i++)
    {
        for (int j = 0; j < varcount - 1; j++)
        {
            if (variables[j] > variables[j + 1])
            {
                std::swap(variables[j], variables[j + 1]);
            }
        }
    }
    std::string result;
    int combinations = 1 << varcount;
    for (int i = 0; i < combinations; i++)
    {
        for (int j = 0; j < varcount; j++)
        {
            value[(variables[varcount - 1 - j] - '0')] = (i & (1 << j)) != 0;
        }
        setVars(root, value);
        bool expressionValue = root->calc();
        result += expressionValue ? "1" : "0";
    }
    return result;
}

std::string BooleanExpression::GetZhegalkin() const
{
    if (zh.empty())
    {
        BooleanExpression q = zhegalkin();
    }
    return zh;
}


void BooleanExpression::setVars(BooleanExpressionNode* node, bool value[]) const
{
    auto varNode = dynamic_cast<VarNode*>(node);
    if (varNode)
    {
        varNode->setValue(value[varNode->str()[1] - '0']);
    }
    auto binNode = dynamic_cast<BinNode*>(node);
    if (binNode)
    {
        setVars(binNode->getLeft(), value);
        setVars(binNode->getRight(), value);
    }
    auto unarNode = dynamic_cast<UnarNode*>(node);
    if (unarNode)
    {
        setVars(unarNode->getNext(), value);
    }
}

std::string BooleanExpression::GetTable() const
{
    std::string result = table();
    size_t size = result.size();
    size_t varcount = 0;
    std::string root_string = root->str();
    std::set<char> set;
    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    std::string temp;
    for (const auto& elem : set) {
        temp += elem;
    }
    while (size >>= 1)
    {
        varcount++;
    }
    std::ostringstream q;
    for (int i = 0; i < varcount; i++)
    {
        q << std::setw(2) << ('x' + std::string(1, temp[i])) << " ";
    }
    q << std::setw(2) << "F" << std::endl;
    int combination = 1 << varcount;
    for (int i = 0; i < combination; ++i)
    {
        for (int j = varcount - 1; j >= 0; --j)
        {
            q << std::setw(2) << ((i & (1 << j)) != 0) << " ";
        }
        q << std::setw(2) << result[i] << std::endl;
    }
    return q.str();
}


BooleanExpression::operator std::string() const
{
    if (root != nullptr)
    {
        return root->str();
    }
    return {};
}

bool isFullSystem(const std::vector<BooleanExpression>& a)
{
    std::vector<std::string> terms;
    for (size_t i = 0; i < a.size(); i++)
    {
        terms.push_back(a[i].table());
    }
    bool SaveZero = false;
    bool SaveOne = false;
    bool SelfDual = false;
    bool Monotonous = false;
    bool Linear = false;
    for (size_t i = 0; i < terms.size(); i++)
    {
        if (terms[i].front() != '0')
        {
            SaveZero = true;
            break;
        }
    }
    for (size_t i = 0; i < terms.size(); i++)
    {
        if (terms[i].back() != '1')
        {
            SaveOne = true;
            break;
        }
    }
    for (size_t i = 0; i < a.size(); i++)
    {
        bool found = false;
        for (size_t j = 0; j < terms[i].size() / 2; j++)
        {
            if (terms[i][j] == terms[i][terms[i].size() - j - 1])
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            SelfDual = true;
            break;
        }
    }
    for (size_t i = 0; i < terms.size(); i++)
    {
        if (terms[i].size() == 1)
        {
            continue;
        }
        if (terms[i].size() == 2)
        {
            if (terms[i][0] > terms[i][1])
            {
                Monotonous = true;
                break;
            }
            continue;
        }
        std::string left = terms[i].substr(0, terms[i].size() / 2);
        std::string right = terms[i].substr(terms[i].size() / 2);
        if (!Monotonic(left, right))
        {
            Monotonous = true;
            break;
        }
    }
    for (size_t i = 0; i < a.size(); i++)
    {
        std::string q = std::string(a[i].zhegalkin());
        if (q.find('&') != std::string::npos)
        {
            Linear = true;
            break;
        }
    }
    return SaveOne && SaveZero && SelfDual && Monotonous && Linear;
}

bool Monotonic(const std::string& left, const std::string& right)
{
    if (left.size() == 1 || right.size() == 1)
    {
        return true;
    }
    for (size_t i = 0; i < left.size(); i++)
    {
        if (left[i] > right[i])
        {
            return false;
        }
    }
    std::string left1 = left.substr(0, left.size() / 2);
    std::string right1 = left.substr(left.size() / 2);
    std::string left2 = right.substr(0, right.size() / 2);
    std::string right2 = right.substr(right.size() / 2);
    return Monotonic(left1, right1) && Monotonic(left2, right2);
}

BooleanExpression::~BooleanExpression()
{
    delete root;
}
