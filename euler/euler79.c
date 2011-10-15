#include<stdio.h>
char passcode[151];
int position[50][3];
int main()
{
	int i,j,k;
	int start;
	char* keylog[50]= {"319", "680", "180", "690", "129", "620", "762", "689", "762", "318", "368", "710", "720", "710", "629", "168", "160", "689", "716", "731", "736", "729", "316", "729", "729", "710", "769", "290", "719", "680", "318", "389", "162", "289", "162", "718", "729", "319", "790", "680", "890", "362", "319", "760", "316", "729", "380", "319", "728", "716"};
	char *p;
	//排序
	for(i=0;i<49;i++)
	{
		for(j=49;j>i;j--)
		{
			if(strcmp(keylog[j],keylog[j-1])<0)
			{
				p=keylog[j];
				keylog[j]=keylog[j-1];
				keylog[j-1]=p;
			}
		}
	}
	for(i=0;i<10;i++)
	{
		printf("%s\n",keylog[i]);
		for(j=0;j<3;j++)
		{
			position[i][j]=0;
			if(j==0)
				start=1;
			else
				start=position[i][j-1];
			for(k=start;k<=passcode[0];k++)
			{
				if(passcode[k]==keylog[i][j])
				{
					position[i][j]=k;
					break;
				}
			}
			if(position[i][j]==0)
			{
				passcode[0]++;
				position[i][j]=passcode[0];
				passcode[position[i][j]]=keylog[i][j];
		//	printf("index=%d\n",position[i][j]);
			}
		}
	//	printf("%d\n",passcode[0]);
		printf("%s\n",passcode+1);
	}

	return 0;
}
