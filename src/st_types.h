#ifndef ST_TYPES_H
#define ST_TYPES_H

#include <iostream>
#include <memory>
#include <vector>

class STNode
{
public:
    void addChild(std::shared_ptr<STNode> child);
    std::vector<std::shared_ptr<STNode>> getChildren();
    virtual std::string toString() const = 0;
    virtual std::string toCompleteString() const;
    virtual std::string getType() const = 0;
    friend std::ostream &operator<<(std::ostream &os, const STNode &node);

protected:
    std::vector<std::shared_ptr<STNode>> children;
};

class TruthValue : public STNode
{
public:
    TruthValue(std::string value);
    TruthValue(bool value);
    bool getValue() const;
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
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
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
    std::shared_ptr<Integer> negate() const;
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
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
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
    int getOrder() const;
    std::shared_ptr<Tuple> getCopy() const;
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
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
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;

private:
    std::string name;
};

class BinaryOperator : public STNode
{
public:
    BinaryOperator(std::string operation);
    BinaryOperator(std::string operation, std::shared_ptr<STNode> left, std::shared_ptr<STNode> right);
    std::string toString() const override;
    std::string getType() const override;

private:
    std::string operation;
};

class UnaryOperator : public STNode
{
public:
    UnaryOperator(std::string operation);
    UnaryOperator(std::string operation, std::shared_ptr<STNode> operand);
    std::string toString() const override;
    std::string getType() const override;

private:
    std::string operation;
};

class Function : public STNode
{
public:
    Function(std::string name, int arity);
    int getArity() const;
    std::vector<std::shared_ptr<STNode>> getArguments() const;
    void addArgument(std::shared_ptr<STNode> argument);
    int getArgumentCount() const;
    bool isFull() const;
    std::shared_ptr<Function> getCopy() const;
    std::string toString() const override;
    std::string getType() const override;

private:
    std::string name;
    int arity;
    std::vector<std::shared_ptr<STNode>> arguments;
    int argumentCount;
};

class Gamma : public STNode
{
public:
    Gamma();
    std::string toString() const override;
    std::string getType() const override;
};

class Lambda : public STNode
{
public:
    Lambda();
    std::shared_ptr<Lambda> getCopy() const;
    virtual std::string toString() const override;
    std::string toCompleteString() const override;
    virtual std::string getType() const override;
    int getBindingCount() const;
    std::vector<std::shared_ptr<Identifier>> getBindings() const;
    void addBinding(std::shared_ptr<Identifier> binding);
    int getIndex() const;
    void setIndex(int index);
    int getEnv() const;
    void setEnv(int env);

protected:
    int bindingCount;
    int index;
    int env;
    std::vector<std::shared_ptr<Identifier>> bindings;
};

class Tau : public STNode
{
public:
    Tau(std::vector<std::shared_ptr<STNode>> children);
    int getSize() const;
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;

private:
    int n;
};

class Arrow : public STNode
{
public:
    Arrow(std::vector<std::shared_ptr<STNode>> children);
    std::string toString() const override;
    std::string getType() const override;
};

class Delta : public STNode
{
public:
    Delta(std::shared_ptr<STNode> child);
    Delta(int index, std::shared_ptr<STNode> child);
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
    void setIndex(int index);
    int getIndex() const;

private:
    int index;
};

class Beta : public STNode
{
public:
    Beta();
    std::string toString() const override;
    std::string getType() const override;
};

class Equal : public STNode
{
public:
    Equal();
    std::string toString() const override;
    std::string getType() const override;
};

class Comma : public STNode
{
public:
    Comma();
    std::string toString() const override;
    std::string getType() const override;
};

class YStar : public STNode
{
public:
    YStar();
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
};

class Eta : public STNode
{
public:
    Eta(std::shared_ptr<Lambda> l);
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
    std::shared_ptr<Lambda> getLambda() const;
    int getBindingCount() const;
    std::vector<std::shared_ptr<Identifier>> getBindings() const;
    int getIndex() const;

private:
    std::shared_ptr<Lambda> l;
};

class Dummy : public STNode
{
public:
    Dummy(std::string value);
    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;

private:
    std::string value;
};

class IncompleteLambda : public Lambda
{
public:
    IncompleteLambda(std::shared_ptr<Lambda> l);
    void addArgument(std::shared_ptr<STNode> argument);
    std::vector<std::shared_ptr<STNode>> getArguments() const;
    int getArgumentCount() const;
    bool isComplete() const;
    std::string toString() const override;
    std::string getType() const override;

private:
    std::vector<std::shared_ptr<STNode>> arguments;
    int argumentCount;
};

#endif // ST_TYPES_H
