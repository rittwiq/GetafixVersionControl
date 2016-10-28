#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<malloc.h>
#include "utils/GVCS.h"

void help() {
	printf("\nGetafix Version Control System (GVCS)\n--------------\n\nVersion Control System used to check in and check out important work\n\
	\n -commit - confirm any changes to the master copy to the GVCS folder\n\n -update - make any changes to the previously editted document\
	\n\n -view [n] - prints the change at version n\n\n");
}
char *catStr(char *str1, char *str2) {
	char c, d; int i=0, j=0;
	int l1=lengthStr(str1), l2=lengthStr(str2);
	char *dest=(char *)malloc(l1+l2-1);
	for(i=0;i<l1;i++)
		dest[i]=str1[i];
	for(j=0;j<l2;j++)
		dest[l1+j]=str2[j];
	dest[l1+j]='\0';
	return dest;
}
int validate_path(char *path) {
	FILE *fp=fopen(path, "r");
	if(fp!=NULL)
		return 1;
	return 0;
}
char *strGetName(char *str) {
	int len=lengthStr(str), i=0;
	char *name=(char *)malloc((len)*sizeof(char));
	while(i<len && str[i]!='.') {
		name[i]=str[i];
		i++;
	}
	name[i]='\0';
	return name;
}

void refreshLog(char *name, int version) {
	FILE *fp=fopen("GVCS.log", "w+");
	int y=lengthStr(name), i=0, k=-1;
	char line[N], ssub[N];
	int l=cvtToText(fp, line);
	fprintf(fp, "%d", version+1);
	fclose(fp);
}
int versionRecovery(char *name) {
	FILE *fp=fopen("GVCS.log", "w+");
	fprintf(fp, "%d\n",0);
	fclose(fp);
	return 0;
}

int getNumberOfVersions(char *name) {
	FILE *fp=fopen("GVCS.log", "a+");
	int y=lengthStr(name), i=0, k=-1;
	char line[N], ssub[N];
	int l=cvtToText(fp, line);
	int n=atoi(line);
	if(n==-1)
		n=versionRecovery(name);
	return n;
}
struct Node *reinit(char *name, int a) {
	int i;
	char *R=name;
	struct Node *r=init(R);
	DIR *dp=NULL;
	struct dirent *dptr=NULL;
	for(i=0;i<a;i++)
		appendChild(r, R, 0);
	return r;
}

