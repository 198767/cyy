#include<stdio.h>
#include "ln.h"
int main(int argc, char** argv)
{
	lN a,b,c;
	a=str2ln(NULL,"0");
	b=str2ln(NULL,"39");
	c=ln_divide_ln(a,b);
	puts(ln2str(c));
	return 0;
}
