//把某文件的单词逆序输出
#include <stdio.h>
#include <String.h>
#include <stdlib.h>

#ifndef N
#define N 1000
#endif

#ifndef M
#define M 20
#endif

int main()
{
	char word[N][M],c;
	int i=0,j=0;
	FILE *fp;
	for (int a = 0; a < N; ++a){
		memset(word[a],0,20);
	}
	if((fp=fopen("./test.txt","rb"))==NULL)
    {
        printf("can not open file\n");
        exit(0);
    }
	while((c=fgetc(fp))!=EOF){
		//printf("%c",c);
		if (c!=' '&&c!='\r'&&c!='\n'){
			word[i][j]=c;
			j++;
		}
		if (c==' '||c=='\r'){
			word[i][j]='\0';
			j=0;
			i++;
		}
		/*if (c=='\r'){
			word[i][j]='\r';
			word[i][j+1]='\n';
			word[i][j+2]='\0';
			j=0;
			i++;
		}*/
	}
	fclose(fp);
	for (; i>=0; i--){
		printf("%s ",word[i]);
	}
	return 0;	
}