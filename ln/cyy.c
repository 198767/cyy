#include<stdio.h>
#include<limits.h>
#include "ln_helper.h"
#include "ln_arithmetic.h"
int main(int argc, char** argv)
{

	ln a=ln_init_str("99999.999933333");
	ln b=ln_init_str("3");
	ln c=ln_divide(a,b,5);
	puts(ln2str(c));

	return 0;
}
