#include <iostream>
#include "test.h"




void Class::method()
{
    std::cout << "Lalala\n";
}


int do_something(
        const Enum type)
{
    if(type == enum1)
        return 1;
    else if(type == enum2)
        return 2;
    else
        return 3;
}


char do_something_else(
	const Struct& record)
{
    Union u;
    std::cout << u.u1;
    std::cout << record.s1;
    return record.s2;
}



void do_math(			// ValueDecl::DeclaratorDecl::FunctionDecl
	int* x) 		// ValueDecl::DeclaratorDecl::VarDecl::ParmVarDecl
{
    *x += 5;			// 
}


int main(			// ValueDecl::DeclaratorDecl::FunctionDecl
	int argc,		// ValueDecl::DeclaratorDecl::VarDecl::ParmVarDecl
	char** argv)		// ValueDecl::DeclaratorDecl::VarDecl::ParmVarDecl
{
    int result = -1, 		// ValueDecl::DeclaratorDecl::VarDecl
        val = 4;		// ValueDecl::DeclaratorDecl::VarDecl

    do_math(			//
            &val);		//
    ClassBase* c = 		// ValueDecl::DeclaratorDecl::VarDecl
        new Class();		//

    c->method();

    return result;		//
}
