#ifndef LN_H
#define LN_H

#define INIT_SIZE 5 //初始分配的节点个数
#define UNIT 10000  //该表示法的进制
#define DIV_PREC 100  //除法默认小数精度
#define DIGIT_NUM 4 //每个节点存放的位数
#define PUT_LINE {printf("[%s %d %s]\n",__FILE__,__LINE__,__FUNCTION__);fflush(stdout);}

//符号用sign表示 表示成 (sign)cell*10^power 
//cell是双向循环链表
struct _ln 
{
	struct _cell *lsd; //最低位
	struct _cell *msd; //最高位
	int power; //表示指数
	int sign; //1-正数 0-符号未定(cell的num可正可负) -1-负数
};
struct _cell //表示ln其中一个节点(一个大数位数)
{
	int num;
	struct _cell* lcell; //下一个cell
	struct _cell* hcell; //上一个cell
};
//定义操作模式枚举
typedef enum{
	newln,	//操作结果作为新的结构返回
	firstln, //操作结果放在第一个操作数
	copy,	//创建分数时浅复制
	deepcopy, //创建分数时深复制
}res_type;
typedef enum{
	trunc_res, //截断
	round_res  //四舍五入
}divide_mode;
typedef struct _ln* ln;
typedef struct _cell* cell;

//导入errno
extern int errno;

#endif
