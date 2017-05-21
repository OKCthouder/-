#include	"windows.h"
#include	"stdio.h"
#include	"conio.h"
#include	"stdlib.h"
#include	"malloc.h"
#include	"time.h"
#include    "string.h"
#include    "errno.h"


#define TRUE 1
#define FALSE 0
#define OVERFLOW -1
#define OK 1
#define ERROR 0
#define M 3//定义阶数
#define Super_Manager_Code 5372  //超级管理员密码
#define MAX_NAME_LEN 20//姓名长度
#define MAX_BKNAME_LEN 50
#define OK 1
#define ERROR 0
#define	bookinfomationfile "bookinfo.dat"	// 图书信息文件
#define	borrowerfile "borrower.dat"		// 借阅者名单文件
typedef int KeyType;
typedef int Status;

/*****************************书和借阅者结点存储定义*************************/
typedef struct ReaderNode {//借阅者
	int IDnum;                      //ID号
	char reader_name[MAX_NAME_LEN]; //姓名
	ReaderNode *rnext;              //指向下一个借阅者的指针
}ReaderNode, *ReaderType;

typedef struct BookNode {
	int booknum;   //书号
	char bookname[MAX_BKNAME_LEN];  //书名
	char writer[MAX_NAME_LEN];      //作者名字
	int current;                    //现存量
	int total;						//总库存
	int published_year;				//出版年份
	float price;					//价格
	ReaderType reader;				//借阅者链表指针
}BookNode, *BookType;				//图书类型

typedef BookNode Record;			//将书的结点定义为记录

/*******************************B树存储定义***************************/

typedef struct BTNode {        //B树结点类型定义
	int keynum;			       //结点中关键字个数，即结点的大小
	KeyType key[M+1];		   //关键字，key[0]未用
	struct BTNode *parent;	   //双亲结点指针
	struct BTNode *ptr[M + 1]; //孩子结点指针数组
	Record *recptr[M + 1];     //记录指针向量，0号单元未用
}BTNode, *BTree;			   //B树结点和B树类型

typedef struct result {
	BTNode *pt;				  //指向找到的结点
	int i;					  //1<=i<=m,在结点中的关键字位序
	int tag;				  //1:查找成功，0:查找失败
}result, *resultPtr;		  //B树的查找结果类型



 /*******************************B树接口定义***************************/


result SearchBTree(BTree T, int k);
/*
	初始条件:树T存在
	操作结果:在m阶B数T上查找关键字k，返回p{pt,i,tag}
*/

Status InsertBTree(BTree &T, int k, BTree q, int i, Record *recptr);
/*
	初始条件:树T存在
	操作结果:在B树T上结点p->pt的key[i]和key[i+1]之间插入关键字k
*/

Status DeleteBTree(BTree &T, int k);
/*
	初始条件:B树上p结点存在
	操作结果:删除B树T上结点p->pt的关键字k
*/

void BTreeTraverse(BTree T, void(*visit)(BTree));
/*
	初始条件:树T存在
	操作结果:遍历B树
*/


int menu();
/*
	输出选择菜单
*/

void ShowBTree(BTree T, short x = 8);
/*
	以凹入表的形式输出B树
*/
int login();
/*
	登陆界面
*/

/*******************************图书馆接口定义***************************/
void InitLibrary(BTree &L);
/*
	初始化一棵空树
*/
void InsertBook(BTree &L, BookType B, result res);
/*
	插入新的书籍
*/
int DeleteBook(BTree &L, BookType B);
/*
	删除已存在的书籍，不存在则提示不存在
*/
int CanBorrow(BTree L, BookType B, ReaderType R);
/*
	判断指定书籍能否出借
*/
void BorrowBook(BTree L, BookType B, ReaderType R);
/*
	书籍出借
*/
int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R);
/*
	书籍归还
*/
void PrintH();
/*
	输出表格的头部
*/
void PrintB(BookType B);
/*
	输出指定书籍的信息
*/
void PrintT();
/*
	输出表格的尾部
*/
void PrintAll(BTree p);
/*
	输出B树结点的全部关键字信息
*/
void PrintBook(BookType B);
/*
	以表格形式输出一本书的信息
*/
void PrintAllbooks(BTree L);
/*
	输出所有书的信息
*/
int ShowBookinfo(BTree L, int booknum);
/*
	输出指定某本书的信息
*/
void PrintBorrower(ReaderType R);
/*
	输出某本书的借阅者信息
*/
void	Welcome(int color);
/*
	显示欢迎界面
*/
void	Creat(BTree &L);
/*
	从文件中读取图书信息
*/