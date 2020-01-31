#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	if(argc !=2){
	printf("Error! You need to enter just one height value!\n");
	return 0;
}
 int i,j,k,n;
 n = atoi(argv[1]);
	 if( n < 0){
	printf("Error! Height value can not be negatif!\n");
	return 0;
}
	else if(n%2 == 0){
	printf("Error! Height value cannot be an even integer!\n");
	return 0;
}
	else{

n=(n+1)/2;
    for(i=1;i<=n;i++)
    {
        for(j=n;j>=i;j--)
            printf(" ");
        for(k=1;k<=(2*i-1);k++)
            printf("*");
        printf("\n");
    }
    for(i=1;i<=(n-1);i++)
    {
        for(j=0;j<=i;j++)
            printf(" ");
        for(k=(2*n-3);k>=(2*i-1);k--)
            printf("*");
        printf("\n");
}
}
}

