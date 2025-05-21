#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <functional>
#include "Error.h"
#include <algorithm>


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
    explicit VarNode(std::string  name, bool value) : name(std::move(name)), value(value) {}
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
        return new VarNode(*this);
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
        return new ConstNode(*this);
    };
};
class UnarNode : public BooleanExpressionNode {
protected:
    BooleanExpressionNode *next;
public:
    explicit UnarNode(BooleanExpressionNode *node) : next(node) {}
    BooleanExpressionNode* getNext(){return next;}
    ~UnarNode() override {delete next;}
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
        return new NegationNode(*this);
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
};
class conjunctionNode : public BinNode
{
public:
    conjunctionNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " & " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() && right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new conjunctionNode(*this);
    }
};
class disjunctionNode : public BinNode
{
public:
    disjunctionNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new disjunctionNode(*this);
    }
};
class squareNode : public BinNode
{
public:
    explicit squareNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
    [[nodiscard]]std::string str() const override
    {
        return "(" + left->str() + " ^ " + right->str() + ")";
    }
    [[nodiscard]] bool calc() const override
    {
        return left->calc() ^ right->calc();
    }
    BooleanExpressionNode* clone() override
    {
        return new squareNode(*this);
    }
};
class implicationNode : public BinNode
{
public:
    implicationNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new implicationNode(*this);
    }
};
class UnimplicationNode : public BinNode
{
public:
    UnimplicationNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new UnimplicationNode(*this);
    }
};
class equivalentNode : public BinNode
{
public:
    equivalentNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new equivalentNode(*this);
    }
};
class SchaefferNode : public BinNode
{
public:
    SchaefferNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new SchaefferNode(*this);
    }
};
class PierNode : public BinNode
{
public:
    PierNode(BooleanExpressionNode* left, BooleanExpressionNode* right): BinNode(left,right){}
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
        return new PierNode(*this);
    }
};
class BooleanExpression
{
private:
    BooleanExpressionNode* root{};
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
    [[nodiscard]] BooleanExpression zhegalkin() const;
    [[nodiscard]] std::string table() const;
    explicit operator std::string() const;
    [[nodiscard]] bool isFullSystem(const std::vector<BooleanExpression>&);
};