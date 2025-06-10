#pragma once
#include <string>
#include <vector>


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
    BooleanExpressionNode* root{};
    mutable std::string zh;
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
    [[nodiscard]] std::string table() const;
    std::string GetZhegalkin() const;
    explicit operator std::string() const;
};
[[nodiscard]] bool isFullSystem(const std::vector<BooleanExpression>&);
bool Monotonic(const std::string& left,const std::string& right);