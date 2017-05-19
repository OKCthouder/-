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
	RecordLogs(0);					// ��¼��־--"����ϵͳ"
	int		i;
	for(i = 0; i <= 10;i++)			// ��ʾ��ӭ����
		{
		Welcome(i%16);
		Sleep(100);
		}  
	InitLibrary(L);					// ��ʼ�����L
	while(1)
	{
	Menu();							// ��ʾ�˵�
	cmd = getch();				
	system("cls");
	switch(cmd-'0')					// �ж��û�ѡ��
		{		
		case 1:	// ͼ�����					
			while(cmd != 'M' && cmd != 'm')
			{
			B = (BookType)malloc(sizeof(BookNode));		
			B->bespeaker = B->reader = NULL;				// ��һ�������ߺ�ԤԼ��ָ���ÿ�
			printf("\n\n\t������Ҫ������ţ�");
			fflush(stdin);
			scanf("%d",&B->booknum);
			res = SearchBTree(L, B->booknum);				// ����������
			if(res.tag)										// ������Ѵ��ڸ���ŵ���
				{
				PrintBook(res.pt->rec[res.i]);				// ��ʾ�Ȿ��
				printf("\n\n\t�����Ѵ�������,����������������: ");
				fflush(stdin);
				scanf("%d",&B->total);						
				InsertBook(L, B, res);						// ��ͼ����⣬��������
				free(B);									
				}
			else{											// ����в����ڸ���ţ�����뵽���L��		
				fflush(stdin);
				printf("\n\t��������� ����: ");
				gets(B->bookname);
				printf("\n\t�������������: ");
				fflush(stdin);
				gets(B->writer);
				printf("\n\t������������: ");
				fflush(stdin);
				scanf("%d",&B->current);
				B->total = B->current;
				printf("\n\t��������������ݣ�");
				fflush(stdin);
				scanf("%d",&B->publishyear);
				fflush(stdin);
				printf("\n\t��������鶨�ۣ�Ԫ����");
				scanf("%f",&B->price);
				InsertBook(L, B, res);
				printf("\n\t�����B������:\n\n");
				ShowBTree(L);								// ��ʾ�����B��״̬
				}
			RecordLogs(1, B, NULL,1);
			printf("\n\n\tͼ�������ɣ���M���������˵������������������ͼ�����....");
			cmd = getch();	
			}
			break;


		case 2:	// ������											
			while(cmd != 'M' && cmd != 'm')
			{
			printf("\n\n\t������Ҫ������ͼ�����: ");
			fflush(stdin);
			scanf("%d",&booknum);
			res = SearchBTree(L, booknum);					// �����û���������
			if(res.tag)										// ������ҵ�
				{
				B = res.pt->rec[res.i];
				PrintBook(B);								// ��ʾ�ҵ�����
				printf("\tȷ��ɾ�������ͼ��<Y/N>?");	// ��ʾ�Ƿ�ȷ��ɾ��
				cmd = getch();
				if(cmd == 'Y' || cmd == 'y')				// ���ȷ��ɾ��
					{							
					RecordLogs(2, B, NULL,1);				// ��¼��־--ɾ��ͼ��ɹ�
					DeleteBook(L, B);						// ɾ��ͼ��
					printf("\n\n\tͼ��%d�������������!\n\n\tɾ����B������",booknum);
					ShowBTree(L);							// ��ʾɾ����B��״̬
					}
				else	RecordLogs(2, B, NULL,0);					
				}
			else	printf("\n\n\t����в��������Ϊ%d����!",booknum);
			printf("\n\n\t��'M'�������˵������������������������...");
			cmd = getch();
			}
			break;


		case 3: // ͼ�����
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t������Ҫ���ĵ�ͼ����ţ� ");
			fflush(stdin);
			scanf("%d",&booknum);
			res = SearchBTree(L, booknum);					// �����������ͼ��booknum
			if(res.tag)										// ����ҵ�
				{
				R = (ReaderType)malloc(sizeof(ReaderNode)); // ������һ�����߿ռ�
				R->nextr = R->nextb = NULL;					// ��һ�������ߺ�ԤԼ��ָ���ÿ�
				B = res.pt->rec[res.i];
				printf("\n\n\t�����ҵ�ͼ�����£�");
				PrintBook(B);								// ��ʾ�ҵ���ͼ��
				printf("\n\n\t���������Ľ���֤�ţ�");		// �����������Ϣ
				fflush(stdin);
				scanf("%d",&R->cardnum);
				printf("\n\n\t��������������: ");
				fflush(stdin);
				gets(R->rname);
				if(CanBorrow(L, B, R))						// ����ý����߿��Խ��ĸ���
					{
					BorrowBook(L, B, R);					// ���һ��
					RecordLogs(4,B,R,1);					// ��¼��־--����ɹ�
					printf("\n\n\t����ɹ�!");
					}
				else{
					printf("\n\n\t�Բ��������ܽ��ĸ���!�����ִ�������0���ѱ�����ԤԼ��");
					RecordLogs(4,B,R,0);					// ��¼��־--����ʧ��
					free(R);								// �ͷŸö��߿ռ�
					}
				}
			else printf("\n\n\t����в�����ͼ��%d!",booknum);
			printf("\n\n\t��'M'�������˵����������������������ͼ��...");
			cmd = getch();
			}
			break;


		case 4: // ͼ��黹
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t��������Ҫ�黹��ͼ���: ");
			fflush(stdin);
			scanf("%d",&booknum);
			printf("\n\n\t��������Ľ���֤��: ");
			fflush(stdin);
			scanf("%d",&cardnum);
			int		k = ReturnBook(L, booknum, cardnum, B, R);// Ϊ����cardnum����
			if(k == 1)										// �������ɹ�
				{
				tm	today;
				GetDate(today);								// ��ȡ��ǰϵͳ����
				if(!Earlier(today, R->retdate))				// �������
					{
					for(k = -1; Earlier(R->retdate,today);k++ )
						AddDate(R->retdate, R->retdate, 1);	// ������������k
					double	price = (float)k * OUT_DATE_PRICE;	// �������ڷ���OUT_DATE_PRICEԪ/��	
					printf("\n\n\t��������Ѿ�����%d�죬��۳����ڷ���%0.1fԪ!",k,price);
					}
				RecordLogs(5, B, R, 1);						// ��¼��־--����ɹ�
				printf("\n\n\t����ɹ�!");
				free(R);									// �ͷŸö��߽����¼
				}
			else if(k == 0)									// ���û�иö��߽��ĸ���ļ�¼
				{
				R = (ReaderType)malloc(sizeof(ReaderNode));
				R->cardnum = cardnum;
				strcpy(R->rname,"###");
				RecordLogs(5,B, R,0);						// ��¼��־--����ʧ��
				printf("\n\n\tû������ͼ��%d�ļ�¼!",booknum);
				free(R);
				}
			else printf("\n\n\t����в�����ͼ��%d!",booknum);
			printf("\n\n\t��'M'�������˵����������������������...");
			cmd = getch();
			}
			break;


		case 5: // ͼ��ԤԼ
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			R = (ReaderType)malloc(sizeof(ReaderNode));
			R->nextr = R->nextb = NULL;
			printf("\n\n\t��������ҪԤԼ�����: ");
			fflush(stdin);
			scanf("%d",&booknum);							// ����ԤԼ���
			res = SearchBTree(L, booknum);					// ����������Ƿ���ڸ���
			if(res.tag)										// �������
				{
				B = res.pt->rec[res.i];						// ������߿ռ�
				PrintBook(B);								// ��ʾ����
				printf("\n\n\t���������Ľ���֤�ţ�");		// �����������Ϣ
				fflush(stdin);
				scanf("%d",&R->cardnum);
				printf("\n\n\t��������������: ");
				fflush(stdin);
				gets(R->rname);
				if(BespeakBook(B, R))						// ����ö��߿���ԤԼ����
					{
					RecordLogs(6, B, R, 1);					// ��¼��־--ԤԼ�ɹ�
					printf("\n\n\tԤԼ�ɹ�!");
					}
				else										// ���������ԤԼ
					{
					printf("\n\n\t�Բ���������ԤԼ��ͼ��,�����ִ�������0���Ѵ�������ԤԼ��¼!");
					RecordLogs(6, B, R, 0);					// ��¼��־--ԤԼʧ��		
					free(R);								// �ͷŽ����߿ռ�
					}	
				}
			else{											// ��������δ�ҵ�����
				printf("\n\n\t����в�����ͼ��%d,��ȷ��!",booknum);
				free(R);									// �ͷŶ��߿ռ�
				}
			printf("\n\n\t��'M'�������˵������������������ԤԼͼ��...");
			cmd = getch();
			}
			break;


		case 6: // �г�ĳ��������������
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t��������Ҫ���ҵ���������: ");
			fflush(stdin);
			gets(author);									// ������������authorΪȫ���ַ��������
			books_counter = 0;
			if(ListAuthor(L)) RecordLogs(7,NULL,NULL,1);	// �ҵ������߼�¼���о٣�����¼��־-
			else{											// ���δ�ҵ�����������
				RecordLogs(7,NULL,NULL,0);					// ��¼��־-�о���������ʧ��
				printf("\n\n\t����в����� %s ������!",author);
				}
			printf("\n\n\t��'M'�������˵�����������������������������ߵ�������...");
			cmd = getch();
			}
			break;			


		case 7: // �鿴ĳ��ͼ��״̬
			while(cmd != 'M' && cmd != 'm')
			{
			system("cls");
			printf("\n\n\t��������Ҫ�鿴��ͼ�����: ");
			fflush(stdin);
			scanf("%d",&booknum);							// �������
			if(ShowBookinfo(L, booknum))					// ����ҵ����飬��ʾ������ϸ��Ϣ
				{
				res = SearchBTree(L, booknum);				
				RecordLogs(3, res.pt->rec[res.i], NULL, 1);	// ��¼��־-�鿴ͼ��״̬�ɹ�
				}
			else{											// �������в����ڸ���
				B = (BookType)malloc(sizeof(BookNode));		
				B->booknum = booknum;
				strcpy(B->bookname,"######");				// ���ڲ����ڸ��飬��������Ϊ######
				RecordLogs(3, B, NULL, 0);					// ��¼��־-�鿴ʧ��
				free(B);
				printf("\n\n\t����в�����ͼ��%d !",booknum);
				}
			printf("\n\n\t��'M'�������˵�������������������鿴ͼ��״̬...");
			cmd = getch();
			}
			break;


		case 8:// �鿴�������ͼ����Ϣ						
			printf("\n\n\t�������ͼ����Ϣ:");
			PrintAllBooks(L);								// ��ӡ���������ͼ����Ϣ
			RecordLogs(9);									// ��¼��־-��ӡ����ͼ����Ϣ�ɹ�
			printf("\n\n\t��������������˵�...");
			cmd = getch();
			break;


		case 9: 
			system("cls");
			printf("\n\n\n\n\n\t�˳�ϵͳ,ȷ��<Y/N>?...");	// ��ʾ�Ƿ�ȷ���˳�ϵͳ
			cmd = getch();
			if(cmd == 'y' || cmd == 'Y')
				{
				RecordLogs(8);								// ��¼��־-�˳�ϵͳ
				exit(0);									// �˳�
				}
			break;


		case 0: Test(L); break;								// ����ģʽ
		// ��ָ���ļ�����ͼ�顢�����ߡ�ԤԼ����Ϣ���������Ļ����ԤԼ�������ԡ�
			
		default: break;
		}
	}
	return 0;
}



