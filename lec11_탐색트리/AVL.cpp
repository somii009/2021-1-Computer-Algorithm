#include <stdio.h>
#include <stdlib.h>

#define max(a,b) (((a) > (b)) ? (a):(b))

typedef struct TreeNode {
	int key, height;
	struct TreeNode* parent, *left, *right;
}TreeNode;

typedef struct
{
	TreeNode* root;
}TreeType;

void initTree(TreeType* T)
{
	T->root = (TreeNode*)malloc(sizeof(TreeNode));
	T->root->parent = T->root->left = T->root->right = NULL;
	T->root->height = 0;
}

int isExternal(TreeNode* w)
{
	return(w->left == NULL && w->right == NULL);
}

void expandExternal(TreeNode* w)
{
	TreeNode* l = (TreeNode*)malloc(sizeof(TreeNode));
	TreeNode* r = (TreeNode*)malloc(sizeof(TreeNode));
	l->left = NULL;
	l->right = NULL;
	l->parent = w;
	r->left = NULL;
	r->right = NULL;
	r->parent = w;
	w->left = l;
	w->right = r;
}

TreeNode* treeSearch(TreeNode* v, int k)
{
	if (isExternal(v))
		return v;
	if (k == v->key)
		return v;
	else if (k < v->key)
		return treeSearch(v->left, k);
	else
		return treeSearch(v->right, k);
}

int findElement(TreeType* T, int k)
{
	TreeNode* w = treeSearch(T->root, k);

	if (isExternal(w))
		return 0;
	else
		return w->key;
}

void rFindAllInRange(TreeNode* v, int k1, int k2)
{
	if (isExternal(v))
		return;
	if (k1 <= v->key && v->key < k2)
		printf("[%d] ", v->key);
	if (v->key <= k1)
		rFindAllInRange(v->right, k1, k2);
	else if (k2 <= v->key)
		rFindAllInRange(v->left, k1, k2);
	else
	{
		rFindAllInRange(v->left, k1, k2);
		rFindAllInRange(v->right, k1, k2);
	}
}

int heightUpdateAndBalanceCheck(TreeNode* w)
{
	TreeNode* l, *r;
	int b;

	if (w == NULL)  //끝까지 올라가면 balance 깨지지 않았다.
		return 1;

	l = w->left;
	r = w->right;

	w->height = max(r->height, l->height) + 1;

	b = r->height - l->height;
	if (b*b < 2)  //음수일수도 있으니 제곱
		return 1;
	else
		return 0;
}

TreeNode* restructure(TreeType* T, TreeNode* x)
{
	TreeNode* y, *z, *a, *b, *c, *T0, *T1, *T2, *T3;
	y = x->parent;
	z = y->parent;
	
	if (z->key < y->key && y->key < x->key) // RR, 다 오른쪽으로 붙어있다
	{
		a = z;
		b = y;
		c = x;
		T0 = a->left;
		T1 = b->left;
		T2 = c->left;
		T3 = c->right;
	}

	else if (x->key < y->key && y->key < z->key) // LL, 다 왼쪽으로 붙어있음
	{
		a = x;
		b = y;
		c = z;
		T0 = a->left;
		T1 = a->right;
		T2 = b->right;
		T3 = c->right;
	}

	else if (z->key < x->key && x->key < y->key) //RL
	{
		a = z;
		b = x;
		c = y;
		T0 = a->left;
		T1 = b->left;
		T2 = b->right;
		T3 = c->right;
	}
	else //LR
	{
		a = y;
		b = x;
		c = z;
		T0 = a->left;
		T1 = b->left;
		T2 = b->right;
		T3 = c->right;
	}

	//3.
	if (z == T->root)  //z가 전체 트리의 루트일때
	{
		T->root = b;
		b->parent = NULL;
	}
	else if (z->parent->left == z)
	{
		z->parent->left = b;
		b->parent = z->parent;
	}
	else
	{
		z->parent->right = b;
		b->parent = z->parent;
	}

	//4.
	a->left = T0;
	T0->parent = a;
	a->right = T1;
	T1->parent = a;
	a->height = max(T0->height, T1->height) + 1;

	//5.
	c->left = T2;
	T2->parent = c;
	c->right = T3;
	T3->parent = c;
	c->height = max(T2->height, T3->height) + 1;

	//6.
	b->left = a;
	a->parent = b;
	b->right = c;
	c->parent = b;
	b->height = max(a->height, c->height) + 1;

	return b;
}

