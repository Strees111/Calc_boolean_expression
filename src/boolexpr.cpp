#include "boolexpr.h"
#include "Error.h"

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
            if (buf.empty() && (ch >= '2' && ch <= '9')) throw ErrorInvalidConstant(buf.c_str(),k);
            if (!buf.empty() && buf[0] == 'x' && buf.size() > 1 && (ch >= '0' && ch <= '9')) throw ErrorOverabundanceOfIndexing(buf.c_str(), k);
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

BooleanExpression::BooleanExpression(const BooleanExpression& a): root(nullptr), ZH(a.ZH)
{
    if (a.root)
    {
        root = a.root->clone();
    }
}

BooleanExpression::BooleanExpression(BooleanExpression&& a) noexcept: root(a.root), ZH(std::move(a.ZH))
{
    a.root = nullptr;
}

BooleanExpression& BooleanExpression::operator=(const BooleanExpression& a)
{
    if (this != &a)
    {
        BooleanExpression tmp(a);
        std::swap(root, tmp.root);
        std::swap(ZH, tmp.ZH);
    }
    return *this;
}

BooleanExpression& BooleanExpression::operator=(BooleanExpression&& a) noexcept
{
    if (this != &a)
    {
        delete root;
        root = a.root;
        ZH = std::move(a.ZH);
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
    ZH = result;
    return BooleanExpression(result.c_str());
}

BooleanExpression BooleanExpression::SDNF() const
{
    std::string table_expressions = table();
    int varcount = 0;
    size_t size = table_expressions.size();
    std::string unique_var;
    std::string root_string = root->str();
    std::set<char> set;
    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    std::set<char>::iterator it = set.begin();
    while (it != set.end())
    {
        unique_var += *it;
        it++;
    }
    while (size >>= 1)
    {
        varcount++;
    }
    int combination = 1 << varcount;
    std::vector<std::string> terms;
    for (int i = 0; i < combination; i++)
    {
        if (table_expressions[i] == '1')
        {
            std::string term;
            for (int j = 0; j < varcount; ++j)
            {
                if (j > 0)
                {
                    term += '&';
                }
                if ((i & (1 << (varcount - 1 - j))))
                {
                    term += std::string("x") + unique_var[j];
                }
                else
                {
                    term += '~' + std::string("x") + unique_var[j];
                }
            }
            terms.push_back(term);
        }
    }
    std::string result1;
    if (terms.empty())
    {
        result1 = "0";
    }
    else
    {
        for (size_t i = 0; i < terms.size(); i++)
        {
            if (i != 0)
            {
                result1 += 'v';
            }
            result1 += terms[i];
        }
    }
    SD = result1;
    return BooleanExpression(result1.c_str());
}

BooleanExpression BooleanExpression::SKNF() const
{
    std::string table_expressions = table();
    int varcount = 0;
    size_t size = table_expressions.size();
    std::string unique_var;
    std::string root_string = root->str();
    std::set<char> set;
    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    std::set<char>::iterator it = set.begin();
    while (it != set.end())
    {
        unique_var += *it;
        it++;
    }
    while (size >>= 1)
    {
        varcount++;
    }
    int combination = 1 << varcount;
    std::vector<std::string> terms;
    for (int i = 0; i < combination; i++)
    {
        if (table_expressions[i] == '0')
        {
            std::string term;
            for (int j = 0; j < varcount; ++j)
            {
                if (j > 0)
                {
                    term += 'v';
                }
                if ((i & (1 << (varcount - 1 - j))))
                {
                    term += '~' + std::string("x") + unique_var[j];
                }
                else
                {
                    term += std::string("x") + unique_var[j];
                }
            }
            terms.push_back(term);
        }
    }
    std::string result1;
    if (terms.empty())
    {
        result1 = "0";
    }
    else
    {
        for (size_t i = 0; i < terms.size(); i++)
        {
            if (i != 0)
            {
                result1 += '&';
            }
            result1 += '(' + terms[i] + ')';
        }
    }
    SK = result1;
    return BooleanExpression(result1.c_str());
}

BooleanExpression BooleanExpression::Minimized() const
{
    std::string table_expressions = table();
    std::string root_string = root->str();
    size_t size = table_expressions.size();

    int varcount = 0;
    std::string unique_var;
    std::set<char> set;

    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    for (const auto& elem : set) {
        unique_var += elem;
    }

    while (size >>= 1)
    {
        varcount++;
    }
    int combination = 1 << varcount;
    std::vector<std::string> binary_codes;
    std::vector<char> truth_table;
    
    for (int i = 0; i < combination; i++)
    {
        std::string binary_str(varcount, '0');
        for (int j = 0; j < varcount; j++)
        {
            if (i & (1 << (varcount - 1 - j)))
            {
                binary_str[j] = '1';
            }
        }
        
        binary_codes.push_back(binary_str);
        truth_table.push_back(table_expressions[i]);
    }

    std::string result = minimizeEspresso(truth_table, binary_codes, varcount, unique_var);
    MDN = result;
    return BooleanExpression(result.c_str());
}

std::vector<BooleanExpression::Cube> BooleanExpression::generateInitialCubes(const std::vector<int>& minterms, int varcount) const
{
    std::vector<Cube> cubes;
    
    for (int minterm : minterms) {
        Cube cube(varcount);
        for (int i = 0; i < varcount; i++) {
            if (minterm & (1 << (varcount - 1 - i))) {
                cube.literals[i] = 1;
            } else {
                cube.literals[i] = 0;
            }
        }
        
        cube.minterms.insert(minterm);
        cubes.push_back(cube);
    }
    
    return cubes;
}

std::vector<BooleanExpression::Cube> BooleanExpression::expand( const std::vector<Cube>& cubes, const std::vector<int>& minterms, const std::vector<int>& dont_cares, int varcount) const
{
    std::vector<Cube> expanded_cubes;
    
    for (const auto& cube : cubes) {
        Cube expanded = cube;

        for (int var = 0; var < varcount; var++) {
            if (expanded.literals[var] != 2) {
                if (canExpand(expanded, var, 2, minterms, dont_cares)) {
                    expanded.literals[var] = 2;
                    expanded.minterms = getCoveredMinterms(expanded, varcount);
                }
            }
        }
        
        expanded_cubes.push_back(expanded);
    }
    
    return expanded_cubes;
}

bool BooleanExpression::canExpand(const Cube& cube, int variable, int value, const std::vector<int>& minterms, const std::vector<int>& dont_cares) const
{
    Cube test_cube = cube;
    test_cube.literals[variable] = value;

    std::set<int> covered = getCoveredMinterms(test_cube, cube.literals.size());
    
    for (int covered_minterm : covered) {
        bool is_valid = false;

        for (int m : minterms) {
            if (m == covered_minterm) {
                is_valid = true;
                break;
            }
        }
        
        if (!is_valid) {
            for (int dc : dont_cares) {
                if (dc == covered_minterm) {
                    is_valid = true;
                    break;
                }
            }
        }
        
        if (!is_valid) return false;
    }
    
    return true;
}
std::string BooleanExpression::minimizeEspresso(std::vector<char> truth_table, std::vector<std::string> binary_codes, int varcount, std::string unique_var) const
{
    std::vector<int> minterms;
    std::vector<int> dont_cares;
    for (size_t i = 0; i < truth_table.size(); i++) {
        if (truth_table[i] == '1') {
            minterms.push_back(static_cast<int>(i));
        }
    }
    
    if (minterms.empty()) return "0";
    if (minterms.size() == truth_table.size()) return "1";

    std::vector<Cube> cubes = generateInitialCubes(minterms, varcount);

    std::vector<Cube> prev_cubes;
    int iteration = 0;
    
    do {
        prev_cubes = cubes;
        iteration++;

        cubes = expand(cubes, minterms, dont_cares, varcount);

        cubes = irredundant(cubes, minterms);

        cubes = reduce(cubes);
        
    } while (cubes != prev_cubes && iteration < 10);

    if (cubes.empty()) return "0";
    
    std::vector<std::string> terms;
    for (const auto& cube : cubes) {
        std::string term = cubeToString(cube, unique_var);
        if (!term.empty()) {
            terms.push_back(term);
        }
    }
    
    if (terms.empty()) return "0";

    std::sort(terms.begin(), terms.end());
    
    std::string result;
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) result += "v";
        result += terms[i];
    }
    return result;
}
std::set<int> BooleanExpression::getCoveredMinterms(const Cube& cube, int varcount) const
{
    std::set<int> covered;
    int dont_care_count = 0;
    std::vector<int> dont_care_positions;
    
    for (int i = 0; i < varcount; i++) {
        if (cube.literals[i] == 2) {
            dont_care_count++;
            dont_care_positions.push_back(i);
        }
    }
    int combinations = 1 << dont_care_count;
    
    for (int combo = 0; combo < combinations; combo++) {
        int minterm = 0;
        
        for (int i = 0; i < varcount; i++) {
            if (cube.literals[i] == 1) {
                minterm |= (1 << (varcount - 1 - i));
            } else if (cube.literals[i] == 2) {
                int dc_pos = 0;
                for (int pos : dont_care_positions) {
                    if (pos == i) break;
                    dc_pos++;
                }
                
                if (combo & (1 << dc_pos)) {
                    minterm |= (1 << (varcount - 1 - i));
                }
            }
        }
        
        covered.insert(minterm);
    }
    
    return covered;
}

