#include "pic.hh"

#ifndef OBJECTS_H
#define OBJECTS_H

enum class ObjectType{
    NUMBER,
    BOOL,
    STRING,
    FUNCTION,
    ERROR
};

struct Object{
    void* value;
	ObjectType otype;

    virtual ~Object() {}
    virtual Object* clone() const = 0;

	virtual string str() const = 0;
};

struct NumberObject : Object{
    NumberObject(double value){
        this->value = new double(value);
        otype = ObjectType::NUMBER;
    }

    NumberObject(const NumberObject& other) {
        value = new double(*(double*)other.value);
        otype = other.otype;
    }
    
    ~NumberObject() override {
        delete (double*)value;
    }

    string str() const override{
        if (abs(*(double*)value - (int)*(double*)value)<0.0000001) {
            return to_string((int)*(double*)value);
        }
        return to_string(*(double*)value);
    }

    NumberObject* clone() const override {
        return new NumberObject(*this);
    }
};

struct BoolObject : Object{
    BoolObject(bool value){
        this->value = new bool(value);
        otype = ObjectType::BOOL;
    }

    BoolObject(const BoolObject& other) {
        value = new bool(*(bool*)other.value);
        otype = other.otype;
    }

    ~BoolObject() override {
        delete (bool*)value;
    }

    string str() const override{
        return *(bool*)value ? "true" : "false";
    }

    BoolObject* clone() const override {
        return new BoolObject(*this);
    }
};

struct StringObject : Object{
    StringObject(string* value){
        this->value = value;
        otype = ObjectType::STRING;
    }

    StringObject(const StringObject& other) {
        value = new string(*(string*)other.value);
        otype = other.otype;
    }

    ~StringObject() override {
        delete (string*)value;
    }

    string str() const override{
        return *(string*)value;
    }

    StringObject* clone() const override {
        return new StringObject(*this);
    }
};

struct FunctionObject : Object{
    vector<string> params;
    vector<ASTNode*>* funcBody;
    unordered_map<string, Object*>* closure;

    FunctionObject() {
        otype = ObjectType::FUNCTION;
        value = NULL;
        funcBody = new vector<ASTNode*>();
        closure = new unordered_map<string, Object*>();
    }

    FunctionObject(const FunctionObject& other) {
        otype = other.otype;
        value = NULL;

        for (auto param : other.params) {
            params.push_back(param);
        }

        funcBody = other.funcBody;

        closure = new unordered_map<string, Object*>();
        for (auto it = other.closure->begin(); it != other.closure->end(); it++) {
            closure->insert({it->first, it->second->clone()});
        }
    }

    ~FunctionObject() override {
        delete (FunctionObject*)value;

        for (auto it = closure->begin(); it != closure->end(); ) {
            delete it->second;
            it = closure->erase(it);
        }
    }

    string str() const override{
        cerr<<"Print for function not implemented"<<endl;
        return "";
    }

    FunctionObject* clone() const override {
        return new FunctionObject(*this);
    }
};

enum class ErrorType{
    BOOL_EXPECTED,
    NUMBER_EXPECTED,
    DIV_BY_ZERO,
    ID_UNDECLARED,
    ID_REDECLARED,
    ID_NOT_FUNC,
    WRONG_ARG_COUNT
};

struct ErrorObject : Object{
    ErrorType etype;

    ErrorObject(ErrorType etype, string value): etype(etype){
        this->value = new string(value);
        otype = ObjectType::ERROR;
    }

    ErrorObject(ErrorType etype): etype(etype){
        this->value = NULL;
        otype = ObjectType::ERROR;
    }

    ErrorObject(const ErrorObject& other) {
        value = new string(*(string*)other.value);
        etype = other.etype;
        otype = other.otype;
    }

    ~ErrorObject() override {
        delete (string*)value;
    }

    string str() const override{
        switch(etype){
            case ErrorType::BOOL_EXPECTED:
                return "Error: Expected a boolean value";
            case ErrorType::NUMBER_EXPECTED:
                return "Error: Expected a number value";
            case ErrorType::DIV_BY_ZERO:
                return "Error: Division by zero";
            case ErrorType::ID_UNDECLARED:
                return "Error: Identifier "+*(string*)value+" not declared";
            case ErrorType::ID_REDECLARED:
                return "Error: Identifier "+*(string*)value+" already declared";
            case ErrorType::ID_NOT_FUNC:
                return "Error: Identifier "+*(string*)value+" is not a function";
            case ErrorType::WRONG_ARG_COUNT:
                return "Error: "+*(string*)value+" received wrong number of arguments";
            default:
                return "Error: Unknown error";
        }
    }

    ErrorObject* clone() const override {
        return new ErrorObject(*this);
    }
};

#endif