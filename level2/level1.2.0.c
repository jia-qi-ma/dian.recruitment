#include<stdio.h>
#include<string.h>

typedef struct//座位状态及预约人
{   char student;
    int istaken;
    int isdeleted;
}SEAT;

typedef struct //用于记录学生及其在这一天预约的位置
{
    char name;
    int reserveFloor;
    int reserveRow;
    int reserveCol;
    int havereservation;
    int lahei;
} STUDENT;

SEAT seat[5][4][4];//5指楼层；4，4指行与列
STUDENT stu[26];
int student_id = -1; // 初始化为-1表示未登录
FILE* file;
char day[100];

void select(int floor, int row, int col, char student)//该函数用于选座
{
    // 边界检查：楼层1-5，行列1-4
    if (row<1||row>4||col<1||col>4||floor<1||floor>5||seat[floor-1][row-1][col-1].isdeleted==1) 
    {
        printf("select failed: wrong seat\n");
        return;
    }
    if (seat[floor-1][row-1][col-1].istaken==1) 
    {
        printf("select failed: seat taken\n");
        return;
    }
    if (stu[student_id].lahei==1) 
    {
        printf("you are blocked\n");
        return;
    }
    seat[floor-1][row-1][col-1].istaken=1;
    seat[floor-1][row-1][col-1].student=student;
    stu[student_id].havereservation=1;
    stu[student_id].reserveFloor=floor;
    stu[student_id].reserveRow=row;
    stu[student_id].reserveCol=col;
    printf("Seat reserved successfully!\n");
}

int order(char* command)//该函数用于执行登录退出等命令
{   
    if(strcmp(command,"Login")==0)return 1;//1代表登录
    if(strcmp(command,"Quit")==0)return 2;//2代表退出程序
    if(strcmp(command,"Admin")==0)return 3;//3表示进入管理员系统
    return 0;
}

void commonview(int floor, char student)//该函数用于显示座位情况
{
    if (floor < 1 || floor > 5) {
        printf("wrong floor\n");
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (seat[floor-1][i][j].isdeleted)
                printf("# "); // 被删除的座位显示为#
            else if (seat[floor-1][i][j].istaken && seat[floor-1][i][j].student == student)
                printf("2 "); // 当前用户预约的座位
            else
                printf("%d ", seat[floor-1][i][j].istaken);
        }
        printf("\n");
    }
}

void adminview(int floor) // 该函数用于管理员查看座位
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {   
            if (seat[floor-1][i][j].isdeleted)
            printf("# "); // 被删除的座位显示为#
            if (seat[floor-1][i][j].istaken == 1) printf("%c", seat[floor-1][i][j].student);
            else printf("%d", seat[floor-1][i][j].istaken);
        }
        printf("\n");
    }
}

void login()//该函数用于登录
{
    char username;
    printf("Enter username (A-Z): ");
    scanf(" %c",&username);
    
    // 检查用户名有效性
    if (username<'A'|| username>'Z') 
    {
        printf("wrong username\n");
        return;
    }
    
    student_id = username - 'A';
    printf("Login succeed! User %c\n",username);
}

void cancel(int student_id)//该函数用于取消预约
    {
    if (stu[student_id].havereservation==0) 
    {
        printf("You have no reservation to cancel\n");
        return;
    }
    
    int f = stu[student_id].reserveFloor;
    int r = stu[student_id].reserveRow;
    int c = stu[student_id].reserveCol;
    
    // 验证座位是否确实属于该用户
    if (seat[f-1][r-1][c-1].istaken==1&&seat[f-1][r-1][c-1].student==stu[student_id].name) 
        {    
        seat[f-1][r-1][c-1].istaken=0;
        seat[f-1][r-1][c-1].student='\0';
        stu[student_id].havereservation=0;
        printf("Reservation canceled successfully!\n");
        } 
}

void getdata(FILE* file)//该函数用于读取数据
{
    char bianli;
    for(int i=0;i<5;i++)
    {   
        for(int x=0;x<4;x++)
        {
            for(int y=0;y<4;y++)
            {
                bianli=fgetc(file);
                if(bianli=='#') // 新增：读取删除标记
                {
                    seat[i][x][y].istaken=0;
                    seat[i][x][y].isdeleted=1;
                }
                else if(bianli=='0')
                {
                seat[i][x][y].istaken=0;
                }
                else if(bianli>='A'&&bianli<='Z')
                {
                seat[i][x][y].istaken=1;
                seat[i][x][y].student=bianli;
                }
                else if(bianli==EOF) break;
            }
       
        }
    }
}

