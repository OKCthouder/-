#include	"windows.h"
#include	"stdio.h"
#include	"conio.h"
#include	"stdlib.h"
#include	"malloc.h"
#include	"ctime"
#include	"time.h"
#include	"dos.h"

#define		OK		1
#define		ERROR	0
#define		TRUE	1
#define		FALSE	0
#define		OVERFLOW -1
#define		MAX_NAME_LEN	20						// 姓名最大长度
#define		MAX_BKNAME_LEN	30						// 书名最大长度
#define		MAX_BOOKS		100						// 书库中一个著者最多著作数
#define		OUT_DATE_PRICE	0.1						// 借书逾期一天的费用
#define		KEEP_DAYS		90						// 图书出借的期限

#define		logfile				"LibraryLogs.log"	// 系统日志文件
#define		bookinfomationfile	"bookinfo.dat"		// 图书信息文件
#define		borrowerfile		"borrower.dat"		// 借阅者名单文件
#define		bespeakerfile		"bespeaker.dat"		// 预约者名单文件
typedef		int		Status;
char 		*books[MAX_BOOKS];						// 某位著者著作名数组
char 		author[MAX_NAME_LEN];					// 著者姓名数组
int			books_counter;							// 著者著作计数



typedef	struct	ReaderNode							// 借阅者/预约者结点
{
	int		cardnum;								// 借阅证号
	char	rname[MAX_NAME_LEN];					// 借阅者姓名
	union{
		struct{										
			tm		bordate;						// 借书日期
			tm		retdate;						// 还书日期
			struct	ReaderNode *nextr;				// 下一个借阅者指针
		};
		struct{
			tm		bespeakdate;					// 预约日期
			struct	ReaderNode *nextb;				// 下一个预约者指针
		};	
	};
}ReaderNode,*ReaderType;							// 读者类型


typedef	struct	BookNode							// 图书结点
{	
	int			booknum;							// 书号
	char		bookname[MAX_BKNAME_LEN];			// 书名
	char		writer[MAX_NAME_LEN];				// 著者名
	int			current, total;						// 现存量和总库存
	int			publishyear;						// 出版年份
	float		price;								// 定价
	ReaderType	reader;								// 读者链表指针
	ReaderType	bespeaker;							// 预约者链表指针
} BookNode,*BookType;								// 图书类型

// ******************公共函数声明****************************
void 	SetColor(unsigned short TextColor = 7 ) ;
short	wherex();
short	wherey();
void 	gotoxy(short x,short y);
void	GetDate(tm &tim);
void	AddDate(tm &date2,tm date1, int	day);
Status	Earlier(tm date1,tm date2);


#include"btree.h"									// B树操作头文件

typedef		BTree	Library;

#include"library.h"									// 书库操作头文件

// ***************************************************************************// 
// 	*********			一些公共函数		********

short	wherex()	// 返回光标的x坐标
{
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbinfo); 
	return csbinfo.dwCursorPosition.X;
}

short	wherey()	// 返回光标的y坐标
{
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbinfo); 
	return csbinfo.dwCursorPosition.Y;
}

void 	gotoxy(short x,short y)
// 移动光标到（x，y）坐标，25>x>=0,80>y>=0	
{
	COORD point = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void 	SetColor(unsigned short TextColor )
// 设置字体和背景颜色
{ 
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hCon,TextColor); 
}

void	GetDate(tm &tim)
// 获取系统时间，赋予tm结构体变量tim		
{
	time_t curtime=time(0); 
	tim = *localtime(&curtime);
	tim.tm_year += 1900;		// tm 年份少1900年
	tim.tm_mon ++;				// tm month 从0-11，故加1
}


void	AddDate(tm &date2,tm date1, int	day)
// 把date1的日期加day天后赋给date2
{
	date2.tm_year = date1.tm_year + (day/30 + date1.tm_mon) / 12;
	date2.tm_mon = (date1.tm_mon + (day / 30)) % 12;
	date2.tm_mday = (date1.tm_mday + day) % 30;
}

