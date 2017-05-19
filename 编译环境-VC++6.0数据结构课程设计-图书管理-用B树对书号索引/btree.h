#define		m 		3				// ����3��B��
typedef	BookNode	Record;			// ��¼ָ��Ϊͼ��������					
typedef	int			KeyType;

typedef		struct	BTNode			// B�����						
{
	int				keynum;			// ÿ�����ؼ��ָ���			
	struct	BTNode	*parent;		// ����ָ��					
	KeyType			key[m+1];		// �ؼ������飬0�ŵ�Ԫδ��
	struct	BTNode	*ptr[m+1];		// ����ָ��
	Record			*rec[m+1];		// ��¼ָ�룬0�ŵ�Ԫδ��
}BTNode,*BTree;						// B���ڵ����ͺ�B������

typedef	struct						
{	
	BTNode	*pt;					// ָ���ҵ��Ľ���Ӧ�ò���Ľ��
	int		i;						// �ؼ������
	int		tag;					// 1��ʾ���ҳɹ���0��ʾ����ʧ��
}Result;							// B�����ҽ������


int 	Search(BTree p, KeyType k) 
// ��B��p����в��ҹؼ���k��λ��i��ʹkey[i]<=k<key[i+1])
{
	int		i;
	for(i=0; i < p->keynum && p->key[i+1] <= k; i++);
	return i;
}




Result	SearchBTree(BTree T, KeyType k)
// ��m��B���ϲ��ҹؼ���k�����ؽ����pt��i��tag���������ҳɹ���������ֵtag=1��ָ��pt
// ��ָ����е�i���ؼ��ֵ���k�����򷵻�����ֵtag=0������k�Ĺؼ���Ӧ������pt��ָ���
// �е�i�͵�i+1���ؼ���֮�䡣
{
	int		i = 1;
	BTree	p = T, q = NULL;						// ��ʼ����pָ������㣬qָ��p��˫��
	int		found = FALSE;
	while(p && !found)
		{
		i = Search(p, k);							// ����k��λ��ʹp->key[i]<=k<p->key[i+1]
		if(i> 0 && k == p->key[i])	found = TRUE;
		else{										// δ�ҵ����������һ��
			q = p;
			p = p->ptr[i];
			}
		} 
	if(found)	{Result	r = {p, i, 1};	return r;}	// ���ҳɹ�
	else		{Result	r = {q, i, 0};	return r;}	// ���Ҳ��ɹ�������k�Ĳ���λ����Ϣ
}



void	Insert(BTree &q, int	i, KeyType k, BTree ap, Record *rec)
// ��k��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1],������ؼ���Ϊk�ļ�¼rec
{
	int		j;
	for(j = q->keynum;j > i; j--)				// ��¼���ؼ��֡�����ָ�����
		{
		q->key[j+1] = q->key[j];
		q->ptr[j+1] = q->ptr[j];
		q->rec[j+1] = q->rec[j];
		}
	q->key[i+1] = k;							// ����
	q->ptr[i+1] = ap;
	q->rec[i+1] = rec;
	q->keynum ++;								// �ؼ��ָ�����1
	if(ap) ap->parent = q;						// ˢ��q������ap�ĸ���ָ��
}


void	Split(BTree	&q, int n, BTree &ap)
// ��nΪ�ֽ罫���q����Ϊq��ap2�����
{
	int		i;
	ap = (BTree)malloc(sizeof(BTNode));			// ������ap�ռ�
	ap->ptr[0] = q->ptr[n]; 
	for(i = n+1;i <= m; i++)					// q��n��Ĺؼ��֡�����ָ�롢��¼ת�Ƶ�ap
		{
		ap->key[i-n] = q->key[i];
		ap->ptr[i-n] = q->ptr[i];
		ap->rec[i-n] = q->rec[i];
		}
	ap->keynum = q->keynum - n;					// ����ap�Ĺؼ��ָ���
	q->keynum = n-1;							// q�Ĺؼ��ָ�������
	ap->parent = q->parent;						
	for (i=0; i<=m-n; i++) 						
		if(ap->ptr[i]) ap->ptr[i]->parent = ap;	// ˢ��ap�������ĸ���ָ��
}

void NewRoot(BTree &T, BTree p, KeyType k, BTree ap,Record *rec)
// ������B��ʱTΪ�ջ��������Ϊq��ap�����ڵ㣬�轨��һ�����ڵ�ռ�
// ������ΪT����һ��ռ䣬����p,k,ap�ͼ�¼rec
{
	T = (BTree)malloc(sizeof(BTNode));
	T->keynum = 1;						
	T->ptr[0] = p;								// ����
	T->ptr[1] = ap;  
	T->key[1] = k;
	T->rec[1] = rec;
	if (p) p->parent= T;						// ˢ��T������ap�ĸ���ָ��
	if (ap) ap->parent = T;	
	T->parent = NULL;							// ���ڵ�˫��ΪNULL
}

