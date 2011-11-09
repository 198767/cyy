/*
 *	该文件里面包含ln的构造及释放的相关函数
 *	
 */
#include "ln.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//创建双向循环链表 这个函数不应该对外使用
lN creat_ln(int size) 
{
	int i;
	Digit p;
	lN n=(lN)malloc(sizeof(struct ln));
	assert(n);
	n->lowestdigit=NULL;
	n->zero=0;
	for(i=0;i<size;i++)
	{
		p=(Digit)malloc(sizeof(struct digit));
		assert(p);
		p->digit=0;
		if(!n->lowestdigit)
		{
			n->lowestdigit=p;
			p->hd=p;
			p->ld=p;
		}
		else
		{
			p->hd=n->lowestdigit;
			p->ld=n->lowestdigit->ld;
			n->lowestdigit->ld->hd=p;
			n->lowestdigit->ld=p;
			n->lowestdigit=p;
		}
	}
	n->highestdigit=n->lowestdigit;
	p=n->lowestdigit;
	return n;
}


//用字符串构造ln str格式为 (+-)?\d+(.\d+)?
lN str2ln(lN n,const char* str)
{
	int i;
	int point=0; //point 小數点数量 验证格式用
	const char *sp,*sp2; //sp 遍历完后指向最后一个数字 sp2 小数点所在位置
	Digit p;

	//str格式验证
	assert(*str=='+' || *str=='-' || (ISDIGIT(*str))); //以合法字符开头
	sp=str+1;
	while(*sp)
	{
		assert(ISDIGIT(*sp) || *sp=='.');
		if(*sp=='.')
		{
			point++;	
			sp2=sp;	
		}
		sp++;
	}
	sp--;
	assert(point<=1);
	assert(*sp !='.');
	
	//创建一个ln
	if(!n)
		n=creat_ln(INIT_SIZE);
	n->sign=(*str=='-')?0:1; //设置符号
	
	//清除前置的0
	if(*str=='+' || *str=='-')
		str++;
	while(str != sp && *str=='0')
		str++;
	if(*str=='.')
		str--;
	//清除后置的0
	if(point)	
	{
		
		while(*sp=='0')
			sp--;
		if(*sp=='.') //小数点后全是0 其实是整数
		{
			sp--;
			point=0;
		}
	}

	//处理指數部分
	n->zero=0;
	if(point) //小数
	{
		//把小数往前移动
		while(sp2<sp)  
		{
			sp2++;
			n->zero--;
		}
	}
	else if(*sp=='0' && sp !=str) //以0结尾的整数 (肯定不是0本身)
	{
		//抽出结尾的0
		while(*sp=='0')  //继续遍历
		{
			sp--;
			n->zero++;
		}
	}
	
	i=1;
	p=n->lowestdigit;
	p->digit=0;
	
	if(point)
	{
		if(*str=='0' && *(str+1)=='.')
			str+=2;
		while(*str =='0')
			str++;
	}
	while(sp>=str) //从最低位向上构造digit部分
	{
		if(*sp!='.')
		{
			p->digit+=(*sp-'0')*i;
			sp--;
			if(i==UNIT/10)
			{
				if(sp>=str)
				{
					i=1;
					if(p->hd==n->lowestdigit && sp>=str)//节点已经用完
						ln_addsize(n,(sp-str+1)/DIGIT_NUM+1);
					p=p->hd;
					p->digit=0;
				}
				else
					break;
			}
			else 
				i*=10;
		}
		else
			sp--;
	}
	n->highestdigit=p;
	return n;
}


//设置ln的值 传入指针为NULL则构造一个
lN ln_setval(lN n,int m)
{
	Digit p=NULL;
	if(!n)
		n=creat_ln(INIT_SIZE);
	if(m<0)
	{
		n->sign=0;
		m=-m;
	}
	else
		n->sign=1;
	n->zero=0;
	p=n->lowestdigit;
	while(1)
	{
		p->digit=m%UNIT;
		m=(m-p->digit)/UNIT;
		if(m !=0) //往前移动一步
		{
			if(p->hd==n->lowestdigit)//节点已经用完
				ln_addsize(n,1);
			p=p->hd;
		}
		else
		{
			n->highestdigit=p;
			break;
		}
	}
	return n;
}
//用指定个数来增加ln的节点
lN ln_addsize(lN n,int size)
{
	int i;
	Digit p;
	if(size<=0) //无须增加
		return n;
	for(i=0;i<size;i++)
	{
		p=(Digit)malloc(sizeof(struct digit));
		assert(p);
		p->hd=n->lowestdigit;
		p->ld=n->lowestdigit->ld;
		n->lowestdigit->ld->hd=p;
		n->lowestdigit->ld=p;
		p->digit=0;
	}
	return n;
}

/*计算digit部分的节点数
 * n 要计算的ln
 * Digit 截止节点
 */
int ln_get_nodenum(lN n,Digit q)
{
	int i=1;
	Digit p=n->lowestdigit;
	while(p!=q)
	{
		p=p->hd;
		i++;
	}
	return i;
}


int ln_get_digitnum(lN n,Digit q)
{
	int i;
	Digit p=n->lowestdigit;
	if(p->digit<10)
		i=1;
	else if(p->digit<100)
		i=2;
	else if(p->digit<1000)
		i=3;
	else
		i=4;
	if(p!=q)
	{
		while(p!=q)
		{
			p=p->hd;
			i+=4;
		}
		i-=4;
		p=q;
		if(p->digit<10)
			i+=1;
		else if(p->digit<100)
			i+=2;
		else if(p->digit<1000)
			i+=3;
		else
			i+=4;
	}
	return i;
}








