/*
 *	该文件里面包含ln的构造及释放的相关函数
 *	
 */
#include "ln.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//获取ln的十进制位数的和
int ln_digitsum(lN n)
{
	Digit p=n->lowestdigit->ld;
	int sum=0;
	int i;
	int j;
	do
	{
		p=p->hd;
		j=1;
		for(i=0;i<DIGIT_NUM;i++)
		{
			sum+=(p->digit/j)%10;
			j*=10;
		}

	}
	while(p !=n->highestdigit);
	return sum;
}


/*
 * 大数和整数相乘
 */
lN ln_multiply_num(lN i,int b,Res resflag)
{
	int res=0;
	int carry=0;
	lN k,temp;
	if(resflag==newln)
	{
		if(b>=UNIT)
		{

			temp=ln_setval(NULL,b);
			multiply_lns(temp,i,firstln);
			return temp;
		}
		else if(b==0)
			return str2ln(creat_ln(1),"0");
		else if(b==1)
			return copy_lns(NULL,i);

		k=creat_ln(ln_get_nodenum(i,i->highestdigit)+2);
		k->zero=i->zero;
		k->sign=i->sign;

		if(b<0)
		{
			k->sign=!(k->sign);
			b=-b;
		}
		Digit a=i->lowestdigit;
		Digit c=k->lowestdigit;
		while(1)
		{
			res=b*a->digit+carry;
			c->digit=res%UNIT;
			carry=res/UNIT;
			if(a==i->highestdigit)
			{
				if(carry>0)
				{
					c->hd->digit=carry;
					k->highestdigit=c->hd;
				}
				else
				{
					k->highestdigit=c;
				}
				break;
			}
			a=a->hd;
			c=c->hd;
		}
		return k;
	}
	else
	{

		if(b>=UNIT)
		{
			temp=ln_setval(NULL,b);
			multiply_lns(i,temp,firstln);
			free_ln(temp);
			return i;
		}
		else if(b==0)
		{
			ln_setval(i,0);
			return i;
		}
		else if(b==1)
			return i;
		if(b<0)
		{
			i->sign=!(i->sign);
			b=-b;
		}


		Digit a=i->lowestdigit;
		while(1)
		{
			res=b*a->digit+carry;
			a->digit=res%UNIT;
			carry=res/UNIT;
			if(a==i->highestdigit)
			{
				ln_addsize(i,1);
				if(carry>0)
				{
					a->hd->digit=carry;
					i->highestdigit=a->hd;
				}
				else
					i->highestdigit=a;
				break;
			}
			a=a->hd;
		}
		return i;
	}
}


/*
   两个大数相加
   resflag firstln 结果放在m中
   resflag newln 结果另外分配
*/

