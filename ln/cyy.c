#include<stdio.h>
#include "ln_helper.h"
int main(int argc, char** argv)
{
	ln a;
	a=ln_init(0);
	ln_info(a);

	printf("%d",ln_digitnum(a));

	return 0;
}