//把ln转换为字符串
char* ln2str(lN n)
{
	Digit p;
	char *str,*head,*tail,*tail2;
	size_t size=0,digitnum=0;
	int i,zero;
	if(n->zero>=0)
	{
		p=n->highestdigit;
		while(p->digit==0 && p != n->lowestdigit)
			p=p->ld;
		i=ln_get_nodenum(n,p);

		str=(char*)malloc(i*DIGIT_NUM+n->zero+10);
		assert(str);
		if(p->digit==0)
		{
			str[0]='0';
			str[1]='\0';
			return str;
		}

		tail=str;
		//打印符号
		if(n->sign==0)
			*tail++='-';
		i=UNIT/10;
		while(i)
		{
			while(p->digit<i)
				i/=10;
			*tail++=(p->digit/i)%10+'0';
			i/=10;
		}

		while(p!=n->lowestdigit)
		{
			p=p->ld;
			if(p->digit==0)
			{
				for(i=0;i<DIGIT_NUM;i++)
					*tail++='0';
			}
			else
			{
				i=UNIT/10;
				while(i)
				{
					*tail++=(p->digit/i)%10+'0';
					i/=10;
				}
			}
		}
		for(i=0;i<n->zero;i++)
			*tail++='0';
		*tail++='\0';
		return str;
	}
	else
	{
		//先计算需要的空间
		zero=-n->zero;
		p=n->highestdigit;
		i=UNIT/10;
		while(i)
		{
			while(p->digit<i)
				i/=10;
			digitnum++;
			i/=10;
		}
		i=ln_get_nodenum(n,n->highestdigit);
		digitnum+=(i-1)*DIGIT_NUM;
		if(zero<digitnum)
			size=digitnum+1;
		else 
			size=2+zero;
		if(n->sign==0)
			size++;

		str=(char*)malloc(size+1);
		assert(str);
		tail=str+size;
		tail2=tail-1;
		head=str;
		*tail--='\0';
		if(n->sign==0)
		{
			*head++='-';
			size--;
		}
		p=n->lowestdigit;
		i=1;
		if(zero<digitnum) //小数点在中间
		{
			size-=zero+1;
			while(zero) //输出小数点后半部分
			{
				*tail--=(p->digit/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hd;
				}
				zero--;
			}
			*tail--='.';
			while(size) //输出小数点前半部分
			{
				*tail--=(p->digit/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hd;
				}
				size--;
			}
		}
		else //小数点在外面
		{
			*head++='0';
			*head++='.';
			zero-=digitnum;
			while(digitnum)
			{
				*tail--=(p->digit/i)%10+'0';
				i*=10;
				if(i==UNIT)
				{
					i=1;
					p=p->hd;
				}
				digitnum--;
			}
			while(zero)
			{
				*head++='0';
				zero--;
			}
		}
		//去除结尾0
		while(*tail2=='0')
			tail2--;
		if(*tail2=='.')
			*tail2='\0';
		else
			*(tail2+1)='\0';
		
		return str;
	}
}

//打印ln
void print_ln(lN n)
{
	char *p=ln2str(n);
	puts(p);
	free(p);
}

//释放ln
lN free_ln(lN n)
{
	Digit p=n->lowestdigit;
	Digit q=p;
	while(p!=q->ld)
	{
		p=p->hd;
		free(p->ld);
	}
	free(p);
	n=NULL;
	return n;
}


/* 复制ln b到ln a
 * a---ln指针
 * b---ln
 * 返回a
 */
lN copy_lns(lN* a,lN b)
{
	int size_b=ln_get_nodenum(b,b->highestdigit);
	int size_a;
	lN c=NULL;
	if(!a)
		c=creat_ln(size_b);
	else
	{
		c=*a;
		size_a=ln_get_nodenum(c,c->highestdigit);
		ln_addsize(c,size_b-size_a); //把a的长度设置成和b一样
	}
	assert(c);
	Digit i=c->lowestdigit;
	Digit j=b->lowestdigit;
	c->sign=b->sign;
	c->zero=b->zero;
	while(size_b--)
	{
		i->digit=j->digit;
		i=i->hd;
		j=j->hd;
	}
	c->highestdigit=i->ld;
	return c;
}

//截取n的digit部分m个节点,如果m过大则n不变
void ln_trunc(lN n,int m)
{
	assert(m>0);
	int i=1;
	Digit p=n->lowestdigit;
	while(i!=m&&p!=n->highestdigit)
	{
		p=p->hd;
		i++;
	}
	n->highestdigit=p;
}
//减少指数部分,多余的0放到整数部分
lN ln_setzero(lN n,int zero)
{
	assert(n);
	assert(n->zero>=zero);
	int a,b;
	int i,j;
	Digit d=NULL;
	a=n->zero-zero;

	n->zero=zero;
	if(a==0)
		return n;
	b=a%DIGIT_NUM;
	j=1;
	for(i=0;i<b;i++)
		j*=10;
	if(j>1)
	{
		ln_multiply_num(n,j,firstln);
	}
	while(a>=DIGIT_NUM)
	{
		d=(Digit)malloc(sizeof(struct digit));
		assert(d);
		d->digit=0;
		d->hd=n->lowestdigit;
		d->ld=n->lowestdigit->ld;
		n->lowestdigit->ld->hd=d;
		n->lowestdigit->ld=d;
		n->lowestdigit=d;
		a-=DIGIT_NUM;
	}
	return n;
}

/* 查看ln内存
 */
void ln_info(lN n)
{
	Digit p=n->lowestdigit;
	printf("sign=%d \n",n->sign);
	printf("zero=%d \n",n->zero);
	
	while(p!=n->highestdigit)
	{
		printf("addr=%d digit=%d \n",(int)p,p->digit);
		p=p->hd;
	}
	printf("addr=%d digit=%d \n",(int)p,p->digit);
}