lN add_lns(lN a,lN b,Res resflag)
{
	int carry=0,zero;
	int a_sign,b_sign;
	lN c=NULL;
	Digit p,q,add_res;
	int cmp_res;

	if(resflag==firstln)
		c=a;
	else
		c=str2ln(creat_ln(1),"0");

	zero=MIN(a->zero,b->zero);
	ln_setzero(a,zero);
	ln_setzero(b,zero);
	c->zero=zero;
	ln_addsize(c,MAX(ln_nodenum(a),ln_nodenum(b))-ln_nodenum(c)+2);

	//加法
	if(a->sign==b->sign)	
	{
		c->sign=a->sign;
		//调整指数部分一致
		add_res=c->lowestdigit;
		p=a->lowestdigit;
		q=b->lowestdigit;
		while(1)
		{
			if(p)
				carry+=p->digit;
			if(q)
				carry+=q->digit;
			if(p==a->highestdigit)
				p=NULL;
			if(q==b->highestdigit)
				q=NULL;
			add_res->digit=carry%UNIT;
			carry=carry/UNIT;
			if(!p&&!q&&!carry)
			{
				c->highestdigit=add_res;
				break;
			}
			if(p)
				p=p->hd;
			if(q)
				q=q->hd;
			add_res=add_res->hd;
		}
		return c;
	}
	else //减法
	{
		a_sign=a->sign;
		a->sign=1;
		b_sign=b->sign;
		b->sign=1;	
		cmp_res=ln_cmp(a,b);
		a->sign=a_sign;
		b->sign=b_sign;
		if(cmp_res==0)
			ln_setval(c,0);
		else
		{
			//确定符号
			if(cmp_res==1)
			{
				p=a->lowestdigit;
				q=b->lowestdigit;
				c->sign=a_sign;
			}
			else	
			{
				p=b->lowestdigit;
				q=a->lowestdigit;
				c->sign=b_sign;
			}

			add_res=c->lowestdigit;
			while(1)
			{
				if(q)
					add_res->digit=p->digit-carry-q->digit;
				else
					add_res->digit=p->digit-carry;
				if(add_res->digit <0)
				{
					carry=1;
					add_res->digit+=UNIT;	
				}
				else
					carry=0;
				if(q==a->highestdigit || q==b->highestdigit)
					q=NULL;
				if(p==a->highestdigit || p==b->highestdigit)
				{
					while(add_res != c->lowestdigit&& add_res->digit==0)
						add_res=add_res->ld;
					c->highestdigit=add_res;
					break;
				}
				p=p->hd;
				if(q)
					q=q->hd;
				add_res=add_res->hd;
			}	
		}		
		return c;
	}
}


lN ln_add_num(lN a,int b,Res resflag)
{
	lN c,d;
	c=ln_setval(NULL,b);
	d=add_lns(a,c,resflag);
	free_ln(c);
	return d;
}


lN minus_lns(lN a,lN b,Res resflag)
{
	lN c,d;
	c=ln_multiply_num(b,-1,newln);
	d=add_lns(a,c,resflag);
	free_ln(c);
	return d;
}





/*
 *用m和大数相乘 返回一个新的大数n
 */
lN multiply_lns(lN i,lN j,Res resflag)
{
	lN k;
	Digit a,b,c,x,y,z;
	int res;
	k=creat_ln(ln_nodenum(i)+ln_nodenum(j));
	if(i->sign==j->sign)
		k->sign=1;
	else
		k->sign=0;
	k->zero=i->zero+j->zero;
	a=i->lowestdigit;
	b=j->lowestdigit;
	c=k->lowestdigit;
	do
	{
		x=a;
		y=b;
		z=c;
		do
		{
			res=(x->digit*y->digit)+z->digit;
			if(res>=UNIT)
			{
				z->digit=res%UNIT;
				z->hd->digit+=res/UNIT;
			}
			else
				z->digit=res;
			y=y->hd;
			z=z->hd;
		}
		while(y != j->highestdigit->hd);
		a=a->hd;
		c=c->hd;
	}
	while(a != i->highestdigit->hd);
	if(z!=k->highestdigit)
		k->highestdigit=z;
	else
		k->highestdigit=z->ld;

	while(k->highestdigit->digit==0 && k->highestdigit !=k->lowestdigit)
		k->highestdigit=k->highestdigit->ld;

	if(resflag==newln)
		return  k;
	else
	{
		copy_lns(&i,k);
		free_ln(k);
		return i;
	}
}

lN ln_exp_int(lN i,int b,Res resflag)
{
	
	lN j=NULL,k=NULL;
	assert(b>=0);
	if(b==0)
	{
		if(resflag==newln)
			return str2ln(NULL,"1");
		else
			return ln_setval(i,1);
	}
	else if(b==1)
	{
		if(resflag==newln)
			return copy_lns(NULL,i);
		else
			return i;
	}
	else if(b==2)
	{
		if(resflag==newln)
			return ln_square(i,newln);
		else
			return ln_square(i,firstln);
	}
	else
	{
		j=copy_lns(NULL,i);
		if(b%2==0)
			k=init_ln(1);
		else
		{
			k=copy_lns(NULL,i);
			b--;	
		}
		while(1)
		{
			ln_square(j,firstln);
			b/=2;
			if(b==1)
				break;
			if(b%2!=0)
			{
				multiply_lns(k,j,firstln);
				b--;
			}
		}
		multiply_lns(k,j,firstln);
		free_ln(j);
		if(resflag==newln)
			return k;
		else
		{
			copy_lns(&i,k);
			free_ln(k);
			return i;
		}
	}
}

