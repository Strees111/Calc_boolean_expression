#pragma once
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstring>
#include <algorithm>
#include <memory>  

class BooleanExpressionNode
{
public:
    [[nodiscard]] virtual std::string str() const = 0;
    [[nodiscard]] virtual bool calc() const = 0;
    virtual ~BooleanExpressionNode() = default;
    virtual BooleanExpressionNode* clone() = 0;
};
class VarNode : public BooleanExpressionNode
{
private:
    std::string name;
    bool value;
public:
    explicit VarNode(std::string name, bool value) : name(std::move(name)), value(value) {}
    void setValue(bool value) { this->value = value; }
    [[nodiscard]] std::string str() const override {
        return name;
    }
    [[nodiscard]] bool calc() const override
    {
        return value;
    }
    BooleanExpressionNode* clone() override
    {
        return new VarNode(name, value);
    };
};
class ConstNode: public BooleanExpressionNode
{
private:
    const bool value;
public:
    explicit ConstNode(bool value) : value(value) {}
    [[nodiscard]] std::string str() const override
    {
        return std::to_string(value);
    }
    [[nodiscard]] bool calc() const override
    {
        return value;
    }
    BooleanExpressionNode* clone() override
    {
        return new ConstNode(value);
    };
};
class UnarNode : public BooleanExpressionNode {
protected:
    BooleanExpressionNode *next;
public:
    explicit UnarNode(BooleanExpressionNode *node) : next(node) {}
    BooleanExpressionNode* getNext(){return next;}
    ~UnarNode() override {delete next;}
    BooleanExpressionNode* clone() override = 0;
};
class NegationNode : public UnarNode
{
public:
    explicit NegationNode(BooleanExpressionNode* op) : UnarNode(op) {}
    [[nodiscard]] std::string str() const override {
        return "~(" + next->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return !next->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new NegationNode(next->clone());
    }
};
class BinNode : public BooleanExpressionNode {
protected:
    BooleanExpressionNode *left, *right;
public:
    BinNode(BooleanExpressionNode* L, BooleanExpressionNode* R) : left(L), right(R) {}
    BooleanExpressionNode* getLeft(){return left;}
    BooleanExpressionNode* getRight(){return right;}
    ~BinNode() override
    {
        delete left;
        delete right;
    }
    BooleanExpressionNode* clone() override = 0;
};
class ConjunctionNode : public BinNode
{
public:
    ConjunctionNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return left->str() + " & " + right->str();
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() && right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new ConjunctionNode(left->clone(),right->clone());
    }
};
class DisjunctionNode : public BinNode
{
public:
    DisjunctionNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " | " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() || right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new DisjunctionNode(left->clone(),right->clone());
    }
};
class XORNode : public BinNode
{
public:
    explicit XORNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " + " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() ^ right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new XORNode(left->clone(),right->clone());
    }
};
class ImplicationNode : public BinNode
{
public:
    ImplicationNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " > " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return !left->calc() || right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new ImplicationNode(left->clone(),right->clone());
    }
};
class ConverseNode : public BinNode
{
public:
    ConverseNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " < " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() || !right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new ConverseNode(left->clone(),right->clone());
    }
};
class EquivalentNode : public BinNode
{
public:
    EquivalentNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " = " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() == right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new EquivalentNode(left->clone(),right->clone());
    }
};
class NANDNode : public BinNode
{
public:
    NANDNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " | " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return !(left->calc() && right->calc());
    }
    BooleanExpressionNode* clone() override
    {
        return new NANDNode(left->clone(),right->clone());
    }
};
class NORNode : public BinNode
{
public:
    NORNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " ^ " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return !(left->calc() || right->calc());
    }
    BooleanExpressionNode* clone() override
    {
        return new NORNode(left->clone(),right->clone());
    }
};
class BooleanExpression
{
private:
    struct Cube {
        std::vector<int> literals;
        std::set<int> minterms;     
        
