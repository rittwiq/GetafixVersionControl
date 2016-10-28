#include<stdio.h>
#include<malloc.h>
#define N 100000

int addingLines(char *str, int l, char *lines, int n, int lno, char *text) {
	int i, j=0, k=0, m=0, ln=1;
	for(i=0;i<l && j<lno;i++)
		if(str[i]=='\n')
			j++;
	k=i;
	for(i=0;i<n;i++)
		if(lines[i]=='\n')
			m++;
	for(i=0;i<k;i++)
		text[i]=str[i];
	for(i=0;i<n;i++)
		text[i+k]=lines[i];
	for(i=k;i<l+n;i++)
		text[i+n]=str[i];
	for(i=0;i<strlen(text)+n;i++)
		str[i]=text[i];
	text[i]='\0';
	for(i=0;i<n;i++)
		if(lines[i]=='\n') ln++;
	return ln;
}
int lineCount(char *str, int end) {
	int ln=1, i=0;
	while(str[i]!='\0' && i<end)
		if(str[i++]=='\n') ln++;
	return ln;
}
int nthLineIndex(char *str, int n) {
	int i=0, ctr=0;
	while(str[i]!=EOF && ctr<n) {
		if(str[i]=='\n') ctr++;
		i++;
	}
	return i;
}
void deleteL(char *str, int l, int st, int end) {
	if((end-st)<l) {
		int len=end-st, i;
		for(i=st;i<l-len;i++) {
			str[i]=str[i+len];
		}
		str[i]='\0';
	}
}
void update(char *src, char *ed, char *store) {
	FILE *fp=fopen(src, "r"), *fq=fopen(ed, "r");
	char str1[N], str2[N], ssub[N], temp[N];
	int l1=cvtToText(fp, str1);
	int l2=cvtToText(fq, str2);
	int i=0,l=0,  line_no=0, j=0, k, off=0, p=lineCount(str1, N);
	while((l=getSubstring(str2, ssub, i))!=-1 && i<l2) {
		if(ssub[0]=='$') {
			k=8;
			line_no=0;
			while(ssub[k]!='\n')
				line_no=line_no*10 + (ssub[k++]-'0');
			line_no+=off;
			j=nthLineIndex(str1, line_no);
			k=nthLineIndex(str1, line_no+1);
			if(ssub[1]=='R') {
				deleteL(str1, l1, j, k);
				l1=l1-(k-j);
				off--;
			}
			else {
				char line[N];
				l=getSubstring(str2, ssub, l);
				while(l!=-1 && ssub[0]!='$') {
					strcat(line, ssub);
					l=getSubstring(str2, ssub, l);
				}
				off+=addingLines(str1, l1, line, strlen(line), line_no, temp);
				l1=l1+strlen(line);
			}
		}
		i=l;
		if(i==-1) break;
	}
	printf("FINALLY \n%s\n", str1);
	str1[l1]=EOF;
	fclose(fp);
	fclose(fq);
	FILE *fw=fopen(store, "w+");
	fprintf(fw, "%s", str1);
	fclose(fw);
}

