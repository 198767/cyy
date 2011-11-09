#include<stdio.h>
#include "add.h"

int main(int argc,char** argv)
{
	char* res;
	if(argc != 5)
	{
		printf("Usage: ./a.out <number1> <base of number1> <number2> <base of number2>\n");
		return -1;
	}

	res=add_nums(argv[1],argv[2],argv[3],argv[4]);
	if(!res) //error
		return -1;
	printf("result:%s\n",res);
	free(res);
	return 0;
}
