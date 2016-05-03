#ifndef ALLEN_TYPES_H
#define ALLEN_TYPES_H
#include <memory>
#include <map>
namespace AST 
{

enum Type
{
    TERROR = -1, TINT, TREAL, TBOOL
};

class ExprType;

typedef std::unique_ptr<ExprType> type_ptr;

class ExprType 
{
public:
    bool compatible(const ExprType * other) const;
    static type_ptr makeType(Type typeCode);
    virtual Type getTypeCode() const = 0;
    virtual type_ptr getNewType(const ExprType* other) const;
protected:
    static const std::map<Type, std::map<Type, Type>>& getTypeMap();
private:
    static std::map<Type, std::map<Type, Type>> typeMap;
};

class TypeInteger: public ExprType 
{
public:
    virtual Type getTypeCode() const;
};

class TypeDouble: public ExprType
{
public:
    virtual Type getTypeCode() const;
};

class TypeBool: public ExprType 
{
public:
    virtual Type getTypeCode() const;
};

}

#endif
