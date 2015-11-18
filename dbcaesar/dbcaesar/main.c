//
//  main.c
//  dbtest
//
//  Created by Xiangtai Hou on 15/11/9.
//  Copyright © 2015 Xiangtai Hou. All rights reserved.
//

#include <mysql.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//using namespace std;

//variables
MYSQL *connection, mysql;

//functions declaration
void get_month(int * month);
void get_year(int * year);
void get_date(int * month, int * day, int * year);
char * get_semester(int month);
void loginStudent();
void studentMenu(char* username);
void transcript(char* username);


//get local time
void get_month( int * month) {
    struct tm *current;
    time_t timenow;
    time(&timenow);
    current = localtime(&timenow);
    *month = current->tm_mon+1;
    return;
}
void get_year( int * year ) {
    struct tm *current;
    time_t timenow;
    time(&timenow);
    current = localtime(&timenow);
    *year = current->tm_year+1900;
    return;
}
void get_date( int * month, int * day, int * year ) {
    struct tm *current;
    time_t timenow;
    time(&timenow);
    current = localtime(&timenow);
    *month = current->tm_mon+1;
    *day = current->tm_mday;
    *year = current->tm_year+1900;
    return;
}
//get semester
char* get_semester(int month) {
    
    char* sem;
    get_month(&month);
    
    if (month >= 9 && month <= 12 ) {
        sem = "Q1";
    }
    else if(month >= 1 && month <= 3) {
        sem = "Q2";
    }
    else if(month >= 4 && month <= 6) {
        sem = "Q3";
    }
    else {
        sem = "Q4";
    }
    return sem;
}

//transcript screen
void transcript(char* username) {
    printf("\n\t\t\t---------------------------------------\n");
    printf("\t\t\t              Transcript               \n");
    printf("\t\t\t---------------------------------------\n");
    //query
    char q[200] = "\0";
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    sprintf(q, "select t.semester, u.UoSCode, t.grade, u.UoSName from unitofstudy u, transcript t where u.UoSCode = t.UoSCode and t.Studid = %s order by t.semester;", username);
    //puts(q);
    mysql_query(connection,q);
    res_set = mysql_store_result(connection);
    int numrows = (int)mysql_num_rows(res_set);
    // Display results
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if( row != NULL )
        {
            printf("\t%s ", row[0]);
            printf("\t%s ", row[1]);
            printf("\t%-5s", row[2]);
            printf("\t%s\n", row[3]);
            
        }
    }
    //free resources
    mysql_free_result(res_set);
    char z[20];
    do {
        printf("\n\t\t\t---------------------------------------\n");
        printf("\t\t\t           Commands List           \n");
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t  [course number]check course details  \n");
        printf("\t\t\t  [0]Back to main menu                 \n");
        printf("\t\t\t---------------------------------------\n\n");
        printf("Please enter the command: ");
        scanf("%s", z);
        //system("color 2f");
        if(strcmp(z, "0") == 0) {
            studentMenu(username);
            break;
        }
        else {
            memset(q, 0, sizeof(q));
            MYSQL_RES *res_set2;
            MYSQL_ROW row2;
            sprintf(q, "select u.UoSName from unitofstudy u, transcript t where u.UoSCode = t.UoSCode and t.Studid = %s and t.Semester = '%s' and t.Year = %d;", username, semester, y);
            //puts(q);
            
            mysql_query(connection,q);
            res_set2 = mysql_store_result(connection);
            int numrows = (int)mysql_num_rows(res_set2);
            
            for (int i = 0; i < numrows; i++)
            {
                row2 = mysql_fetch_row(res_set2);
                if( row2 != NULL )
                {
                    printf("\t\t\t%s\n", row2[0]);
                    
                }
            }
            
        }
   
    }
    while(1);

}

//enroll screen
void enroll() {}

//withdraw screen
void withdraw() {}

//logout
void logoutStudent() {}

//exit
void exitSystem() {}

