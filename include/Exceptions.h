#ifndef AST_EXCP_H
#define AST_EXCP_H
#include <exception>
#include "Types.h"
namespace AST
{

class IncompatibleTypeException : public std::exception
{
public:
    IncompatibleTypeException(Type tl, Type tr);
private:
    Type leftType, rightType;
};

class InvalidType : public std::exception
{
public:
    InvalidType(Type typeCode);
private:
    Type type;
};

}

#endif
