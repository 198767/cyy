#include<stdio.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("1.200");
	ln b=ln_init_str("-100");
	ln_multiply_int(a,14,firstln);
	puts(ln2str(a));

	return 0;
}
