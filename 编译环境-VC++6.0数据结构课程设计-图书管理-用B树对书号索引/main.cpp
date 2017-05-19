#include"base.h"
void	Test(Library &L);
int		main()
{	
	Library		L;					
	int			booknum,cardnum;	
	char		cmd;				
	BookType	B;				
	Result		res; 
	ReaderType	R;				
	RecordLogs(0);					// 记录日志--"进入系统"
	int		i;
	for(i = 0; i <= 10;i++)			// 显示欢迎界面
		{
		Welcome(i%16);
		Sleep(100);
		}  
	InitLibrary(L);					// 初始化书库L
	while(1)
	{
	Menu();							// 显示菜单
	cmd = getch();				
	system("cls");
	switch(cmd-'0')					// 判断用户选择
		{		
		case 1:	// 图书入库					
			while(cmd != 'M' && cmd != 'm')
			{
			B = (BookType)malloc(sizeof(BookNode));		
			B->bespeaker = B->reader = NULL;				// 下一个借阅者和预约者指针置空
			printf("\n\n\t请输入要入库的书号：");
			fflush(stdin);
			scanf("%d",&B->booknum);
			res = SearchBTree(L, B->booknum);				// 查找入库书号
			if(res.tag)										// 书库中已存在该书号的书
				{
				PrintBook(res.pt->rec[res.i]);				// 显示这本书
				printf("\n\n\t该书已存在如上,请输入新增入库册数: ");
				fflush(stdin);
				scanf("%d",&B->total);						
				InsertBook(L, B, res);						// 该图书入库，数量增加
				free(B);									
				}
			else{											// 书库中不存在该书号，则插入到书库L中		
				fflush(stdin);
				printf("\n\t请输入该书 书名: ");
				gets(B->bookname);
				printf("\n\t请输入该书著者: ");
				fflush(stdin);
				gets(B->writer);
				printf("\n\t请输入该书册数: ");
				fflush(stdin);
				scanf("%d",&B->current);
				B->total = B->current;
				printf("\n\t请输入该书出版年份：");
				fflush(stdin);
				scanf("%d",&B->publishyear);
				fflush(stdin);
				printf("\n\t请输入该书定价（元）：");
				scanf("%f",&B->price);
				InsertBook(L, B, res);
				printf("\n\t插入后B树如下:\n\n");
				ShowBTree(L);								// 显示插入后B树状态
				}
			RecordLogs(1, B, NULL,1);
			printf("\n\n\t图书入库完成，按M键返回主菜单，按其他任意键继续图书入库....");
			cmd = getch();	
			}
			break;


		case 2:	// 清除库存											
			while(cmd != 'M' && cmd != 'm')
			{
			printf("\n\n\t请输入要清除库存图书书号: ");
			fflush(stdin);
			scanf("%d",&booknum);
			res = SearchBTree(L, booknum);					// 查找用户输入的书号
			if(res.tag)										// 如果查找到
				{
				B = res.pt->rec[res.i];
				PrintBook(B);								// 显示找到的书
				printf("\t确认删除上面的图书<Y/N>?");	// 提示是否确认删除
				cmd = getch();
				if(cmd == 'Y' || cmd == 'y')				// 如果确认删除
					{							
					RecordLogs(2, B, NULL,1);				// 记录日志--删除图书成功
					DeleteBook(L, B);						// 删除图书
					printf("\n\n\t图书%d从书库中清除完毕!\n\n\t删除后B树如下",booknum);
					ShowBTree(L);							// 显示删除后B树状态
					}
				else	RecordLogs(2, B, NULL,0);					
				}
			else	printf("\n\n\t书库中不存在书号为%d的书!",booknum);
			printf("\n\n\t按'M'返回主菜单，按其他任意键继续清除库存...");
			cmd = getch();
			}
			break;


		case 3: // 图书出借
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t请输入要借阅的图书书号： ");
			fflush(stdin);
			scanf("%d",&booknum);
			res = SearchBTree(L, booknum);					// 在书库中搜索图书booknum
			if(res.tag)										// 如果找到
				{
				R = (ReaderType)malloc(sizeof(ReaderNode)); // 新申请一个读者空间
				R->nextr = R->nextb = NULL;					// 下一个借阅者和预约者指针置空
				B = res.pt->rec[res.i];
				printf("\n\n\t您查找的图书如下：");
				PrintBook(B);								// 显示找到的图书
				printf("\n\n\t请输入您的借书证号：");		// 读入借阅者信息
				fflush(stdin);
				scanf("%d",&R->cardnum);
				printf("\n\n\t请输入您的姓名: ");
				fflush(stdin);
				gets(R->rname);
				if(CanBorrow(L, B, R))						// 如果该借阅者可以借阅该书
					{
					BorrowBook(L, B, R);					// 借出一本
					RecordLogs(4,B,R,1);					// 记录日志--借书成功
					printf("\n\n\t借书成功!");
					}
				else{
					printf("\n\n\t对不起，您不能借阅该书!该书现存量少于0或已被他人预约。");
					RecordLogs(4,B,R,0);					// 记录日志--借书失败
					free(R);								// 释放该读者空间
					}
				}
			else printf("\n\n\t书库中不存在图书%d!",booknum);
			printf("\n\n\t按'M'返回主菜单，按其他任意键继续借阅图书...");
			cmd = getch();
			}
			break;


		case 4: // 图书归还
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t请输入你要归还的图书号: ");
			fflush(stdin);
			scanf("%d",&booknum);
			printf("\n\n\t请输入你的借书证号: ");
			fflush(stdin);
			scanf("%d",&cardnum);
			int		k = ReturnBook(L, booknum, cardnum, B, R);// 为读者cardnum还书
			if(k == 1)										// 如果还书成功
				{
				tm	today;
				GetDate(today);								// 获取当前系统日期
				if(!Earlier(today, R->retdate))				// 如果逾期
					{
					for(k = -1; Earlier(R->retdate,today);k++ )
						AddDate(R->retdate, R->retdate, 1);	// 计算逾期天数k
					double	price = (float)k * OUT_DATE_PRICE;	// 计算逾期费用OUT_DATE_PRICE元/天	
					printf("\n\n\t您借的书已经逾期%d天，需扣除逾期费用%0.1f元!",k,price);
					}
				RecordLogs(5, B, R, 1);						// 记录日志--还书成功
				printf("\n\n\t还书成功!");
				free(R);									// 释放该读者借书记录
				}
			else if(k == 0)									// 如果没有该读者借阅该书的记录
				{
				R = (ReaderType)malloc(sizeof(ReaderNode));
				R->cardnum = cardnum;
				strcpy(R->rname,"###");
				RecordLogs(5,B, R,0);						// 记录日志--还书失败
				printf("\n\n\t没有您借图书%d的记录!",booknum);
				free(R);
				}
			else printf("\n\n\t书库中不存在图书%d!",booknum);
			printf("\n\n\t按'M'返回主菜单，按其它任意键继续还书...");
			cmd = getch();
			}
			break;


		case 5: // 图书预约
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			R = (ReaderType)malloc(sizeof(ReaderNode));
			R->nextr = R->nextb = NULL;
			printf("\n\n\t请输入您要预约的书号: ");
			fflush(stdin);
			scanf("%d",&booknum);							// 读入预约书号
			res = SearchBTree(L, booknum);					// 搜索书库中是否存在该书
			if(res.tag)										// 如果存在
				{
				B = res.pt->rec[res.i];						// 申请读者空间
				PrintBook(B);								// 显示该书
				printf("\n\n\t请输入您的借阅证号：");		// 读入借阅者信息
				fflush(stdin);
				scanf("%d",&R->cardnum);
				printf("\n\n\t请输入您的姓名: ");
				fflush(stdin);
				gets(R->rname);
				if(BespeakBook(B, R))						// 如果该读者可以预约该书
					{
					RecordLogs(6, B, R, 1);					// 记录日志--预约成功
					printf("\n\n\t预约成功!");
					}
				else										// 如果不可以预约
					{
					printf("\n\n\t对不起，您不能预约该图书,该书现存量大于0或已存在您的预约记录!");
					RecordLogs(6, B, R, 0);					// 记录日志--预约失败		
					free(R);								// 释放借阅者空间
					}	
				}
			else{											// 如果书库中未找到该书
				printf("\n\n\t书库中不存在图书%d,请确认!",booknum);
				free(R);									// 释放读者空间
				}
			printf("\n\n\t按'M'返回主菜单，按其它任意键继续预约图书...");
			cmd = getch();
			}
			break;


		case 6: // 列出某著者所有著作名
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t请输入你要查找的著者名字: ");
			fflush(stdin);
			gets(author);									// 读入著者名，author为全局字符数组变量
			books_counter = 0;
			if(ListAuthor(L)) RecordLogs(7,NULL,NULL,1);	// 找到该著者记录，列举，并记录日志-
			else{											// 如果未找到该著者著作
				RecordLogs(7,NULL,NULL,0);					// 记录日志-列举著者著作失败
				printf("\n\n\t书库中不存在 %s 的著作!",author);
				}
			printf("\n\n\t按'M'返回主菜单，按其它任意键继续查找其它著者的著作名...");
			cmd = getch();
			}
			break;			


		case 7: // 查看某部图书状态
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t请输入你要查看的图书书号: ");
			fflush(stdin);
			scanf("%d",&booknum);							// 读入书号
			if(ShowBookinfo(L, booknum))					// 如果找到该书，显示该书详细信息
				{
				res = SearchBTree(L, booknum);				
				RecordLogs(3, res.pt->rec[res.i], NULL, 1);	// 记录日志-查看图书状态成功
				}
			else{											// 如果书库中不存在该书
				B = (BookType)malloc(sizeof(BookNode));		
				B->booknum = booknum;
				strcpy(B->bookname,"######");				// 由于不存在该书，令其书名为######
				RecordLogs(3, B, NULL, 0);					// 记录日志-查看失败
				free(B);
				printf("\n\n\t书库中不存在图书%d !",booknum);
				}
			printf("\n\n\t按'M'返回主菜单，按其它任意键继续查看图书状态...");
			cmd = getch();
			}
			break;


		case 8:// 查看书库所有图书信息						
			printf("\n\n\t书库所有图书信息:");
			PrintAllBooks(L);								// 打印书库中所有图书信息
			RecordLogs(9);									// 记录日志-打印所有图书信息成功
			printf("\n\n\t按任意键返回主菜单...");
			cmd = getch();
			break;


		case 9: 
			system("cls");
			printf("\n\n\n\n\n\t退出系统,确认<Y/N>?...");	// 提示是否确认退出系统
			cmd = getch();
			if(cmd == 'y' || cmd == 'Y')
				{
				RecordLogs(8);								// 记录日志-退出系统
				exit(0);									// 退出
				}
			break;


		case 0: Test(L); break;								// 测试模式
		// 从指定文件读入图书、借阅者、预约者信息，方便后面的还书和预约操作测试。
			
		default: break;
		}
	}
	return 0;
}



