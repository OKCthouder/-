#include"Library.h"


void InitLibrary(BTree &L) {
	L = NULL;
}

void InsertBook(BTree &L, BookType B, result res) {
	//书库L已存在，res包含B书在书库L中的位置或应该插入的位置
	//如果书库中已存在B书，则只将B书的库存量增加，否则插入B书到书库L中

	if (res.tag == 0) InsertBTree(L, B->booknum, res.pt, res.i, B);
	//书库中不存在该书，则插入
	else {
		BookType b = res.pt->recptr[res.i];
		b->current = b->current + B->total;//现存量增加
		b->total = b->total + B->total;//总库存增加
	}
}

int DeleteBook(BTree &L, BookType B) {
	//如果书库中存在B书，则从书库中删除B书的信息，并返回OK，否则返回ERROR
	if (DeleteBTree(L, B->booknum)) return OK;//删除成功
	else return ERROR;//删除失败
}

int CanBorrow(BTree L, BookType B, ReaderType R) {
	//如果书库中存在B书，若B书现存量大于0，则可出借返回OK，否则返回ERROR
	if(B->current>0) return TRUE;//现存量大于零
	else return FALSE;//其他情况均不允许出借
}

void	BorrowBook(BTree L, BookType B, ReaderType R)
// 书库L存在，B书是书库中的书并且可被读者R借阅（已通过CanBorrow()判断）
// 借出一本B书，登记借阅者R的信息，改变现存量，记录借书日期，最迟应还日期等信息。
{
	ReaderType	r;
	if (!B->reader) B->reader = R;				// 无其他借阅者，则直接登记
	else {
		for (r = B->reader; r->rnext; r = r->rnext);
		r->rnext = R;							// 否则到借阅者链表尾，登记
	}
	B->current--;								// 现存量减1

}

int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R) {
	//booknum为还书书号，IDnum是借阅者的ID号
	//若书库中不存在书号为booknum的书，则搜索出错，返回-1
	//若有借阅记录，则注销该记录，并用B和R返回图书信息和借阅者信息并返回1
	//若没有r借阅的记录，则用B返回图书信息，并返回0
	result res = SearchBTree(L, booknum); //在B树上按书号搜索
	if (res.tag == 0) return -1;
	B = res.pt->recptr[res.i];
	ReaderType p = res.pt->recptr[res.i]->reader, pre = p;
	while (p) {
		if (pre == p&&p->IDnum == IDnum) {
			R = p;
			B->current++;//现存量加一
			return 1;
		}
		if (p->IDnum == IDnum) {//多个借书者
			R = p;
			pre->rnext = p->rnext;
			B->current++;//现存量+1
			return 1;
		}
		pre = p;
		p = p->rnext;
	}
	return 0;
}

void PrintH() {//表格头部格式
	printf("\n");
	printf("|*********************************图书基本信息**********************************|\n"); 
	printf("|  书号  |       书名         |      作者     | 现存 | 总库存 | 出版年份 | 定价 |\n");
}

void PrintB(BookType B) { //显示B书基本信息
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
	printf("|  %-4d  |%-20s|%-15s|", B->booknum,B->bookname,B->writer);
	printf(" %-5d|   %-4d | %-6d   |%-6.1f|\n",  B->current, B->total, B->published_year, B->price);
}

void PrintT() {//表格底部格式
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
}

void PrintAll(BTree p) {//显示B树每个结点的记录信息
	int i;
	for (i = 1; i <= p->keynum; ++i) {
		PrintB(p->recptr[i]);
	}
}

void PrintBook(BookType B) {//以表格形式显示一本书的基本信息
	PrintH();
	PrintB(B);
	PrintT();
	printf("\n");
}

void PrintBorrower(ReaderType R) {//以表格形式显示一本书的借阅者信息
	ReaderType p = R;
	printf("|------------------------|\n");
	while (p) {
		printf("|  %-5d  |  %-10s  |\n", p->IDnum, p->reader_name);
		p = p->rnext;
	}
	printf("|------------------------|\n");
}

void PrintAllbooks(BTree L) {//显示书库L所有图书信息
	PrintH();
	BTreeTraverse(L, PrintAll);
	PrintT();
}

int ShowBookinfo(BTree L, int booknum) {
	//若书库L中存在书号为booknum的书，则现在该书的所有信息并返回OK，书库L不存在则
	//返回ERROR
	result res = SearchBTree(L, booknum);
	if (!res.tag) return ERROR;
	BookType B = res.pt->recptr[res.i];
	PrintBook(B);
	return OK;
}