Status InsertBTree(BTree &T, KeyType k, BTree q, int i,Record *rec) 
//  ��m��B��T�Ͻ��*q��key[i]��key[i+1]֮�����ؼ���K�ͼ�¼rec��
//  ���������������˫�������б�Ҫ�Ľ����ѵ�����ʹT����m��B����
{
	BTree ap = NULL;
	int finished = FALSE;
	if (!q)    NewRoot(T, NULL, k, NULL,rec);	// T�ǿ��������ɽ����ؼ���K�ĸ����*T
	else{
		while (!finished) 
			{
			Insert(q, i, k, ap,rec);			// ��k��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1]
			if (q->keynum < m) finished = TRUE; // �������
			else{
				Split(q, (m+1)/2, ap);			// ���ѽ��Q
				k = q->key[(m+1)/2];
				rec = q->rec[(m+1)/2];
				if (q->parent) 
					{							// ��˫�׽��*q�в���k�Ĳ���λ��
					q = q->parent;  
					i = Search(q, k);  
					} 
				else finished = OVERFLOW;		// ���ڵ��ѷ���Ϊ*q��*ap�������
				} 
			} 
		if (finished == OVERFLOW)				// ������ѷ���Ϊ���*q��*ap
			NewRoot(T, q, k, ap,rec);			// �������¸����*T,q��apΪ����ָ��
		}
	return OK;
} //  InsertBTree


void	TakePlace(BTree &q, int &i)
// *q���ĵ�i���ؼ���Ϊk����q�ĺ�̹ؼ������q������qָ�������ڽ�㣬
{
	BTree	p = q;
	q = q->ptr[i];								
	while(q->ptr[0]) q = q->ptr[0];				// ����p�ĺ��
	p->key[i] = q->key[1];						// �ؼ��ִ���
	p->rec[i] = q->rec[1];						// ��¼����
	i = 1;										// �����Ӧ��ɾ��q��ָ���ĵ�1���ؼ���
}


void	Del(BTree q, int i)
// ɾ��q��ָ����i���ؼ��ּ����¼
{
	for(;i < q->keynum ;i++)					// �ؼ��ֺͼ�¼ָ��ǰ��
		{
		q->key[i] = q->key[i+1];
		q->rec[i] = q->rec[i+1];
		}
	q->keynum --;								// �ؼ�����Ŀ��1
}



Status	Borrow(BTree q)
// ��q���ֵܽ��ؼ��ִ���(m-1)/2,����ֵܽ��������С������󣩵Ĺؼ��ֵ�˫�׽�㣬
// ����˫�׽����С��(����ڣ��ҽ����ùؼ��ֵĹؼ���������q��,������OK�����򷵻�EREOR��
{
	int		i;
	BTree	p = q->parent, b;					// pָ��q��˫�׽��
	for(i = 0 ; p->ptr[i] != q;i++) ;			// ����q��˫��p������λ��
	if(i >= 0 && i+1 <= p->keynum && p->ptr[i+1]->keynum > (m-1)/2)
		{										// ��q�����ֵܹؼ��ָ�������(m-1)/2
		b = p->ptr[i+1];						// bָ�����ֵܽ��
		q->ptr[1] = b->ptr[0];					// ����ָ��ҲҪͬ���ƶ�
		q->key[1] = p->key[i+1];				// �Ӹ��ڵ���i+1���ؼ���
		q->rec[1] = p->rec[i+1];
		p->key[i+1] = b->key[1];				// b��һ���ؼ������Ƶ����ڵ�
		p->rec[i+1] = b->rec[1];
		for(i =1 ;i <= b->keynum;i++)			// b��һ���ؼ������ƣ����ʣ���¼ǰ��һλ
			{
			b->key[i] = b->key[i+1];
			b->rec[i] = b->rec[i+1];
			b->ptr[i-1] = b->ptr[i];
			}
		}
	else if(i > 0 &&  p->ptr[i-1]->keynum > (m-1)/2)
		{										// ��q�����ֵܹؼ��ָ�����Լ(m-1)/2
		b = p->ptr[i-1];						// bָ�����ֵܽ��
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];			
		q->key[1] = p->key[i];					// �Ӹ��ڵ���i���ؼ���
		q->rec[1] = p->rec[i];
		p->key[i] = b->key[b->keynum];			// ��b���һ���ؼ������Ƶ����ڵ�
		p->rec[i] = b->rec[b->keynum];
		}
	else return ERROR;							// �޹ؼ��ִ���(m-1)/2���ֵ�
	q->keynum ++;
	b->keynum --;
	for(i = 0 ;i <=q->keynum; i++)
		if(q->ptr[i]) q->ptr[i]->parent = q;	// ˢ��q���ӽ���˫��ָ��
	return OK;
}