void	Test(Library &L)
// **********************************************************************************
// 测试函数。当显示菜单时按0即可进入本测试函数。
// L为书库。
// 从指定文件读入图书信息进行图书入库。
// 然后从文件读入借阅者名字，用随机数产生借阅证号和借阅书号，进行图书借阅，并记录日志，
// 接着读入预约者名字，用随机数产生借阅证号和图书证号，进行图书预约，并记录日志
// 完成后即可返回主菜单，进行还书、预约或其他测试。
{
	FILE		*fp;
	BookType	B;	
	Result		res; 
	ReaderType	R;
	fp = fopen(bookinfomationfile,"r");				// 打开图书信息文件
	if(!fp) 
		{
		printf("不能打开图书信息文件,请确认%s文件已放到盘根目录...",bookinfomationfile);
		getch();
		return ;
		}
	printf("\n下面将从文件%s读入图书信息。按任意键继续...",bookinfomationfile);
	getch();
	while(!feof(fp))								// 当未到文件结束
		{
		B = (BookType)malloc(sizeof(BookNode));		// 申请图书空间
		fscanf(fp,"%d %s %s %d %d %d %f",&B->booknum, B->bookname, B->writer, &B->current,
										&B->total, &B->publishyear, &B->price);// 读入图书数据
		B->reader = B->bespeaker = NULL;			// 读者和借阅者指针置空
		res = SearchBTree(L,B->booknum);			// 查找插入位置
		if(!res.tag)								// 书库中不存在该书，则插入
			{
			InsertBook(L, B, res);					// 插入
			RecordLogs(1, B);						// 记录日志-入库成功
			printf("\n\n\n 插入下面的图书");			
			PrintBook(B);							// 显示该图书
			printf("\nB树的状态\n\n");				
			ShowBTree(L);							// 显示插入后B树的状态
			}
		}
	fclose(fp);
	printf("\n\n读入的所有图书信息");
	PrintAllBooks(L);								// 显示所有图书
	printf("\n文件图书信息读入完毕!下面从文件读入借书者信息，按任意键继续...");
	getch();
// ***************************************************************************************
	system("cls");
	int		booknum, i = 0;
	fp = fopen(borrowerfile,"r");					// 打开借阅者名单文件
	if(!fp) 
		{
		printf("\n\n\n不能打开读者信息文件,请确认%s文件已放到盘根目录...",borrowerfile);
		getch();
		return;
		}

	while(i++ <4000 && !feof(fp))					
		{
		R = (ReaderType)malloc(sizeof(ReaderNode));
		fscanf(fp,"%6s",&R->rname);					// 从文件读入借阅者名字
		R->cardnum = rand() % 100;					// 用随机数产生借阅证号
		R->nextb = R->nextr = NULL;					// 后续借阅者预约者指针置空
		booknum = rand() % 100+1;					// 随机产生借阅书号
		res = SearchBTree(L, booknum);				// 搜索二叉树
		if(res.tag )								// 如果找到该书号
			{
			B = res.pt->rec[res.i];					
			if(CanBorrow(L, B, R))					// 判断该借阅者是否可以借阅该书
				{
				BorrowBook(L, B, R);				// 如果可以则借出一本
				RecordLogs(4,B,R,1);				// 记录日志--借书成功
				}
			else									// 如果不能借
				{		
				RecordLogs(4,B,R,0);				// 记录日志-借书失败 
				}
			}
		}
		
		fclose(fp);
		PrintAllBooks(L);							// 显示所有图书（可以看到现存量减少）
		printf("\n借书信息读入完毕，下面将从文件读入预约者信息。按任意键继续....");
		getch();
// ************************************************************************************
	system("cls");
	fp = fopen(bespeakerfile,"r");					// 打开预约者名单文件
	if(!fp) 
		{
		printf("\n\n\n不能打开读者信息文件,请确认%s文件已放到盘根目录...",bespeakerfile);
		getch();
		return;
		}
	while(i-- > 0 && !feof(fp))
		{
		R = (ReaderType)malloc(sizeof(ReaderNode));
		fscanf(fp,"%6s",&R->rname);					// 从文件读入预约者姓名
		R->nextb = R->nextr = NULL;					
		R->cardnum = rand() % 100;					// 随机产生借阅证号
		booknum = rand() % 100+1;					// 随机产生预约书号
		res = SearchBTree(L, booknum);				// 在书库中搜索
		if(res.tag)									// 如果找到
			{
			B = res.pt->rec[res.i];	
			if(BespeakBook(B, R)) RecordLogs(6,B,R,1);	// 如果预约成功，则记录日志-预约成功
			else	RecordLogs(6,B,R,0);			// 否则记录日志-预约失败
			}
		}
	printf("\n所有信息载入完毕，按任意键返回主菜单...");
	getch();
	fclose(fp);										// 关闭文件
}												
