
void	InitLibrary(Library &L )
// ��ʼ�����LΪ����⡣
{
	L = NULL;
}
	
void	InsertBook(Library &L ,BookType B , Result res)
// ���L�Ѵ��ڣ�res����B�������L�е�λ�û�Ӧ�ò����λ��
// ���������Ѵ���B�飬��ֻ��B��Ŀ�������ӣ��������B�鵽���L�С�
{
	if(res.tag == 0) 
		InsertBTree(L, B->booknum, res.pt, res.i, B);	// �������в����ڸ��飬�����
	else{										// ����Ѵ���
		BookType b = res.pt->rec[res.i];				
		b->current = b->current + B->total;		// �ִ������ܿ������
		b->total = b->total + B->total; 
		}
}

Status	DeleteBook(Library &L ,BookType B)
// �������д���B�飬��������ɾ��B�����Ϣ��������OK�����򷵻�ERROR
{
	if(DeleteBTree(L,B->booknum))	return OK;	// ���ɾ���ɹ�������OK
	else return ERROR;							// ����ɾ�����ɹ�������ERROR
}


Status	CanBorrow(Library L,BookType B ,ReaderType R)
// ���L�д���B�顣��B���ִ�������0��δ�����ˣ�����R���⣩ԤԼ��
// ���иö���ԤԼ��Ϣ��ɾ������Ϣ������TRUE
// ���򷵻�FALSE��
{
	int	tag = 0, count = 0;						// tagָʾ�ö����Ƿ�ԤԼ
	ReaderType b, pre;							
	if(B->current > 0)							// ���ִ�������0
		{
		if(!B->bespeaker) return TRUE;			// ����ԤԼ����ɳ���
		for(b = B->bespeaker; b ; pre = b,b = b->nextb,count++) 
			{											
			if(b->cardnum == R->cardnum) 		// �ж���R��ԤԼ��Ϣ
				{
				pre->nextb = b->nextb;
				free(b);						// ɾ����ԤԼ��Ϣ
				return TRUE;					
				}
			}
		if(B->current-count > 0 ) return TRUE;	// �ִ�������ԤԼ������Ҳ�ɳ���
		}		
	return FALSE;								// �ִ���Ϊ0�����ɳ���
}


void	BorrowBook(Library L ,BookType B ,ReaderType R)
// ���L���ڣ�B��������е��鲢�ҿɱ�����R���ģ���ͨ��CanBorrow()�жϣ�
// ���һ��B�飬�Ǽǽ�����R����Ϣ���ı��ִ�������¼�������ڣ����Ӧ�����ڵ���Ϣ��
{
	ReaderType	r;
	GetDate(R->bordate);						// ��ȡ��ǰ����Ϊ��������
	AddDate(R->retdate, R->bordate, KEEP_DAYS);	// ��ǰ���ڼ�90��Ϊ��ٻ�������
	if(!B->reader) B->reader = R;				// �����������ߣ���ֱ�ӵǼ�
	else{
		for(r = B->reader; r->nextr; r= r->nextr) ;
		r->nextr = R;							// ���򵽽���������β���Ǽ�
		}
	B->current--;								// �ִ�����1

}
			  
int		ReturnBook(Library L ,int b ,int r,BookType &B ,ReaderType &R)
// bΪ������ţ�rΪ�����߽���֤�ţ�
// ������в��������Ϊb���飬�򷵻�-1
// ����r����b��ļ�¼����ע���ü�¼������B��R����ͼ����Ϣ�ͽ�������Ϣ������1��
// ��û��r����b��ļ�¼������B����ͼ����Ϣ��������0
{
	Result		res = SearchBTree(L, b);		// ����
	if(!res.tag)	return -1;					// δ������������-1
	B = res.pt->rec[res.i];						// ��B��¼ͼ����Ϣ
	ReaderType	pre, p=res.pt->rec[res.i]->reader;
	for( ; p ;pre = p,p = p->nextr)				// ��������������
		if(p->cardnum == r)						// �ҵ�����R���ؽ�������Ϣ
			{
			R = p;
			pre ->nextr = p->nextr;
			B->current++;						// �ִ�����1
			return 1;
			}
	return 0;									// �޸ö��߽��ĸ�����Ϣ�򷵻�0
}


void	ListBookName(BTree p)
// ���B��p����м�¼��ͼ����������ȫ�ֱ���author��ͬ��
// �򱣴浽��ά����books��
{
	int		i;
	for(i = 1;i <= p->keynum; i++)				// ������¼
		{
		if(!strcmp(author,p->rec[i]->writer))	// ���ͼ������������������ͬ
			{
			books[books_counter] = (char *)malloc(sizeof(char));
			strcpy(books[books_counter++],p->rec[i]->bookname);	// copy��books����		
			}
		}
}

