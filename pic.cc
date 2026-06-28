#include "pic.hh"
#include "objects.cpp"
#include "env.cpp"

Object* executeAST(ASTNode* node, Scope* sTable, Object*& return_obj){
    if (return_obj != NULL){
        return NULL;
    }

    switch (node->type) {
        case INT:
        case FLOAT:
            return new NumberObject(stod(*node->value));
        case BOOLEAN:
            return new BoolObject(*node->value == "true");
        case STRING:
            return new StringObject(node->value);
        case IDENTIFIER:
            return sTable->checkAndReturnClone(*node->value);

        case ARITHMETIC_OPERATOR:
        {
            if (*node->value == "UMINUS"){
                Object* exp = executeAST((*node->children)[0], sTable, return_obj);

                if (exp->otype == ObjectType::ERROR){
                    return exp;
                }
                else if (exp->otype != ObjectType::NUMBER){
                    delete exp;
                    return new ErrorObject(ErrorType::NUMBER_EXPECTED);
                }
                else{
                    NumberObject* numExp = dynamic_cast<NumberObject*>(exp);
                    *(double*)(numExp->value) = -(*(double*)(numExp->value));
                    return exp;
                }
            }
            else{
                Object* lhs = executeAST((*node->children)[0], sTable, return_obj);
                Object* rhs = executeAST((*node->children)[1], sTable, return_obj);
                
                if (lhs->otype == ObjectType::ERROR){
                    delete rhs;
                    return lhs;
                }
                else if (rhs->otype == ObjectType::ERROR){
                    delete lhs;
                    return rhs;
                }
                else if (lhs->otype != ObjectType::NUMBER || rhs->otype != ObjectType::NUMBER){
                    return new ErrorObject(ErrorType::NUMBER_EXPECTED);
                }
                else{
                    NumberObject* numLhs = dynamic_cast<NumberObject*>(lhs);
                    NumberObject* numRhs = dynamic_cast<NumberObject*>(rhs);
                    
                    if (*node->value == "PLUS"){
                        *(double*)(numLhs->value) += *(double*)(numRhs->value);
                    }
                    else if (*node->value == "MINUS"){
                        *(double*)(numLhs->value) -= *(double*)(numRhs->value);
                    }
                    else if (*node->value == "MULTIPLY"){
                        *(double*)(numLhs->value) *= *(double*)(numRhs->value);
                    }
                    else if (*node->value == "DIVIDE"){
                        if (*(double*)(numRhs->value) == 0){
                            delete lhs; delete rhs;
                            return new ErrorObject(ErrorType::DIV_BY_ZERO);
                        }
                        else
                            *(double*)(numLhs->value) /= *(double*)(numRhs->value);
                    }
                    else if (*node->value == "POWER"){
                        *(double*)(numLhs->value) = pow(*(double*)(numLhs->value), *(double*)(numRhs->value));
                    }
                    else {
                        cerr<<"Unknown operator"<<endl;
                        delete lhs; delete rhs;
                        return NULL;
                    }
                    delete rhs;
                    return lhs;
                }
            }
        }

        case BOOLEAN_OPERATOR:
        {
            if (*node->value == "NOT"){
                Object* exp = executeAST((*node->children)[0], sTable, return_obj);
                
                if (exp->otype == ObjectType::ERROR){
                    return exp;
                }
                else if (exp->otype != ObjectType::BOOL){
                    delete exp;
                    return new ErrorObject(ErrorType::BOOL_EXPECTED);
                }
                else{
                    BoolObject* boolExp = dynamic_cast<BoolObject*>(exp);
                    *(bool*)(boolExp->value) = !(*(bool*)(boolExp->value));
                    return exp;
                }
            }
            else{
                Object* lhs = executeAST((*node->children)[0], sTable, return_obj);
                Object* rhs = executeAST((*node->children)[1], sTable, return_obj);

                if (lhs->otype == ObjectType::ERROR){
                    delete rhs;
                    return lhs;
                }
                else if (rhs->otype == ObjectType::ERROR){
                    delete lhs;
                    return rhs;
                }
                else if (lhs->otype != ObjectType::BOOL || rhs->otype != ObjectType::BOOL){
                    delete lhs; delete rhs;
                    return new ErrorObject(ErrorType::BOOL_EXPECTED);
                }
                else{
                    BoolObject* boolLhs = dynamic_cast<BoolObject*>(lhs);
                    BoolObject* boolRhs = dynamic_cast<BoolObject*>(rhs);

                    if (*node->value == "AND"){
                        *(bool*)(boolLhs->value) = (*(bool*)(boolLhs->value) && *(bool*)(boolRhs->value));
                    }
                    else if (*node->value == "OR"){
                        *(bool*)(boolLhs->value) = (*(bool*)(boolLhs->value) || *(bool*)(boolRhs->value));
                    }
                    else {
                        cerr<<"Unknown operator"<<endl;
                        delete lhs; delete rhs;
                        return NULL;
                    }
                    delete rhs;
                    return lhs;
                }
            }
        }

        case COMPARISION_OPERATOR:
        {
            Object* lhs = executeAST((*node->children)[0], sTable, return_obj);
            Object* rhs = executeAST((*node->children)[1], sTable, return_obj);

            if (lhs->otype == ObjectType::ERROR){
                delete rhs;
                return lhs;
            }
            else if (rhs->otype == ObjectType::ERROR){
                delete lhs;
                return rhs;
            }
            else if (lhs->otype != ObjectType::NUMBER || rhs->otype != ObjectType::NUMBER){
                return new ErrorObject(ErrorType::NUMBER_EXPECTED);
            }
            else{
                NumberObject* numLhs = dynamic_cast<NumberObject*>(lhs);
                NumberObject* numRhs = dynamic_cast<NumberObject*>(rhs);

                if (*node->value == "EQ"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) == *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else if (*node->value == "NEQ"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) != *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else if (*node->value == "LT"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) < *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else if (*node->value == "GT"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) > *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else if (*node->value == "LTEQ"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) <= *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else if (*node->value == "GTEQ"){
                    BoolObject* boolobj = new BoolObject(*(double*)(numLhs->value) >= *(double*)(numRhs->value));
                    delete lhs; delete rhs;
                    return boolobj;
                }
                else {
                    cerr<<"Unknown operator"<<endl;
                    delete lhs; delete rhs;
                    return NULL;
                }
            }
        }

        case TERNARY_OPERATOR:
        {
            Object* condition = executeAST((*node->children)[0], sTable, return_obj);

            if (condition->otype == ObjectType::ERROR){
                return condition;
            }
            else if (condition->otype != ObjectType::BOOL){
                delete condition;
                return new ErrorObject(ErrorType::BOOL_EXPECTED);
            }
            else{
                BoolObject* boolCondition = dynamic_cast<BoolObject*>(condition);
                Object* result = NULL;

                if (*(bool*)(boolCondition->value)){
                    result = executeAST((*node->children)[1], sTable, return_obj);
                }
                else{
                    result = executeAST((*node->children)[2], sTable, return_obj);
                }
                delete condition;
                return result;
            }
        }

        case DECLARATION_ASSIGNMENT:
        {
            Object* result = executeAST((*node->children)[1], sTable, return_obj);
            if (result->otype == ObjectType::ERROR){
                cerr<<result->str()<<endl;
                delete result;
                return NULL;
            }
            sTable->insertNew(*(*node->children)[0]->value, result);
            return NULL;
        }
        case ASSIGNMENT:
        {
            Object* result = executeAST((*node->children)[1], sTable, return_obj);
            if (result->otype == ObjectType::ERROR){
                cerr<<result->str()<<endl;
                delete result;
                return NULL;
            }
            sTable->checkAndUpdateVal(*(*node->children)[0]->value, result);
            return NULL;
        }
        case EMPTY:
            return NULL;

        case COMPOUND_STATEMENT:
        {
            Scope* newScope = new Scope(sTable);
            for (ASTNode* child : *node->children){
                executeAST(child, newScope, return_obj);
            }
            delete newScope;
            return NULL;
        }

        case PRINT_STATEMENT:
        {
            Object* result = executeAST((*node->children)[0], sTable, return_obj);
            cout<<result->str()<<endl;
            delete result;
            return NULL;
        }

        case FUNCTION_DEFINITION:
        {   
            FunctionObject* func = new FunctionObject();

            for (auto param : *(*node->children)[0]->children) {
                func->params.push_back(*param->value);
            }

            func->funcBody = (*node->children)[1]->children;
            return func;
        }

        case RETURN_STATEMENT:
            return_obj = executeAST((*node->children)[0], sTable, return_obj);
            if (return_obj->otype == ObjectType::ERROR){
                cerr<<return_obj->str()<<endl;
                return_obj=NULL;
            }
            return NULL;

        case FUNCTION_CALL:
        {
            Object* obj = sTable->checkAndReturnClone(*(*node->children)[0]->value);
            // Is a reference to the func in the symbol table and not a deep copy
            if (obj->otype != ObjectType::FUNCTION){
                delete obj;
                return new ErrorObject(ErrorType::ID_NOT_FUNC);
            }
            FunctionObject* func = dynamic_cast<FunctionObject*>(obj);

            vector<Object*> args_call;
            for (ASTNode* arg : *(*node->children)[1]->children){
                args_call.push_back(executeAST(arg, sTable, return_obj));
            }

            if (args_call.size() != func->params.size()){
                for (Object* arg : args_call){
                    delete arg;
                }
                delete obj;
                return new ErrorObject(ErrorType::WRONG_ARG_COUNT);
            }
            
            Scope* funcScope = new Scope(sTable);

            for (auto it = func->closure->begin(); it != func->closure->end(); it++){
                funcScope->insertNew(it->first, it->second->clone());
            }

            for (int i = 0; i < args_call.size(); i++){
                funcScope->insertNew(func->params[i], args_call[i]);
            }

            for (ASTNode* stmt : *(func->funcBody)){
                executeAST(stmt, funcScope, return_obj);
            }

            if (return_obj != NULL){
                Object* return_obj_store = return_obj;
                return_obj = NULL;
                
                if (return_obj_store->otype == ObjectType::FUNCTION){
                    FunctionObject* ret_func = dynamic_cast<FunctionObject*>(return_obj_store);                    
                    for (auto it = funcScope->symbolTable.begin(); it != funcScope->symbolTable.end(); it++) {
                        (*ret_func->closure)[it->first] = it->second->clone();
                    }
                }
                delete funcScope;
                return return_obj_store;
            }
            else {
                cerr<<"No return statement found"<<endl;
                return NULL;
            }
        }

        default:
            cerr << "Unknown Type" << endl;
            return NULL;
    }
}