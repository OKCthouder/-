#include "Library.h"

/********************************B树接口实现****************************/

int Search(BTree p, int k) {
	//在B树p中查找关键字k的位置i，使得p->node[i].key<=K<p->node[i+1].key
	int i;
	for (i = 0; i < p->keynum&&p->key[i + 1] <= k; i++);
	return i;
}

result	SearchBTree(BTree T, KeyType k)
// 在m阶B树上查找关键字k，返回结果（pt，i，tag）。若查找成功，则特征值tag=1，指针pt
// 所指结点中第i个关键字等于k；否则返回特征值tag=0，等于k的关键字应插入在pt所指结点
// 中第i和第i+1个关键字之间。
{
	int		i = 1;
	BTree	p = T, q = NULL;						// 初始化，p指向待查结点，q指向p的双亲
	int		found = FALSE;
	while (p && !found)
	{
		i = Search(p, k);							// 查找k的位置使p->key[i]<=k<p->key[i+1]
		if (i> 0 && k == p->key[i])	found = TRUE;
		else {										// 未找到，则查找下一层
			q = p;
			p = p->ptr[i];
		}
	}
	if (found) { result	r = { p, i, 1 };	return r; }	// 查找成功
	else { result	r = { q, i, 0 };	return r; }	// 查找不成功，返回k的插入位置信息
}

void split(BTree &q, int s, BTree &ap) {
	//将q结点分裂成两个结点，前一半保留，后一半移入新结点ap
	int i, n = q->keynum;
	ap = (BTNode*)malloc(sizeof(BTNode));//生成新结点ap
	ap->ptr[0] = q->ptr[s];
	for (i = s + 1; i <= M; i++) {//后一半移入ap结点
		ap->key[i-s] = q->key[i];
		ap->ptr[i-s] = q->ptr[i];
		ap->recptr[i - s] = q->recptr[i];
	}
	ap->keynum = n - s;//计算ap的关键字个数
	ap->parent = q->parent;
	for (i = 0; i <= M - s; i++) {
		if (ap->ptr[i])
			ap->ptr[i]->parent = ap;//将ap所有孩子结点指向ap
	}
	q->keynum = s - 1;//q结点的前一半保留，修改keynum
}

void newroot(BTree &T, BTree p, int k, BTree ap,Record *recptr) {//生成新的根结点
	T = (BTNode*)malloc(sizeof(BTNode));
	T->keynum = 1;
	T->ptr[0] = p;
	T->ptr[1] = ap;
	T->key[1] = k;
	T->recptr[1] = recptr;  //T的子树ap的父亲指针
	if (p != NULL) p->parent = T;
	if (ap != NULL) ap->parent = T;
	T->parent = NULL;//新根的双亲是空指针
}

void Insert(BTree &q, int i, int k, BTree ap,Record *recptr) {//k和ap分别插到q->key[i+1]和q->ptr[i+1]
															//并插入关键字为k的记录recprt
	int j, n = q->keynum;
	for (j = n; j > i; j--) {
		q->key[j + 1] = q->key[j];//关键字指针向后移一位
		q->ptr[j + 1] = q->ptr[j];//孩子结点指针向后移一位
		q->recptr[j + 1] = q->recptr[j];
	}
	q->key[i+1] = k;//赋值
	q->ptr[i+1] = ap;
	q->recptr[i + 1] = recptr;
	if (ap != NULL) ap->parent = q;
	q->keynum++;//关键字数+1
}

Status InsertBTree(BTree &T, KeyType k, BTree q, int i, Record *rec)
//  在m阶B树T上结点*q的key[i]与key[i+1]之间插入关键字K和记录rec。
//  若引起结点过大，则沿双亲链进行必要的结点分裂调整，使T仍是m阶B树。
{
	BTree ap = NULL;
	int finished = FALSE;
	if (!q)    newroot(T, NULL, k, NULL, rec);	// T是空树，生成仅含关键字K的根结点*T
	else {
		while (!finished)
		{
			Insert(q, i, k, ap, rec);			// 将k和ap分别插入到q->key[i+1]和q->ptr[i+1]
			if (q->keynum < M) finished = TRUE; // 插入完成
			else {
				split(q, (M + 1) / 2, ap);			// 分裂结点Q
				k = q->key[(M + 1) / 2];
				rec = q->recptr[(M + 1) / 2];
				if (q->parent)
				{							// 在双亲结点*q中查找k的插入位置
					q = q->parent;
					i = Search(q, k);
				}
				else finished = OVERFLOW;		// 根节点已分裂为*q和*ap两个结点
			}
		}
		if (finished == OVERFLOW)				// 根结点已分裂为结点*q和*ap
			newroot(T, q, k, ap, rec);			// 需生成新根结点*T,q和ap为子树指针
	}
	return OK;
} //  InsertBTree