//student menu screen
void studentMenu(char* username) {
    //query
    char q[150] = "\0";
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    strcat(q, "SELECT name FROM student where id = ");
    strcat(q, username);
    
    mysql_query(connection,q);
    res_set = mysql_store_result(connection);
    
    row = mysql_fetch_row(res_set);
    
    int z;
    int m = 0, y = 0;
    char* semester;
    get_year(&y);
    semester = get_semester(m);
    
    printf("\t\t\t---------------------------------------\n");
    printf("\t\t\t           Student Menu           \n");
    printf("\t\t\t---------------------------------------\n\n");
    printf("\t\t\tHi, %s\n", row[0]);
    
    printf("\n\t\t\t%s, %d\n", semester, y);
    memset(q, 0, sizeof(q));
    
    MYSQL_RES *res_set2;
    MYSQL_ROW row2;
    
//    strcat(q, "select u.UoSName from unitofstudy u, transcript t where u.UoSCode = t.UoSCode and t.Studid = 3213 and t.Semester = 'Q1' and t.Year = 2015; ");
    sprintf(q, "select u.UoSName from unitofstudy u, transcript t where u.UoSCode = t.UoSCode and t.Studid = %s and t.Semester = '%s' and t.Year = %d;", username, semester, y);
    //puts(q);
    
    mysql_query(connection,q);
    res_set2 = mysql_store_result(connection);
    int numrows = (int)mysql_num_rows(res_set2);
    
    for (int i = 0; i < numrows; i++)
    {
        row2 = mysql_fetch_row(res_set2);
        if( row2 != NULL )
        {
            printf("\t\t\t%s\n", row2[0]);

        }
    }
    
    printf("\n\t\t\t---------------------------------------\n");
    
    //free?
    if(semester != NULL) {
        //free(semester);
        semester = NULL;
    }
    
    // free resources
    mysql_free_result(res_set);
    mysql_free_result(res_set2);
    
    do
    {
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t           Commands List           \n");
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t    [1]Transcript               \n");
        printf("\t\t\t    [2]Enroll Class             \n");
        printf("\t\t\t    [3]Withdraw Class           \n");
        printf("\t\t\t    [4]Personal Details         \n");
        printf("\t\t\t    [5]Logout                   \n");
        printf("\t\t\t    [0]Exit                     \n");
        printf("\t\t\t---------------------------------------\n\n");
        printf("Please enter the command: ");
        scanf("%d", &z);
        //system("color 2f");
        switch(z)
        {
            case 0 :return;
            case 1 :transcript(username);break;
            case 2 :enroll();break;
            case 3 :withdraw();break;
            case 4 :logoutStudent();break;
            case 5 :exitSystem();break;
            default:printf("\n INVALID COMMAND");
        }
    }
    while( 1 );
}

//login screen
void loginStudent() {
    char username[11] = "\0";
    int userid = 0;
    char password[10] = "\0";
    
    do {
        //query
        char q[150] = "\0";
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        printf("\nPlease enter student id: ");
        gets(username);
        userid = atoi(username);
        
        printf("\nPlease enter password: ");
        gets(password);
        
        strcat(q, "SELECT password FROM student where id = ");
        strcat(q, username);
        
        mysql_query(connection,q);
        res_set = mysql_store_result(connection);
        
        row = mysql_fetch_row(res_set);
        
        if (row == NULL) {
            printf("User does not exist.");
        }
        else if(strcmp(row[0], password) != 0) {
            printf("Wrong password.");
        }
        else {
            mysql_free_result( res_set );
            break;
        }
        mysql_free_result( res_set );
    }
    while(1);
    
    //login success
    //printf("\nlogging in...\n");
    studentMenu(username);
    
}

int main (int argc, const char * argv[]) {
    
    //    int d = 0, m = 0, y = 0;
    //    get_date(&m, &d, &y);
    //    printf("date is %d", d);
    
    mysql_init(&mysql);
    
    connection = mysql_real_connect(&mysql, "localhost", "root", "", "project3-nudb", 0, 0, 0);
    
    //test the db conection

    if (connection == NULL) {
        //unable to connect
        printf("Cannot connect to the database.\n");
        return 0;
    }
    //    else {
    //        printf("Positive!!!\n");
    //    }
    
    //login screen
    loginStudent();
    
    
    
}
