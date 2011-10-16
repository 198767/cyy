#include<stdio.h>
#include "ln.h"
int main()
{
	lN a=str2ln(NULL,"2");
	if(!a)
	{
		printf("创建失败");
		return -1;
	}
	a=ln_exp_int(a,1000,firstln);
	printf("%d",ln_digitsum(a));
	return 0;
}