std::vector<BooleanExpression::Cube> BooleanExpression::irredundant(
    const std::vector<Cube>& cubes, const std::vector<int>& minterms) const
{
    std::vector<Cube> essential_cubes;
    std::set<int> covered_minterms;
    for (const auto& cube : cubes) {
        bool is_essential = false;
        
        for (int minterm : cube.minterms) {
            int cover_count = 0;
            for (const auto& other_cube : cubes) {
                if (other_cube.minterms.count(minterm)) {
                    cover_count++;
                }
            }
            
            if (cover_count == 1) {
                is_essential = true;
                break;
            }
        }
        
        if (is_essential) {
            essential_cubes.push_back(cube);
            for (int m : cube.minterms) {
                covered_minterms.insert(m);
            }
        }
    }
    std::vector<Cube> remaining_cubes;
    for (const auto& cube : cubes) {
        bool is_essential = false;
        for (const auto& ess : essential_cubes) {
            if (&cube == &ess) {
                is_essential = true;
                break;
            }
        }
        if (!is_essential) {
            remaining_cubes.push_back(cube);
        }
    }
    
    while (covered_minterms.size() < minterms.size()) {
        int best_cube_idx = -1;
        int max_new_coverage = 0;
        
        for (size_t i = 0; i < remaining_cubes.size(); i++) {
            int new_coverage = 0;
            for (int m : remaining_cubes[i].minterms) {
                if (covered_minterms.find(m) == covered_minterms.end()) {
                    new_coverage++;
                }
            }
            
            if (new_coverage > max_new_coverage) {
                max_new_coverage = new_coverage;
                best_cube_idx = i;
            }
        }
        
        if (best_cube_idx == -1) break;
        
        essential_cubes.push_back(remaining_cubes[best_cube_idx]);
        for (int m : remaining_cubes[best_cube_idx].minterms) {
            covered_minterms.insert(m);
        }
        remaining_cubes.erase(remaining_cubes.begin() + best_cube_idx);
    }
    
    return essential_cubes;
}

