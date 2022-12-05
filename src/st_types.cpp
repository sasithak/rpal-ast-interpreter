#include <iostream>
#include <cmath>
#include <memory>
#include <string>
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

string STNode::toCompleteString() const
{
    return toString();
}

void STNode::print() const
{
    cout << *this;
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

string TruthValue::toCompleteString() const
{
    return value ? "<true>" : "<false>";
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

string Integer::toCompleteString() const
{
    return "<INT:" + to_string(value) + ">";
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
    return make_shared<Integer>((int)pow(value, other->getValue()));
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

string String::toCompleteString() const
{
    return "<STR:'" + value + "'>";
}

string String::getType() const
{
    return "String";
}

void String::print() const
{
    bool backslash = false;
    for (char c : value)
    {
        if (backslash)
        {
            // Correctly print escaped characters
            switch (c)
            {
            case 'n':
                cout << endl;
                break;
            case 't':
                cout << '\t';
                break;
            case '\\':
                cout << '\\';
                break;
            default:
                cout << c;
                break;
            }
            backslash = false;
        }
        else
        {
            if (c == '\\')
            {
                // Escape character
                backslash = true;
            }
            else
            {
                cout << c;
            }
        }
    }
}

shared_ptr<TruthValue> String::operator==(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value == other->getValue());
}

shared_ptr<TruthValue> String::operator!=(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value != other->getValue());
}

shared_ptr<TruthValue> String::operator<(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value.compare(other->getValue()) < 0);
}

shared_ptr<TruthValue> String::operator<=(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value.compare(other->getValue()) <= 0);
}

shared_ptr<TruthValue> String::operator>(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value.compare(other->getValue()) > 0);
}

shared_ptr<TruthValue> String::operator>=(shared_ptr<String> other) const
{
    return make_shared<TruthValue>(value.compare(other->getValue()) >= 0);
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

shared_ptr<Tuple> Tuple::getCopy() const
{
    shared_ptr<Tuple> t = make_shared<Tuple>();
    for (auto value : values)
    {
        string valueStr = value->getType();
        if (valueStr == "Tuple")
        {
            t->push_back(dynamic_pointer_cast<Tuple>(value)->getCopy());
        }
        else if (valueStr == "Function")
        {
            t->push_back(dynamic_pointer_cast<Function>(value)->getCopy());
        }
        else if (valueStr == "Lambda")
        {
            t->push_back(dynamic_pointer_cast<Lambda>(value)->getCopy());
        }
        else
        {
            t->push_back(value);
        }
    }
    return t;
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

string Tuple::toCompleteString() const
{
    if (size == 0)
        return "<nil>";

    string str = "(";
    for (int i = 0; i < size; ++i)
    {
        str += values[i]->toCompleteString();
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

void Tuple::print() const
{
    if (size == 0)
    {
        cout << "nil";
        return;
    }

    cout << "(";
    for (int i = 0; i < size; ++i)
    {
        values[i]->print();
        if (i != size - 1)
        {
            cout << ", ";
        }
    }
    cout << ")";
}

void Tuple::push_back(shared_ptr<STNode> value)
{
    values.push_back(value);
    ++size;
}

shared_ptr<STNode> Tuple::operator[](int index) const
{
    if (index < 0 || index >= size)
    {
        return nullptr;
    }
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

string Identifier::toCompleteString() const
{
    return "<ID:" + name + ">";
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

Function::Function(string name, int arity)
{
    this->name = name;
    this->arity = arity;
    this->argumentCount = 0;
}

int Function::getArity() const
{
    return arity;
}

vector<shared_ptr<STNode>> Function::getArguments() const
{
    return arguments;
}

void Function::addArgument(shared_ptr<STNode> argument)
{
    arguments.push_back(argument);
    ++argumentCount;
}

int Function::getArgumentCount() const
{
    return argumentCount;
}

bool Function::isFull() const
{
    return argumentCount == arity;
}

shared_ptr<Function> Function::getCopy() const
{
    return make_shared<Function>(name, arity);
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
    this->env = -1;
}

shared_ptr<Lambda> Lambda::getCopy() const
{
    shared_ptr<Lambda> copy = make_shared<Lambda>();
    copy->index = index;
    copy->env = env;
    copy->bindingCount = bindingCount;
    copy->bindings = bindings;
    return copy;
}

string Lambda::toString() const
{
    string s = "";

    if (env >= 0)
    {
        s += to_string(env) + ".";
    }

    s += "lambda";

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

string Lambda::toCompleteString() const
{
    return "lambda";
}

string Lambda::getType() const
{
    return "Lambda";
}

void Lambda::print() const
{
    cout << "[lambda closure: ";
    for (int i = 0; i < bindingCount; ++i)
    {
        cout << bindings[i]->toString() << (i != bindingCount - 1 ? ", " : ": ");
    }
    cout << index << "]";
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

int Lambda::getEnv() const
{
    return env;
}

void Lambda::setEnv(int env)
{
    this->env = env;
}

Tau::Tau(vector<shared_ptr<STNode>> children)
{
    n = children.size();
    for (int i = 0; i < n; ++i)
    {
        this->addChild(children[i]);
    }
}

int Tau::getSize() const
{
    return n;
}

string Tau::toString() const
{
    return "tau_" + to_string(n);
}

string Tau::toCompleteString() const
{
    return "tau";
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

string Delta::toCompleteString() const
{
    return "delta";
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

string YStar::toCompleteString() const
{
    return "<Y*>";
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
    int env = l->getEnv();
    auto bindings = l->getBindings();

    string s = "";

    if (env >= 0)
    {
        s += to_string(env) + ".";
    }

    s += "eta";
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

string Eta::toCompleteString() const
{
    return "eta";
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

Dummy::Dummy()
{
}

string Dummy::toString() const
{
    return "dummy";
}

string Dummy::getType() const
{
    return "Dummy";
}
