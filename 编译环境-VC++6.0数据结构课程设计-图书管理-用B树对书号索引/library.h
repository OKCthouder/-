
void	InitLibrary(Library &L )
// 初始化书库L为空书库。
{
	L = NULL;
}
	
void	InsertBook(Library &L ,BookType B , Result res)
// 书库L已存在，res包含B书在书库L中的位置或应该插入的位置
// 如果书库中已存在B书，则只将B书的库存量增加，否则插入B书到书库L中。
{
	if(res.tag == 0) 
		InsertBTree(L, B->booknum, res.pt, res.i, B);	// 如果书库中不存在该书，则插入
	else{										// 如果已存在
		BookType b = res.pt->rec[res.i];				
		b->current = b->current + B->total;		// 现存量和总库存增加
		b->total = b->total + B->total; 
		}
}

Status	DeleteBook(Library &L ,BookType B)
// 如果书库中存在B书，则从书库中删除B书的信息，并返回OK，否则返回ERROR
{
	if(DeleteBTree(L,B->booknum))	return OK;	// 如果删除成功，返回OK
	else return ERROR;							// 否则（删除不成功）返回ERROR
}


Status	CanBorrow(Library L,BookType B ,ReaderType R)
// 书库L中存在B书。若B书现存量大于0且未被他人（读者R除外）预约，
// 若有该读者预约信息，删除该信息并返回TRUE
// 否则返回FALSE。
{
	int	tag = 0, count = 0;						// tag指示该读者是否预约
	ReaderType b, pre;							
	if(B->current > 0)							// 若现存量大于0
		{
		if(!B->bespeaker) return TRUE;			// 无人预约，则可出借
		for(b = B->bespeaker; b ; pre = b,b = b->nextb,count++) 
			{											
			if(b->cardnum == R->cardnum) 		// 有读者R的预约信息
				{
				pre->nextb = b->nextb;
				free(b);						// 删除该预约信息
				return TRUE;					
				}
			}
		if(B->current-count > 0 ) return TRUE;	// 现存量大于预约人数，也可出借
		}		
	return FALSE;								// 现存量为0，不可出借
}


void	BorrowBook(Library L ,BookType B ,ReaderType R)
// 书库L存在，B书是书库中的书并且可被读者R借阅（已通过CanBorrow()判断）
// 借出一本B书，登记借阅者R的信息，改变现存量，记录借书日期，最迟应还日期等信息。
{
	ReaderType	r;
	GetDate(R->bordate);						// 获取当前日期为借书日期
	AddDate(R->retdate, R->bordate, KEEP_DAYS);	// 当前日期加90天为最迟还书日期
	if(!B->reader) B->reader = R;				// 无其他借阅者，则直接登记
	else{
		for(r = B->reader; r->nextr; r= r->nextr) ;
		r->nextr = R;							// 否则到借阅者链表尾，登记
		}
	B->current--;								// 现存量减1

}
			  
int		ReturnBook(Library L ,int b ,int r,BookType &B ,ReaderType &R)
// b为还书书号，r为还书者借阅证号，
// 若书库中不存在书号为b的书，则返回-1
// 若有r借阅b书的记录，则注销该记录，并用B和R返回图书信息和借阅者信息并返回1，
// 若没有r借阅b书的记录，则用B返回图书信息，并返回0
{
	Result		res = SearchBTree(L, b);		// 搜索
	if(!res.tag)	return -1;					// 未搜索到，返回-1
	B = res.pt->rec[res.i];						// 用B记录图书信息
	ReaderType	pre, p=res.pt->rec[res.i]->reader;
	for( ; p ;pre = p,p = p->nextr)				// 搜索借书者链表
		if(p->cardnum == r)						// 找到则用R返回借阅者信息
			{
			R = p;
			pre ->nextr = p->nextr;
			B->current++;						// 现存量增1
			return 1;
			}
	return 0;									// 无该读者借阅该书信息则返回0
}


void	ListBookName(BTree p)
// 如果B树p结点中记录里图书著者名与全局变量author相同，
// 则保存到二维数组books中
{
	int		i;
	for(i = 1;i <= p->keynum; i++)				// 搜索记录
		{
		if(!strcmp(author,p->rec[i]->writer))	// 如果图书著者与所求著者相同
			{
			books[books_counter] = (char *)malloc(sizeof(char));
			strcpy(books[books_counter++],p->rec[i]->bookname);	// copy到books数组		
			}
		}
}

