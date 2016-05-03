#ifndef AST_EXCP_H
#define AST_EXCP_H
#include <exception>
namespace AST
{

class IncompatibleTypeException : public Exception
{
public:
    IncompatibleTypeException(Type tl, Type tr);
};

class InvalidType : public Exception
{
public:
    InvalidType(Type typeCode);
};

}

#endif
