#ifndef TEST_H
#define TEST_H
#include <string>
#include <vector>


#define MAX(X,Y) ((X) < (Y) ? (X) : (Y))

int global = 1; 				// ValueDecl::DeclaratorDecl::VarDecl

enum Enum					// TypeDecl::TagDecl::EnumDecl
{
    enum1,					// ValueDecl::EnumConstantDecl
    enum2					// ValueDecl::EnumConstantDecl
};


typedef std::vector<std::string> tStrings;	// NamedDecl::TypeDecl::TypedefNameDecl::TypedefDecl


class ClassBase					// TypeDecl::TagDecl::RecordDecl::CXXRecordDecl
{
    public:
    	ClassBase() {}				// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl::CXXConstructorDecl
	virtual ~ClassBase() {}			// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl::CXXDestructorDecl

	virtual void method() {}		// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl
    protected:
        char attr0;				// ValueDecl::DeclaratorDecl::FieldDecl
    private:
        std::string attr1;			// ValueDecl::DeclaratorDecl::FieldDecl
	int attr2;				// ValueDecl::DeclaratorDecl::FieldDecl
};

class Class 					// TypeDecl::TagDecl::RecordDecl::CXXRecordDecl
	: public ClassBase			// 
{
    public:
        Class() {}				// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl::CXXConstructorDecl
	virtual ~Class() {}			// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl::CXXDestructorDecl

	void method();				// ValueDecl::DeclaratorDecl::FunctionDecl::CXXMethodDecl
    protected:
    private:
        std::string attr1;			// ValueDecl::DeclaratorDecl::FieldDecl
	int attr2;				// ValueDecl::DeclaratorDecl::FieldDecl
};

struct Struct					// TypeDecl::TagDecl::RecordDecl::CXXRecordDecl
{
    int s1;					// ValueDecl::DeclaratorDecl::FieldDecl
    char s2;					// ValueDecl::DeclaratorDecl::FieldDecl
};

union Union					// TypeDecl::TagDecl::RecordDecl::CXXRecordDecl
{
    char u1;					// ValueDecl::DeclaratorDecl::FieldDecl
    short int u2;				// ValueDecl::DeclaratorDecl::FieldDecl
};


namespace lala					// NamespaceDecl
{
int global = 10;				// ValueDecl::DeclaratorDecl::VarDecl
};

#endif
