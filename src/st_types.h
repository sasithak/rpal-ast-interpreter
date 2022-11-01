#ifndef ST_TYPES_H
#define ST_TYPES_H

#include <iostream>
#include <memory>
#include <vector>

class STNode
{
public:
    void addChild(STNode *child);
    std::vector<std::shared_ptr<STNode>> getChildren();
    virtual std::string toString() const = 0;

private:
    std::vector<std::shared_ptr<STNode>> children;
};

class TruthValue : public STNode
{
public:
    TruthValue(std::string value);
    TruthValue(bool value);
    bool getValue() const;
    std::string toString() const;
    std::shared_ptr<TruthValue> operator&&(std::shared_ptr<TruthValue> other) const;
    std::shared_ptr<TruthValue> operator||(std::shared_ptr<TruthValue> other) const;
    std::shared_ptr<TruthValue> operator!() const;
    std::shared_ptr<TruthValue> operator==(std::shared_ptr<TruthValue> other) const;
    std::shared_ptr<TruthValue> operator!=(std::shared_ptr<TruthValue> other) const;

private:
    bool value;
};

class Integer : public STNode
{
public:
    Integer(std::string value);
    Integer(int value);
    int getValue() const;
    std::string toString() const;
    std::shared_ptr<Integer> operator+(std::shared_ptr<Integer> other) const;
    std::shared_ptr<Integer> operator-(std::shared_ptr<Integer> other) const;
    std::shared_ptr<Integer> operator*(std::shared_ptr<Integer> other) const;
    std::shared_ptr<Integer> operator/(std::shared_ptr<Integer> other) const;
    std::shared_ptr<Integer> operator^(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator==(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator!=(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator<(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator<=(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator>(std::shared_ptr<Integer> other) const;
    std::shared_ptr<TruthValue> operator>=(std::shared_ptr<Integer> other) const;

private:
    int value;
};

class String : public STNode
{
public:
    String(std::string value);
    String(const char &value);
    std::string getValue() const;
    std::string toString() const;
    std::shared_ptr<TruthValue> operator==(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator!=(std::shared_ptr<String> other) const;
    std::shared_ptr<String> operator+(std::shared_ptr<String> other) const;
    std::shared_ptr<String> stem() const;
    std::shared_ptr<String> stern() const;

private:
    std::string value;
};

class Tuple : public STNode
{
public:
    Tuple();
    Tuple(std::vector<std::shared_ptr<STNode>> values);
    std::vector<std::shared_ptr<STNode>> getValues() const;
    std::string toString() const;
    void push_back(std::shared_ptr<STNode> value);
    std::shared_ptr<STNode> operator[](int index) const;

private:
    std::vector<std::shared_ptr<STNode>> values;
    int size;
};

class Identifier : public STNode
{
public:
    Identifier(std::string name);
    std::string getName() const;
    std::string toString() const;

private:
    std::string name;
};

class BinaryOperator : public STNode
{
public:
    BinaryOperator(std::string operation);
    std::string toString() const;

private:
    std::string operation;
};

class UnaryOperator : public STNode
{
public:
    UnaryOperator(std::string operation);
    std::string toString() const;

private:
    std::string operation;
};

class Function : public STNode
{
public:
    Function(std::string name);
    std::string toString() const;

private:
    std::string name;
};

class Gamma : public STNode
{
public:
    Gamma();
    std::string toString() const;
};

class Lambda : public STNode
{
public:
    Lambda();
    std::string toString() const;
};

#endif // ST_TYPES_H
