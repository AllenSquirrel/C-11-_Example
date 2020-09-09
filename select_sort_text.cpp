#include<iostream>
#include<math.h>
#include<windows.h>
using namespace std;

void sort(int a[],int len)
{
	int i, inner,temp;
	for (i = 1; i < len; i++)
	{
		temp = a[i];
		inner = i;
		for (; inner > 0 && a[inner - 1] >= temp; inner--)
		{
			a[inner] = a[inner - 1];
		}
		a[inner] = temp;
	}
	for (i = 0; i<len; i++)
	{
		printf("%d",a[i]);
	}
}

int main()
{
	int arr[10] = { 7,2,4,3,8,9,6,0,5,1 };
	sort(arr, 10);
	system("pause");
	return 0;
}


//struct Node* sort1(struct Node* head)                    
//{                                                                         
//	struct Node *f, *t, *p = NULL, *q, *h;
//	h = head->next;
//	f = h->next;                  //f指向旧链的第一个节点，即等待在新链中“安家落户”（插入）的节点
//	h->next = NULL;               //将原链的第一个节点单拿出来作为新链（待插入链）的第一个节点，默认此节点是关键值最大的节点
//	while (f != NULL)    //当f=NULL，旧链中的节点都插入到了新链，排序完成
//	{
//		for (t = f, q = h; (q != NULL && (q->data > t->data)); p = q, q = q->next);//t和f同指，当找到插入位置，f指向旧链的下一个节点时，用t来进行
//														 //插入操作；q先指向新链的第一个节点，q不断在新链中后移，以找到f（即t）所指节点的插入位置
//														 //p作为q的前驱，用来完成插入。整个语句的作用是：在新链遍历完（q != NULL）的前提下，在新
//														 //链中找到第一个关键值比f（即t）所指节点关键值小的节点，毫无疑问，q的前驱，即p（如果有的
//														 //话）的关键值一大于定f（即t）所指节点关键值（否则q怎么会后移到当前位置呢?）；如果没有，
//														 //那说明当前新链的头节点关键值比f（即t）所指节点关键值小；如果最后q = NULL了，说明当前新
//														 //链的最后一个节点（此时p正指向它）的关键值都比f（即t）所指节点关键值大。不管哪种情况，f
//														  //（即t）所指节点都应插在q所指节点前，p所指节点后（如果有的话）
//		f = f->next;         //在进行插入操作前，先使f后移
//		if (q == h) h = t;   //如果当前新链的头节点关键值比f（即t）所指节点关键值小，需要将f（即t）所指节点插在该头节点前，先让新链头节点指针指向
//							 //f（即t）所指节点，作为新链的新的头节点
//		else  p->next = t;   //否则，将f（即t）所指节点连在p所指节点后
//		t->next = q; //不管if还是else，都需要将f（即t）所指节点连在q所指节点前，如果q=NULL，就是让f（即t）所指节点的next域指向NULL,这显然也是正确的
//	}
//	return head;  //返回新链（排好序的链）的头节点指针
//}