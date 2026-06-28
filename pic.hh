#include <bits/stdc++.h>
using namespace std;

#ifndef ASTNODE_H
#define ASTNODE_H

enum Type {
    IDENTIFIER,

    INT,
    FLOAT,
    BOOLEAN,
    STRING,

    ARITHMETIC_OPERATOR,
    BOOLEAN_OPERATOR,
    COMPARISION_OPERATOR,
    TERNARY_OPERATOR,

    DECLARATION_ASSIGNMENT,
    ASSIGNMENT,
    EMPTY,
    COMPOUND_STATEMENT,

    PRINT_STATEMENT,

    IF_STATEMENT,
    IF_CONDITION,
    IF_THEN_STATEMENT,
    ELSE_STATEMENT,

    WHILE_STATEMENT,
    WHILE_CONDITION,
    WHILE_BODY,
    DO_WHILE_STATEMENT,
    DO_WHILE_BODY,
    DO_WHILE_CONDITION,

    CALL_FUNC_PARAM_LIST,
    FUNCTION_CALL,
    FORMAL_FUNC_PARAM_LIST,
    FUNCTION_DEFINITION,

    RETURN_STATEMENT
};

struct ASTNode {
    Type type;
    string* value;
    
    vector<ASTNode*>* children;

    ASTNode(const Type& type, string* value) 
        : type(type), value(value) 
        { children = new vector<ASTNode*>(); }
    
    ASTNode(const Type& type, string* value, ASTNode* child) 
        : type(type), value(value) 
        { children = new vector<ASTNode*>{child}; }
    
    ASTNode(const Type& type, string* value, ASTNode* child1, ASTNode* child2) 
        : type(type), value(value) 
        { children = new vector<ASTNode*>{child1, child2}; }
    
    ASTNode(const Type& type, string* value, ASTNode* child1, ASTNode* child2, ASTNode* child3) 
        : type(type), value(value) 
        { children = new vector<ASTNode*>{child1, child2, child3}; }
    
    ASTNode(const Type& type, string* value, vector<ASTNode*>* children) 
        : type(type), value(value), children(children) { }

    void addChild(ASTNode* child) {
        children->push_back(child);
    }

    ASTNode* deepCopyNode() {
        ASTNode* copy = new ASTNode(type, value);
        for (ASTNode* child : *children) {
            copy->addChild(child->deepCopyNode());
        }
    return copy;
    }

    ~ASTNode() {
        delete value;
        for (ASTNode* child : *children) {
            delete child;
        }
        delete children;
    }
};

#endif