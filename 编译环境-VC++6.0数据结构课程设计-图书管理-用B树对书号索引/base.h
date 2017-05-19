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
#define		MAX_NAME_LEN	20						// ������󳤶�
#define		MAX_BKNAME_LEN	30						// ������󳤶�
#define		MAX_BOOKS		100						// �����һ���������������
#define		OUT_DATE_PRICE	0.1						// ��������һ��ķ���
#define		KEEP_DAYS		90						// ͼ����������

#define		logfile				"LibraryLogs.log"	// ϵͳ��־�ļ�
#define		bookinfomationfile	"bookinfo.dat"		// ͼ����Ϣ�ļ�
#define		borrowerfile		"borrower.dat"		// �����������ļ�
#define		bespeakerfile		"bespeaker.dat"		// ԤԼ�������ļ�
typedef		int		Status;
char 		*books[MAX_BOOKS];						// ĳλ��������������
char 		author[MAX_NAME_LEN];					// ������������
int			books_counter;							// ������������



typedef	struct	ReaderNode							// ������/ԤԼ�߽��
{
	int		cardnum;								// ����֤��
	char	rname[MAX_NAME_LEN];					// ����������
	union{
		struct{										
			tm		bordate;						// ��������
			tm		retdate;						// ��������
			struct	ReaderNode *nextr;				// ��һ��������ָ��
		};
		struct{
			tm		bespeakdate;					// ԤԼ����
			struct	ReaderNode *nextb;				// ��һ��ԤԼ��ָ��
		};	
	};
}ReaderNode,*ReaderType;							// ��������


typedef	struct	BookNode							// ͼ����
{	
	int			booknum;							// ���
	char		bookname[MAX_BKNAME_LEN];			// ����
	char		writer[MAX_NAME_LEN];				// ������
	int			current, total;						// �ִ������ܿ��
	int			publishyear;						// �������
	float		price;								// ����
	ReaderType	reader;								// ��������ָ��
	ReaderType	bespeaker;							// ԤԼ������ָ��
} BookNode,*BookType;								// ͼ������

// ******************������������****************************
void 	SetColor(unsigned short TextColor = 7 ) ;
short	wherex();
short	wherey();
void 	gotoxy(short x,short y);
void	GetDate(tm &tim);
void	AddDate(tm &date2,tm date1, int	day);
Status	Earlier(tm date1,tm date2);


#include"btree.h"									// B������ͷ�ļ�

typedef		BTree	Library;

#include"library.h"									// ������ͷ�ļ�

// ***************************************************************************// 
// 	*********			һЩ��������		********

short	wherex()	// ���ع���x����
{
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbinfo); 
	return csbinfo.dwCursorPosition.X;
}

short	wherey()	// ���ع���y����
{
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbinfo); 
	return csbinfo.dwCursorPosition.Y;
}

void 	gotoxy(short x,short y)
// �ƶ���굽��x��y�����꣬25>x>=0,80>y>=0	
{
	COORD point = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void 	SetColor(unsigned short TextColor )
// ��������ͱ�����ɫ
{ 
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hCon,TextColor); 
}

void	GetDate(tm &tim)
// ��ȡϵͳʱ�䣬����tm�ṹ�����tim		
{
	time_t curtime=time(0); 
	tim = *localtime(&curtime);
	tim.tm_year += 1900;		// tm �����1900��
	tim.tm_mon ++;				// tm month ��0-11���ʼ�1
}


void	AddDate(tm &date2,tm date1, int	day)
// ��date1�����ڼ�day��󸳸�date2
{
	date2.tm_year = date1.tm_year + (day/30 + date1.tm_mon) / 12;
	date2.tm_mon = (date1.tm_mon + (day / 30)) % 12;
	date2.tm_mday = (date1.tm_mday + day) % 30;
}

Status	Earlier(tm date1,tm date2)
// �Ƚ�data1��date2���ڵĳ����磬���date1���ڻ����date2�򷵻�TRUE�����򷵻�FALSE��
{
	if(date1.tm_year < date2.tm_year)	return TRUE;
	if(date1.tm_year > date2.tm_year)	return ERROR;
	if(date1.tm_mon < date2.tm_mon)		return TRUE;
	if(date1.tm_mon > date2.tm_mon)		return ERROR;
	if(date1.tm_mday <date2.tm_mday)	return TRUE;
	return ERROR;
}


