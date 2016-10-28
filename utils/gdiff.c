#include<stdio.h>
#include<malloc.h>
#include<string.h>
#define N 100000

void printAddedTargets(char *str, int st, int end, int ln, FILE *fw) {
	int i;
	fprintf(fw, "$ADDING %d\n", ln);
	for(i=st;i<end;i++)
		fprintf(fw, "%c", str[i]);
}
void gdiff(char *old, char *mod, char *wr) {
	FILE *fp1=fopen(old, "r"), *fp2=fopen(mod, "r"), *fw=fopen(wr, "w+");
	char str1[N], str2[N], ssub[1000];
	int l1=cvtToText(fp1, str1);
	int l2=cvtToText(fp2, str2);
	int i=0, line_no=0, j=0, l=0, pin=0;
	printf("%s\n\n", str2);
	while((l=getSubstring(str1, ssub, i))!=-1) {
		printf("%s\n", ssub);
		int k=horspoolMatching(str2, ssub, j);
		printf("k=%d i=%d l=%d j=%d\n", k, i, l, j);
		if(k==-1) { fprintf(fw, "$REMOVE %d\n", line_no); pin-=l-i;}// If not there
		else if(k!=i+pin){ printAddedTargets(str2, i+pin, k, line_no, fw); j=k; pin=k-i;} // it added in between
		else j=k;
		line_no++;
		i=l;
	}
	j=getSubstring(str2, ssub, j);
	if(j<l2-1) printAddedTargets(str2, j, l2-1, line_no, fw);
}/*
int main(int argc, char **argv) {
	gdiff(argv[1], argv[2], argv[3]);
}*/
