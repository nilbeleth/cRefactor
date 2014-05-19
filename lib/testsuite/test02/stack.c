#include<stdio.h>

int my_sum1(int a)
{
	int x = rand();
	int y = rand();
	int z = x - y;
	return (x + y) * (a + z);
}


int my_sum2(int a)
{
	int x = rand();
	int y = rand();
	int z = x + y - 3;
	return 4 * z + a;
}


int main(int argc, char *argv[]) {
	int a = rand();
	int b = rand() + 2;
	int c = 0;
	int d = 0;
	if (a > b) {
		printf("hello");
		c = my_sum1(a);
	}
	else {
		printf("greetings");
		c = my_sum2(b);
	}
	return a-b-(c+d);
}
