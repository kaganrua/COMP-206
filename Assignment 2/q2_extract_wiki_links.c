#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){

	int a=0;
	char ch;
	FILE *fp;
	char str[1000000]={'\0'};
	fp = fopen(argv[1], "r"); // read mode

	if (fp == NULL){
		printf("Error while opening the file.\n");
		return 0;
	}
  
 	while((ch = fgetc(fp)) != EOF){
 		str[a]=ch;
 		a++;
 	}
 	fclose(fp);

 	for (char *chr=str; chr!='\0'; ){
 		if(strstr(chr,"<a")==NULL)
 			break;
 		if(strstr(chr, "<a") > strstr(chr,"\n")){
 			chr=strstr(chr,"\n")+1;
 			continue;
 		}
 		chr=strstr(chr,"<a");
 		if(strstr(chr,"href=\"/wiki/")==NULL)
 			break;
 		if(strstr(chr, "href=\"/wiki/")>strstr(chr,"\n")){
 			chr=strstr(chr,"\n")+1;
 			continue;
 		}
 		chr=strstr(chr,"href=\"/wiki/");
 		char pageName[100000]={'\0'};
 		char *c;
 		for(c=chr+12 ; c!='\0' ; c++){
 			if(*c=='\"')
 				break;
 			pageName[c-chr-12]=*c;
 		}
 		if(c =='\0')
 			break;
 		chr=c+1;
 		char str2[8]="title=\"\0";
 		if(strstr(chr,str2) == NULL)
 			break;
 		if(strstr(chr,str2)>strstr(chr,"\n")){
 			chr=strstr(chr,"\n")+1;
 			continue;
 		}
 		chr=strstr(chr,str2);
 		char str3[3]="\">\0";
 		if(strstr(chr,str3)==NULL)
 			break;
 		if(strstr(chr,str3)>strstr(chr, "\n")){
 			chr=strstr(chr, "\n")+1;
 			continue;
 		}
 		chr=strstr(chr,str3);
 		char str4[5]="</a>\0";
 		if(strstr(chr,str4)==NULL)
 			break;
 		if(strstr(chr,str4)>strstr(chr, "\n")){
 			chr=strstr(chr, "\n")+1;
 			continue;
 		}
 		printf("%s\n",pageName);
 		chr=strstr(chr,str4)+4;
 	}

 	return 0;
}