std::vector<BooleanExpression::Cube> BooleanExpression::reduce(
    const std::vector<Cube>& cubes) const
{
    std::vector<Cube> reduced_cubes;
    
    for (const auto& cube : cubes) {
        Cube reduced = cube;

        for (size_t i = 0; i < reduced.literals.size(); i++) {
            if (reduced.literals[i] != 2) {
                int original = reduced.literals[i];
                reduced.literals[i] = 2;

                bool is_valid = true;

                reduced.literals[i] = original;
            }
        }
        
        reduced_cubes.push_back(reduced);
    }
    
    return reduced_cubes;
}

std::string BooleanExpression::cubeToString(const Cube& cube, const std::string& unique_var) const
{
    std::string term;
    
    for (size_t i = 0; i < cube.literals.size(); i++) {
        if (cube.literals[i] != 2) { // Не don't care
            if (!term.empty()) {
                term += "&";
            }
            
            if (cube.literals[i] == 0) {
                term += "~";
            }
            term += "x" + std::string(1, unique_var[i]);
        }
    }
    
    return term.empty() ? "1" : term;
}
std::string BooleanExpression::minimizeEspressoDual(std::vector<char> truth_table, 
                                                   std::vector<std::string> binary_codes, 
                                                   int varcount, std::string unique_var) const
{
    std::vector<int> maxterms; 
    for (size_t i = 0; i < truth_table.size(); i++) {
        if (truth_table[i] == '0') {
            maxterms.push_back(static_cast<int>(i));
        }
    }
    
    if (maxterms.empty()) return "1";
    if (maxterms.size() == truth_table.size()) return "0";

    
    std::vector<char> inverted_table = truth_table;
    for (char& bit : inverted_table) {
        bit = (bit == '0') ? '1' : '0';
    }
    
    std::string minimized_dnf = minimizeEspresso(inverted_table, binary_codes, varcount, unique_var);
    
    if (minimized_dnf == "0") return "1";
    if (minimized_dnf == "1") return "0"; 
    std::string result = convertDNFtoCNF(minimized_dnf, unique_var);

    return result;
}
std::string BooleanExpression::convertDNFtoCNF(const std::string& dnf, const std::string& unique_var) const
{
    if (dnf == "0" || dnf == "1") return dnf;

    std::vector<std::string> dnf_terms;
    std::string current_term;
    
    for (char c : dnf) {
        if (c == 'v') {
            if (!current_term.empty()) {
                dnf_terms.push_back(current_term);
                current_term.clear();
            }
        } else {
            current_term += c;
        }
    }
    if (!current_term.empty()) {
        dnf_terms.push_back(current_term);
    }

    std::vector<std::string> cnf_clauses;
    
    for (const std::string& term : dnf_terms) {
        std::string clause = invertTerm(term);
        if (!clause.empty()) {
            cnf_clauses.push_back(clause);
        }
    }
    
    if (cnf_clauses.empty()) return "1";
    if (cnf_clauses.size() == 1) return cnf_clauses[0];
    std::string result;
    for (size_t i = 0; i < cnf_clauses.size(); i++) {
        if (i > 0) result += "&";
        result += "(" + cnf_clauses[i] + ")";
    }
    
    return result;
}