Status	ListAuthor(Library L )
// �ҳ����������author(authorΪȫ�ֱ���)�����������������浽��ά����books�����С�
// ����ҵ���author���������򷵻�TRUE�����򷵻�ERROR
{
	int		i;
	BTreeTraverse(L, ListBookName);				// ������⣬����ListBookName
	if(books_counter)							// ���ҵ����ߵ��飬���ӡ����
	{
	printf("\n\n");
	printf("     �q�T�T�T�T�T�T�T�T�T�T�T�T�T�T�r\n");
	SetColor(2);
	printf("     �U   �� %-7s���������� ��  �U\n",author);
	SetColor();
	printf("     �U�T�T�T�T�T�T�T�T�T�T�T�T�T�T�U\n");  
	for(i = 0; i<books_counter ;i++)
		{
		printf("     �U  %d  %-23s�U\n",i+1,books[i]); 
		printf("     �U�����������������������������U\n");
		}
	printf("     �t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s\n");
	return TRUE;
	}
	else return ERROR;							// δ�ҵ�			
}

Status	BespeakBook(BookType B ,ReaderType R)
// Ϊ����RԤԼB�顣���B���ִ���Ϊ0������û�ж���RԤԼ����ļ�¼��
// ��Ǽ�R��ԤԼ��Ϣ�����ɹ�����OK�����򷵻�ERROR��
{
	if(B->current > 0)	return ERROR;			// �ִ�������0�����ɳ���
	ReaderType		r ,pre;
	GetDate(R->bespeakdate);					// ��ȡ��ǰ����ΪԤԼ����
	if(!B->bespeaker) B->bespeaker = R;			// ����ԤԼ��Ǽ�ԤԼ����Ϣ
	else{
		for( pre = r = B->bespeaker; r ; pre = r, r = r->nextb)
			if(r->cardnum == R->cardnum) return ERROR;// ����ԤԼ���򷵻�ERROR
		pre->nextb = R;							// ����Ǽ�ԤԼ��Ϣ
		}
	return OK;
}


void	PrintH()
// ��ӡͼ�����ͷ
{
	printf("\n");
	printf("                    �q�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�r              \n");
	printf("�q�T�T�T�T�T�T�T�T�T�U       ��  ͼ����Ϣ  ��       �U�T�T�T�T�T�T�T�T�T�T�T�T�r");
	printf("�U�������Щ����������t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s�������Щ��������Щ������U");  
	SetColor(2);
	printf("�U���  ��    ����              �� ����       ���ִ橦�ܿ�橦������ݩ�����  �U"); 
	SetColor();
}

void	PrintT()
// ��ӡͼ�����β
{
	printf("�U�������੤���������������������੤�����������੤���੤�����੤�������੤�����U");
	printf("�t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s\n");
}

void	PrintD(BookType B )
// ��ʾB��Ļ�����Ϣ��
{
	printf("�U�������੤���������������������੤�����������੤���੤�����੤�������੤�����U");
	printf("�U %-4d ����%s��",B->booknum,	B->bookname);
	gotoxy(32,wherey());
	printf("�� %-11s��%-4d�� %-4d ��%-6d  ��%-6.1f�U",B->writer,
					B->current,B->total,B->publishyear,B->price);
}

void	PrintBook(BookType B)
// �Ա����ʽ��ʾһ����Ļ�����Ϣ����ţ����������ߣ��ִ������ܿ������������ݣ��۸�
{
	PrintH();		// ��ͷ
	PrintD(B);		// ����
	PrintT();		// ��β
	printf("\n");
}
void	PrintAll(BTree p)
// ��ʾB��ÿ���ڵ�ļ�¼��Ϣ
{
	int		i;
	for(i = 1 ;i <= p->keynum;i++)
	PrintD(p->rec[i]);
}
void	PrintAllBooks(Library L)
// ��ʾ���L����ͼ����Ϣ
{
	PrintH();
	BTreeTraverse(L, PrintAll);
	PrintT();
}



void	SortBorrower(BookType B)
// ��B��Ľ�����������а��������������������ǰ��
{
	int		count;
	ReaderType	p, pre, head;
	if(B->reader)
		{
		for(count = 0 ,p = B->reader; p; p = p->nextr) count++;	// ��ͳ�ƽ�������
		head = (ReaderType)malloc(sizeof(ReaderNode));			// ����һ��ͷ��㷽������
		head->nextr = B->reader;
		while(count-- > 1)										// ��ð�ݷ�����count-1������
			{
			pre = head;
			p = pre->nextr->nextr;	
			while(p)
				{
				if(!Earlier(pre->nextr->bordate,p->bordate))	// �������������ǰ��
					{
					pre->nextr->nextr = p->nextr;
					p->nextr = pre->nextr;
					pre->nextr = p;
					pre = p;
					p = p->nextr->nextr;
					}											// ����
				else{
					p = p->nextr;
					pre = pre->nextr;
					}
				}
			}
		B->reader = head->nextr;
		free(head);
		}
}

