//n满足 num(p|n^2)>N 且n最小  设n^2=2^a*3^b*5^c ... 则有(a+1)*(b+1)*(c+1)...  >2*N-1 于是归结为找出a,b,c..的一个组合使得n^2最小 即n最小
#include<stdio.h>
#include<stdlib.h>
#define N  1000
#define M  999999
int p[M][3];
int main()
{
	int i,j;
	int res;
	int min=2*N-1;
	for(i=0;i<M;i++)
	{
		p[i][0]=i;
		p[i][1]=1;
		p[i][2]=1;
	}
	for(i=2;i<M;i++)
	{
		if(p[i][1]==1)
		{
			j=2*i;
			while(j<M)
			{
				p[j][1]=0;
				res=1;
				p[j][0]/=i;
				while(p[j][0]%i==0)
				{
					p[j][0]/=i;
					res++;
				}
				p[j][2]*=(2*res+1);
				j+=i;
			}
		}
		else
		{
			if(p[i][2]>min)
			{
				printf("%d",i);
				break;
			}
		}
	}
	return 0;
/*

	int p[20][2];
	int i,j;
	int a,b,c;
	int res1,res2;
	int flag;
	p[0][0]=2;
	p[1][0]=3;
	p[2][0]=5;
	p[3][0]=7;
	p[4][0]=11;
p[5][0]=13;
p[6][0]=17;
p[7][0]=19;
p[8][0]=23;
p[9][0]=29;
p[10][0]=31;
p[11][0]=37;
p[12][0]=41;
p[13][0]=43;
p[14][0]=47;
p[15][0]=53;
p[16][0]=59;
p[17][0]=61;
p[18][0]=67;
p[19][0]=71;

for(i=0;i<20;i++)
	p[i][1]=0;
	
1260=10*126=10*2*63=4*5*7*9=2^4*5^2*7^2*9^2
		5*3*3*5
		25*9
	p[0][1]=(min+1)/2;
	for(i=0;i<20;i++)
	{
		if(i==19 && p[i][1]>1)
		{
			printf("质数不够!");
			exit(1);
		}
		while(1)
		{
			flag=0;
			c=1;
			for(j=0;j<i;j++)
				c*=(2*p[j][1]+1);

			for(j=0;j<p[i][1]-1;j++)
			{
				a=p[i][1]-j-1;
				b=p[i+1][1]+1+j;
				while(c*(2*a+1)*(2*b+1) <=min)
					b++;
				if(a+b>p[i][1]+p[i+1][1])
					continue;

				res1=cyy_exp(p[i][0],j+1);	
				res2=cyy_exp(p[i+1][0],b-p[i+1][1]);	
				if(res1>res2)
				{
					printf("%d %d\n",j+1,b-p[i+1][1]);

					flag=1;
					break;
				}
			}
			if(flag)
			{
				p[i][1]=a;
				p[i+1][1]=b;
			}
			else
			{

				//for(j=0;j<20;j++)
				//	printf("%d %d",p[j][1],p[i+1][1]);
			puts("\n");

				break;
			}
		}

	}

	(2p+1) >199

		p=100

		99 1

		2^100
		201

		(2a+1)(2b+1)

		2N-1
		p+1/2>N
		p>2N-1
		printf("%d",num);
	for(i=0;i<20;i++)
		printf("%d %d\n",p[i][0],p[i][1]);
		*/
	return 0;

}

