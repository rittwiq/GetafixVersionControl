#include<string.h>
int max(int a, int b) {
	return (a>b)?a:b;
}
void createShiftTable(int *Ar, char *str, int l) {
	int i;
	for(i=0;i<128;i++) Ar[i]=l;
	for(i=0;i<l-1;i++) Ar[str[i]]=l-1-i;
	//for(i=0;i<128;i++) printf("%c: %d; ", (char)i, Ar[i]);
}
int horspoolMatching(char *text, char *pattern, int st) {
	int Ar[255];
	int tl=strlen(text);
	int pl=strlen(pattern);
	createShiftTable(Ar, pattern, pl);
	int i=max(pl-1, st), j;
	while(i<tl) {
		int k=0;
		while(k<=pl-1 && pattern[pl-1-k]==text[i-k])  k++;
		if(k==pl) return i-pl+1;
		else i+=Ar[text[i]];
	}
	return -1;
}