std::string BooleanExpression::invertTerm(const std::string& term) const
{
    std::vector<std::string> literals;
    std::string current_literal;
    bool in_negation = false;
    
    for (size_t i = 0; i < term.size(); i++) {
        char c = term[i];
        
        if (c == '&') {
            if (!current_literal.empty()) {
                literals.push_back(current_literal);
                current_literal.clear();
            }
        } else if (c == '~') {
            current_literal += c;
        } else if (c == 'x') {
            current_literal += c;
        } else if (c >= '0' && c <= '9') {
            current_literal += c;
        }
    }
    
    if (!current_literal.empty()) {
        literals.push_back(current_literal);
    }
    
    std::vector<std::string> inverted_literals;
    for (const std::string& lit : literals) {
        if (lit.substr(0, 1) == "~") {
            inverted_literals.push_back(lit.substr(1));
        } else {
            inverted_literals.push_back("~" + lit);
        }
    }

    std::string result;
    for (size_t i = 0; i < inverted_literals.size(); i++) {
        if (i > 0) result += "v";
        result += inverted_literals[i];
    }
    
    return result;
}

BooleanExpression BooleanExpression::MinimizedCNF() const
{
    std::string table_expressions = table();
    std::string root_string = root->str();
    size_t size = table_expressions.size();

    int varcount = 0;
    std::string unique_var;
    std::set<char> set;

    for (size_t i = 0; i < root_string.size(); i++)
    {
        if (root_string[i] >= '0' && root_string[i] <= '9')
        {
            set.insert(root_string[i]);
        }
    }
    for (const auto& elem : set) {
        unique_var += elem;
    }

    while (size >>= 1)
    {
        varcount++;
    }
    int combination = 1 << varcount;
    std::vector<std::string> binary_codes;
    std::vector<char> truth_table;
    
    for (int i = 0; i < combination; i++)
    {
        std::string binary_str(varcount, '0');
        for (int j = 0; j < varcount; j++)
        {
            if (i & (1 << (varcount - 1 - j)))
            {
                binary_str[j] = '1';
            }
        }
        
        binary_codes.push_back(binary_str);
        truth_table.push_back(table_expressions[i]);
    }

    std::string result = minimizeEspressoDual(truth_table, binary_codes, varcount, unique_var);
    MCN = result;
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
    if (ZH.empty())
    {
        BooleanExpression q = zhegalkin();
    }
    return ZH;
}

