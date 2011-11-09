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
struct ln //正负用sign表示 表示成 digit*10^zero   digit是双向循环链表
{
	struct digit *lowestdigit; //最低位
	struct digit *highestdigit; //最高位
	int zero; //表示结尾的0  
	int sign; //1-正数 0-负数
};
struct digit
{
	int digit;
	struct digit* ld; //低一位
	struct digit* hd; //高一位
};
//定义操作模式枚举
typedef enum{
	newln=0,
	firstln=1
}Res;
typedef enum{
	trunc,
	round
}Divide_mode;
typedef struct ln* lN;
typedef struct digit* Digit;

/*--------------------基础函数---------------------*/
//创建双向循环链表 这个函数不应该对外使用
lN creat_ln(int size);
//把字符串转换为大数
lN str2ln(lN n,const char* str);
//赋值
lN ln_setval(lN n,int m);
//增加数据结构的容量
lN ln_addsize(lN n,int size);
//计算直到digit部分的节点数
int ln_get_nodenum(lN n,Digit q);
#define ln_nodenum(n) ln_get_nodenum(n,(n)->highestdigit)
//把ln转换为字符串
char* ln2str(lN n);
//打印数字
void print_ln(lN n);
//释放ln
lN free_ln(lN n);
// 复制ln b到ln a
lN copy_lns(lN* a,lN b);
//截取n的digit部分m个节点,如果m过大则n不变
void ln_trunc(lN n,int m);
//减少指数部分,多余的0放到整數部分
lN ln_setzero(lN n,int zero);
//除法
lN ln_divide_num(lN i,int b,int precision,Divide_mode mode,Res resflag);
int ln_divideable_num(const lN i,int b);

void ln_info(lN n);

/*--------------------操纵函数-----------------------*/
int ln_cmp(lN a,lN b);
int ln_cmp_num(lN a,int b);
//获取ln的十进制位数的和
int ln_digitsum(lN n);
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

#endif
