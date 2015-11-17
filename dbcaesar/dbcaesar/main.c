//
//  main.c
//  dbcaesar
//
//  Created by Xiangtai Houon 15/11/10.
//  Copyright © 2015 Xiangtai Hou. All rights reserved.
//

#include <mysql.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    printf("Hello, Xiangtai!\n");
    
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "localhost", "root", "", "project-moviedb", 0, 0, 0);
    
    
    if (connection == NULL)
    {
        //unable to connect
        printf("Oh No!\n");
    }
    else
    {
        printf("You are now connected. Welcome!\n");
    }
}