struct Node *openFolder(char *path) {
	DIR *d;
	struct dirent *dir;
	char *name=strGetName(path);
	d=opendir(name);
	struct Node *root;
	if(!d) {
		makeF(name);
		root=init(name);
	}
	else {
		int ame=getNumberOfVersions(name);
		root=reinit(name, ame);
		printf("%d", ame);
		printTree(root);
	}
	return root;
}
void addFile(char *src, char *dest, char *mode) {
	FILE *source;
	FILE *destination=fopen(dest, mode);
	if(validate_path(src))	{
		source=fopen(src, "r");
		char line[N];
		int l=cvtToText(source, line); line[l-1]='\0';
		fprintf(destination, "%s", line);
	}
	else source=fopen(src, "w+");
	fclose(source);
	fclose(destination);
}
void commit(char *path_name){
	if(!validate_path) printf("Invalid Path File!\n\n");
	else {
		printf("Path %s is valid\n", path_name);
		char *buffer, *str=strGetName(path_name), *dest;
		memset(buffer,0,sizeof(buffer));
		buffer=copySt(str);
		struct Node *root=openFolder(path_name), *tmp, *curr;
		tmp=getFirstChild(root, buffer);
		while(tmp!=NULL && getRightSibling(tmp, buffer)!=NULL)
			tmp=getRightSibling(tmp, buffer);
		dest=getOneUp(tmp, buffer);
		dest=concatStr(dest, path_name);
		printf("\n%s\n", dest);
		gdiff("ccom.txt", "locked.txt", dest);
		
	}
}
void vupdate(char *path_name){
	if(!validate_path(path_name))
		printf("invalid Pathname\n");
	else {
		char *str=strGetName(path_name); // creates aa copy of master file
		char *master_file_name=concatStr(str, path_name);
		addFile("locked.txt", "ccom.txt", "w+");
		char *buffer, *m;
		buffer=copySt(str);
		int i=0;
		DIR *dp=NULL;
		struct dirent *dptr=NULL;
		struct Node *root=openFolder(path_name), *tmp=NULL, *curr=NULL;
		printf("%s\n", buffer);
		appendChild(root, buffer, 1);
		tmp=getFirstChild(root, buffer);
		if(tmp!=NULL) {
			char *p=getOneUp(tmp, buffer);
			m=concatStr(p, path_name);
			addFile(m, "key.txt", "a+");
			tmp=getRightSibling(tmp, buffer);
			while(tmp!=NULL && getRightSibling(tmp, buffer)!=NULL) {
				p=getOneUp(tmp, buffer);
				m=concatStr(p, path_name);
				addFile(p, "key.txt", "w+");
				tmp=getRightSibling(root, buffer);
				//update("honey.txt", "key.txt", "locked.txt");
				//addFile("locked.txt", "ccom.txt", "w+");
				i++;
			}
		}
		/*if(i>0) {
			update("honey.txt", "key.txt", "locked.txt");
			addFile("locked.txt", "ccom.txt", "w+");
		}*//*
		if(i==0) {
			addFile(master_file_name, "locked.txt", "w+");
			addFile("locked.txt", "ccom.txt", "w+");
		}*/
		refreshLog(str, getNumberOfVersions(str));
	}
}
void view(char *path_name, int version){
	if(!validate_path(path_name))
		printf("invalid Pathname\n");
	else {
		char *str=strGetName(path_name); // creates aa copy of master file
		char *master_file_name=concatStr(str, path_name);
		addFile(master_file_name, "honey.txt", "w+");
		char *buffer, *m;
		buffer=copySt(str);
		int i=0;
		DIR *dp=NULL;
		struct dirent *dptr=NULL;
		struct Node *root=openFolder(path_name), *tmp=NULL, *curr=NULL;
		printf("%s\n", buffer);
		tmp=getFirstChild(root, buffer);
		if(tmp!=NULL) {
			char *p=getOneUp(tmp, buffer);
			m=concatStr(p, path_name);
			addFile(m, "key.txt", "a+");
			tmp=getRightSibling(tmp, buffer);
			while(tmp!=NULL) {
				p=getOneUp(tmp, buffer);
				m=concatStr(p, path_name);
				addFile(p, "key.txt", "w+");
				update("honey.txt", "key.txt", "honey.txt");
				tmp=getRightSibling(root, buffer);
				i++;
			}
		}
		//if(i>0) {
			addFile("honey.txt", "locked.txt", "w+");
			//update("locked.txt", "key.txt", "locked.txt");
			//update("ccom.txt", "key.txt", "locked.txt");
		/*}
		else {
			addFile("locked.txt", "ccom.txt", "w+");
		}*/
	}
}
void createFirstFile(char *path_name) { // Create Master Copy
	if(!validate_path(path_name))
		printf("invalid Pathname\n");
	else {
		openFolder(path_name);
		char *str=strGetName(path_name);
		char *master_file_name=concatStr(str, path_name);
		addFile(path_name, master_file_name, "w+");
		addFile(master_file_name, "locked.txt", "w+");
		
	}
}
int main(int argc, char **argv) {
	if(argc>4)
		help();
	else {
		if(argc==3 && strcmp(argv[2], "-commit")==0)
			commit(argv[1]);
		else if(argc==3 && strcmp(argv[2], "-update")==0)
			vupdate(argv[1]);
		else if(argc==4 && strcmp(argv[2], "-view")==0)
			view(argv[1], atoi(argv[3]));
		else if(argc==2 && strcmp(argv[1], "-help")==0)
			help();
		else if(argc==2)
			createFirstFile(argv[1]);
		else help();
	}
	return 1;
}