void	RecordLogs(int	op, BookType B = NULL,ReaderType R = NULL,int succ = 1)
// op��ͬ��ֵ��Ӧ��ͬ�Ĳ�����seccΪ1��������ɹ���Ϊ0�������ʧ��
// ����op��ͬ�Ĳ������Ѳ�����¼����־�ļ�logfile��
{
	FILE	*logfp;
	tm		date;
	char	s[][6] = {"ʧ��","�ɹ�"};
	char	opera[][20] = {"����ϵͳ","���","���","�鿴ͼ��","����","�黹","ԤԼ",
						"�г�����","�˳�ϵͳ","��ӡ����ͼ����Ϣ"};			// �������ַ���			
	if(!(logfp = fopen(logfile,"a+")))										// ����־�ļ�
		{
		puts("\n���ܴ���־�ļ�!!\n");
		return ;
		}
	GetDate(date);	
				
	fprintf(logfp,"%-4d/%02d/%02d %02d:%02d:%02d ",date.tm_year,
			date.tm_mon,date.tm_mday,date.tm_hour,date.tm_min,date.tm_sec);	// ��¼��ǰʱ��
	if(op == 0 || op == 8)													// ����/�ǳ�ϵͳ
		fprintf(logfp,"**************************************************\n%s\n",opera[op]);			
	if(op > 0 && op <= 3)													// ���/���/�鿴	
		fprintf(logfp,"%8s% -6d\t��%s��\t\t%s\n",opera[op], B->booknum,B->bookname,s[succ]);
	if(op > 3 && op <= 6)												
		fprintf(logfp,"����֤�� %-4d%\t%s\t%s\t��� %-4d��%s��\t\t%s\n",R->cardnum,
			R->rname,opera[op],B->booknum,B->bookname,s[succ]);				// ����/�黹/ԤԼ
	if(op == 7)																// �о�������
		fprintf(logfp,"%s\t\t%s ������������ %s\n",opera[op],author,s[succ]);
	if(op == 9) fprintf(logfp,"%s\n",opera[9]);								// ��ӡ����ͼ��
	fclose(logfp);															// �ر���־�ļ�
}

void	Welcome(int color)
// ��ʾ��ӭ����
{
	SetColor(color);
	gotoxy(2,5);
	printf("      �q�r�q�r�q�����r�q�r�����q�����r�q�����r�q�q�r�r�q�����r\n");
	printf("        �����������q���s�����������q���s���q�r�������������q���s\n");
	printf("        ���t�s�����t���r�����������������������������������t���r\n");
	printf("        �����������q���s���������������������������q�r�����q���s\n");
	printf("        �����������t���r���t���r���t���r���t�s�������������t���r\n");
	printf("        �t�t�s�s�t�����s�t�����s�t�����s�t�����s�t�s�t�s�t�����s\n");
	SetColor(11);
	printf("                      �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[\n");
	printf("                      �U    ��ӭ����ͼ�����ϵͳ    �U\n");
	printf("                      �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n");
	SetColor();
}

void	PrintLib()
// ����Ļ�Ҳ���ʾ��ͼ�����"
{
	int		x = 63, y = -1;
	gotoxy(x,++y);printf("�X�T�T�T�T�[");
	gotoxy(x,++y);printf("�U�d�T�T�[�U");
	gotoxy(x,++y);printf("�U�s�T�X�s�U");
	gotoxy(x,++y);printf("�U�T�j�s�T�U");
	gotoxy(x,++y);printf("�U�^�T�T�r�U");
	gotoxy(x,++y);printf("�^�T�T�T�T�s");
	gotoxy(x,++y);printf("���X�����X�r");
	gotoxy(x,++y);printf("�X�d�T�T�[�U");
	gotoxy(x,++y);printf("���U�����U�a");
	gotoxy(x,++y);printf("�X�d�T�T�m�[");
	gotoxy(x,++y);printf("���U�������U");
	gotoxy(x,++y);printf("���a���^�T�s");
	gotoxy(x,++y);printf("�X�j�r�T�j�r");
	gotoxy(x,++y);printf("�X�T�T�m�T�[");
	gotoxy(x,++y);printf("�X�T�T�T�T�[");
	gotoxy(x,++y);printf("�d�T�T�T�T�s");
	gotoxy(x,++y);printf("�d�T�T�T�T�[");
	gotoxy(x,++y);printf("�^�T�T�T�T�s");
	gotoxy(x,++y);printf("�X�j�X�T�j�r");
	gotoxy(x,++y);printf("���U�U���U�U");
	gotoxy(x,++y);printf("�q�d�d�T�d�g");
	gotoxy(x,++y);printf("���U�^�T�d�a");
	gotoxy(x,++y);printf("���U�^�T�d�s");
	gotoxy(x,++y);printf("�t�m�t�T�m�a");
}


