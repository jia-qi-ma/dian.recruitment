#include<stdio.h>
#include<string.h>
int main()
{
    char putin[100],bianli[20][1024],key[20][1024],ans[20][1024];
    int fenjie=0,len=0;//fenjie是冒号位置，len是行数
    FILE* file=fopen("data.txt","r");

    //该循环用于读取文件并录入字符串
    for(int i=0;;i++)
    {
        if(fgets(bianli[i],20,file)==NULL)break;
        len++;//表示行数
        int isright=0; //用于检测错行
        for(int j=0;;j++)//此处循环用于录入“键”并检测错行
            {  
            if(bianli[i][j]==':'){fenjie=j;isright=1;break;}
            key[i][j]=bianli[i][j];
            if(bianli[i][j]=='\0'||bianli[i][j]=='\n')break;
            }
        if(isright==0)printf("No.%d line is wrong\n",len);//若为错行，则输出错误行数
        //printf("%s\n",key[i]);//测试打印键（删）
        for(int j=fenjie;;j++)//此处循环用于录入“值”
            {
            if(bianli[i][j]=='\0')break;
            ans[i][j-fenjie]=bianli[i][j+1];
            }
        //printf("%s\n",ans[i]);//测试打印值（删）

        //printf("%s\n",bianli[i]);//测试打印遍历（删）
    }

    //该循环用于键盘录入并比对输出
    while(1)
    {
    printf("put in:\n");
    gets(putin);//键盘录入
    if (strcmp(putin,"Quit")==0)break;//是Quit就退出
    printf("put out:\n");
    int isright=0;//用于检测是否有对应的键
    for(int i=0;i<len;i++)
    if(strcmp(putin,key[i])==0)
    {
        printf("%s\n",ans[i]);
        isright=1;
    }
    if(isright==0)printf("Error\n");
    }
    fclose(file);
    return 0;
} 
       