void	Combine(BTree &q)
// ��qʣ�ಿ�ֺ�q�ĸ�������عؼ��ֺϲ���q�ֵ���,Ȼ���ͷ�q����qָ���޸ĵ��ֵ�
{
	int		i, j ;
	BTree p = q->parent, b;						// pָ��q�ĸ���
	for(i = 0; p->ptr[i] != q; i++) ;			// ���q�ڸ���p�е�����λ��
	if(i == 0)									// ��Ϊ0������ϲ�Ϊ�ֵܵĵ�һ���ؼ���
		{
		b = p->ptr[i+1];
		for(j = b->keynum ; j >= 0 ;j--)		// ��b�Ĺؼ��ֺͼ�¼����һλ
			{
			b->key[j+1] = b->key[j];
			b->rec[j+1] = b->rec[j];
			b->ptr[j+1] = b->ptr[j];
			}
		b->ptr[0] = q->ptr[0];					// �ϲ�
		b->key[1] = p->key[1];
		b->rec[1] = p->rec[1];
		}
	else if(i > 0)								// ��q�ڸ��׵�����λ�ô�Լ0
		{										// ��ϲ�Ϊ�ֵ�b�����һ���ؼ���
		b = p->ptr[i-1];
		b->key[b->keynum+1] = p->key[i];		// �ϲ�
		b->rec[b->keynum+1] = p->rec[i];
		b->ptr[b->keynum+1] = q->ptr[0];
		}
	if(i == 0 || i == 1)						// ��iΪ0��1���轫���ڵ�p�ؼ���ǰ��һλ
	for( ; i < p->keynum; i++)
		{
		p->key[i] = p->key[i+1];
		p->ptr[i] = p->ptr[i+1];
		p->rec[i] = p->rec[i+1];
		}
	p->keynum --;
	b->keynum ++;
	free(q);
	q = b;										// qָ���޸ĵ��ֵܽ��
	for(i = 0;i <= b->keynum; i++)
		if(b->ptr[i]) b->ptr[i]->parent = b;	// ˢ��b���ӽ���˫��ָ��
}

Status	DeleteBTree(BTree &T,KeyType k)
// ��m��B��T��ɾ���ؼ���k�����Ӧ��¼��������OK��
// ��T�ϲ����ڹؼ���k���򷵻�ERROR��
{
	KeyType	x=k;
	BTree	q,b = NULL;	
	int		finished = FALSE,i = 1;
	Result res = SearchBTree(T,k);				// ��T�в��ҹؼ���k
	if(res.tag == 0 ) return ERROR;				// δ������
	else
		{
		q = res.pt;								// qָ���ɾ���
		i = res.i;	
		if(q->ptr[0]) TakePlace(q, i);			// ��q���������գ�(�ǵײ���)
												// �������̴�֮������qָ�������ڽ��
		Del(q,i);								// ɾ��q��ָ�����е�i���ؼ��ּ���¼
		if(q->keynum>=(m-1)/2||!q->parent)		// ��ɾ����ؼ��ָ�����С��(m-1)/2��q�Ǹ��ڵ�
			{
			finished = TRUE;					// ɾ�����
			if(q->keynum == 0 ) T = NULL;		// ��q�Ĺؼ��ָ���Ϊ0 ����Ϊ����
			}
		while(!finished)
			{
			if(Borrow(q))	finished = TRUE;	// ��q�������ֵܽ��ؼ��ִ���(m-1)/2,��Ӹ�
												// �ֵܽ������һ����󣨻���С���ؼ��ֵ�
												// ���ڵ㣬�Ӹ��ڵ��һ�ؼ��ֵ�q
			else{								// ��q�����ֵܹؼ��ָ��������ک�m /2��-1
				Combine(q);	// ��q�е�ʣ�ಿ�ֺ�˫���е���عؼ��ֺϲ���q��һ���ֵ���
				q = q->parent;					// ���˫��
				if(q == T && T->keynum ==0 )	// ����ɾ���ĸ��ڵ��Ǹ�T��T�Ĺؼ��ָ���Ϊ0
					{
					T = T->ptr[0];				// �¸�	
					T->parent = NULL;		
					free(q);					// ɾ��ԭ˫�׽��
					finished = TRUE;			
					}
				else if(q->keynum >= m/2) finished = TRUE;
				}								// �ϲ���˫�׹ؼ��ָ���������(m-1)/2�����
			}
		}
	return OK ;
}

void	BTreeTraverse(BTree T,void ( *Visit)(BTree p))
// ����B��T����ÿ��������Visit����
{
	int		i;
	if(!T)	return ;
	Visit(T);
	for(i = 0 ;i <= T->keynum; i++)				// �ݹ�����������
		if(T->ptr[i]) BTreeTraverse(T->ptr[i],Visit);
}

void	ShowBTree(BTree T,short  x = 8)
 // �ݹ��԰������ʽ��ʾB��T,ÿ���������Ϊx����ʼ������Ϊ8
{
	SetColor(3);								// ����ɫ
	if(!T)	return ;
	int		i;
	printf("\n");
	for(i = 0;i<=x;i++) putchar(' ');			// ����x
	for(i = 1 ;i <= T->keynum;i++)
		{printf("%d,",T->key[i]);				
		}
	for(i = 0 ;i <= T->keynum;i++)				// �ݹ���ʾ�������ؼ���
		ShowBTree(T->ptr[i],x+7);
	SetColor();									// ������ɫתΪĬ�ϰ�ɫ
}

