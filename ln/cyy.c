#include<stdio.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("3.33333");
	ln_divide_int(a,3,2,trunc_res,firstln);
	puts(ln2str(a));

	return 0;
}
