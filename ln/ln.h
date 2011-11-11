#ifndef LN_H
#define LN_H

#define INIT_SIZE 5 //初始分配的节点个数
#define UNIT 10000  //该表示法的进制
#define DIV_PREC 100  //除法有效数字个数
#define DIGIT_NUM 4 //每个节点存放的位数
#define MAX(a,b)  (a>b?a:b) //求最大值
#define MIN(a,b)  (a<b?a:b) //求最小值
#define ISDIGIT(a)  ((a)>='0'&&(a)<='9') //字符是否数字
#define PUT_LINE {printf("%d\n",__LINE__);fflush(stdout);}
#define init_ln(i) ln_setval(NULL,i)
struct _ln //正负用sign表示 表示成 cell*10^power cell是双向循环链表
{
	struct _cell *lsd; //最低位
	struct _cell *msd; //最高位
	int power; //表示指数
	int sign; //1-正数 0-负数
};
struct _cell //表示ln其中一个节点(一个大数位数)
{
	int num;
	struct _cell* lcell; //下一个cell
	struct _cell* hcell; //上一个cell
};
//定义操作模式枚举
typedef enum{
	newln,
	firstln
}res_type;
typedef enum{
	trunc,
	round
}divide_mode;
typedef struct _ln* ln;
typedef struct _cell* cell;

/*
//除法
lN ln_divide_num(lN i,int b,int precision,Divide_mode mode,Res resflag);
int ln_divideable_num(const lN i,int b);
lN ln_divide_ln(lN i,lN j);

void ln_info(lN n);

int ln_cmp(lN a,lN b);
int ln_cmp_num(lN a,int b);
//获取ln的十进制位数的和
int ln_cellsum(lN n);
//ln相加
lN add_lns(lN m,lN n,Res res);
lN minus_lns(lN a,lN b,Res resflag);
lN ln_add_num(lN a,int b,Res resflag);
//数乘
lN ln_multiply_num(lN i,int b,Res resflag);
//乘法
lN multiply_lns(lN i,lN j,Res resflag);
//求平方
lN ln_square(lN a,Res resflag);
//求幂
lN ln_exp_int(lN i,int b,Res resflag);
 * ------------------操纵函数-----------------------*/

#endif
