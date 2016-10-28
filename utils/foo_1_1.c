#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
struct Node {
	struct Node *right_sibling, *first_child, *parent, *left_sibling;
	char *file_name; // right_sibling: right node, first child: left node,  parent: right parent, left_sibling: left parent
} *root;

struct stat sbs;

/** function */
struct Node *getFirstChild(struct Node *node,char *buffer); // returns first child of node
struct Node *getRightSibling(struct Node *node,char *buffer); // returns right sibling of node
struct Node *getLeftSibling(struct Node *node,char *buffer); // returns left sibling of node
struct Node *getParent(struct Node *node,char *buffer);// returns parent of node
char *getAncestry(struct Node *node, char *buffer); // returns a path in string  from node to root ADDED
struct Node *init(char *file_name); // initializes the root
struct Node *appendChild(struct Node *parent,char *buffer, int mode); //adds a child to folder
void printTree(struct Node *node); // useless representation of sub tree
void printNode(struct Node *node); // prints of current node
char *copySt(char *src); // copys a string pointer
char *concatStr(char *str1, char *str2); // concatinates two string pointers with a / ADDED
char *assignVersion(char *src, int ver); // assigns filename
void makeF(char *fname); // creates the directory under the fname
char* openDir(char *buffer,char *dest);
char *getOneUp(struct Node *node, char *buffer);

char* openDir(char *buffer,char *dest) {
	char temp[128];
	strcpy(temp,buffer);
	strncpy(temp+strlen(temp),"/",2);
	strncpy(temp+strlen(temp),dest,20);
	strncpy(temp+strlen(temp),"/",2);
	char *rr=copySt(temp);
	printf("RR is %s;>\n",rr);
	return rr;
}
void makeF(char *fname) {
	int e;
	e=mkdir(fname,S_IRWXU);
	if(e==0) {
		printf("directory created %s\n",fname);
	}
}
int lengthStr(char *str) {
	int ctr=0; char c;
	while((c=str[ctr])!='\0')
		ctr++;
	return ctr;
}

