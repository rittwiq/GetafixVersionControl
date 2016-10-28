int cvtToText(FILE *fp, char *str) {
	int i=0; char ch;
	do {
		ch=fgetc(fp);
		str[i++]=ch;
	} while(ch!=EOF);
	return i;
}
int getSubstring(char *str, char *sstring, int pos) {
	if(str[pos]==EOF) return -1;
	int i=0;
	char ch;
	do {
		ch=str[pos+i];
		sstring[i]=ch;
		i++;
	} while(ch!='\n');
	sstring[i]='\0';
	return pos+i;
}