void searchAndFixAfterInsertion(TreeType* T, TreeNode* w)
{
	TreeNode* z = w;
	TreeNode* y, *x;
	y = z->left;
	x = y->left;

	//1.----------
	while (heightUpdateAndBalanceCheck(z))
	{
		if (z == NULL)  //끝까지 다 올라감
			return;
		z = z->parent;
	}

	//2. -------------
	if (z->left->height > z->right->height)
		y = z->left;
	else
		y = z->right;


	//3. -------------
	if (y->left->height > y->right->height)
		x = y->left;
	else if (y->left->height < y->right->height)
		x = y->right;

	//4. --------------
	restructure(T, x);

}

void insertItem(TreeType* T, int k)
{
	TreeNode* w = treeSearch(T->root, k);
	if (!isExternal(w)) //inInternal 대신해서 사용
		return;
	else
	{
		w->key = k;
		expandExternal(w);
		searchAndFixAfterInsertion(T, w);
	}
}

TreeNode* sibling(TreeNode* z)
{
	if (z->parent->left == z)
		return z->parent->right;
	else
		return z->parent->left;
}

TreeNode* reduceExternal(TreeType* T, TreeNode* z)
{
	TreeNode* w = z->parent;
	TreeNode* g = w->parent;
	TreeNode* zs = sibling(z);
	zs->parent = g;

	if (g == NULL)
		T->root = zs;
	else
	{
		if (w == g->left) //삭제되는 노드가 부모노드의 오른쪽인지 왼쪽인지 확인
			g->left = zs;
		else
			g->right = zs;
	}
	free(z);
	free(w);
	return zs;
}

TreeNode* inOrderSucc(TreeNode* w)
{
	w = w->right;
	while (w->left != NULL)
		w = w->left;
	return w;
}

void searchAndFixAfterRemoval(TreeType* T, TreeNode* w)  //수도코드 보면서 비교해보기
{
	TreeNode* z = w;
	TreeNode* y, *x, *b;
	x = NULL;
	while (heightUpdateAndBalanceCheck(z))
	{
		if (z == NULL)
			return;
		z = z->parent;
	}

	//2. ---------------
	if (z->left->height > z->right->height)
		y = z->left;
	else
		y = z->right;

	//3. ---------------
	if (y->left->height > y->right->height)
		x = y->left;
	else if (y->left->height < y->right->height)
		x = y->right;
	else if (z->left == y)  //높이가 같은 경우 타고온 방향대로
		x = y->left;
	else if (z->right == y)
		x = y->right;

	//4. -----------------
	b = restructure(T, x);

	//5. ------------------
	searchAndFixAfterRemoval(T, b);
}

int removeElement(TreeType* T, int k)
{
	TreeNode* w = treeSearch(T->root, k);

	if (isExternal(w))
		return 0;

	TreeNode* z, *y, *zs;
	z = w->left;
	if (!isExternal(z))
		z = w->right;
	if (isExternal(z))
		zs = reduceExternal(T, z); //추가된 부분
	else
	{
		z = inOrderSucc(w); //중위계승자 찾는 함수
		y = z->parent;
		w->key = y->key;
		zs = reduceExternal(T, z);  //추가
	}
	searchAndFixAfterRemoval(T, zs->parent); //삭제된 위치의 서브트리의 부모를 기준으로 균형 맞추기
	return k;
}

void preOrder(TreeNode* root)
{
	if (isExternal(root))  //이부분이 없으면 보조노드의 비어있는 값이 같이 출력돼서 쓰레기값이 함께출력됨.
		return;
	printf("[%d] ", root->key);
	preOrder(root->left);
	preOrder(root->right);
}

int main()
{
	TreeType* T = (TreeType*)malloc(sizeof(TreeType));
	initTree(T);

	insertItem(T, 44);
	insertItem(T, 17);
	insertItem(T, 32);
	preOrder(T->root); printf("\n");
	insertItem(T, 78);
	insertItem(T, 50);
	insertItem(T, 88);
	preOrder(T->root); printf("\n");
	insertItem(T, 48);
	insertItem(T, 62);
	insertItem(T, 54);
	preOrder(T->root); printf("\n");
	
}
