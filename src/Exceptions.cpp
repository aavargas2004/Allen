#include "Exceptions.h"
#include "Types/Types.h"

using namespace AST;

IncompatibleTypeException::IncompatibleTypeException(Type tl, Type tr) : leftType(tl), rightType(tr)
{
}

InvalidType::InvalidType(Type typeCode) : type(typeCode)
{
}