void	Test(Library &L)
// **********************************************************************************
// ���Ժ���������ʾ�˵�ʱ��0���ɽ��뱾���Ժ�����
// LΪ��⡣
// ��ָ���ļ�����ͼ����Ϣ����ͼ����⡣
// Ȼ����ļ�������������֣����������������֤�źͽ�����ţ�����ͼ����ģ�����¼��־��
// ���Ŷ���ԤԼ�����֣����������������֤�ź�ͼ��֤�ţ�����ͼ��ԤԼ������¼��־
// ��ɺ󼴿ɷ������˵������л��顢ԤԼ���������ԡ�
{
	FILE		*fp;
	BookType	B;	
	Result		res; 
	ReaderType	R;
	fp = fopen(bookinfomationfile,"r");				// ��ͼ����Ϣ�ļ�
	if(!fp) 
		{
		printf("���ܴ�ͼ����Ϣ�ļ�,��ȷ��%s�ļ��ѷŵ��̸�Ŀ¼...",bookinfomationfile);
		getch();
		return ;
		}
	printf("\n���潫���ļ�%s����ͼ����Ϣ�������������...",bookinfomationfile);
	getch();
	while(!feof(fp))								// ��δ���ļ�����
		{
		B = (BookType)malloc(sizeof(BookNode));		// ����ͼ��ռ�
		fscanf(fp,"%d %s %s %d %d %d %f",&B->booknum, B->bookname, B->writer, &B->current,
										&B->total, &B->publishyear, &B->price);// ����ͼ������
		B->reader = B->bespeaker = NULL;			// ���ߺͽ�����ָ���ÿ�
		res = SearchBTree(L,B->booknum);			// ���Ҳ���λ��
		if(!res.tag)								// ����в����ڸ��飬�����
			{
			InsertBook(L, B, res);					// ����
			RecordLogs(1, B);						// ��¼��־-���ɹ�
			printf("\n\n\n ���������ͼ��");			
			PrintBook(B);							// ��ʾ��ͼ��
			printf("\nB����״̬\n\n");				
			ShowBTree(L);							// ��ʾ�����B����״̬
			}
		}
	fclose(fp);
	printf("\n\n���������ͼ����Ϣ");
	PrintAllBooks(L);								// ��ʾ����ͼ��
	printf("\n�ļ�ͼ����Ϣ�������!������ļ������������Ϣ�������������...");
	getch();
// ***************************************************************************************
	system("cls");
	int		booknum, i = 0;
	fp = fopen(borrowerfile,"r");					// �򿪽����������ļ�
	if(!fp) 
		{
		printf("\n\n\n���ܴ򿪶�����Ϣ�ļ�,��ȷ��%s�ļ��ѷŵ��̸�Ŀ¼...",borrowerfile);
		getch();
		return;
		}

	while(i++ <4000 && !feof(fp))					
		{
		R = (ReaderType)malloc(sizeof(ReaderNode));
		fscanf(fp,"%6s",&R->rname);					// ���ļ��������������
		R->cardnum = rand() % 100;					// ���������������֤��
		R->nextb = R->nextr = NULL;					// ����������ԤԼ��ָ���ÿ�
		booknum = rand() % 100+1;					// ��������������
		res = SearchBTree(L, booknum);				// ����������
		if(res.tag )								// ����ҵ������
			{
			B = res.pt->rec[res.i];					
			if(CanBorrow(L, B, R))					// �жϸý������Ƿ���Խ��ĸ���
				{
				BorrowBook(L, B, R);				// �����������һ��
				RecordLogs(4,B,R,1);				// ��¼��־--����ɹ�
				}
			else									// ������ܽ�
				{		
				RecordLogs(4,B,R,0);				// ��¼��־-����ʧ�� 
				}
			}
		}
		
		fclose(fp);
		PrintAllBooks(L);							// ��ʾ����ͼ�飨���Կ����ִ������٣�
		printf("\n������Ϣ������ϣ����潫���ļ�����ԤԼ����Ϣ�������������....");
		getch();
// ************************************************************************************
	system("cls");
	fp = fopen(bespeakerfile,"r");					// ��ԤԼ�������ļ�
	if(!fp) 
		{
		printf("\n\n\n���ܴ򿪶�����Ϣ�ļ�,��ȷ��%s�ļ��ѷŵ��̸�Ŀ¼...",bespeakerfile);
		getch();
		return;
		}
	while(i-- > 0 && !feof(fp))
		{
		R = (ReaderType)malloc(sizeof(ReaderNode));
		fscanf(fp,"%6s",&R->rname);					// ���ļ�����ԤԼ������
		R->nextb = R->nextr = NULL;					
		R->cardnum = rand() % 100;					// �����������֤��
		booknum = rand() % 100+1;					// �������ԤԼ���
		res = SearchBTree(L, booknum);				// �����������
		if(res.tag)									// ����ҵ�
			{
			B = res.pt->rec[res.i];	
			if(BespeakBook(B, R)) RecordLogs(6,B,R,1);	// ���ԤԼ�ɹ������¼��־-ԤԼ�ɹ�
			else	RecordLogs(6,B,R,0);			// �����¼��־-ԤԼʧ��
			}
		}
	printf("\n������Ϣ������ϣ���������������˵�...");
	getch();
	fclose(fp);										// �ر��ļ�
}												
