#define		m 		3				// 定义3叉B树
typedef	BookNode	Record;			// 记录指针为图书结点类型					
typedef	int			KeyType;

typedef		struct	BTNode			// B树结点						
{
	int				keynum;			// 每个结点关键字个数			
	struct	BTNode	*parent;		// 父亲指针					
	KeyType			key[m+1];		// 关键字数组，0号单元未用
	struct	BTNode	*ptr[m+1];		// 子数指针
	Record			*rec[m+1];		// 记录指针，0号单元未用
}BTNode,*BTree;						// B树节点类型和B树类型

typedef	struct						
{	
	BTNode	*pt;					// 指向找到的结点或应该插入的结点
	int		i;						// 关键字序号
	int		tag;					// 1表示查找成功，0表示查找失败
}Result;							// B树查找结果类型


int 	Search(BTree p, KeyType k) 
// 在B树p结点中查找关键字k的位置i，使key[i]<=k<key[i+1])
{
	int		i;
	for(i=0; i < p->keynum && p->key[i+1] <= k; i++);
	return i;
}




Result	SearchBTree(BTree T, KeyType k)
// 在m阶B树上查找关键字k，返回结果（pt，i，tag）。若查找成功，则特征值tag=1，指针pt
// 所指结点中第i个关键字等于k；否则返回特征值tag=0，等于k的关键字应插入在pt所指结点
// 中第i和第i+1个关键字之间。
{
	int		i = 1;
	BTree	p = T, q = NULL;						// 初始化，p指向待查结点，q指向p的双亲
	int		found = FALSE;
	while(p && !found)
		{
		i = Search(p, k);							// 查找k的位置使p->key[i]<=k<p->key[i+1]
		if(i> 0 && k == p->key[i])	found = TRUE;
		else{										// 未找到，则查找下一层
			q = p;
			p = p->ptr[i];
			}
		} 
	if(found)	{Result	r = {p, i, 1};	return r;}	// 查找成功
	else		{Result	r = {q, i, 0};	return r;}	// 查找不成功，返回k的插入位置信息
}



void	Insert(BTree &q, int	i, KeyType k, BTree ap, Record *rec)
// 将k和ap分别插入到q->key[i+1]和q->ptr[i+1],并插入关键字为k的记录rec
{
	int		j;
	for(j = q->keynum;j > i; j--)				// 记录、关键字、子树指针后移
		{
		q->key[j+1] = q->key[j];
		q->ptr[j+1] = q->ptr[j];
		q->rec[j+1] = q->rec[j];
		}
	q->key[i+1] = k;							// 插入
	q->ptr[i+1] = ap;
	q->rec[i+1] = rec;
	q->keynum ++;								// 关键字个数增1
	if(ap) ap->parent = q;						// 刷新q的子树ap的父亲指针
}


void	Split(BTree	&q, int n, BTree &ap)
// 以n为分界将结点q分裂为q和ap2个结点
{
	int		i;
	ap = (BTree)malloc(sizeof(BTNode));			// 新申请ap空间
	ap->ptr[0] = q->ptr[n]; 
	for(i = n+1;i <= m; i++)					// q上n后的关键字、子树指针、记录转移到ap
		{
		ap->key[i-n] = q->key[i];
		ap->ptr[i-n] = q->ptr[i];
		ap->rec[i-n] = q->rec[i];
		}
	ap->keynum = q->keynum - n;					// 计算ap的关键字个数
	q->keynum = n-1;							// q的关键字个数减少
	ap->parent = q->parent;						
	for (i=0; i<=m-n; i++) 						
		if(ap->ptr[i]) ap->ptr[i]->parent = ap;	// 刷新ap的子树的父亲指针
}

void NewRoot(BTree &T, BTree p, KeyType k, BTree ap,Record *rec)
// 当插入B树时T为空或根结点分裂为q和ap两个节点，需建立一个根节点空间
// 本函数为T申请一块空间，插入p,k,ap和记录rec
{
	T = (BTree)malloc(sizeof(BTNode));
	T->keynum = 1;						
	T->ptr[0] = p;								// 插入
	T->ptr[1] = ap;  
	T->key[1] = k;
	T->rec[1] = rec;
	if (p) p->parent= T;						// 刷新T的子树ap的父亲指针
	if (ap) ap->parent = T;	
	T->parent = NULL;							// 根节点双亲为NULL
}

