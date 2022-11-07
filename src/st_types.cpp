#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include "st_types.h"

using namespace std;

void STNode::addChild(shared_ptr<STNode> child)
{
    children.push_back(child);
}

vector<shared_ptr<STNode>> STNode::getChildren()
{
    return children;
}

ostream &operator<<(ostream &os, const STNode &node)
{
    os << node.toString();
    return os;
}

TruthValue::TruthValue(string value)
{
    this->value = value == "true";
}

TruthValue::TruthValue(bool value)
{
    this->value = value;
}

bool TruthValue::getValue() const
{
    return value;
}

string TruthValue::toString() const
{
    return value ? "true" : "false";
}

string TruthValue::getType() const
{
    return "TruthValue";
}

shared_ptr<TruthValue> TruthValue::operator&&(shared_ptr<TruthValue> other) const
{
    return make_shared<TruthValue>(value && other->getValue());
}

shared_ptr<TruthValue> TruthValue::operator||(shared_ptr<TruthValue> other) const
{
    return make_shared<TruthValue>(value || other->getValue());
}

shared_ptr<TruthValue> TruthValue::operator!() const
{
    return make_shared<TruthValue>(!value);
}

shared_ptr<TruthValue> TruthValue::operator==(shared_ptr<TruthValue> other) const
{
    return make_shared<TruthValue>(value == other->getValue());
}

shared_ptr<TruthValue> TruthValue::operator!=(shared_ptr<TruthValue> other) const
{
    return make_shared<TruthValue>(value != other->getValue());
}

Integer::Integer(string value)
{
    this->value = stoi(value);
}

Integer::Integer(int value)
{
    this->value = value;
}

int Integer::getValue() const
{
    return value;
}

string Integer::toString() const
{
    return to_string(value);
}

string Integer::getType() const
{
    return "Integer";
}

shared_ptr<Integer> Integer::negate() const
{
    return make_shared<Integer>(-value);
}

shared_ptr<Integer> Integer::operator+(shared_ptr<Integer> other) const
{
    return make_shared<Integer>(value + other->getValue());
}

shared_ptr<Integer> Integer::operator-(shared_ptr<Integer> other) const
{
    return make_shared<Integer>(value - other->getValue());
}

shared_ptr<Integer> Integer::operator*(shared_ptr<Integer> other) const
{
    return make_shared<Integer>(value * other->getValue());
}

shared_ptr<Integer> Integer::operator/(shared_ptr<Integer> other) const
{
    return make_shared<Integer>(value / other->getValue());
}

shared_ptr<Integer> Integer::operator^(shared_ptr<Integer> other) const
{
    return make_shared<Integer>(pow(value, other->getValue()));
}

shared_ptr<TruthValue> Integer::operator==(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value == other->getValue());
}

shared_ptr<TruthValue> Integer::operator!=(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value != other->getValue());
}

shared_ptr<TruthValue> Integer::operator<(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value < other->getValue());
}

shared_ptr<TruthValue> Integer::operator<=(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value <= other->getValue());
}

shared_ptr<TruthValue> Integer::operator>(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value > other->getValue());
}

shared_ptr<TruthValue> Integer::operator>=(shared_ptr<Integer> other) const
{
    return make_shared<TruthValue>(value >= other->getValue());
}

String::String(string value)
{
    this->value = value;
}

String::String(const char &value)
{
    this->value = value;
}

string String::getValue() const
{
    return value;
}

string String::toString() const
{
    return "'" + value + "'";
}

string String::getType() const
{
    return "String";
}

shared_ptr<TruthValue> String::operator==(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value == other->getValue());
}

shared_ptr<TruthValue> String::operator!=(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value != other->getValue());
}

shared_ptr<String> String::operator+(shared_ptr<String> other) const
{
    return make_shared<String>(value + other->getValue());
}

shared_ptr<String> String::stem() const
{
    return make_shared<String>(value[0]);
}

shared_ptr<String> String::stern() const
{
    return make_shared<String>(value.substr(1, value.length() - 1));
}

Tuple::Tuple()
{
    this->size = 0;
}

Tuple::Tuple(vector<shared_ptr<STNode>> values)
{
    this->values = values;
    this->size = values.size();
}

vector<shared_ptr<STNode>> Tuple::getValues() const
{
    return values;
}

int Tuple::getOrder() const
{
    return size;
}

string Tuple::toString() const
{
    if (size == 0)
        return "nil";

    string str = "(";
    for (int i = 0; i < size; ++i)
    {
        str += values[i]->toString();
        if (i != size - 1)
        {
            str += ", ";
        }
    }
    str += ")";
    return str;
}

string Tuple::getType() const
{
    return "Tuple";
}

void Tuple::push_back(shared_ptr<STNode> value)
{
    values.push_back(value);
    ++size;
}

shared_ptr<STNode> Tuple::operator[](int index) const
{
    return values[index];
}

Identifier::Identifier(string name)
{
    this->name = name;
}

string Identifier::getName() const
{
    return name;
}

