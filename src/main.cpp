#include "AST.h"
#include "Types.h"
#include <iostream>
using AST::ExprType;
using AST::Type;
using std::cout;
using std::cerr;
using std::endl;


int main() {
    auto type_ptrI = ExprType::makeType(Type::TERROR);
    auto type_ptrD = ExprType::makeType(Type::TERROR);
    auto newType = type_ptrI->getNewType(type_ptrD.get());
    switch(newType->getTypeCode())
    {
        case Type::TINT : {
            cout << "Resultado : Int " << endl;
            break;
        }
        case Type::TREAL : {
            cout << "Resultado : Real" << endl;
            break; 
        }
        case Type::TBOOL : {
            cout << "Resultado : Bool" << endl;
            break;
        }
        case Type::TERROR : {
            cerr << "err" << endl;
            break;
        }
        default : {
            cerr << "err" << endl;
            break;
        }
    }
}
