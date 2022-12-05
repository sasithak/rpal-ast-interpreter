#ifndef ST_TYPES_H
#define ST_TYPES_H

#include <iostream>
#include <memory>
#include <vector>

class STNode
{
public:
    /**
     * @brief Add a children to the node
     * @param child The child to add
     */
    void addChild(std::shared_ptr<STNode> child);

    /**
     * @brief Get the children of the node
     * @returns A vector containing children of the node
     */
    std::vector<std::shared_ptr<STNode>> getChildren();

    /**
     * @brief Get the value of the node
     * @return The value of the node as a string
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Get the value of the node
     * @return The value of the node with formatting as a string
     */
    virtual std::string toCompleteString() const;

    /**
     * @brief Get the type of the node
     * @return The type of the node as a string
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Prints the node for the 'Print' function
     */
    virtual void print() const;

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
    void print() const override;
    std::shared_ptr<TruthValue> operator==(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator!=(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator<(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator<=(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator>(std::shared_ptr<String> other) const;
    std::shared_ptr<TruthValue> operator>=(std::shared_ptr<String> other) const;
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

    /**
     * @brief Get the order of the tuple
     * @return The order of the tuple
     */
    int getOrder() const;

    /**
     * @brief Create a copy of the tuple
     * @return A copy of the tuple
     */
    std::shared_ptr<Tuple> getCopy() const;

    std::string toString() const override;
    std::string toCompleteString() const override;
    std::string getType() const override;
    void print() const override;

    /**
     * @brief Insert an element to the tuple
     * @param value The element to insert
     */
    void push_back(std::shared_ptr<STNode> value);

    /**
     * @brief Get the element at the given index
     * @return nullptr if the index is out of range, the element at the given index otherwise
     */
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

    /**
     * @brief Get the arity of the function
     * @return The arity
     */
    int getArity() const;

    /**
     * @brief Get the stored arguments of the function
     * @return A vector of the stored arguments
     */
    std::vector<std::shared_ptr<STNode>> getArguments() const;

    /**
     * @brief Store an argument
     * @param argument The argument to store
     */
    void addArgument(std::shared_ptr<STNode> argument);

    /**
     * @brief Get the number of arguments stored
     * @return The number of arguments stored
     */
    int getArgumentCount() const;

    /**
     * @brief Check if the function is fully applied
     * @return True if the arguments count is equal to the arity, false otherwise
     */
    bool isFull() const;

    /**
     * @brief Create a copy of the function
     * @return A copy of the function
     */
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

    /**
     * @brief Create a copy of the lambda
     * @return A copy of the lambda
     */
    std::shared_ptr<Lambda> getCopy() const;

    virtual std::string toString() const override;
    std::string toCompleteString() const override;
    virtual std::string getType() const override;
    virtual void print() const override;

    /**
     * @brief Get the number of bindings stored
     * @return The number of bindings
     */
    int getBindingCount() const;

    /**
     * @brief Get the stored bindings
     * @return A vector of the stored bindings
     */
    std::vector<std::shared_ptr<Identifier>> getBindings() const;

    /**
     * @brief Bind an identifier to the lambda
     * @param binding The identifier to bind
     */
    void addBinding(std::shared_ptr<Identifier> binding);

    /**
     * @brief Get the index of the lambda
     * @return The index
     */
    int getIndex() const;

    /**
     * @brief Set the index of the lambda
     * @param index The index to set
     */
    void setIndex(int index);

    /**
     * @brief Get the environment for which lambda is bound to
     * @return The environment
     */
    int getEnv() const;

    /**
     * @brief Set the environment for which lambda is bound to
     * @param env The environment to set
     */
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

    /**
     * @brief Get the size of the tau
     * @return The size
     */
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

    /**
     * @brief Set the index of the delta
     * @param index The index to set
     */
    void setIndex(int index);

    /**
     * @brief Get the index of the delta
     * @return The index
     */
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

    /**
     * @brief Get the lambda for which eta is the fixed point
     * @return The lambda
     */
    std::shared_ptr<Lambda> getLambda() const;

    /**
     * @brief Get the binding count of the corresponding lambda
     * @return The binding count
     */
    int getBindingCount() const;

    /**
     * @brief Get the bindings of the corresponding lambda
     * @return A vector of the bindings
     */
    std::vector<std::shared_ptr<Identifier>> getBindings() const;

    /**
     * @brief Get the index of the corresponding lambda
     * @return The index
     */
    int getIndex() const;

private:
    std::shared_ptr<Lambda> l;
};

class Dummy : public STNode
{
public:
    Dummy();
    std::string toString() const override;
    std::string getType() const override;
};

#endif // ST_TYPES_H