string Identifier::toString() const
{
    return name;
}

string Identifier::getType() const
{
    return "Identifier";
}

BinaryOperator::BinaryOperator(string op)
{
    this->operation = op;
}

BinaryOperator::BinaryOperator(string op, shared_ptr<STNode> left, shared_ptr<STNode> right)
{
    this->operation = op;
    this->addChild(left);
    this->addChild(right);
}

string BinaryOperator::toString() const
{
    return operation;
}

string BinaryOperator::getType() const
{
    return "BinaryOperator";
}

UnaryOperator::UnaryOperator(string op)
{
    this->operation = op;
}

UnaryOperator::UnaryOperator(string op, shared_ptr<STNode> child)
{
    this->operation = op;
    this->addChild(child);
}

string UnaryOperator::toString() const
{
    return operation;
}

string UnaryOperator::getType() const
{
    return "UnaryOperator";
}

Function::Function(string name)
{
    this->name = name;
}

string Function::toString() const
{
    return name;
}

string Function::getType() const
{
    return "Function";
}

Gamma::Gamma()
{
}

string Gamma::toString() const
{
    return "gamma";
}

string Gamma::getType() const
{
    return "Gamma";
}

Lambda::Lambda()
{
    this->bindingCount = 0;
    this->index = 0;
}

string Lambda::toString() const
{
    string s = "lambda";
    if (index > 0)
        s += "_" + to_string(index);
    s += "^";
    if (bindingCount > 1)
    {
        s += "(";
        for (int i = 0; i < bindingCount; ++i)
        {
            s += bindings[i]->toString();
            if (i != bindingCount - 1)
            {
                s += ",";
            }
        }
        s += ")";
    }
    else
    {
        s += bindings[0]->toString();
    }

    return s;
}

string Lambda::getType() const
{
    return "Lambda";
}

int Lambda::getBindingCount() const
{
    return bindingCount;
}

vector<shared_ptr<Identifier>> Lambda::getBindings() const
{
    return bindings;
}

void Lambda::addBinding(shared_ptr<Identifier> binding)
{
    bindings.push_back(binding);
    ++bindingCount;
}

int Lambda::getIndex() const
{
    return index;
}

void Lambda::setIndex(int index)
{
    this->index = index;
}

Tau::Tau(vector<shared_ptr<STNode>> children)
{
    n = children.size();
    for (int i = 0; i < n; ++i)
    {
        this->addChild(children[i]);
    }
}

string Tau::toString() const
{
    return "tau_" + to_string(n);
}

string Tau::getType() const
{
    return "Tau";
}

Arrow::Arrow(vector<shared_ptr<STNode>> children)
{
    int n = children.size();
    for (int i = 0; i < n; ++i)
    {
        this->addChild(children[i]);
    }
}

string Arrow::toString() const
{
    return "->";
}

string Arrow::getType() const
{
    return "Arrow";
}

Delta::Delta(shared_ptr<STNode> child)
{
    this->index = -1;
    this->addChild(child);
}

Delta::Delta(int index, shared_ptr<STNode> child)
{
    this->index = index;
    this->addChild(child);
}

string Delta::toString() const
{
    return "delta" + (index >= 0 ? "_" + to_string(index) : "");
}

string Delta::getType() const
{
    return "Delta";
}

void Delta::setIndex(int index)
{
    this->index = index;
}

int Delta::getIndex() const
{
    return index;
}

Beta::Beta()
{
}

string Beta::toString() const
{
    return "beta";
}

string Beta::getType() const
{
    return "Beta";
}

Equal::Equal()
{
}

string Equal::toString() const
{
    return "=";
}

string Equal::getType() const
{
    return "Equal";
}

Comma::Comma()
{
}

string Comma::toString() const
{
    return ",";
}

string Comma::getType() const
{
    return "Comma";
}

YStar::YStar()
{
}

string YStar::toString() const
{
    return "Y";
}

string YStar::getType() const
{
    return "YStar";
}

Eta::Eta(shared_ptr<Lambda> l)
{
    this->l = l;
}

string Eta::toString() const
{
    int index = l->getIndex();
    int bindingCount = l->getBindingCount();
    auto bindings = l->getBindings();

    string s = "eta";
    if (index > 0)
        s += "_" + to_string(index);
    s += "^";
    if (bindingCount > 1)
    {
        s += "(";
        for (int i = 0; i < bindingCount; ++i)
        {
            s += bindings[i]->toString();
            if (i != bindingCount - 1)
            {
                s += ",";
            }
        }
        s += ")";
    }
    else
    {
        s += bindings[0]->toString();
    }

    return s;
}

string Eta::getType() const
{
    return "Eta";
}

shared_ptr<Lambda> Eta::getLambda() const
{
    return l;
}

int Eta::getBindingCount() const
{
    return l->getBindingCount();
}

vector<shared_ptr<Identifier>> Eta::getBindings() const
{
    return l->getBindings();
}

int Eta::getIndex() const
{
    return l->getIndex();
}
