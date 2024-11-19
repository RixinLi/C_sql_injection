//Created by Rixin Li rli130@jh.edu
//This is a simple example for sql injection
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

    /*setting up connection to mysql*/
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "root";      /* username */
    const char *password = "kali"; /* password */
    const char *database = "test_db";

    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    else{
        fprintf(stdout, "You're searching in %s, please use right gammer to search for information\n",database);
    }

    while(1){

        char input[1024] = "0";

        //User interface tips
        fprintf(stdout,"\n\tplease enter database instructions: \t\n");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            printf("input string: %s\n", input);
        }
        else{
            printf("Error reading input.\n");
        }


            /* Send SQL query */
        if (mysql_query(conn, "SHOW TABLES")) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        res = mysql_store_result(conn);

        /* Output table names */
        printf("Tables in MySQL database:\n");
        while ((row = mysql_fetch_row(res)) != NULL) {
            printf("%s\n", row[0]);
        }

    }

    /* Close connection */
    mysql_free_result(res);
    mysql_close(conn);
    return 0;

}