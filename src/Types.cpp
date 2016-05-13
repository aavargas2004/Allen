#include "Types.h"
#include <memory>
#include "Exceptions.h"
#include <map>
#include <string>
using std::map;
using std::string;
using namespace AST;

std::map<Type, std::map<Type, Type>> ExprType::typeMap = std::map<Type, std::map<Type, Type>>();

ExprType::~ExprType() {

}

string TypeInteger::getTypeName() const {
	return string("Integer");
}

string TypeDouble::getTypeName() const {
	return string("Real");
}

string TypeBool::getTypeName() const {
	return string("Bool");
}

type_ptr ExprType::makeType(Type typeCode) 
{
    type_ptr ret;    
    switch(typeCode) 
    {
        case TINT : 
        {
            ret = type_ptr(new TypeInteger());
            break;
        }
        case TREAL :
        {
            ret = type_ptr(new TypeDouble());
            break;
        }
        case TBOOL :
        {
            ret = type_ptr(new TypeBool());
            break;
        }
        default :
        {
            throw InvalidType(typeCode);
        }
    }
    return ret;
}


const map<Type, map<Type, Type>>& ExprType::getTypeMap()
{
    struct init
    {
        init()
        {
            typeMap[TINT][TREAL] = TREAL;
            typeMap[TREAL][TINT] = TREAL; 
        }
    };
    static init initialize;
    return typeMap;
}


bool ExprType::compatible(const ExprType * other) const
{
    auto map = ExprType::getTypeMap();
    return map[getTypeCode()].find(other->getTypeCode()) != map[getTypeCode()].end();
}

Type TypeInteger::getTypeCode() const
{
    return TINT;
}
 
type_ptr ExprType::getNewType(const ExprType* other) const
{
    if(getTypeCode() == other->getTypeCode())
    {
        return ExprType::makeType(getTypeCode());
    }
    if(!compatible(other))
    {
        throw IncompatibleTypeException(getTypeCode(), other->getTypeCode());
    }
    auto mapVal = ExprType::getTypeMap();
    Type tp = mapVal[this->getTypeCode()][other->getTypeCode()];
    return ExprType::makeType(tp);
}

Type TypeDouble::getTypeCode() const
{
    return TREAL;
}

Type TypeBool::getTypeCode() const
{
    return TBOOL;
}

