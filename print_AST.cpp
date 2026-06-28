#include "pic.hh"

# ifndef PRINT_AST_H
# define PRINT_AST_H

void printAST(ASTNode* node, int depth) {
    // Indentation for better readability
    for (int i = 0; i < depth; ++i) {
        cout << " ~";
    }

    // Print node type and value
    switch (node->type) {
        case IDENTIFIER:
            cout << "Identifier: " << *node->value << endl;
            break;
        case INT:
            cout << "Int: " << *node->value << endl;
            break;
        case FLOAT:
            cout << "Float: " << *node->value << endl;
            break;
        case BOOLEAN:
            cout << "Boolean: " << *node->value << endl;
            break;
        case STRING:
            cout << "String: " << *node->value << endl;
            break;
        case ARITHMETIC_OPERATOR:
            cout << "Arithmetic Operator: " << *node->value << endl;
            break;
        case BOOLEAN_OPERATOR:
            cout << "Boolean Operator: " << *node->value << endl;
            break;
        case COMPARISION_OPERATOR:
            cout << "Comparision Operator: " << *node->value << endl;
            break;
        case TERNARY_OPERATOR:
            cout << "Ternary Operator: " << endl;
            break;
        case DECLARATION_ASSIGNMENT:
            cout << "Declaration Assignment: " << endl;
            break;
        case ASSIGNMENT:
            cout << "Assignment: " << endl;
            break;
        case EMPTY:
            cout << "Empty" << endl;
            break;
        case COMPOUND_STATEMENT:
            cout << "Compound Statement: " << endl;
            break;
        case PRINT_STATEMENT:
            cout << "Print Statement: " << endl;
            break;
        case IF_STATEMENT:
            cout << "If Statement: " << endl;
            break;
        case IF_CONDITION:
            cout << "Condition: " << endl;
            break;
        case IF_THEN_STATEMENT:
            cout << "Then Statement: " << endl;
            break;
        case ELSE_STATEMENT:
            cout << "Else Statement: " << endl;
            break;
        case WHILE_STATEMENT:
            cout << "While Statement: " << endl;
            break;
        case WHILE_CONDITION:
            cout << "Condition: " << endl;
            break;
        case WHILE_BODY:
            cout << "Body: " << endl;
            break;
        case DO_WHILE_STATEMENT:
            cout << "Do While Statement: " << endl;
            break;
        case DO_WHILE_BODY:
            cout << "Body: " << endl;
            break;
        case DO_WHILE_CONDITION:
            cout << "Condition: " << endl;
            break;
        case CALL_FUNC_PARAM_LIST:
            cout << "Function Parameter List at call: " << endl;
            break;
        case FUNCTION_CALL:
            cout << "Function Call: " << endl;
            break;
        case FORMAL_FUNC_PARAM_LIST:
            cout << "Formal Function Parameter List: " << endl;
            break;
        case FUNCTION_DEFINITION:
            cout << "Function Definition: " << endl;
            break;
        case RETURN_STATEMENT:
            cout << "Return Statement: " << endl;
            break;
        default:
            cout << "Unknown Type" << endl;
    }

    for (ASTNode* child : *(node->children)) {
        printAST(child, depth + 1);
    }
}

void printProgram(vector<ASTNode*>* program) {
    if (!program) return;
    for (ASTNode* node : *program) {
        printAST(node, 0);
    }
}

# endif