        Cube(int varcount) : literals(varcount, 2) {}
        bool operator==(const Cube& other) const {
            return literals == other.literals && minterms == other.minterms;
        }
        bool operator!=(const Cube& other) const {
            return !(*this == other);
        }
        bool covers(const Cube& other) const {
            for (size_t i = 0; i < literals.size(); i++) {
                if (literals[i] != 2 && other.literals[i] != 2 && 
                    literals[i] != other.literals[i]) {
                    return false;
                }
            }
            return true;
        }
        
        bool intersects(const Cube& other) const {
            for (size_t i = 0; i < literals.size(); i++) {
                if (literals[i] != 2 && other.literals[i] != 2 && 
                    literals[i] != other.literals[i]) {
                    return false;
                }
            }
            return true;
        }
        
        Cube intersect(const Cube& other) const {
            Cube result(literals.size());
            for (size_t i = 0; i < literals.size(); i++) {
                if (literals[i] == 2) {
                    result.literals[i] = other.literals[i];
                } else if (other.literals[i] == 2) {
                    result.literals[i] = literals[i];
                } else if (literals[i] == other.literals[i]) {
                    result.literals[i] = literals[i];
                } else {
                    return Cube(0); // Пересечение пустое
                }
            }
            return result;
        }
        
        bool isEmpty() const {
            return literals.empty();
        }
        
        int literalCount() const {
            int count = 0;
            for (int lit : literals) {
                if (lit != 2) count++;
            }
            return count;
        }
    };
    std::string minimizeEspresso(std::vector<char> truth_table, std::vector<std::string> binary_codes, int varcount, std::string unique_var) const;
    std::vector<Cube> generateInitialCubes(const std::vector<int>& minterms, int varcount) const;
    std::vector<Cube> expand(const std::vector<Cube>& cubes, const std::vector<int>& minterms, const std::vector<int>& dont_cares, int varcount) const;
    std::vector<Cube> irredundant(const std::vector<Cube>& cubes, const std::vector<int>& minterms) const;
    std::vector<Cube> reduce(const std::vector<Cube>& cubes) const;
    bool canExpand(const Cube& cube, int variable, int value, const std::vector<int>& minterms, const std::vector<int>& dont_cares) const;
    std::set<int> getCoveredMinterms(const Cube& cube, int varcount) const;
    std::string cubeToString(const Cube& cube, const std::string& unique_var) const;
    std::string minimizeEspressoDual(std::vector<char> truth_table, std::vector<std::string> binary_codes, int varcount, std::string unique_var) const;
    std::string convertDNFtoCNF(const std::string& dnf, const std::string& unique_var) const;
    std::string invertTerm(const std::string& term) const;

    BooleanExpressionNode* root{};
    mutable std::string ZH;
    mutable std::string SD;
    mutable std::string SK;
    mutable std::string MDN;
    mutable std::string MCN;   
    static BooleanExpressionNode* Postfix2Tree(const char* expr);
    void InfixFilter(const char *instr, char *outstr);
    void Infix2Postfix(const char *instr, char *outstr);
public:
    BooleanExpression() = default;
    explicit BooleanExpression(const char *str);
    ~BooleanExpression();
    BooleanExpression(const BooleanExpression &);
    BooleanExpression(BooleanExpression &&) noexcept;
    BooleanExpression& operator=(const BooleanExpression &);
    BooleanExpression& operator=(BooleanExpression &&) noexcept;
    void setVars(BooleanExpressionNode* Node, bool value[]) const;
    std::string GetTable() const;
    [[nodiscard]] BooleanExpression zhegalkin() const;
    [[nodiscard]] BooleanExpression SDNF() const;
    [[nodiscard]] BooleanExpression SKNF() const;
    [[nodiscard]] BooleanExpression Minimized() const;
    [[nodiscard]] std::string table() const;
    BooleanExpression MinimizedCNF() const;
    std::string GetMinimizedCNF() const; 
    std::string GetZhegalkin() const;
    std::string GetSDNF() const;
    std::string GetSKNF() const;
    std::string GetMinimizedDNF() const;
    explicit operator std::string() const;
};
[[nodiscard]] std::string isFullSystem(const std::vector<BooleanExpression>&);
bool Monotonic(const std::string& left,const std::string& right);