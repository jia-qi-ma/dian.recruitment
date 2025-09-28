#include<stdio.h>
#include<string.h>
int main()
{
    char Dian[]="Dian",Quit[]="Quit",putin[20];
    while(1)
    {
    printf("put in:\n");
    scanf("%s",putin);
    if(strcmp(Dian,putin)==0)printf("2002\n");
    else if(strcmp(Quit,putin)==0)return 0;
    else printf("Error\n");
    }
}