//Created by Rixin Li rli130@jh.edu
//This is a simple example for sql injection
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// parameterized query
void execute_query(MYSQL *conn, char *username, char *password) {
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2];

    // Prepare the SQL statement
    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed\n");
        exit(EXIT_FAILURE);
    }

    const char *query = "SELECT * FROM users WHERE username = ? AND password = ?";
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed\n");
        mysql_stmt_close(stmt);
        exit(EXIT_FAILURE);
    }

    // Bind the parameters
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed\n");
        mysql_stmt_close(stmt);
        exit(EXIT_FAILURE);
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed\n");
        mysql_stmt_close(stmt);
        exit(EXIT_FAILURE);
    }

     // Process the result 
    MYSQL_RES *result = mysql_stmt_result_metadata(stmt);
    if (result == NULL) {
        fprintf(stderr, "mysql_stmt_result_metadata() failed\n");
        mysql_stmt_close(stmt);
        exit(EXIT_FAILURE);
    }

    // Allocate enough buffer to get the query result
    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    MYSQL_BIND *bind_result = (MYSQL_BIND *)malloc(sizeof(MYSQL_BIND) * num_fields);
    unsigned long *lengths = (unsigned long *)malloc(sizeof(unsigned long) * num_fields);
    memset(bind_result, 0, sizeof(MYSQL_BIND) * num_fields);
    // Allocate a larger buffer size
    for (int i = 0; i < num_fields; i++) {
        bind_result[i].buffer_type = fields[i].type;
        bind_result[i].buffer = malloc(1024); // Allocate 1024 bytes for each field
        bind_result[i].buffer_length = 1024;
        bind_result[i].length = &lengths[i];
    }

    if (mysql_stmt_bind_result(stmt, bind_result) != 0) {
        fprintf(stderr, "mysql_stmt_bind_result() failed\n");
        mysql_stmt_close(stmt);
        exit(EXIT_FAILURE);
    }

    // Fetch and print the results
    bool pass = false;
    while (mysql_stmt_fetch(stmt) == 0) {
        pass = true;
        printf("Welcome user %.*s!\n",(int)lengths[0], (char *)bind_result[0].buffer);
        for (int i = 0; i < num_fields; i++) {
            printf("%.*s ", (int)lengths[i], (char *)bind_result[i].buffer);
        }
        printf("\n");
    }
    
    if(!pass){
        printf("Please input correct username and password.\n");
    }

    for (int i = 0; i < num_fields; i++) {
        free(bind_result[i].buffer);
    }
    free(bind_result);
    free(lengths);
    mysql_free_result(result);
    mysql_stmt_close(stmt);
}

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


#ifdef DESIGNER /*This is used for designer when doing any operations*/

        char input[1024] = "0";

        //User interface tips
        fprintf(stdout,"\nplease enter database instructions: \t");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // printf("input string: %s\n", input);
        }
        else{
            printf("Error reading input.\n");
        }

        /* Send SQL query */
        if (mysql_query(conn, input)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            continue;
        }

        // Check whether this query will return nothing
        if (mysql_field_count(conn) == 0) {
            // Query does not return data (e.g., INSERT, UPDATE, DELETE)
            int affected_rows = mysql_affected_rows(conn);
            printf("Query OK, %d rows affected\n", affected_rows);
        } else {
            // Query returns data (e.g., SELECT)
            res = mysql_store_result(conn);
            if (res == NULL) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                continue;
            }

            // Get the number of fields in the result
            int num_fields = mysql_num_fields(res);

            
            printf("\nquery output:\n\n\t");
            while ((row = mysql_fetch_row(res)) != NULL) {
                for (int i = 0; i < num_fields; i++) {
                    printf("%s ", row[i] ? row[i] : "NULL");
                }
                printf("\n\t");
            }
        }
#endif

#ifdef LOGIN /*This is a scenario of LOGIN sql injection*/

        char user_account[1024] = "0";
        char user_password[1024] = "0";

        //User interface for login:
        fprintf(stdout,"\nplease enter user account: \t");
        if (fgets(user_account, sizeof(user_account), stdin) != NULL) {
            // printf("input string: %s\n", input);
        }
        else{
            printf("Error reading input.\n");
        }

        fprintf(stdout,"\nplease enter user password: \t");
        if (fgets(user_password, sizeof(user_password), stdin) != NULL) {
            // printf("input string: %s\n", input);
        }
        else{
            printf("Error reading input.\n");
        }

        // remove the '\n' after input 
        strtok(user_account, "\n");
        strtok(user_password, "\n");

#ifdef PARAMETERIZED /*use parameterized queries to prevent sql injection*/
        execute_query(conn, user_account, user_password);
        continue;
#endif
        
        
        // SELECT * FROM users WHERE username = 'your_username' AND password = 'your_password';
        char operation[1024] = "SELECT * FROM users WHERE username = '";
        strcat(operation,user_account);
        strcat(operation,"' AND password = '");
        strcat(operation,user_password);
        strcat(operation,"';");

        printf("%s\n",operation);

        /* Send SQL query */
        if (mysql_query(conn, operation)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            continue;
        }



        // Check whether this query will return nothing
        if (mysql_field_count(conn) == 0) {
            // Query does not return data (e.g., INSERT, UPDATE, DELETE)
            int affected_rows = mysql_affected_rows(conn);
            printf("Query OK, %d rows affected\n", affected_rows);
        } else {
            // Query returns data (e.g., SELECT)
            res = mysql_store_result(conn);
            if (res == NULL) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                continue;
            }

            /* Check whether the query is empty set*/
            if (mysql_num_rows(res) == 0) {
                printf("The query returned an empty set. Please try again to log in\n");
                continue;
            } 

            // Get the number of fields in the result
            int num_fields = mysql_num_fields(res);

            // print the res
            while ((row = mysql_fetch_row(res)) != NULL) {
                for (int i = 0; i < num_fields; i++) {
                    if(i==0){
                        printf("\nHello, welcome user %s\n\n\t",row[0]);
                    }
                    printf("%s ", row[i] ? row[i] : "NULL");
                }
                printf("\n\t");
            }
        }

#endif

    }

    /* Close connection */
    mysql_free_result(res);
    mysql_close(conn);
    return 0;

}