std::string BooleanExpression::GetSDNF() const
{
    if (SD.empty())
    {
        BooleanExpression q = SDNF();
    }
    return SD;
}

std::string BooleanExpression::GetSKNF() const
{
    if (SK.empty())
    {
        BooleanExpression q = SKNF();
    }
    return SK;
}

std::string BooleanExpression::GetMinimizedDNF() const
{
    if (MDN.empty())
    {
        BooleanExpression q = Minimized();
    }
    return MDN;
}

std::string BooleanExpression::GetMinimizedCNF() const
{
    if (MCN.empty())
    {
        std::string dnf_minimized = GetMinimizedDNF();
        if (dnf_minimized == "0") {
            MCN = "0";
        } else if (dnf_minimized == "1") {
            MCN = "1";
        } else if (dnf_minimized.find('v') == std::string::npos) {
            MCN = dnf_minimized;
        } else {
                BooleanExpression q = MinimizedCNF();
        }
    }
    return MCN;
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

bool isSaveZero(std::vector<std::string> terms){
    bool SaveZero = false;
    for (size_t i = 0; i < terms.size(); i++)
    {
        if (terms[i].front() != '0')
        {
            SaveZero = true;
            break;
        }
    }
    return SaveZero;
}

bool isSaveOne(std::vector<std::string> terms){
    bool SaveOne = false;
    for (size_t i = 0; i < terms.size(); i++)
    {
        if (terms[i].back() != '1')
        {
            SaveOne = true;
            break;
        }
    }
    return SaveOne;
}

bool isSaveDual(std::vector<std::string> terms, size_t len){
    bool SelfDual = false;
    for (size_t i = 0; i < len; i++)
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
    return SelfDual;
}

bool isLinear(std::vector<std::string> terms, size_t len, const std::vector<BooleanExpression>& a){
    bool Linear = false;
    for (size_t i = 0; i < len; i++)
    {
        std::string q = std::string(a[i].zhegalkin());
        if (q.find('&') != std::string::npos)
        {
            Linear = true;
            break;
        }
    }
    return Linear;
}

bool isMonotonous(std::vector<std::string> terms){
    bool Monotonous = false;
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
    return Monotonous;
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

std::string isFullSystem(const std::vector<BooleanExpression>& a)
{
    std::vector<std::string> terms;
    for (size_t i = 0; i < a.size(); i++)
    {
        terms.push_back(a[i].table());
    }
    size_t len = a.size();
    std::string result;
    isSaveZero(terms) ? result.append("T0: -\n") : result.append("T0: +\n");
    isSaveOne(terms) ? result.append("T1: -\n") : result.append("T1: +\n");
    isSaveDual(terms,len) ? result.append("S: -\n") : result.append("S: +\n");
    isMonotonous(terms) ? result.append("M: -\n") : result.append("M: +\n");
    isLinear(terms,len,a) ? result.append("L: -\n") : result.append("L: +\n");
    return result;
}


BooleanExpression::~BooleanExpression()
{
    delete root;
}
