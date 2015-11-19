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

//variables
MYSQL *connection, mysql;

//functions declaration
void get_month(int * month);
void get_year(int * year);
void get_date(int * month, int * day, int * year);
char * get_semester(int month);
char * get_next_semester(int month);
int get_yearofnextsemester(int year, char * semester);
void loginStudent();
void studentMenu(char * username);
void transcript(char * username);
void enroll(char * username);
void withdraw(char * username);
void personaldetail(char * username);
void logoutStudent(char * username);

//clock
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
char * get_semester(int month) {
    
    char * sem;
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
char * get_next_semester(int month) {
    char * next_sem;
    get_month(&month);
    if (month >= 9 && month <= 12 ) {
        next_sem = "Q2";
    }
    else if(month >= 1 && month <= 3) {
        next_sem = "Q3";
    }
    else if(month >= 4 && month <= 6) {
        next_sem = "Q4";
    }
    else {
        next_sem = "Q1";
    }
    return next_sem;
    
}
int get_yearofnextsemester(int year, char * semester) {
    int year_next_semester = year;
    if(strcmp(semester, "Q1") == 0)
        year_next_semester = year + 1;
    return year_next_semester;
}

//transcript screen
void transcript(char * username) {
    printf("\n\t\t\t---------------------------------------\n");
    printf("\t\t\t              Transcript               \n");
    printf("\t\t\t---------------------------------------\n");
    //query
    char q[500] = "\0";
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
            printf("\t%-10s", row[2]);
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
        printf("\t\t\t  [course number]Check course details  \n");
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
            sprintf(q, "select t.UoSCode, u.uosname, o.year, o.semester, o.Enrollment, o.MaxEnrollment, f.name, t.grade from uosoffering o, transcript t, unitofstudy u, faculty f where t.UoSCode = o.UoSCode and f.id = o.instructorid and t.uoscode = u.uoscode and t.semester = o.semester and t.year = o.year and t.studid = %s and t.uoscode = '%s';", username, z);
            //puts(q);
            mysql_query(connection,q);
            res_set2 = mysql_store_result(connection);

                int numrows = (int)mysql_num_rows(res_set2);
                
                for (int i = 0; i < numrows; i++)
                {
                    row2 = mysql_fetch_row(res_set2);
                    if( row2 != NULL )
                    {
                        printf("\n\t\t\t\tcourse number: %s\n", row2[0]);
                        printf("\t\t\t\ttitle: %s\n", row2[1]);
                        printf("\t\t\t\tyear: %s\n", row2[2]);
                        printf("\t\t\t\tquarter: %s\n", row2[3]);
                        printf("\t\t\t\tenrollment: %s\n", row2[4]);
                        printf("\t\t\t\tcapacity: %s\n", row2[5]);
                        printf("\t\t\t\tlecturer: %s\n", row2[6]);
                        printf("\t\t\t\tgrade: %s\n", row2[7]);
                        
                    }
                }
                if (numrows == 0) {
                    printf("\nInvalid course number.\n");
                }
            //free resources
            mysql_free_result(res_set2);
            }
    }
    while(1);

}

//enroll screen
void enroll(char * username) {
    printf("\n\t\t\t---------------------------------------\n");
    printf("\t\t\t              Enroll Class               \n");
    printf("\t\t\t---------------------------------------\n");
    
    int m = 0, y = 0;
    char * semester;
    char * next_semester;
    
    get_year(&y);
    semester = get_semester(m);
    next_semester = get_next_semester(m);
    int y_next_semester = get_yearofnextsemester(y, semester);
    //print current quarter course list
    printf("\tCurrent Quarter: %s, %d\n\n", semester, y);
    
    //query
    char q[500] = "\0";
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    sprintf(q, "select u.UoSCode, u.UoSName, o.Enrollment, o.MaxEnrollment,u.Credits from uosoffering o, unitofstudy u where o.uoscode = u.uoscode and o.semester ='%s' and o.year = %d;", semester, y);
    //puts(q);
    mysql_query(connection,q);
    res_set = mysql_store_result(connection);
    int numrows = (int)mysql_num_rows(res_set);
    // Display results
    printf("\tUoSCode");
    printf("\tCourse\t\t\t\t\t\t");
    printf("\tEnrollment");
    printf("\tCapacity");
    printf("\tCredit\n");
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);

        if( row != NULL )
        {
            printf("\t%s ", row[0]);
            printf("\t%-40s ", row[1]);
            printf("\t%s", row[2]);
            printf("\t\t%s", row[3]);
            printf("\t\t%s\n", row[4]);
            
        }
    }
    //free resources
    mysql_free_result(res_set);
    
    //print next quarter course list
    printf("\n\tNext Quarter: %s, %d\n\n", next_semester, y_next_semester);
    //query
    memset(q, 0, sizeof(q));
    MYSQL_RES *res_set2;
    MYSQL_ROW row2;
    
    sprintf(q, "select u.UoSCode, u.UoSName, o.Enrollment, o.MaxEnrollment,u.Credits from uosoffering o, unitofstudy u where o.uoscode = u.uoscode and o.semester ='%s' and o.year = %d;", next_semester, y_next_semester);
    //puts(q);
    mysql_query(connection,q);
    res_set2 = mysql_store_result(connection);
    int numrows2 = (int)mysql_num_rows(res_set2);
    // Display results
    printf("\tUoSCode");
    printf("\tCourse\t\t\t\t\t\t");
    printf("\tEnrollment");
    printf("\tCapacity");
    printf("\tCredit\n");
    for (int i = 0; i < numrows2; i++)
    {
        row2 = mysql_fetch_row(res_set2);
        
        if( row2 != NULL )
        {
            printf("\t%s ", row2[0]);
            printf("\t%-40s ", row2[1]);
            printf("\t%s", row2[2]);
            printf("\t\t%s", row2[3]);
            printf("\t\t%s\n", row2[4]);
            
        }
    }
    //free resources
    mysql_free_result(res_set2);
    char z[20];
    do {
        printf("\n\t\t\t---------------------------------------\n");
        printf("\t\t\t           Commands List           \n");
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t  [course number]Enroll a class\n");
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
            //call procedure enrollclass here
            
            
            
            
            
            
            
            
            
            
            

            
        }
    }
    while(1);
    
}

