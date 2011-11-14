#include<stdio.h>
#include<limits.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init(INT_MIN);
	ln b=ln_init(INT_MAX);
	ln_output(a);
	ln_output(b);
	//ln a=ln_init_str("919191919190");
	ln c=ln_divide(a,b,1000,trunc_res,newln);
	puts(ln2str(c));

	return 0;
}