void TakePlace(BTree &q, int &i) {
	//*q结点的第i个关键字为k，用q的后继关键字代替q，且令q指向后继所在结点
	BTree p = q;
	q = q->ptr[i];
	while (q->ptr[0]) q = q->ptr[0]; //查找p的后继
	p->key[i] = q->key[1]; //记录代替
	p->recptr[i] = q->recptr[1];
	i = 1;  //代替后应该删除q所指结点的第1个关键字
}

void Del(BTree q, int i) {
	//删除q所指结点第i个关键字及其记录
	for (; i < q->keynum; i++) {//关键字和记录指针前移
		q->key[i] = q->key[i + 1];
		q->recptr[i] = q->recptr[i + 1];
	}
	q->keynum--;//关键字数目减1
}

Status	Borrow(BTree q)
// 若q的兄弟结点关键字大于(m-1)/2,则从兄弟结点上移最小（或最大）的关键字到双亲结点，
// 而将双亲结点中小于(或大于）且紧靠该关键字的关键字下移至q中,并返回OK，否则返回EREOR。
{
	int	i;
	BTree p = q->parent, b = NULL;					// p指向q的双亲结点
	for (i = 0; p->ptr[i] != q; i++);			// 查找q在双亲p的子树位置
	if (i >= 0 && i + 1 <= p->keynum && p->ptr[i + 1]->keynum > (M - 1) / 2)
	{										// 若q的右兄弟关键字个数大于(m-1)/2
		b = p->ptr[i + 1];						// b指向右兄弟结点
		q->ptr[1] = b->ptr[0];					// 子树指针也要同步移动
		q->key[1] = p->key[i + 1];				// 从父节点借第i+1个关键字
		q->recptr[1] = p->recptr[i + 1];
		p->key[i + 1] = b->key[1];				// b第一个关键字上移到父节点
		p->recptr[i + 1] = b->recptr[1];
		for (i = 1; i <= b->keynum; i++)			// b第一个关键字上移，需把剩余记录前移一位
		{
			b->key[i] = b->key[i + 1];
			b->recptr[i] = b->recptr[i + 1];
			b->ptr[i - 1] = b->ptr[i];
		}
	}
	else if (i > 0 && p->ptr[i - 1]->keynum > (M - 1) / 2)
	{										// 若q的左兄弟关键字个数大约(m-1)/2
		b = p->ptr[i - 1];						// b指向左兄弟结点
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];
		q->key[1] = p->key[i];					// 从父节点借第i个关键字
		q->recptr[1] = p->recptr[i];
		p->key[i] = b->key[b->keynum];			// 将b最后一个关键字上移到父节点
		p->recptr[i] = b->recptr[b->keynum];
	}
	else return ERROR;							// 无关键字大于(m-1)/2的兄弟
	q->keynum++;
	b->keynum--;
	for (i = 0; i <= q->keynum; i++)
		if (q->ptr[i]) q->ptr[i]->parent = q;	// 刷新q的子结点的双亲指针
	return OK;
}

void Combine(BTree &q) {
	int i, j;
	BTree p = q->parent, b = NULL;//p指向q的父亲
	for (i = 0; p->ptr[i] != q; i++);//查找q在父亲p中的子树位置
	if (i == 0) {//若为0，则需合并为兄弟的第一个关键字
		b = p->ptr[i + 1];
			for (j = b->keynum; j >= 0; j--) {//将b的关键字和记录后移一位
				b->key[j + 1] = b->key[j];
				b->recptr[j + 1] = b->recptr[j];
				b->ptr[j + 1] = b->ptr[j];
			}
			b->ptr[0] = q->ptr[0];//合并
			b->key[1] = p->key[1];
			b->recptr[1] = p->recptr[1];
		}
	else if (i > 0) {//若q在父亲的子树位置大于0，需合并为兄弟b的最后一个关键字
		b = p->ptr[i - 1];
		b->key[b->keynum + 1] = p->key[i];//合并
		b->recptr[b->keynum + 1] = p->recptr[i];
		b->ptr[b->keynum + 1] = q->ptr[0];
	}
	if(i==0||i==1)//若i为0或1，需将父结点p关键字前移一位
		for (; i < p->keynum; i++) {
			p->key[i] = p->key[i + 1];
			p->ptr[i] = p->ptr[i + 1];
			p->recptr[i] = p->recptr[i + 1];
		}
	p->keynum--;
	b->keynum++;
	free(q);
	q = b;   //q指向修改的兄弟结点
	for (i = 0; i <= b->keynum; i++)
		if (b->ptr[i]) b->ptr[i]->parent = b;//刷新b的子结点的双亲指针
}


