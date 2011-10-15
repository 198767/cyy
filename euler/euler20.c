#include "largenum.h"
#include <stdio.h>
#include <limits.h>
int main()
{
	int i,sum;
	largeNum num=init_largenum(1);
	for(i=2;i<=100;i++)
	{
		largenum_multiply_int(num,i,firstnumber);
	}
	show_largenum(num);
	printf("\n");
	sum=get_digitsum(num);
	printf("%d",sum);
	free_largenum(num);
	return 0;
}