int ln_cmp(lN a,lN b)
{
	assert(a&&b);
	int zero_a=1,zero_b=1;
	int minzero;
	int res=0;
	Digit p;
	Digit q;
	
	//验证0	
	p=a->highestdigit;
	while(1)
	{
		if(p->digit !=0)
		{
			zero_a=0;
			break;
		}
		if(p==a->lowestdigit)
			break;
		p=p->ld;
	}
	a->highestdigit=p;
	q=b->highestdigit;
	while(1)
	{
		if(q->digit !=0)
		{
			zero_b=0;
			break;
		}
		if(q==b->lowestdigit)
			break;
		q=q->ld;
	}
	b->highestdigit=q;



	if(zero_a && zero_b)
		return 0;
	if(zero_a)
	{
		if(b->sign==1)
			return -1;
		else
			return 1;
	}
	if(zero_b)
	{
		if(a->sign==1)
			return 1;
		else
			return -1;
	}

	//一正一负
	if(a->sign>b->sign)
		return 1;
	else if(a->sign<b->sign)
		return -1;
	//符号相等
	minzero=MIN(a->zero,b->zero);
	ln_setzero(a,minzero);
	ln_setzero(b,minzero);
	if(a->zero>b->zero)
		res=1;
	else if(a->zero<b->zero)
		res=-1;
	else
	{	
		p=a->lowestdigit;
		q=b->lowestdigit;
		while(1)
		{
			if(p->digit>q->digit)
				res=1;
			else if(p->digit<q->digit)
				res=-1;
			if(p==a->highestdigit)
			{
				if(q!= b->highestdigit)
				{
					res=-1;
				}
				break;
			}
			if(q==b->highestdigit)
			{
				if(p!= a->highestdigit)
					res=1;
				break;
			}
			p=p->hd;
			q=q->hd;
		}
	}
	if(a->sign==0) //负数
		res*=-1;
	return res;
}


int ln_cmp_num(lN a,int b)
{
	int res;
	lN c;
	c=ln_setval(NULL,b);
	res=ln_cmp(a,c);
	free_ln(c);
	return res;
}




/*
 * 大数和整数相除
 */