Status	DeleteBTree(BTree &T, KeyType k)
// 在m阶B树T上删除关键字k及其对应记录，并返回OK。
// 如T上不存在关键字k，则返回ERROR。
{
	KeyType	x = k;
	BTree	q, b = NULL;
	int		finished = FALSE, i = 1;
	result res = SearchBTree(T, k);				// 在T中查找关键字k
	if (res.tag == 0) return ERROR;				// 未搜索到
	else
	{
		q = res.pt;								// q指向待删结点
		i = res.i;
		if (q->ptr[0]) TakePlace(q, i);			// 若q的子树不空，(非底层结点)
												// 则以其后继代之，且令q指向后继所在结点
		Del(q, i);								// 删除q所指向结点中第i个关键字及记录
		if (q->keynum >= (M - 1) / 2 || !q->parent)		// 若删除后关键字个数不小于(m-1)/2或q是根节点
		{
			finished = TRUE;					// 删除完成
			if (q->keynum == 0) T = NULL;		// 若q的关键字个数为0 ，则为空树
		}
		while (!finished)
		{
			if (Borrow(q))	finished = TRUE;	// 若q的相邻兄弟结点关键字大于(m-1)/2,则从该
												// 兄弟结点上移一个最大（或最小）关键字到
												// 父节点，从父节点借一关键字到q
			else {								// 若q相邻兄弟关键字个数均等于┌m /2┑-1
				Combine(q);	// 将q中的剩余部分和双亲中的相关关键字合并至q的一个兄弟中
				q = q->parent;					// 检查双亲
				if (q == T && T->keynum == 0)	// 若被删结点的父节点是根T且T的关键字个数为0
				{
					T = T->ptr[0];				// 新根	
					T->parent = NULL;
					free(q);					// 删除原双亲结点
					finished = TRUE;
				}
				else if (q->keynum >= M / 2) finished = TRUE;
			}								// 合并后双亲关键字个数不少于(m-1)/2，完成
		}
	}
	return OK;
}


void BTreeTraverse(BTree T, void (*visit)(BTree )) {
	//遍历B树T，对每个结点调用visit函数
	if (!T) return;
	visit(T);
	for (int i = 0; i <= T->keynum; ++i) {
		if (T->ptr[i]) BTreeTraverse(T->ptr[i], visit);
	}
}

void	ShowBTree(BTree T, short  x)
// 递归以凹入表形式显示B树T,每层的缩进量为x，初始缩进量为8
{
	if (!T)	return;
	int		i;
	printf("\n");
	for (i = 0; i <= x; i++) putchar(' ');			// 缩进x
	for (i = 1; i <= T->keynum; i++)
	{
		printf("%d,", T->key[i]);
	}
	for (i = 0; i <= T->keynum; i++)				// 递归显示子树结点关键字
		ShowBTree(T->ptr[i], x + 7);
}

int menu() {//菜单
	int choice;
		printf("\n\n\t\t\t|**********************************************|\n");
		printf("\t\t\t|**********************************************|\n");
		printf("\t\t\t ______________________________________________|\n");
		printf("\t\t\t|                 图书馆管理系统               |\n");
		printf("\t\t\t|                                              |\n");
		printf("\t\t\t|   1.采编入库           2.清除库存            |\n");
		printf("\t\t\t|                                              |\n");
		printf("\t\t\t|   3.借阅图书           4.归还图书            |\n");
		printf("\t\t\t|                                              |\n");
		printf("\t\t\t|   5.查看图书馆全部图书 6.查看某图书信息      |\n");
		printf("\t\t\t|                                              |\n");
		printf("\t\t\t|   7.查看某书借阅者信息 8.读取图书信息        |\n");
		printf("\t\t\t|                                              |\n");
		printf("\t\t\t|   0.退出                                     |\n");
		printf("\t\t\t|______________________________________________|\n");
		printf("\t\t\t|**********************************************|\n");
		printf("\t\t\t|**********************************************|\n");
		printf("\t\t\t|              15软件工程(4)班                 |\n");
		printf("\t\t\t|                 3115005372                   |\n");
		printf("\t\t\t|                   杨宇杰                     |\n");
		printf("\t\t\t|********************S**************************|\n");
		do {
			printf("\t\t\t请选择功能（输入0-8任意一个数字）:");
			scanf_s("%d", &choice);
		} while (choice < 0 || choice > 8);//避免非法输入
		return choice;
}


void gotoxy(HANDLE hOut, int x, int y) {//定位光标
	COORD pos;
	pos.X = x;             //横坐标
	pos.Y = y;            //纵坐标
	SetConsoleCursorPosition(hOut, pos);
}


int login() {//登陆界面
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//定义显示器句柄变量
	int secret_code, i;	
	printf("\n\n\t\t\t ______________________________________\n");
	printf("\t\t\t|                                      |\n");
	printf("\t\t\t|                                      |\n");
	printf("\t\t\t|         请输入图书管理员密码！       |\n");
	printf("\t\t\t|            密码：____                |\n");//密码为5372
	printf("\t\t\t|______________________________________|\n");
	gotoxy(hOut, 43, 6);
	scanf_s("%d", &secret_code);
	fflush(stdin);


	printf("\n\t\t\tLoding：");//进入动画
	for (i = 0; i<11; i++) {
		Sleep(50);
		printf("█ ");
	}
	printf("\n\t\t\t载入完成，进入下一级系统");
	Sleep(1000);
	system("cls");//清屏

	if (secret_code == Super_Manager_Code)//判断是不是管理员
		return 1;
	else
		return 0;
}