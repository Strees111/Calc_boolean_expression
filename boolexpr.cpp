#include "boolexpr.h"

#include <memory>
#include <set>

BooleanExpressionNode* BooleanExpression::Postfix2Tree(const char* str)
{
    int index = 0;
    std::stack<BooleanExpressionNode*> S{};
    std::string str1;
    BooleanExpressionNode *result = nullptr;
    BooleanExpressionNode *right = nullptr;
    BooleanExpressionNode *left = nullptr;

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
                result = new conjunctionNode(left, right);
                break;
            case 'v':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new disjunctionNode(left, right);
                break;
            case '>':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new implicationNode(left, right);
                break;
            case '<':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new UnimplicationNode(left, right);
                break;
            case '=':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new equivalentNode(left, right);
                break;
            case '|':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new SchaefferNode(left, right);
                break;
            case '^':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                if (S.empty()) throw 1;
                left = S.top();
                S.pop();
                result = new PierNode(left, right);
                break;
            case '+':
                if (S.empty()) throw 1;
                right = S.top();
                S.pop();
                left = S.top();
                S.pop();
                result = new squareNode(left, right);
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
                    result = new VarNode(str1,0);
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
    char ch;
    std::string buf;
    try
    {
        while ((ch = instr[i++]) != '\0')
        {
            if (!buf.empty() && buf[0] == 'x' && buf.size() < 2 && ch == ' ') throw ErrorLackOfIndexing(buf.c_str(), i);
            if (!buf.empty() && ch == 'x') throw ErrorDuplicationOfVariables(instr, i);
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
                else if (ch == '>' )
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
                else if (ch == '^' )
                {
                    outstr[j++] = '^';
                }
                else if (ch == '(' || ch == ')')
                {
                    outstr[j++] = ch;
                }
                else
                {
                    throw ErrorUnknownSymbol(instr, i);
                }
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
    for (size_t i = 0; i < len; i++)
    {
        std::cout << str[i];
    }
    std::cout << std::endl;
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

BooleanExpression::BooleanExpression(const BooleanExpression& a): root(nullptr)
{
    if (a.root)
    {
        root = a.root->clone();
    }
}

BooleanExpression::BooleanExpression(BooleanExpression&& a) noexcept: root(a.root)
{
    a.root = nullptr;
}

BooleanExpression& BooleanExpression::operator=(const BooleanExpression& a)
{
    if (this != &a)
    {
        BooleanExpression tmp(a);
        std::swap(root, tmp.root);
    }
    return *this;
}

BooleanExpression& BooleanExpression::operator=(BooleanExpression&& a) noexcept
{
    if (this != &a)
    {
        delete root;
        root = a.root;
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
    // int min_index = INT_MAX;
    std::set<char> set;
    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '1' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    std::set<char>::iterator it = set.begin();
    std::string temp;
    while (it != set.end())
    {
        temp += *it;
        it++;
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
    std::vector<bool> left_column(q.size(),false);
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
                    term += '^';
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
    std::function<void(BooleanExpressionNode*)> setVars = [&](BooleanExpressionNode* node)
    {
        auto varNode = dynamic_cast<VarNode*>(node);
        if (varNode)
        {
            varNode->setValue(value[varNode->str()[1] - '0']);
        }
        auto binNode = dynamic_cast<BinNode*>(node);
        if (binNode)
        {
            setVars(binNode->getLeft());
            setVars(binNode->getRight());
        }
        auto unarNode = dynamic_cast<UnarNode*>(node);
        if (unarNode)
        {
            setVars(unarNode->getNext());
        }
    };
    std::string result;
    int combinations = 1 << varcount;
    for (int i = 0; i < combinations; i++) {
        for (int j = 0; j < varcount; j++) {
            value[(variables[varcount - 1 - j] - '0')] = (i & (1 << j)) != 0;
        }
        setVars(root);
        bool expressionValue = root->calc();
        result += expressionValue ? "1" : "0";
    }
    return result;
}

BooleanExpression::operator std::string() const
{
    if (root != nullptr)
    {
        return root->str();
    }
    return {};
}

bool BooleanExpression::isFullSystem(const std::vector<BooleanExpression>&)
{
    return false;
}

BooleanExpression::~BooleanExpression()
{
    delete root;
}