void	PrintName()
// ��ӡ������Ϣ
{
	SetColor(2);	// ����ɫ
	int		x = 38 ,y = 0;
	gotoxy(x,++y);printf(" �q�����������r");   
	gotoxy(x,++y);printf(" ��  2007��  ��");  
	gotoxy(x,++y);printf(" ��  �ƿ�6�� ��");
	gotoxy(x,++y);printf(" ��3107006587��");    
	gotoxy(x,++y);printf(" ��  �˸���  ��");    
	gotoxy(x,++y);printf(" ��2009-6-13 ��");    
	gotoxy(x,++y);printf(" �t�����������s");    
	SetColor();
}



void	PrintB()
// �ö��Ŵ�ӡ������ͼ�Ρ�������
{
	int x =32 ,y =wherey()-7 ;
	gotoxy(x,++y);printf(" ,,,                       ,,,"); 
	gotoxy(x,++y);printf("    ,,,                 ,,,  ");
	gotoxy(x,++y);printf("       ,,,           ,,,   ");
	gotoxy(x,++y);printf("��        ,,,    ,,,�� ");
	gotoxy(x,++y);printf("              ,,   ");
}

void	PrintDate()
// ��ʾ��ǰ���ں�ʱ��
{
	int		x = 40 ,y = 9;
	char	a[][3] = {"��","һ","��","��","��","��","��"};			// tm�ṹ�����ڴ�0-6		
	tm		date;
	GetDate(date);													// ��ȡ��������
	gotoxy(x,y); 
	printf("��ǰʱ�䣺");
	gotoxy(x,y+2);
	printf("%d��%2d��%2d��",date.tm_year,date.tm_mon,date.tm_mday);	// ��ʾ��ǰ����
	gotoxy(x+8,y+3);printf("����%s",a[date.tm_wday]);				
	while(!_kbhit())												// ��δ��⵽����
		{
		gotoxy(x,y+4);
		GetDate(date);												// ��ȡʱ��
		printf("%02d:%02d:%02d ",date.tm_hour,date.tm_min,date.tm_sec);
		Sleep(500);													// ��ʾʱ��
		}	
}





void	Menu()
// ��ʾͼ�����ϵͳ�˵�
{    
	system("cls");
	printf("        �q�T�T�T�T�T�T�r       \n");
	printf("        �U���ө����ө��U       \n");
	printf("  �q�T�T�ȩĲ˩̩ĵ��̩��T�T�r \n");    
	printf("  �U    �U���۩����۩��U    �U \n");  
	printf("  �U    �t�T�T�T�T�T�T�s    �U \n");
	printf("  �U                        �U \n");   
	printf("  �U  1     �������        �U \n");    
	printf("  �U                        �U \n");
	printf("  �U  2     ������        �U \n");   
	printf("  �U                        �U \n");
	printf("  �U  3     ͼ�����        �U \n");   
	printf("  �U                        �U \n");    
	printf("  �U  4     ͼ��黹        �U \n");    
	printf("  �U                        �U \n");
	printf("  �U  5     ͼ��ԤԼ        �U \n");   
	printf("  �U                        �U \n");    
	printf("  �U  6     �г���������    �U \n");
	printf("  �U                        �U \n");
	printf("  �U  7     �鿴ͼ��״̬    �U \n");
	printf("  �U                        �U \n");
	printf("  �U  8     �������        �U \n");
	printf("  �U                        �U \n");
	printf("  �U  9     �˳�ϵͳ        �U \n");
	printf("  �U                        �U \n");
	printf("  �t�T�T�T�T�T�T�T�T�T�T�T�T�s \n");
	PrintLib();			// ��ʾ��ͼ�����
	PrintB();			// ��ʾ������
	PrintName();		// ��ʾ������Ϣ
	PrintDate();		// ��ʾ��ǰʱ��
}