Status	Earlier(tm date1,tm date2)
// 比较data1与date2日期的迟与早，如果date1早于或等于date2则返回TRUE，否则返回FALSE。
{
	if(date1.tm_year < date2.tm_year)	return TRUE;
	if(date1.tm_year > date2.tm_year)	return ERROR;
	if(date1.tm_mon < date2.tm_mon)		return TRUE;
	if(date1.tm_mon > date2.tm_mon)		return ERROR;
	if(date1.tm_mday <date2.tm_mday)	return TRUE;
	return ERROR;
}


void	RecordLogs(int	op, BookType B = NULL,ReaderType R = NULL,int succ = 1)
// op不同的值对应不同的操作。secc为1代表操作成功，为0代表操作失败
// 根据op不同的操作，把操作记录到日志文件logfile中
{
	FILE	*logfp;
	tm		date;
	char	s[][6] = {"失败","成功"};
	char	opera[][20] = {"进入系统","入库","清除","查看图书","借阅","归还","预约",
						"列出著者","退出系统","打印所有图书信息"};			// 操作名字符串			
	if(!(logfp = fopen(logfile,"a+")))										// 打开日志文件
		{
		puts("\n不能打开日志文件!!\n");
		return ;
		}
	GetDate(date);	
				
	fprintf(logfp,"%-4d/%02d/%02d %02d:%02d:%02d ",date.tm_year,
			date.tm_mon,date.tm_mday,date.tm_hour,date.tm_min,date.tm_sec);	// 记录当前时间
	if(op == 0 || op == 8)													// 登入/登出系统
		fprintf(logfp,"**************************************************\n%s\n",opera[op]);			
	if(op > 0 && op <= 3)													// 入库/清除/查看	
		fprintf(logfp,"%8s% -6d\t《%s》\t\t%s\n",opera[op], B->booknum,B->bookname,s[succ]);
	if(op > 3 && op <= 6)												
		fprintf(logfp,"借阅证号 %-4d%\t%s\t%s\t书号 %-4d《%s》\t\t%s\n",R->cardnum,
			R->rname,opera[op],B->booknum,B->bookname,s[succ]);				// 借书/归还/预约
	if(op == 7)																// 列举著作名
		fprintf(logfp,"%s\t\t%s 的所有著作名 %s\n",opera[op],author,s[succ]);
	if(op == 9) fprintf(logfp,"%s\n",opera[9]);								// 打印所有图书
	fclose(logfp);															// 关闭日志文件
}

void	Welcome(int color)
// 显示欢迎界面
{
	SetColor(color);
	gotoxy(2,5);
	printf("      ╭╮╭╮╭──╮╭╮　　╭──╮╭──╮╭╭╮╮╭──╮\n");
	printf("        │││││╭─╯││　　│╭─╯│╭╮││　　││╭─╯\n");
	printf("        │╰╯││╰─╮││　　││　　│││││││││╰─╮\n");
	printf("        │││││╭─╯││　　││　　│││││╭╮││╭─╯\n");
	printf("        │　　││╰─╮│╰─╮│╰─╮│╰╯││││││╰─╮\n");
	printf("        ╰╰╯╯╰──╯╰──╯╰──╯╰──╯╰╯╰╯╰──╯\n");
	SetColor(11);
	printf("                      ╔══════════════╗\n");
	printf("                      ║    欢迎进入图书管理系统    ║\n");
	printf("                      ╚══════════════╝\n");
	SetColor();
}

void	PrintLib()
// 在屏幕右侧显示“图书管理"
{
	int		x = 63, y = -1;
	gotoxy(x,++y);printf("╔════╗");
	gotoxy(x,++y);printf("║╠══╗║");
	gotoxy(x,++y);printf("║╯═╔╯║");
	gotoxy(x,++y);printf("║═╦╯═║");
	gotoxy(x,++y);printf("║╚══╮║");
	gotoxy(x,++y);printf("╚════╯");
	gotoxy(x,++y);printf("　╔　　╔╮");
	gotoxy(x,++y);printf("╔╠══╗║");
	gotoxy(x,++y);printf("　║　　║╝");
	gotoxy(x,++y);printf("╔╠══╩╗");
	gotoxy(x,++y);printf("　║　　　║");
	gotoxy(x,++y);printf("　╝　╚═╯");
	gotoxy(x,++y);printf("╔╦╮═╦╮");
	gotoxy(x,++y);printf("╔══╩═╗");
	gotoxy(x,++y);printf("╔════╗");
	gotoxy(x,++y);printf("╠════╯");
	gotoxy(x,++y);printf("╠════╗");
	gotoxy(x,++y);printf("╚════╯");
	gotoxy(x,++y);printf("╔╦╔═╦╮");
	gotoxy(x,++y);printf("　║║　║║");
	gotoxy(x,++y);printf("╭╠╠═╠╣");
	gotoxy(x,++y);printf("　║╚═╠╝");
	gotoxy(x,++y);printf("　║╚═╠╯");
	gotoxy(x,++y);printf("╰╩╰═╩╝");
}


