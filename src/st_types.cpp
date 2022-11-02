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
    return value;
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

string Tuple::toString() const
{
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

Function::Function(string name)
{
    this->name = name;
}

string Function::toString() const
{
    return name;
}

Gamma::Gamma()
{
}

string Gamma::toString() const
{
    return "gamma";
}

Lambda::Lambda()
{
}

string Lambda::toString() const
{
    return "lambda";
}