Status InsertBTree(BTree &T, KeyType k, BTree q, int i,Record *rec) 
//  在m阶B树T上结点*q的key[i]与key[i+1]之间插入关键字K和记录rec。
//  若引起结点过大，则沿双亲链进行必要的结点分裂调整，使T仍是m阶B树。
{
	BTree ap = NULL;
	int finished = FALSE;
	if (!q)    NewRoot(T, NULL, k, NULL,rec);	// T是空树，生成仅含关键字K的根结点*T
	else{
		while (!finished) 
			{
			Insert(q, i, k, ap,rec);			// 将k和ap分别插入到q->key[i+1]和q->ptr[i+1]
			if (q->keynum < m) finished = TRUE; // 插入完成
			else{
				Split(q, (m+1)/2, ap);			// 分裂结点Q
				k = q->key[(m+1)/2];
				rec = q->rec[(m+1)/2];
				if (q->parent) 
					{							// 在双亲结点*q中查找k的插入位置
					q = q->parent;  
					i = Search(q, k);  
					} 
				else finished = OVERFLOW;		// 根节点已分裂为*q和*ap两个结点
				} 
			} 
		if (finished == OVERFLOW)				// 根结点已分裂为结点*q和*ap
			NewRoot(T, q, k, ap,rec);			// 需生成新根结点*T,q和ap为子树指针
		}
	return OK;
} //  InsertBTree


void	TakePlace(BTree &q, int &i)
// *q结点的第i个关键字为k，用q的后继关键字替代q，且令q指向后继所在结点，
{
	BTree	p = q;
	q = q->ptr[i];								
	while(q->ptr[0]) q = q->ptr[0];				// 搜索p的后继
	p->key[i] = q->key[1];						// 关键字代替
	p->rec[i] = q->rec[1];						// 记录代替
	i = 1;										// 代替后应该删除q所指结点的第1个关键字
}


void	Del(BTree q, int i)
// 删除q所指结点第i个关键字及其记录
{
	for(;i < q->keynum ;i++)					// 关键字和记录指针前移
		{
		q->key[i] = q->key[i+1];
		q->rec[i] = q->rec[i+1];
		}
	q->keynum --;								// 关键字数目减1
}



Status	Borrow(BTree q)
// 若q的兄弟结点关键字大于(m-1)/2,则从兄弟结点上移最小（或最大）的关键字到双亲结点，
// 而将双亲结点中小于(或大于）且紧靠该关键字的关键字下移至q中,并返回OK，否则返回EREOR。
{
	int		i;
	BTree	p = q->parent, b;					// p指向q的双亲结点
	for(i = 0 ; p->ptr[i] != q;i++) ;			// 查找q在双亲p的子树位置
	if(i >= 0 && i+1 <= p->keynum && p->ptr[i+1]->keynum > (m-1)/2)
		{										// 若q的右兄弟关键字个数大于(m-1)/2
		b = p->ptr[i+1];						// b指向右兄弟结点
		q->ptr[1] = b->ptr[0];					// 子树指针也要同步移动
		q->key[1] = p->key[i+1];				// 从父节点借第i+1个关键字
		q->rec[1] = p->rec[i+1];
		p->key[i+1] = b->key[1];				// b第一个关键字上移到父节点
		p->rec[i+1] = b->rec[1];
		for(i =1 ;i <= b->keynum;i++)			// b第一个关键字上移，需把剩余记录前移一位
			{
			b->key[i] = b->key[i+1];
			b->rec[i] = b->rec[i+1];
			b->ptr[i-1] = b->ptr[i];
			}
		}
	else if(i > 0 &&  p->ptr[i-1]->keynum > (m-1)/2)
		{										// 若q的左兄弟关键字个数大约(m-1)/2
		b = p->ptr[i-1];						// b指向左兄弟结点
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];			
		q->key[1] = p->key[i];					// 从父节点借第i个关键字
		q->rec[1] = p->rec[i];
		p->key[i] = b->key[b->keynum];			// 将b最后一个关键字上移到父节点
		p->rec[i] = b->rec[b->keynum];
		}
	else return ERROR;							// 无关键字大于(m-1)/2的兄弟
	q->keynum ++;
	b->keynum --;
	for(i = 0 ;i <=q->keynum; i++)
		if(q->ptr[i]) q->ptr[i]->parent = q;	// 刷新q的子结点的双亲指针
	return OK;
}