lN ln_divide_num(lN i,int b,int precision,Divide_mode mode,Res resflag)
{
	int res=0;
	int carry=0;
	int prec=0;
	lN k;
	assert(b!=0);
	assert(precision>=0);
	if(precision<0)
		precision=DIV_PREC;

	if(resflag==newln)
	{
		if(b==1|| b==-1 )
		{
			k= copy_lns(NULL,i);
			return ln_multiply_num(k,b,firstln);
		}
		k=creat_ln(ln_get_nodenum(i,i->highestdigit));
		k->zero=i->zero;
		if(b>0)
			k->sign=i->sign;
		else
		{
			k->sign=!(i->sign);
			b=-b;
		}
		Digit a=i->highestdigit;
		while(a!= i->lowestdigit && a->digit==0)
			a=a->ld;
		i->highestdigit=a;
		k->highestdigit=k->lowestdigit->ld;
		Digit c=k->highestdigit;
		res=a->digit;
		while(1)
		{
			res=res+carry*UNIT;
			c->digit=res/b;
			carry=res%b;
			if(k->zero<0)
			{
				if(prec==0)
				{
					if(c->digit !=0) //开始计算小数点个数
						prec=-k->zero;
				}
				else
					prec+=DIGIT_NUM;
			}
			if(a==i->lowestdigit)
			{
				if(carry==0) //除得尽
				{
					k->lowestdigit=c;
					break;
				}
				else if(prec>=precision) //除不尽，四舍五入保留precision位小数
				{
					if(prec==precision)
					{
						if(mode==round)
						{
							if(a !=i->lowestdigit)
								res=(a->ld->digit+carry*UNIT)/b;
							else
								res=(carry*UNIT)/b;
							if(res>=UNIT/2)
								c->digit++;
						}
					}
					else
					{
						int l,j=1;
						for(l=0;l<prec-precision;l++)
							j*=10;
						res=c->digit%j;
						if(mode==round && res>=5*(j/10))
							c->digit=c->digit-res+j;
						else
							c->digit=c->digit-res;
					}
					k->lowestdigit=c;
					break;
				}
			}
			if(a !=i->lowestdigit)
			{
				a=a->ld;
				res=a->digit;
			}
			else
			{
				res=0;
				k->zero-=DIGIT_NUM;
			}
			if(c==k->lowestdigit)
			{
				ln_addsize(k,5);
				k->lowestdigit=k->highestdigit->hd;
			}
			c=c->ld;
		}
		return k;
	}
	else
	{
		if(b==1|| b==-1 )
		{
			return ln_multiply_num(i,b,firstln);
		}
		if(b<0)
		{
			i->sign=!(i->sign);
			b=-b;
		}
		Digit a=i->highestdigit;
		while(a != i->lowestdigit && a->digit==0)
			a=a->ld;
		Digit c=i->lowestdigit->ld;
		Digit d=i->lowestdigit;
		i->highestdigit=c;
		res=a->digit;
		while(1)
		{
			res=res+carry*UNIT;
			c->digit=res/b;
			carry=res%b;
			if(i->zero<0)
			{
				if(prec==0)
				{
					if(c->digit !=0) //开始计算小数点个数
						prec=-i->zero;
				}
				else
					prec+=DIGIT_NUM;
			}
			if(a==d)
			{
				if(carry==0) //除得尽
				{
					i->lowestdigit=c;
					break;
				}
				else if(prec>=precision) //除不尽，四舍五入保留precision位小数
				{
					if(prec==precision)
					{
						if(mode==round)
						{
							if(a !=d)
								res=(a->ld->digit+carry*UNIT)/b;
							else
								res=(carry*UNIT)/b;
							if(res>=UNIT/2)
								c->digit++;
						}
					}
					else
					{
						int l,j=1;
						for(l=0;l<prec-precision;l++)
							j*=10;
						res=c->digit%j;
						if(mode==round && res>=5*(j/10))
							c->digit=c->digit-res+j;
						else
							c->digit=c->digit-res;
					}
					i->lowestdigit=c;
					break;
				}
			}
			if(a!=d)
			{
				a=a->ld;
				res=a->digit;
			}
			else
			{
				res=0;
				i->zero-=DIGIT_NUM;
			}
			if(c==i->lowestdigit)
			{
				ln_addsize(i,5);
				i->lowestdigit=i->highestdigit->hd;
			}
			c=c->ld;
		}
		return i;
	}
}


int ln_divideable_num(const lN i,int b)
{
	Digit a;
	int res=0;
	int carry=0;
	lN j;
	assert(b!=0);
	assert(i->zero==0);
	if(b==1|| b==-1 )
		return 1;
	j=copy_lns(NULL,i);
	a=j->highestdigit;
	res=a->digit;
	while(1)
	{
		res=res+carry*UNIT;
		carry=res%b;
		if(a==j->lowestdigit)
		{
			free_ln(j);
			if(carry==0) //除得尽
				return 1;
			else
				return 0;
		}
		a=a->ld;
		res=a->digit;
	}
	return 0;
}





//求平方

