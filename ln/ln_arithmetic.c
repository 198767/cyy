/*
 *	该文件里面包含ln的基本运算函数
 */
#include "ln_arithmetic.h"

/*
 * 作用:把a加上b
 * 参数:
 *	a,b:要相加的ln
 *	restype:结果存放方式
 * 返回值:
 * 	成功:返回相加结果
 * 	失败:NULL
 */
ln ln_add(ln a,ln b,res_type restype)
{
	ln c;
	int carry,
	    int power;
	int a_sign,b_sign;
	cell p,q,add_res;
	int cmp_res;

	//验证参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;	
	}

	//把指数调整为一致
	if(a->power > b->power)
		ln_adjustpower(a,b->power-a->power);
	if(a->power < b->power)
		ln_adjustpower(b,a->power-b->power);

	//根据存放方式设置c
	if(restype==firstln) 
	{
		c=a;

		//增加节点
		//
		
	}
	else
	{
		c=ln_init(0);
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;	
		}
	}

	

	ln_addsize(c,MAX(ln_nodenum(a),ln_nodenum(b))-ln_nodenum(c)+2);

	//加法
	if(a->sign==b->sign)	
	{
		c->sign=a->sign;
		//调整指数部分一致
		add_res=c->lsd;
		p=a->lsd;
		q=b->lsd;
		while(1)
		{
			if(p)
				carry+=p->num;
			if(q)
				carry+=q->num;
			if(p==a->msd)
				p=NULL;
			if(q==b->msd)
				q=NULL;
			add_res->num=carry%UNIT;
			carry=carry/UNIT;
			if(!p&&!q&&!carry)
			{
				c->msd=add_res;
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
				p=a->lsd;
				q=b->lsd;
				c->sign=a_sign;
			}
			else	
			{
				p=b->lsd;
				q=a->lsd;
				c->sign=b_sign;
			}

			add_res=c->lsd;
			while(1)
			{
				if(q)
					add_res->num=p->num-carry-q->num;
				else
					add_res->num=p->num-carry;
				if(add_res->num <0)
				{
					carry=1;
					add_res->num+=UNIT;	
				}
				else
					carry=0;
				if(q==a->msd || q==b->msd)
					q=NULL;
				if(p==a->msd || p==b->msd)
				{
					while(add_res_type != c->lsd&& add_res->num==0)
						add_res=add_res->ld;
					c->msd=add_res;
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

#ifdef dsa

/*
 * 大数和整数相乘
 */
ln ln_multiply_num(ln i,int b,res_type restype)
{
	int res=0;
	int carry=0;
	ln k,temp;
	if(restype==newln)
	{
		if(b>=UNIT)
		{

			temp=ln_setval(NULL,b);
			multiply_lns(temp,i,firstln);
			return temp;
		}
		else if(b==0)
			return str2ln(ln_creat(1),"0");
		else if(b==1)
			return copy_lns(NULL,i);

		k=ln_creat(ln_get_nodenum(i,i->msd)+2);
		k->power=i->power;
		k->sign=i->sign;

		if(b<0)
		{
			k->sign=!(k->sign);
			b=-b;
		}
		cell a=i->lsd;
		cell c=k->lsd;
		while(1)
		{
			res=b*a->num+carry;
			c->num=res%UNIT;
			carry=res/UNIT;
			if(a==i->msd)
			{
				if(carry>0)
				{
					c->hd->num=carry;
					k->msd=c->hd;
				}
				else
				{
					k->msd=c;
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


		cell a=i->lsd;
		while(1)
		{
			res=b*a->num+carry;
			a->num=res%UNIT;
			carry=res/UNIT;
			if(a==i->msd)
			{
				ln_addsize(i,1);
				if(carry>0)
				{
					a->hd->num=carry;
					i->msd=a->hd;
				}
				else
					i->msd=a;
				break;
			}
			a=a->hd;
		}
		return i;
	}
}



ln ln_add_num(ln a,int b,res_type restype)
{
	ln c,d;
	c=ln_setval(NULL,b);
	d=add_lns(a,c,restype);
	free_ln(c);
	return d;
}

ln minus_lns(ln a,ln b,res_type restype)
{
	ln c,d;
	c=ln_multiply_num(b,-1,newln);
	d=add_lns(a,c,restype);
	free_ln(c);
	return d;
}





/*
 *用m和大数相乘 返回一个新的大数n
 */
ln multiply_lns(ln i,ln j,res_type restype)
{
	ln k;
	cell a,b,c,x,y,z;
	int res;
	k=ln_creat(ln_nodenum(i)+ln_nodenum(j));
	if(i->sign==j->sign)
		k->sign=1;
	else
		k->sign=0;
	k->power=i->power+j->power;
	a=i->lsd;
	b=j->lsd;
	c=k->lsd;
	do
	{
		x=a;
		y=b;
		z=c;
		do
		{
			res=(x->num*y->num)+z->num;
			if(res>=UNIT)
			{
				z->num=res%UNIT;
				z->hd->num+=res/UNIT;
			}
			else
				z->num=res;
			y=y->hd;
			z=z->hd;
		}
		while(y != j->msd->hd);
		a=a->hd;
		c=c->hd;
	}
	while(a != i->msd->hd);
	if(z!=k->msd)
		k->msd=z;
	else
		k->msd=z->ld;

	while(k->msd->num==0 && k->msd !=k->lsd)
		k->msd=k->msd->ld;

	if(restype==newln)
		return  k;
	else
	{
		copy_lns(&i,k);
		free_ln(k);
		return i;
	}
}

ln ln_exp_int(ln i,int b,res_type restype)
{
	
	ln j=NULL,k=NULL;
	assert(b>=0);
	if(b==0)
	{
		if(restype==newln)
			return str2ln(NULL,"1");
		else
			return ln_setval(i,1);
	}
	else if(b==1)
	{
		if(restype==newln)
			return copy_lns(NULL,i);
		else
			return i;
	}
	else if(b==2)
	{
		if(restype==newln)
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
		if(restype==newln)
			return k;
		else
		{
			copy_lns(&i,k);
			free_ln(k);
			return i;
		}
	}
}

int ln_cmp(ln a,ln b)
{
	assert(a&&b);
	int zero_a=1,zero_b=1;
	int minzero;
	int res=0;
	cell p;
	cell q;
	
	//验证0	
	p=a->msd;
	while(1)
	{
		if(p->num !=0)
		{
			zero_a=0;
			break;
		}
		if(p==a->lsd)
			break;
		p=p->ld;
	}
	a->msd=p;
	q=b->msd;
	while(1)
	{
		if(q->num !=0)
		{
			zero_b=0;
			break;
		}
		if(q==b->lsd)
			break;
		q=q->ld;
	}
	b->msd=q;



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
	minzero=MIN(a->power,b->power);
	ln_setzero(a,minzero);
	ln_setzero(b,minzero);
	if(a->power>b->power)
		res=1;
	else if(a->power<b->power)
		res=-1;
	else
	{	
		p=a->lsd;
		q=b->lsd;
		while(1)
		{
			if(p->num>q->num)
				res=1;
			else if(p->num<q->num)
				res=-1;
			if(p==a->msd)
			{
				if(q!= b->msd)
				{
					res=-1;
				}
				break;
			}
			if(q==b->msd)
			{
				if(p!= a->msd)
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


int ln_cmp_num(ln a,int b)
{
	int res;
	ln c;
	c=ln_setval(NULL,b);
	res=ln_cmp(a,c);
	free_ln(c);
	return res;
}




/*
 * 大数和整数相除
 */
ln ln_divide_num(ln i,int b,int precision,Divide_mode mode,res_type restype)
{
	int res=0;
	int carry=0;
	int prec=0;
	ln k;
	assert(b!=0);
	assert(precision>=0);
	if(precision<0)
		precision=DIV_PREC;

	if(restype==newln)
	{
		if(b==1|| b==-1 )
		{
			k= copy_lns(NULL,i);
			return ln_multiply_num(k,b,firstln);
		}
		k=ln_creat(ln_get_nodenum(i,i->msd));
		k->power=i->power;
		if(b>0)
			k->sign=i->sign;
		else
		{
			k->sign=!(i->sign);
			b=-b;
		}
		cell a=i->msd;
		while(a!= i->lsd && a->num==0)
			a=a->ld;
		i->msd=a;
		k->msd=k->lsd->ld;
		cell c=k->msd;
		res=a->num;
		while(1)
		{
			res=res+carry*UNIT;
			c->num=res/b;
			carry=res%b;
			if(k->power<0)
			{
				if(prec==0)
				{
					if(c->num !=0) //开始计算小数点个数
						prec=-k->power;
				}
				else
					prec+=DIGIT_NUM;
			}
			if(a==i->lsd)
			{
				if(carry==0) //除得尽
				{
					k->lsd=c;
					break;
				}
				else if(prec>=precision) //除不尽，四舍五入保留precision位小数
				{
					if(prec==precision)
					{
						if(mode==round)
						{
							if(a !=i->lsd)
								res=(a->ld->num+carry*UNIT)/b;
							else
								res=(carry*UNIT)/b;
							if(res>=UNIT/2)
								c->num++;
						}
					}
					else
					{
						int l,j=1;
						for(l=0;l<prec-precision;l++)
							j*=10;
						res=c->num%j;
						if(mode==round && res>=5*(j/10))
							c->num=c->num-res+j;
						else
							c->num=c->num-res;
					}
					k->lsd=c;
					break;
				}
			}
			if(a !=i->lsd)
			{
				a=a->ld;
				res=a->num;
			}
			else
			{
				res=0;
				k->power-=DIGIT_NUM;
			}
			if(c==k->lsd)
			{
				ln_addsize(k,5);
				k->lsd=k->msd->hd;
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
		cell a=i->msd;
		while(a != i->lsd && a->num==0)
			a=a->ld;
		cell c=i->lsd->ld;
		cell d=i->lsd;
		i->msd=c;
		res=a->num;
		while(1)
		{
			res=res+carry*UNIT;
			c->num=res/b;
			carry=res%b;
			if(i->power<0)
			{
				if(prec==0)
				{
					if(c->num !=0) //开始计算小数点个数
						prec=-i->power;
				}
				else
					prec+=DIGIT_NUM;
			}
			if(a==d)
			{
				if(carry==0) //除得尽
				{
					i->lsd=c;
					break;
				}
				else if(prec>=precision) //除不尽，四舍五入保留precision位小数
				{
					if(prec==precision)
					{
						if(mode==round)
						{
							if(a !=d)
								res=(a->ld->num+carry*UNIT)/b;
							else
								res=(carry*UNIT)/b;
							if(res>=UNIT/2)
								c->num++;
						}
					}
					else
					{
						int l,j=1;
						for(l=0;l<prec-precision;l++)
							j*=10;
						res=c->num%j;
						if(mode==round && res>=5*(j/10))
							c->num=c->num-res+j;
						else
							c->num=c->num-res;
					}
					i->lsd=c;
					break;
				}
			}
			if(a!=d)
			{
				a=a->ld;
				res=a->num;
			}
			else
			{
				res=0;
				i->power-=DIGIT_NUM;
			}
			if(c==i->lsd)
			{
				ln_addsize(i,5);
				i->lsd=i->msd->hd;
			}
			c=c->ld;
		}
		return i;
	}
}


int ln_divideable_num(const ln i,int b)
{
	cell a;
	int res=0;
	int carry=0;
	ln j;
	assert(b!=0);
	assert(i->power==0);
	if(b==1|| b==-1 )
		return 1;
	j=copy_lns(NULL,i);
	a=j->msd;
	res=a->num;
	while(1)
	{
		res=res+carry*UNIT;
		carry=res%b;
		if(a==j->lsd)
		{
			free_ln(j);
			if(carry==0) //除得尽
				return 1;
			else
				return 0;
		}
		a=a->ld;
		res=a->num;
	}
	return 0;
}





//求平方

ln ln_square(ln a,res_type restype)
{
	ln b;
	cell i,j,k,l;
	int res;
	if(a->lsd==a->msd)
	{
		if(restype==newln)
			return  multiply_lns(a,a,newln);
		else
			return  multiply_lns(a,a,firstln);
	}
	b=ln_creat(ln_nodenum(a)*2);
	b->sign=1;
	b->power=a->power*2;
	i=a->lsd;
	j=b->lsd;
	do
	{
		k=i;
		l=j;
		if(i->num !=0)
		{
			do
			{
				if(i==k)
					res=i->num*k->num+l->num;
				else
					res=(i->num*k->num*2)+l->num;
				if(res>=UNIT)
				{
					l->num=res%UNIT;
					l->hd->num+=res/UNIT;
				}
				else
					l->num=res;
				k=k->hd;
				l=l->hd;
			}
			while(k != a->msd->hd);
		}
		i=i->hd;
		j=j->hd->hd;
	}
	while(i != a->msd->hd);
	b->msd=j->ld;
	
	if(restype==newln)
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

ln ln_divide_ln(ln i,ln j)//,int precision,Divide_mode mode),res_type restype)
{
	ln a,b,c,q;
	int zero;
	int a_digitnum;
	int b_digitnum;
	int flag=0;
	cell x,y,z;
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
		zero=a->power-(b_digitnum-a_digitnum)*DIGIT_NUM;
		ln_setzero(a,zero);
	}
	else if(a_digitnum>b_digitnum)
	{
		zero=b->power-(a_digitnum-b_digitnum)*DIGIT_NUM;
		ln_setzero(b,zero);
	}
	zero=a->power-b->power;

	a->sign=1;
	a->power=0;
	b->power=0;
	c->power=0;

	x=a->msd;
	y=b->msd;
	z=c->msd;

	for(o=1;o<4;o++)
	{

		if(x!=a->msd) 
			z->num=(x->num+x->hd->num*UNIT)/y->num; //求出z位数
		else
			z->num=x->num/y->num; //求出z位数
		printf("before z->num=%d\n",z->num);
		if(z->num !=0)
		{
			b->sign=0; //b变成负数
			q=ln_multiply_num(b,z->num,newln);
			add_lns(a,q,firstln);
			free_ln(q);
			while(a->sign==1) //少减了，再减去
			{
				b->sign=0;
				add_lns(a,b,firstln);
				z->num++;
			}
			while(a->sign==0) //多减了，补上
			{
				b->sign=1;
				add_lns(a,b,firstln);
				z->num--;
			}
		}

		printf("after z->num=%d\n",z->num);
		c->lsd=z;
		if(flag==1)
			zero-=DIGIT_NUM;
		flag=1;
		if(ln_cmp_num(a,0)==0)
			break;

		if(a->msd==a->lsd->ld)
		{
			ln_addsize(a,INIT_SIZE);
			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		}
		if(a->msd->num==0)
			a->msd=a->msd->ld;
		a->lsd=a->lsd->ld;
		a->lsd->num=0;
		x=x->ld;

		puts(ln2str(a));

		if(c->lsd->ld==c->msd)
			ln_addsize(c,INIT_SIZE);
		z=z->ld;
		
	}
	while(c->msd!=c->lsd && c->msd->num==0)
		c->msd=c->msd->ld;
	c->power=zero;
	ln_info(c);
	free_ln(a);
	free_ln(b);
	return c;
}


//获取ln的十进制位数的和
/*
int ln_digitsum(ln n)
{
	cell p=n->lsd->ld;
	int sum=0;
	int i;
	int j;
	do
	{
		p=p->hd;
		j=1;
		for(i=0;i<DIGIT_NUM;i++)
		{
			sum+=(p->num/j)%10;
			j*=10;
		}

	}
	while(p !=n->msd);
	return sum;
}
*/

#endif