Status	ListAuthor(Library L )
// 找出书库中著者author(author为全局变量)的所有著作名，保存到二维数组books数组中。
// 如果找到该author的著作，则返回TRUE，否则返回ERROR
{
	int		i;
	BTreeTraverse(L, ListBookName);				// 遍历书库，调用ListBookName
	if(books_counter)							// 如找到著者的书，则打印出来
	{
	printf("\n\n");
	printf("     ╭══════════════╮\n");
	SetColor(2);
	printf("     ║   【 %-7s的所有著作 】  ║\n",author);
	SetColor();
	printf("     ║══════════════║\n");  
	for(i = 0; i<books_counter ;i++)
		{
		printf("     ║  %d  %-23s║\n",i+1,books[i]); 
		printf("     ║──────────────║\n");
		}
	printf("     ╰══════════════╯\n");
	return TRUE;
	}
	else return ERROR;							// 未找到			
}

Status	BespeakBook(BookType B ,ReaderType R)
// 为读者R预约B书。如果B书现存量为0，并且没有读者R预约该书的记录，
// 则登记R的预约信息，并成功返回OK，否则返回ERROR。
{
	if(B->current > 0)	return ERROR;			// 现存量大于0，不可出借
	ReaderType		r ,pre;
	GetDate(R->bespeakdate);					// 获取当前日期为预约日期
	if(!B->bespeaker) B->bespeaker = R;			// 无人预约则登记预约者信息
	else{
		for( pre = r = B->bespeaker; r ; pre = r, r = r->nextb)
			if(r->cardnum == R->cardnum) return ERROR;// 如已预约，则返回ERROR
		pre->nextb = R;							// 否则登记预约信息
		}
	return OK;
}


void	PrintH()
// 打印图书表格表头
{
	printf("\n");
	printf("                    ╭═══════════════╮              \n");
	printf("╭═════════║       【  图书信息  】       ║════════════╮");
	printf("║───┬─────╰═══════════════╯───┬────┬───║");  
	SetColor(2);
	printf("║书号  │    书名              │ 著者       │现存│总库存│出版年份│定价  ║"); 
	SetColor();
}

void	PrintT()
// 打印图书表格表尾
{
	printf("║───┼───────────┼──────┼──┼───┼────┼───║");
	printf("╰══════════════════════════════════════╯\n");
}

void	PrintD(BookType B )
// 显示B书的基本信息。
{
	printf("║───┼───────────┼──────┼──┼───┼────┼───║");
	printf("║ %-4d │《%s》",B->booknum,	B->bookname);
	gotoxy(32,wherey());
	printf("│ %-11s│%-4d│ %-4d │%-6d  │%-6.1f║",B->writer,
					B->current,B->total,B->publishyear,B->price);
}

void	PrintBook(BookType B)
// 以表格形式显示一本书的基本信息（书号，书名，著者，现存量，总库存量，出版年份，价格）
{
	PrintH();		// 表头
	PrintD(B);		// 数据
	PrintT();		// 表尾
	printf("\n");
}
void	PrintAll(BTree p)
// 显示B树每个节点的记录信息
{
	int		i;
	for(i = 1 ;i <= p->keynum;i++)
	PrintD(p->rec[i]);
}
void	PrintAllBooks(Library L)
// 显示书库L所有图书信息
{
	PrintH();
	BTreeTraverse(L, PrintAll);
	PrintT();
}



