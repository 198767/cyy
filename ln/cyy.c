#include<stdio.h>
#include<limits.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("2");
	ln_info(a);
		ln_divide_int(a,200000,200,round_res,firstln);
	puts(ln2str(a));

	return 0;
}
