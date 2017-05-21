#include"Library.h"

int main() {
	BTree L = NULL;
	BookType B;
	result res;
	ReaderType R;
	int booknum, IDnum, k, book_num;
	char flag;
	int i;
	for (i = 0; i <= 10; i++){			// 显示欢迎界面
		Welcome(i % 16);
		Sleep(100);
	}
	Sleep(800);
	system("cls");//清屏
	if (login()) {
		for (;;) {
			switch (menu())
			{
			case 1://采编图书
				B = (BookType)malloc(sizeof(BookNode));
				B->reader = NULL;
				printf("请输入要入库的书号：\n");
				fflush(stdin);
				scanf_s("%d", &B->booknum);
				getchar();
				res = SearchBTree(L, B->booknum);
				if (!res.tag) {
					fflush(stdin);
					printf("请输入该书书名：\n");
					fflush(stdin);
					gets_s(B->bookname);
					printf("请输入该书作者：\n");
					fflush(stdin);
					gets_s(B->writer);
					printf("请输入入库数：\n");
					fflush(stdin);
					scanf_s("%d", &B->current);
					printf("请输入出版年份：\n");
					fflush(stdin);
					scanf_s("%d", &B->published_year);
					printf("请输入书本价格：\n");
					fflush(stdin);
					scanf_s("%f", &B->price);
					B->total = B->current;
					InsertBook(L, B, res);
					printf("用凹入表形式显示B树如下：\n");
					ShowBTree(L, 8);
					break;
				}
				else {//图书已存在，只增加库存
					printf("书库已有该书记录，请输入增加的册数：\n");
					fflush(stdin);
					scanf_s("%d", &B->total);
					InsertBook(L, B, res);
					B = res.pt->recptr[res.i];
					printf("用凹入表形式显示B树如下：\n");
					ShowBTree(L);
					printf("新书入库操作完成！\n");
					system("pause");
					break;
				}


			case 2://清空库存
				printf("请输入要删除的图书书号：\n");
				fflush(stdin);
				scanf_s("%d", &booknum);
				res = SearchBTree(L, booknum);
				if (res.tag) {
					B = res.pt->recptr[res.i];
					PrintBook(B);
					printf("\n您确认删除上面的图书<Y确认，其余按键返回主菜单>?");
					fflush(stdin);
					getchar();
					flag = getchar();
					if (flag == 'Y' || flag == 'y') {
						DeleteBook(L, B);
						printf("\n清除完毕！\t删除后用凹入表显示B树如下：\n");
						ShowBTree(L, 8);
					}
				}
				else {
					printf("\n书库不存在此书，删除失败！\n");
				}
				printf("\t");
				system("pause");
				break;
			case 3://借阅图书
				R = (ReaderType)malloc(sizeof(ReaderNode));
				R->rnext = NULL;
				printf("请输入借阅图书书号：\n");
				fflush(stdin);
				scanf_s("%d", &booknum);
				getchar();
				res = SearchBTree(L, booknum);
				if (res.tag) {
					B = res.pt->recptr[res.i];
					printf("请输入您的姓名：\n");
					fflush(stdin);
					gets_s(R->reader_name);
					printf("请输入您的ID号：\n");
					fflush(stdin);
					scanf_s("%d", &R->IDnum);
					if (CanBorrow(L, B, R)) {
						BorrowBook(L, B, R);
						printf("\n借书完成！");
					}
					else {
						printf("\n库存不足，借阅失败！");
						free(R);
					}
				}
				else {
					printf("\n库存无此书，借阅失败！");
					free(R);
				}
				printf("\t");
				system("pause");
				break;
			case 4://归还图书
				printf("\n请输入图书号：\n");
				scanf_s("%d", &booknum);
				printf("请输入您的ID号：\n");
				scanf_s("%d", &IDnum);
				k = ReturnBook(L, booknum, IDnum, B, R);
				if (k == 1) {
					printf("\n还书完成！");
					R = NULL;
				}
				else if (k == 0) {
					printf("\n还书失败，系统没有您借该书记录！");
				}
				else {
					printf("\n书库中不存在此书!");
				}
				printf("\t");
				system("pause");
				break;
			case 5://输出总库存
				printf("图书馆总库存信息如下：\n");
				PrintAllbooks(L);
				break;
			case 6://查找某种图书
				printf("请输入要查找的图书的编号：\n");
				scanf_s("%d", &book_num);
				ShowBookinfo(L, book_num);
				break;
			case 7://输出某图书的借阅者信息
				printf("请输入要查找的图书的编号：\n");
				scanf_s("%d", &book_num);
				res = SearchBTree(L, book_num);
				if (res.tag) {
					B = res.pt->recptr[res.i];
					printf("借阅者信息如下：\n");
					PrintBorrower(B->reader);
				}
				else {
					printf("\n书库不存在此书，查找失败！\n");
				}
				printf("\t");
				system("pause");
				break;
			case 8:Creat(L);
				system("cls");
				break;
			case 0:  //退出管理系统
				exit(0);
			default:
				break;
			}
		}
	}
	else {
		printf("密码输入错误！\n");
		system("pause");
	}
	return 0;
}