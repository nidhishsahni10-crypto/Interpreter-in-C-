#include "pic.hh"
#include "objects.cpp"

struct Scope{
    unordered_map<string, Object*> symbolTable;
    Scope* parent = NULL;
    
    Scope(Scope* parent = NULL){
        this->parent = parent;
    }
    
    ~Scope(){
        for (auto it = symbolTable.begin(); it != symbolTable.end(); ) {
            delete it->second;
            it = symbolTable.erase(it);
        }
    }

    void insertNew(string key, Object* value){
        // Check if key is already present in the current scope
        if(symbolTable.find(key) != symbolTable.end()){
            ErrorObject* obj =  new ErrorObject(ErrorType::ID_REDECLARED, key);
            cerr<<obj->str()<<endl;
            return;
        }
        symbolTable[key] = value;
    }

    Object* checkAndReturnClone(string key){
        Scope* scope = this;

        // Check if key is present in the current scope or any of its parent scopes
        while (scope != NULL){
            if(scope->symbolTable.find(key) != scope->symbolTable.end()){
                return scope->symbolTable[key]->clone();
            }
            scope = scope->parent;
        }
        return new ErrorObject(ErrorType::ID_UNDECLARED, key);
    }

    void checkAndUpdateVal(string key, Object* value){
        Scope* scope = this;

        // Check if key is present in the current scope or any of its parent scopes
        while (scope != NULL){
            if(scope->symbolTable.find(key) != scope->symbolTable.end()){
                scope->symbolTable[key] = value;
                return;
            }
            scope = scope->parent;
        }
        if (scope == NULL){
            ErrorObject* obj = new ErrorObject(ErrorType::ID_UNDECLARED, key);
            cerr<<obj->str()<<endl;
            return;
        }
    }
};