void	SortBorrower(BookType B)
// 对B书的借书者链表进行按日期排序，日期早的排在前面
{
	int		count;
	ReaderType	p, pre, head;
	if(B->reader)
		{
		for(count = 0 ,p = B->reader; p; p = p->nextr) count++;	// 先统计借书人数
		head = (ReaderType)malloc(sizeof(ReaderNode));			// 附加一个头结点方便排序
		head->nextr = B->reader;
		while(count-- > 1)										// 用冒泡法进行count-1趟排序
			{
			pre = head;
			p = pre->nextr->nextr;	
			while(p)
				{
				if(!Earlier(pre->nextr->bordate,p->bordate))	// 把日期早的排在前面
					{
					pre->nextr->nextr = p->nextr;
					p->nextr = pre->nextr;
					pre->nextr = p;
					pre = p;
					p = p->nextr->nextr;
					}											// 交换
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
// 对B书的预约者链表进行按日期排序，日期早的排在前面
{
	int		count;
	ReaderType	p, pre, head;
	if(B->bespeaker)
		{
		for(count = 0 ,p = B->bespeaker; p; p = p->nextb) count++;	// 统计预约人数
		head = (ReaderType)malloc(sizeof(ReaderNode));
		head->nextb = B->bespeaker;
		while(count-- > 1)											// 进行count-1趟冒泡排序
			{
			pre = head;
			p = pre->nextb->nextb;	
			while(p)
				{
				if(!Earlier(pre->nextb->bespeakdate,p->bespeakdate))// 把日期早的排在前面
					{
					pre->nextb->nextb = p->nextb;
					p->nextb = pre->nextb;
					pre->nextb = p;
					pre = p;
					p = p->nextb->nextb;
					}												// 交换
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
// 按应还日期打印B书的借阅者名单
{
	ReaderType	p;
	char	rpos[2][7]={"已逾期","良好"};		// 借阅状态数组
	tm			today;
	GetDate(today);
	printf("                     ╭═══════════════╮                  \n");
	printf("     ╭═══════║        【 借阅名单 】        ║════════╮\n");
	printf("     ║────┬──╰═══════════════╯────┬───║\n");  
	SetColor(2);
	printf("     ║借阅证号│   姓名   │   出借日期     │ 最迟应还日期   │ 状态 ║\n"); 
	SetColor();
	printf("     ║────┼─────┼────────┼────────┼───║\n");
	for(p = B->reader; p; p = p->nextr)
		{
		printf("     ║ %-7d│  %-8s│ %-4d年%2d月%2d日 │ %-4d年%2d月%2d日 │%-6s║\n",
		p->cardnum,p->rname,p->bordate.tm_year,p->bordate.tm_mon,p->bordate.tm_mday,
		p->retdate.tm_year,p->retdate.tm_mon,p->retdate.tm_mday,rpos[Earlier(today,p->retdate)]);
										// 打印借阅详细信息，比较今天与应还期，判断是否逾期
		printf("     ║────┼─────┼────────┼────────┼───║\n");
		}
	printf("     ╰════════════════════════════════╯\n");

}


void	PrintBespeaker(BookType B)
// 按预约失效日期打印B书的预约者名单
{
	ReaderType	p;
	char		bpos[2][7] = {"失效","预约中"};	// 预约状态数组
	tm			today,temp;
	GetDate(today);
	printf("\n");
	printf("                     ╭═══════════════╮                  \n");
	printf("     ╭═══════║        【 预约名单 】        ║════════╮\n");
	printf("     ║────┬──╰═══════════════╯────┬───║\n"); 
	SetColor(2); 
	printf("     ║预约证号│   姓名   │   预约日期     │ 预约失效日期   │ 状态 ║\n"); 
	SetColor();
	printf("     ║────┼─────┼────────┼────────┼───║\n");
	for(p = B->bespeaker; p; p = p->nextb)
		{
		AddDate(temp,p->bespeakdate,30);			// 用temp计算预约过期日期（预约超过30天过期）
		printf("     ║ %-7d│  %-8s│ %-4d年%2d月%2d日 │ %-4d年%2d月%2d日 │%-6s║\n",
			p->cardnum,p->rname,p->bespeakdate.tm_year,p->bespeakdate.tm_mon,p->bespeakdate.tm_mday,
			temp.tm_year,temp.tm_mon,temp.tm_mday,bpos[Earlier(today,temp)]);
						// 打印预约详细信息，比较今天与预约过期日期，判断预约是否过期（超过一个月）
		printf("     ║────┼─────┼────────┼────────┼───║\n");
		}
	printf("     ╰════════════════════════════════╯\n");

}

Status	ShowBookinfo(Library L ,int booknum)
// 若书库L中存在书号为booknum的书，则显示该书的状态,包括书的基本信息和
// 借阅者名单，预约者名单，日期等，并返回OK，书库L不存在该书返回ERROR。
{
	Result	res = SearchBTree(L,booknum);	// 搜索
	if(!res.tag)	return ERROR;			// 未找到
	BookType B = res.pt->rec[res.i];	
	SortBorrower(B);						// 对借阅者链表按日期迟到早排序
	SortBespeaker(B);						// 对预约者链表按日期迟到早排序
	PrintBook(B);							// 显示图书基本信息
	PrintBorrower(B);						// 显示借阅名单
	PrintBespeaker(B);						// 显示预约名单
	return OK;
}