//withdraw screen
void withdraw(char * username) {
    printf("\n\t\t\t---------------------------------------\n");
    printf("\t\t\t              Withdraw Class               \n");
    printf("\t\t\t---------------------------------------\n");
    
    int m = 0, y = 0;
    char * semester;
    get_year(&y);
    semester = get_semester(m);

    //print current quarter course list
    printf("\tCurrent Registered classes: \n\n\t%s, %d\n\n", semester, y);
    
    //query
    char q[500] = "\0";
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    sprintf(q, "select t.uoscode, u.uosname, t.grade from transcript t, unitofstudy u where t.uoscode = u.uoscode and t.studid = %s and t.semester = '%s' and t.year = %d;",username , semester, y);
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
            printf("\t%-40s ", row[1]);
            printf("\t%s\n", row[2]);
            
        }
    }
    //free resources
    mysql_free_result(res_set);
    
    char z[20];
    do {
        printf("\n\t\t\t---------------------------------------\n");
        printf("\t\t\t           Commands List           \n");
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t  [course number]Withdraw a class\n");
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
        //call procedure withdraw here
            memset(q, 0, sizeof(q));
            MYSQL_RES *res_set2;
            //MYSQL_ROW row2;
            sprintf(q, "call withdraw(%s, '%s');", username, username);
            //puts(q);
            mysql_query(connection,q);
            res_set2 = mysql_store_result(connection);
            
            //free resources
            mysql_free_result(res_set2);
        }
    }
    while(1);
}

//personal detail screen
void personaldetail(char * username) {
    printf("\n\t\t\t---------------------------------------\n");
    printf("\t\t\t             Personal Details               \n");
    printf("\t\t\t---------------------------------------\n\n");
    

    //query
    char q[500] = "\0";
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    sprintf(q, "select * from student where id = %s;",username);
    //puts(q);
    mysql_query(connection,q);
    res_set = mysql_store_result(connection);
    int numrows = (int)mysql_num_rows(res_set);
    // Display results
    printf("\t\t\tID: ");
    printf("\tName:");
    printf("\t\t\tPassword:");
    printf("\tAddress:\n");
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        
        if( row != NULL )
        {
            printf("\t\t\t%s ", row[0]);
            printf("\t%s ", row[1]);
            printf("\t%s ", row[2]);
            printf("\t%s\n", row[3]);
            
        }
    }
    //free resources
    mysql_free_result(res_set);
    
    char z[20];
    char np[20];
    char na[30];
    do {
        printf("\n\t\t\t---------------------------------------\n");
        printf("\t\t\t           Commands List           \n");
        printf("\t\t\t---------------------------------------\n");
        printf("\t\t\t  [1]Change password\n");
        printf("\t\t\t  [2]Change address\n");
        printf("\t\t\t  [0]Back to main menu                 \n");
        printf("\t\t\t---------------------------------------\n\n");
        printf("Please enter the command: ");
        scanf("%s", z);
        //system("color 2f");
        if(strcmp(z, "0") == 0) {
            studentMenu(username);
            break;
        }
        else if(strcmp(z,"1") == 0){
            printf("\nPlease enter the new password: ");
            scanf("%s", np);
            //call procedure change password here
            
            memset(q, 0, sizeof(q));
            MYSQL_RES *res_set2;
            //MYSQL_ROW row2;
            sprintf(q, "call changepassword(%s, '%s');", username, np);
            //puts(q);
            mysql_query(connection,q);
            res_set2 = mysql_store_result(connection);
            
            //int numrows2 = (int)mysql_num_rows(res_set2);
            
            //free resources
            mysql_free_result(res_set2);
            
        }
        else if(strcmp(z, "2") == 0){
            
            printf("\nPlease enter the new address: ");
            scanf("%s", na);
            //call procedure chage address here
            
            memset(q, 0, sizeof(q));
            MYSQL_RES *res_set2;
            //MYSQL_ROW row2;
            sprintf(q, "call changeaddress(%s, '%s');", username, na);
            //puts(q);
            mysql_query(connection,q);
            res_set2 = mysql_store_result(connection);
            
            //int numrows2 = (int)mysql_num_rows(res_set2);
            
            //free resources
            mysql_free_result(res_set2);
            
        }
        else {
            printf("INVALID COMMAND.\n");
        }
    }
    while(1);
}

void logoutStudent(char * username) {

    loginStudent();
}

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
            case 2 :enroll(username);break;
            case 3 :withdraw(username);break;
            case 4 :personaldetail(username);break;
            case 5 :logoutStudent(username);break;
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
        scanf("%s", username);
        userid = atoi(username);
        
        printf("\nPlease enter password: ");
        scanf("%s", password);

        
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
