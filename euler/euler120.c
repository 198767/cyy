//(a-1)^n=(-1)^n+na*(-1)^(n-1) mod a^2
//(a+1)^n=(1)^n+na*(1)^(n-1)  mod a^2
//当n为偶数 sum=2 else sum=2na  n为奇数  n/a=1 3  a-1
#include<stdio.h>
int rmax(int a);
int main()
{
	int max=0,i;
	for(i=3;i<=1000;i++)
		max+=rmax(i);
	printf("%d\n",max);
	return 0;
}
int rmax(int a)
{
	int i;
	int max=2;
	int temp=0;
	int d;
	if(a%2==0)
		d=2;
	else
		d=1;
	for(i=1;i<=a-1;i+=d)
	{
		temp=2*a*i;
		temp=temp%(a*a);
		if(temp>max)
			max=temp;
	}
	return max;

}
