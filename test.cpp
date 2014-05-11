#include <iostream>
#include "test.h"




bool ClassBase::method2()
{
    return true;
}

void Class::method()
{
    std::cout << "Lalala\n";
    lala::global = 1;
    lala::alal::character = 'r';
    tStrings retazce;
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
    Enum e;
    e = enum1;
    *x += 5;			// 
}


int main(			// ValueDecl::DeclaratorDecl::FunctionDecl
	int argc,		// ValueDecl::DeclaratorDecl::VarDecl::ParmVarDecl
	char** argv)		// ValueDecl::DeclaratorDecl::VarDecl::ParmVarDecl
{
    int result = -1, 		// ValueDecl::DeclaratorDecl::VarDecl
        val = 4;		// ValueDecl::DeclaratorDecl::VarDecl

anchor1:

    do_math(			//
            &val);		//
    ClassBase* c = 		// ValueDecl::DeclaratorDecl::VarDecl
        new Class();		//

    int pi = ClassBase::pi;
    c->method();

    c->~ClassBase();
    goto anchor1;

    return result;		//
}