void writedata(FILE* file)//该函数用于写入文件
{
     for(int i=0;i<5;i++)
    {   
        for(int x=0;x<4;x++)
        {
            for(int y=0;y<4;y++)
            {   if(seat[i][x][y].isdeleted) 
                    fputc('#', file);
                if(seat[i][x][y].istaken==0)
                fputc('0',file);
                if(seat[i][x][y].istaken==1)
                fputc(seat[i][x][y].student,file);
            }
        }
    }
}

int getday(char day[])
{   
    if(strcmp(day,"Monday")==0) return 1;
    else if(strcmp(day,"Tuesday")==0)return 2;
    else if(strcmp(day,"Wednesday")==0)return 3;
    else if(strcmp(day,"Thursday")==0)return 4;
    else if(strcmp(day,"Friday")==0)return 5;
    else if(strcmp(day,"Satuiday")==0)return 6;
    else if(strcmp(day,"Sunday")==0)return 7;
    else return -1;
}

void openfileread()//用于读取文件时打开对应文件
{
    printf("Enter the day of the week:\n");
                    scanf("%s",day);
                    switch(getday(day))
                    {
                        case 1:{file=fopen("Monday.txt","r");break;}
                        case 2:{file=fopen("Tuesday.txt","r");break;}
                        case 3:{file=fopen("Wednesday.txt","r");break;}
                        case 4:{file=fopen("Thursday.txt","r");break;}
                        case 5:{file=fopen("Friday.txt","r");break;}
                        case 6:{file=fopen("Saturday.txt","r");break;}
                        case 7:{file=fopen("Sunday.txt","r");break;}
                    }
                    getdata(file);
}
void openfilewrite()//用于读取文件时打开对应文件
{
    printf("Enter the day of the week:\n");
                    scanf("%s",day);
                    switch(getday(day))
                    {
                        case 1:{file=fopen("Monday.txt","w");break;}
                        case 2:{file=fopen("Tuesday.txt","w");break;}
                        case 3:{file=fopen("Wednesday.txt","w");break;}
                        case 4:{file=fopen("Thursday.txt","w");break;}
                        case 5:{file=fopen("Friday.txt","w");break;}
                        case 6:{file=fopen("Saturday.txt","w");break;}
                        case 7:{file=fopen("Sunday.txt","w");break;}
                    }
                    getdata(file);
}
void deleteSeat(int floor, int row, int col)
{
    if (row < 1 || row > 4 || col < 1 || col > 4 || floor < 1 || floor > 5) 
    {
        printf("delete failed: wrong seat\n");
        return;
    }
    
    seat[floor-1][row-1][col-1].isdeleted = 1;
    seat[floor-1][row-1][col-1].istaken = 0;
    seat[floor-1][row-1][col-1].student = '\0';
    
    // 如果该座位有预约，取消预约
    for (int i = 0; i < 26; i++) 
    {
        if (stu[i].havereservation &&  stu[i].reserveFloor == floor&&stu[i].reserveRow == row &&stu[i].reserveCol == col)
        {
            stu[i].havereservation = 0;
        }
    }
    
    printf("Seat deleted permanently!\n");
}

void blockUser(char username)
{
    if (username < 'A' || username > 'Z') {
        printf("Invalid username\n");
        return;
    }
    
    int id = username - 'A';
    if (stu[id].lahei == 1) {
        printf("User %c is already blocked\n", username);
        return;
    }
    
    stu[id].lahei = 1;
    printf("User %c has been blocked\n", username);
    
    openfilewrite();  
    if(stu[id].havereservation==1)
    cancel(id);
    writedata(file);
}

void unblockUser(char username)
{
    if (username < 'A' || username > 'Z') {
        printf("Invalid username\n");
        return;
    }
    
    int id = username - 'A';
    if (stu[id].lahei == 0) {
        printf("User %c is not blocked\n", username);
        return;
    }
    
    stu[id].lahei = 0;
    printf("User %c has been unblocked\n", username);
}

