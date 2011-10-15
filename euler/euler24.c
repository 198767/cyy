#include<stdio.h>
int main(int argc,char** argv)
{
#define N 10
	char a[N];
	int b[N];
	int i,j,k,l;
	int n=atoi(argv[1]);
	a[0]='0';
	b[0]=1;
	for(i=2;i<=N;i++)
	{
		b[i-1]=i*b[i-2];
		a[i-1]='0'+i-1;
	}
	
	k=N;

	for(i=0;i<k;i++)
	{
		if(n>=b[k-2]*i+1 && n<=b[k-2]*(i+1))
		{
			putchar(a[i]);
			for(l=i+1;l<k;l++)
				a[l-1]=a[l];
			n-=b[k-2]*i;
			k--;
			if(k==1)
			{
				putchar(a[0]);
				return 0;
			}
			i=-1;
		}
	}

}