void 	gotoxy(short x, short y)
// 移动光标到（x，y）坐标，25>x>=0,80>y>=0	
{
	COORD point = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void 	SetColor(unsigned short TextColor)
// 设置字体和背景颜色
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, TextColor);
}

void	Welcome(int color)
// 显示欢迎界面
{
	SetColor(color);
	gotoxy(2, 5);
	printf("        ╭╮╭╮╭──╮╭╮　　╭──╮╭──╮╭╭╮╮╭──╮\n");
	printf("        │││││╭─╯││　　│╭─╯│╭╮││　　││╭─╯\n");
	printf("        │╰╯││╰─╮││　　││　　│││││││││╰─╮\n");
	printf("        │││││╭─╯││　　││　　│││││╭╮││╭─╯\n");
	printf("        │　　││╰─╮│╰─╮│╰─╮│╰╯││││││╰─╮\n");
	printf("        ╰╰╯╯╰──╯╰──╯╰──╯╰──╯╰╯╰╯╰──╯\n");
	SetColor(11);
	printf("                      ╔══════════════╗\n");
	printf("                      ║    欢迎进入图书管理系统    ║\n");
	printf("                      ╚══════════════╝\n");
	SetColor(color+1);
}

void	Creat(BTree &L) {
	// **********************************************************************************
	// 创建函数。当显示菜单时按8即可进入本创建函数。
	// L为书库。
	// 从指定文件读入图书信息进行图书入库。
	// 然后从文件读入借阅者名字，用随机数产生借阅证号和借阅书号，进行图书借阅，并记录日志，
	// 接着读入预约者名字，用随机数产生借阅证号和图书证号，进行图书预约，并记录日志
	// 完成后即可返回主菜单，进行还书、预约或其他测试。
	FILE		*fp;
	BookType	B;
	result		res;
	errno_t err;
	ReaderType	R;
	//fp = fopen_s(bookinfomationfile, "r");				// 打开图书信息文件
	if ((err = fopen_s(&fp, "bookinfo.dat", "r")) != 0)
	{
		printf("不能打开图书信息文件,请确认%s文件已放到盘根目录...", bookinfomationfile);
		getchar();
		return;
	}
	printf("\n下面将从文件%s读入图书信息。按任意键继续...", bookinfomationfile);
	getchar();
	while (!feof(fp))								// 当未到文件结束
	{
		B = (BookType)malloc(sizeof(BookNode));		// 申请图书空间
		fscanf(fp, "%d %s %s %d %d %d %f", &B->booknum, B->bookname, B->writer, &B->current,
			&B->total, &B->published_year, &B->price);// 读入图书数据
		B->reader = NULL;			// 读者和借阅者指针置空
		res = SearchBTree(L, B->booknum);			// 查找插入位置
		if (!res.tag)								// 书库中不存在该书，则插入
		{
			InsertBook(L, B, res);					// 插入
			printf("\n\n\n 插入下面的图书");
			PrintBook(B);							// 显示该图书
			printf("\nB树的状态\n\n");
			ShowBTree(L);							// 显示插入后B树的状态
		}
	}
	fclose(fp);
	printf("\n\n读入的所有图书信息");
	PrintAllbooks(L);								// 显示所有图书
	printf("\n文件图书信息读入完毕!下面从文件读入借书者信息，按任意键继续...\n");
	getchar();
// **********************************************************************************
	system("cls");
	int		booknum, i = 0;
	fp = fopen(borrowerfile, "r");					// 打开借阅者名单文件
	if (!fp)
	{
		printf("\n\n\n不能打开读者信息文件,请确认%s文件已放到盘根目录...", borrowerfile);
		getch();
		return;
	}

	while (i++ <4000 && !feof(fp))
	{
		R = (ReaderType)malloc(sizeof(ReaderNode));
		fscanf(fp, "%6s", &R->reader_name);					// 从文件读入借阅者名字
		R->IDnum = rand() % 100;					// 用随机数产生借阅证号
		R->rnext = NULL;					// 后续借阅者预约者指针置空
		booknum = rand() % 100 + 1;					// 随机产生借阅书号
		res = SearchBTree(L, booknum);				// 搜索二叉树
		if (res.tag)								// 如果找到该书号
		{
			B = res.pt->recptr[res.i];
			if (CanBorrow(L, B, R))					// 判断该借阅者是否可以借阅该书
			{
				BorrowBook(L, B, R);				// 如果可以则借出一本
			}
		}
	}

	fclose(fp);
	PrintAllbooks(L);							// 显示所有图书（可以看到现存量减少）
	printf("\n借书信息读入完毕，下面将从文件读入预约者信息。按任意键继续....");
	getch();
}