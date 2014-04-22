#ifndef TEST_H
#define TEST_H
#include <string>
#include <vector>


#define MAX(X,Y) ((X) < (Y) ? (X) : (Y))

int global = 1;

enum Enum
{
    enum1,
    enum2,
    enum3,
    enum4
};


typedef std::vector<std::string> tStrings;


class Class
{
    public:
    	Class() {}
	virtual ~Class() {}

	void method() {}
    protected:
    private:
        std::string attr1;
	int attr2;
};

struct Struct
{
    int s1;
    char s2;
};

union Union
{
    char u1;
    short int u2;
};


namespace lala
{
int global = 10;
};

#endif