lN ln_square(lN a,Res resflag)
{
	lN b;
	Digit i,j,k,l;
	int res;
	if(a->lowestdigit==a->highestdigit)
	{
		if(resflag==newln)
			return  multiply_lns(a,a,newln);
		else
			return  multiply_lns(a,a,firstln);
	}
	b=creat_ln(ln_nodenum(a)*2);
	b->sign=1;
	b->zero=a->zero*2;
	i=a->lowestdigit;
	j=b->lowestdigit;
	do
	{
		k=i;
		l=j;
		if(i->digit !=0)
		{
			do
			{
				if(i==k)
					res=i->digit*k->digit+l->digit;
				else
					res=(i->digit*k->digit*2)+l->digit;
				if(res>=UNIT)
				{
					l->digit=res%UNIT;
					l->hd->digit+=res/UNIT;
				}
				else
					l->digit=res;
				k=k->hd;
				l=l->hd;
			}
			while(k != a->highestdigit->hd);
		}
		i=i->hd;
		j=j->hd->hd;
	}
	while(i != a->highestdigit->hd);
	b->highestdigit=j->ld;
	
	if(resflag==newln)
		return  b;
	else
	{
		copy_lns(&a,b);
		free_ln(b);
		return a;
	}
}

/*
 * 大数和大数想除
 */

lN ln_divide_ln(lN i,lN j)//,int precision,Divide_mode mode),Res resflag)
{
	lN a,b,c,q;
	int zero;
	int a_digitnum;
	int b_digitnum;
	int flag=0;
	Digit x,y,z;
	int o;

	a=copy_lns(NULL,i);
	b=copy_lns(NULL,j);
	c=init_ln(0);

	if(a->sign==b->sign)
		c->sign=1;
	else
		c->sign=0;
	a_digitnum=ln_nodenum(a);
	b_digitnum=ln_nodenum(b);
	if(a_digitnum<b_digitnum)
	{
		zero=a->zero-(b_digitnum-a_digitnum)*DIGIT_NUM;
		ln_setzero(a,zero);
	}
	else if(a_digitnum>b_digitnum)
	{
		zero=b->zero-(a_digitnum-b_digitnum)*DIGIT_NUM;
		ln_setzero(b,zero);
	}
	zero=a->zero-b->zero;

	a->sign=1;
	a->zero=0;
	b->zero=0;
	c->zero=0;

	x=a->highestdigit;
	y=b->highestdigit;
	z=c->highestdigit;

	for(o=1;o<4;o++)
	{

		if(x!=a->highestdigit) 
			z->digit=(x->digit+x->hd->digit*UNIT)/y->digit; //求出z位数
		else
			z->digit=x->digit/y->digit; //求出z位数
		printf("before z->digit=%d\n",z->digit);
		if(z->digit !=0)
		{
			b->sign=0; //b变成负数
			q=ln_multiply_num(b,z->digit,newln);
			add_lns(a,q,firstln);
			free_ln(q);
			while(a->sign==1) //少减了，再减去
			{
				b->sign=0;
				add_lns(a,b,firstln);
				z->digit++;
			}
			while(a->sign==0) //多减了，补上
			{
				b->sign=1;
				add_lns(a,b,firstln);
				z->digit--;
			}
		}

		printf("after z->digit=%d\n",z->digit);
		c->lowestdigit=z;
		if(flag==1)
			zero-=DIGIT_NUM;
		flag=1;
		if(ln_cmp_num(a,0)==0)
			break;

		if(a->highestdigit==a->lowestdigit->ld)
		{
			ln_addsize(a,INIT_SIZE);
			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		}
		if(a->highestdigit->digit==0)
			a->highestdigit=a->highestdigit->ld;
		a->lowestdigit=a->lowestdigit->ld;
		a->lowestdigit->digit=0;
		x=x->ld;

		puts(ln2str(a));

		if(c->lowestdigit->ld==c->highestdigit)
			ln_addsize(c,INIT_SIZE);
		z=z->ld;
		
	}
	while(c->highestdigit!=c->lowestdigit && c->highestdigit->digit==0)
		c->highestdigit=c->highestdigit->ld;
	c->zero=zero;
	ln_info(c);
	free_ln(a);
	free_ln(b);
	return c;
}