char *concatStr(char *str1, char *str2) {
	char c, d; int i=0, j=0;
	int l1=lengthStr(str1), l2=lengthStr(str2);
	char *dest=(char *)malloc(l1+l2-1);
	for(i=0;i<l1;i++)
		dest[i]=str1[i];
	dest[l1]='/';
	for(j=0;j<l2;j++)
		dest[l1+j+1]=str2[j];
	dest[l1+j+1]='\0';
	return dest;
}
char *assignVersion(char *src, int ver) {
	char c; int i=0, l=lengthStr(src);
	char *dest=(char *)malloc(l+3);
	for(i=0;i<l;i++)
		dest[i] = src[i];
	dest[l]='.';
	dest[l+1]='0'+(int)ver/10;
	dest[l+2]='0'+ver%10;
	dest[l+3]='\0';
	return dest;
}
char *copySt(char *src) {
	char c; int i=0, l=lengthStr(src);
	char *dest=(char *)malloc(l+3);
	for(i=0;i<l;i++)
		dest[i] = src[i];
	dest[i]='\0';
	return dest;
}
struct Node *getRightMostSibling(struct Node *node, char *buffer) {
	if(node->right_sibling ==NULL) return node;
	else return getRightMostSibling(node->right_sibling,buffer);
}
struct Node *getFirstChild(struct Node *node,char *buffer) {
	/*printf("buffer now %s\n",buffer);
	printf("name:%s\n",node->file_name);
	buffer=openDir(node->file_name,assignVersion(node->file_name,1));
	printf("buff:%s\n",buffer);*/
	return node->first_child;
}
struct Node *getRightSibling(struct Node *node,char *buffer) {
	return node->right_sibling;
}
struct Node *getLeftSibling(struct Node *node,char *buffer) {
	return node->left_sibling;
}
struct Node *getParent(struct Node *node,char *buffer) {
	if(node->left_sibling ==NULL) return node->parent;
	else return getParent(node->left_sibling,buffer);
}
struct Node *init(char *file_name) {
	struct Node *node=(struct Node *)malloc(sizeof(struct Node));
	node->right_sibling=NULL;
	node->first_child=NULL;
	node->file_name=copySt(file_name);
	return node;
}
void initN(struct Node *node) {
	node->right_sibling=NULL;
	node->first_child=NULL;
	node->left_sibling=NULL;
	node->parent=NULL;
}
struct Node *appendChild(struct Node *parent,char *buffer, int mode) {
	char *fname;
	struct Node *tmp=getFirstChild(parent,buffer);
	//if no child
	if(tmp==NULL) {
		parent->first_child=(struct Node *)malloc(sizeof(struct Node));
		initN(parent->first_child);
		parent->first_child->parent=parent;
		parent->first_child->file_name=assignVersion(parent->file_name, 1);
		if(mode) {
			fname=getAncestry(parent->first_child, buffer);
			char *pil=getOneUp(parent->first_child, buffer);
			printf("%s", pil);
			makeF(pil);
		}
	}
	else {
		struct Node *p=NULL;
		int y=1;
		while(tmp!=NULL) {
			p=tmp;
			tmp=tmp->right_sibling;
			y++;
		}
		p->right_sibling=(struct Node *)malloc(sizeof(struct Node));
		initN(p->right_sibling);
		p->right_sibling->left_sibling=p;
		p->right_sibling->file_name=assignVersion(parent->file_name, y);
		if(mode) {
			fname=getAncestry(p->right_sibling, buffer);
			char *pil=getOneUp(p->right_sibling, buffer);
			makeF(pil);
		}
	}
	return parent;
}
char *getAncestry(struct Node *node, char *buffer) {
	char *t=NULL;
	struct Node *tmp=getParent(node, buffer);
	if(tmp!=NULL) {
		char *u=getAncestry(tmp, buffer);
		t=concatStr(u, node->file_name);
	}
	else {
		if(node!=NULL)
			return node->file_name;
	}
	return t;
}
void printTree(struct Node *node) {
	if(node!=NULL) {
		printf("%s\t", node->file_name);
		printf("Going to sister\t");
		printTree(node->right_sibling);
		printf("Going to child\n");
		printTree(node->first_child);
	}
}
void printNode(struct Node *node) {
	printf("%s\n", node->file_name);
}

char *getOneUp(struct Node *node, char *buffer) {
	char *a=getParent(node, buffer)->file_name;
	char *b=concatStr(a, node->file_name);
	return b;
}
/*
int main() {
	printf("Master File Name\n");
	char R[20];
	scanf("%s", R);
	root=init(R);
	makeF(R);
	DIR *dp=NULL;
	struct dirent *dptr=NULL;
	char *buffer;
	//memset(buffer,0,sizeof(buffer));
	//strcpy(buffer,R);
	struct Node *curr=root;
	printNode(curr);
	struct Node *tmp;
	int ch=0;
	while(ch<8) {
		printf("\nChoose:\n1.goto Child\n2.goto Parent\n3.goto Left Sister\n4.goto Right Sister\
		\n5. Insert Child\n6. Print Tree\n7. Print Subtree Tree\n--------------\n");
		printf("Currenly in Node: \n"); printNode(curr);
		scanf("%d", &ch);
		switch(ch) {
			case 1: 
				tmp=getFirstChild(curr,buffer);
				//buffer
				if(tmp==NULL) printf("Does not exist\n");
				else curr=tmp;
				break;
			case 2: tmp=getParent(curr,buffer);
				if(tmp==NULL) printf("Does not exist\n");
				else curr=tmp;
				break;
			case 3: tmp=getLeftSibling(curr,buffer);
				if(tmp==NULL) printf("Does not exist\n");
				else curr=tmp;
				break;
			case 4: tmp=getRightSibling(curr,buffer);
				if(tmp==NULL) printf("Does not exist\n");
				else curr=tmp;
				break;
			case 5: appendChild(curr,buffer,1);
				break;
			case 6: printTree(root);
				break;
			case 7:	printTree(curr);
				break;
			default:printf("Exiting\n");		
		}
	}
	return 0;
}*/