void getdatastu(FILE* file)//该函数用于读取数据
{
    file=fopen("stu.txt","r");
    int bianli;
            for(int y=0;y<26;y++)
            {
                bianli=fgetc(file);
                stu[y].lahei=bianli;
            }
    fclose(file);
}
void writedatastu(FILE*file)
{
    file=fopen("stu.txt","w");
    for(int y=0;y<26;y++)
    {
        fputc(stu[y].lahei,file);
    }
    fclose(file);
}

void showblock()
{
    printf("These users are blocked\n");
    for(int i=0;i<26;i++)
    {
        if(stu[i].lahei==1)printf("%c ",stu[i].name);
    }
    printf("\n");
}

int main()
{   
    // 此循环用于录入学生用户名
    for (int i = 0; i < 26; i++) 
    {
        stu[i].name = 'A' + i;
        stu[i].havereservation=0;
        stu[i].lahei=0;
    }
    
    getdatastu(file);
    
    // // 此循环用于初始化座位状态（之后改成从文件中读取）
    for (int f = 0; f < 5; f++) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) 
            {
                seat[f][r][c].istaken=0;
            }
        }
    }

    
    char command[100];//输入命令
    char action[100];
    while (1) {
        printf("\nEnter command (Login/Quit/Admin): ");
        scanf("%s", command);
        switch (order(command)) 
        {
            case 1: //login后登录
                login();
                while (1) {
                    
                    printf("\nEnter command (View/Reserve/Exit/Cancel): ");
                    scanf("%s", action);
                    if (strcmp(action, "Exit")==0) 
                    {
                        printf("Logged out\n");
                        student_id = -1;
                        break;
                    }

                    
                    
                    if (strcmp(action, "View")==0) 
                    {
                        openfileread();
                        int f;
                        printf("Enter floor: ");
                        scanf("%d",&f);
                        commonview(f, stu[student_id].name);
                    } 
                    else if (strcmp(action, "Reserve")==0) 
                    {   openfilewrite();
                        int f, r, c;
                        printf("Enter floor, row, col: ");
                        scanf("%d %d %d", &f, &r, &c);
                        select(f, r, c, stu[student_id].name);
                        writedata(file);
                    }
                    else if (strcmp(action, "Cancel")==0)
                    {   
                        openfilewrite();
                        cancel(student_id);
                        writedata(file);
                    }
                    else 
                    {
                        printf("wrong command\n");
                    }
                    
                }
                break;
                
            case 2: // Quit后退出
                printf("Exiting program\n");
                return 0;

            case 3:
                while (1) {
                    printf("\nEnter command (View/Exit/Clear/Delete/Block/Unblock): ");
                    scanf("%s", action);
                    if (strcmp(action, "Exit")==0) 
                    {
                        printf("Logged out\n");
                        break;
                    }

                    if (strcmp(action, "View")==0) 
                    {
                        openfileread();
                        int f;
                        printf("Enter floor: ");
                        scanf("%d",&f);
                        adminview(f);
                    } 

                    else if (strcmp(action, "Clear")==0) //管理员可清空当天所有预约
                    {
                        openfilewrite();
                        for(int i=0;i<26;i++)
                        {   
                            if(stu[i].havereservation==1)
                            cancel(i);
                        }
                        writedata(file);
                    }

                    else if(strcmp(action, "Delete")==0) // 永久删除座位
                    {
                        openfilewrite();
                        int f, r, c;
                        printf("Enter floor, row, col to delete permanently: ");
                        scanf("%d %d %d", &f, &r, &c);
                        deleteSeat(f, r, c);
                        writedata(file);
                    }

                    else if(strcmp(action, "Block")==0)
                    { 
                        getdatastu(file);
                        char username;
                        showblock();
                        printf("Enter username to block (A-Z): ");
                        scanf(" %c", &username);
                        blockUser(username);
                        writedatastu(file);
                    }

                    else if(strcmp(action, "Unblock")==0)
                    { 
                        getdatastu(file);
                        char username;
                        showblock();
                        printf("Enter username to block (A-Z): ");
                        scanf(" %c", &username);
                        unblockUser(username);
                        writedatastu(file);
                    }


                    else
                    {
                        printf("wrong command\n");
                    }
                }
                break;


            default:
                printf("Unknown command\n");
        }
    }
    fclose(file);
    return 0;
}