void	SortBespeaker(BookType B)
// ��B���ԤԼ��������а��������������������ǰ��
{
	int		count;
	ReaderType	p, pre, head;
	if(B->bespeaker)
		{
		for(count = 0 ,p = B->bespeaker; p; p = p->nextb) count++;	// ͳ��ԤԼ����
		head = (ReaderType)malloc(sizeof(ReaderNode));
		head->nextb = B->bespeaker;
		while(count-- > 1)											// ����count-1��ð������
			{
			pre = head;
			p = pre->nextb->nextb;	
			while(p)
				{
				if(!Earlier(pre->nextb->bespeakdate,p->bespeakdate))// �������������ǰ��
					{
					pre->nextb->nextb = p->nextb;
					p->nextb = pre->nextb;
					pre->nextb = p;
					pre = p;
					p = p->nextb->nextb;
					}												// ����
				else{
					p = p->nextb;
					pre = pre->nextb;
					}
				}
			}
		B->bespeaker = head->nextb;
		free(head);
		}
}


void	PrintBorrower(BookType B)
// ��Ӧ�����ڴ�ӡB��Ľ���������
{
	ReaderType	p;
	char	rpos[2][7]={"������","����"};		// ����״̬����
	tm			today;
	GetDate(today);
	printf("                     �q�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�r                  \n");
	printf("     �q�T�T�T�T�T�T�T�U        �� �������� ��        �U�T�T�T�T�T�T�T�T�r\n");
	printf("     �U���������Щ����t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s���������Щ������U\n");  
	SetColor(2);
	printf("     �U����֤�ũ�   ����   ��   ��������     �� ���Ӧ������   �� ״̬ �U\n"); 
	SetColor();
	printf("     �U���������੤���������੤���������������੤���������������੤�����U\n");
	for(p = B->reader; p; p = p->nextr)
		{
		printf("     �U %-7d��  %-8s�� %-4d��%2d��%2d�� �� %-4d��%2d��%2d�� ��%-6s�U\n",
		p->cardnum,p->rname,p->bordate.tm_year,p->bordate.tm_mon,p->bordate.tm_mday,
		p->retdate.tm_year,p->retdate.tm_mon,p->retdate.tm_mday,rpos[Earlier(today,p->retdate)]);
										// ��ӡ������ϸ��Ϣ���ȽϽ�����Ӧ���ڣ��ж��Ƿ�����
		printf("     �U���������੤���������੤���������������੤���������������੤�����U\n");
		}
	printf("     �t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s\n");

}


void	PrintBespeaker(BookType B)
// ��ԤԼʧЧ���ڴ�ӡB���ԤԼ������
{
	ReaderType	p;
	char		bpos[2][7] = {"ʧЧ","ԤԼ��"};	// ԤԼ״̬����
	tm			today,temp;
	GetDate(today);
	printf("\n");
	printf("                     �q�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�r                  \n");
	printf("     �q�T�T�T�T�T�T�T�U        �� ԤԼ���� ��        �U�T�T�T�T�T�T�T�T�r\n");
	printf("     �U���������Щ����t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s���������Щ������U\n"); 
	SetColor(2); 
	printf("     �UԤԼ֤�ũ�   ����   ��   ԤԼ����     �� ԤԼʧЧ����   �� ״̬ �U\n"); 
	SetColor();
	printf("     �U���������੤���������੤���������������੤���������������੤�����U\n");
	for(p = B->bespeaker; p; p = p->nextb)
		{
		AddDate(temp,p->bespeakdate,30);			// ��temp����ԤԼ�������ڣ�ԤԼ����30����ڣ�
		printf("     �U %-7d��  %-8s�� %-4d��%2d��%2d�� �� %-4d��%2d��%2d�� ��%-6s�U\n",
			p->cardnum,p->rname,p->bespeakdate.tm_year,p->bespeakdate.tm_mon,p->bespeakdate.tm_mday,
			temp.tm_year,temp.tm_mon,temp.tm_mday,bpos[Earlier(today,temp)]);
						// ��ӡԤԼ��ϸ��Ϣ���ȽϽ�����ԤԼ�������ڣ��ж�ԤԼ�Ƿ���ڣ�����һ���£�
		printf("     �U���������੤���������੤���������������੤���������������੤�����U\n");
		}
	printf("     �t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s\n");

}

Status	ShowBookinfo(Library L ,int booknum)
// �����L�д������Ϊbooknum���飬����ʾ�����״̬,������Ļ�����Ϣ��
// ������������ԤԼ�����������ڵȣ�������OK�����L�����ڸ��鷵��ERROR��
{
	Result	res = SearchBTree(L,booknum);	// ����
	if(!res.tag)	return ERROR;			// δ�ҵ�
	BookType B = res.pt->rec[res.i];	
	SortBorrower(B);						// �Խ������������ڳٵ�������
	SortBespeaker(B);						// ��ԤԼ���������ڳٵ�������
	PrintBook(B);							// ��ʾͼ�������Ϣ
	PrintBorrower(B);						// ��ʾ��������
	PrintBespeaker(B);						// ��ʾԤԼ����
	return OK;
}