void	PrintName()
// 打印作者信息
{
	SetColor(2);	// 深绿色
	int		x = 38 ,y = 0;
	gotoxy(x,++y);printf(" ╭─────╮");   
	gotoxy(x,++y);printf(" │  2007级  │");  
	gotoxy(x,++y);printf(" │  计科6班 │");
	gotoxy(x,++y);printf(" │3107006587│");    
	gotoxy(x,++y);printf(" │  潘福凯  │");    
	gotoxy(x,++y);printf(" │2009-6-13 │");    
	gotoxy(x,++y);printf(" ╰─────╯");    
	SetColor();
}



void	PrintB()
// 用逗号打印书名号图形。。。。
{
	int x =32 ,y =wherey()-7 ;
	gotoxy(x,++y);printf(" ,,,                       ,,,"); 
	gotoxy(x,++y);printf("    ,,,                 ,,,  ");
	gotoxy(x,++y);printf("       ,,,           ,,,   ");
	gotoxy(x,++y);printf("　        ,,,    ,,,　 ");
	gotoxy(x,++y);printf("              ,,   ");
}

void	PrintDate()
// 显示当前日期和时间
{
	int		x = 40 ,y = 9;
	char	a[][3] = {"日","一","二","三","四","五","六"};			// tm结构体星期从0-6		
	tm		date;
	GetDate(date);													// 获取当期日期
	gotoxy(x,y); 
	printf("当前时间：");
	gotoxy(x,y+2);
	printf("%d年%2d月%2d日",date.tm_year,date.tm_mon,date.tm_mday);	// 显示当前日期
	gotoxy(x+8,y+3);printf("星期%s",a[date.tm_wday]);				
	while(!_kbhit())												// 当未检测到按键
		{
		gotoxy(x,y+4);
		GetDate(date);												// 获取时间
		printf("%02d:%02d:%02d ",date.tm_hour,date.tm_min,date.tm_sec);
		Sleep(500);													// 显示时间
		}	
}





void	Menu()
// 显示图书管理系统菜单
{    
	system("cls");
	printf("        ╭══════╮       \n");
	printf("        ║┏┯┓┏┯┓║       \n");
	printf("  ╭══┤┠菜┨┠单┨├══╮ \n");    
	printf("  ║    ║┗┷┛┗┷┛║    ║ \n");  
	printf("  ║    ╰══════╯    ║ \n");
	printf("  ║                        ║ \n");   
	printf("  ║  1     新书入库        ║ \n");    
	printf("  ║                        ║ \n");
	printf("  ║  2     清除库存        ║ \n");   
	printf("  ║                        ║ \n");
	printf("  ║  3     图书出借        ║ \n");   
	printf("  ║                        ║ \n");    
	printf("  ║  4     图书归还        ║ \n");    
	printf("  ║                        ║ \n");
	printf("  ║  5     图书预约        ║ \n");   
	printf("  ║                        ║ \n");    
	printf("  ║  6     列出著者著作    ║ \n");
	printf("  ║                        ║ \n");
	printf("  ║  7     查看图书状态    ║ \n");
	printf("  ║                        ║ \n");
	printf("  ║  8     遍历书库        ║ \n");
	printf("  ║                        ║ \n");
	printf("  ║  9     退出系统        ║ \n");
	printf("  ║                        ║ \n");
	printf("  ╰════════════╯ \n");
	PrintLib();			// 显示“图书管理”
	PrintB();			// 显示书名号
	PrintName();		// 显示作者信息
	PrintDate();		// 显示当前时间
}