void	Combine(BTree &q)
// 将q剩余部分和q的父结点的相关关键字合并到q兄弟中,然后释放q，令q指向修改的兄弟
{
	int		i, j ;
	BTree p = q->parent, b;						// p指向q的父亲
	for(i = 0; p->ptr[i] != q; i++) ;			// 插好q在父亲p中的子树位置
	if(i == 0)									// 如为0，则需合并为兄弟的第一个关键字
		{
		b = p->ptr[i+1];
		for(j = b->keynum ; j >= 0 ;j--)		// 将b的关键字和记录后移一位
			{
			b->key[j+1] = b->key[j];
			b->rec[j+1] = b->rec[j];
			b->ptr[j+1] = b->ptr[j];
			}
		b->ptr[0] = q->ptr[0];					// 合并
		b->key[1] = p->key[1];
		b->rec[1] = p->rec[1];
		}
	else if(i > 0)								// 若q在父亲的子树位置大约0
		{										// 需合并为兄弟b的最后一个关键字
		b = p->ptr[i-1];
		b->key[b->keynum+1] = p->key[i];		// 合并
		b->rec[b->keynum+1] = p->rec[i];
		b->ptr[b->keynum+1] = q->ptr[0];
		}
	if(i == 0 || i == 1)						// 若i为0或1，需将父节点p关键字前移一位
	for( ; i < p->keynum; i++)
		{
		p->key[i] = p->key[i+1];
		p->ptr[i] = p->ptr[i+1];
		p->rec[i] = p->rec[i+1];
		}
	p->keynum --;
	b->keynum ++;
	free(q);
	q = b;										// q指向修改的兄弟结点
	for(i = 0;i <= b->keynum; i++)
		if(b->ptr[i]) b->ptr[i]->parent = b;	// 刷新b的子结点的双亲指针
}

Status	DeleteBTree(BTree &T,KeyType k)
// 在m阶B树T上删除关键字k及其对应记录，并返回OK。
// 如T上不存在关键字k，则返回ERROR。
{
	KeyType	x=k;
	BTree	q,b = NULL;	
	int		finished = FALSE,i = 1;
	Result res = SearchBTree(T,k);				// 在T中查找关键字k
	if(res.tag == 0 ) return ERROR;				// 未搜索到
	else
		{
		q = res.pt;								// q指向待删结点
		i = res.i;	
		if(q->ptr[0]) TakePlace(q, i);			// 若q的子树不空，(非底层结点)
												// 则以其后继代之，且令q指向后继所在结点
		Del(q,i);								// 删除q所指向结点中第i个关键字及记录
		if(q->keynum>=(m-1)/2||!q->parent)		// 若删除后关键字个数不小于(m-1)/2或q是根节点
			{
			finished = TRUE;					// 删除完成
			if(q->keynum == 0 ) T = NULL;		// 若q的关键字个数为0 ，则为空树
			}
		while(!finished)
			{
			if(Borrow(q))	finished = TRUE;	// 若q的相邻兄弟结点关键字大于(m-1)/2,则从该
												// 兄弟结点上移一个最大（或最小）关键字到
												// 父节点，从父节点借一关键字到q
			else{								// 若q相邻兄弟关键字个数均等于┌m /2┑-1
				Combine(q);	// 将q中的剩余部分和双亲中的相关关键字合并至q的一个兄弟中
				q = q->parent;					// 检查双亲
				if(q == T && T->keynum ==0 )	// 若被删结点的父节点是根T且T的关键字个数为0
					{
					T = T->ptr[0];				// 新根	
					T->parent = NULL;		
					free(q);					// 删除原双亲结点
					finished = TRUE;			
					}
				else if(q->keynum >= m/2) finished = TRUE;
				}								// 合并后双亲关键字个数不少于(m-1)/2，完成
			}
		}
	return OK ;
}

void	BTreeTraverse(BTree T,void ( *Visit)(BTree p))
// 遍历B树T，对每个结点调用Visit函数
{
	int		i;
	if(!T)	return ;
	Visit(T);
	for(i = 0 ;i <= T->keynum; i++)				// 递归遍历子树结点
		if(T->ptr[i]) BTreeTraverse(T->ptr[i],Visit);
}

void	ShowBTree(BTree T,short  x = 8)
 // 递归以凹入表形式显示B树T,每层的缩进量为x，初始缩进量为8
{
	SetColor(3);								// 深绿色
	if(!T)	return ;
	int		i;
	printf("\n");
	for(i = 0;i<=x;i++) putchar(' ');			// 缩进x
	for(i = 1 ;i <= T->keynum;i++)
		{printf("%d,",T->key[i]);				
		}
	for(i = 0 ;i <= T->keynum;i++)				// 递归显示子树结点关键字
		ShowBTree(T->ptr[i],x+7);
	SetColor();									// 字体颜色转为默认白色
}

