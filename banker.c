/* 
    Implementation of solve_banker_algorithm
    
    Student Name: YIU, Tung Hon
    ITSC email: thyiuaa@connect.ust.hk

    TODO: Please write the explanation here (i.e. near the top of the source code)
    
    For example:

    Step 1: ....
    Step 2: ....
        Step 2.1: .....
        Step 2.2: .....
            Step 2.2.1: .....
    Step 3: ....

 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the MAX_RESOURCE and MAX_PROCESS
#define MAX_RESOURCE 10
#define MAX_PROCESS 10

// Useful string template used in printf()
// We will use diff program to auto-grade the PA2 submissions
// Please use the following templates in printf to avoid formatting errors
//
// Examples:
//
//   printf(template_step_i, 0)    # print === Step 0 ===\n on the screen
//   printf(template_safety_safe)  # print The system is safe\n on the screen

const char template_step_i[] = "=== Step %d ===\n";
const char template_safety_safe[] = "The system is safe\n";
const char template_safety_not_safe[] = "The system is not safe\n";
const char template_rr_error_1[] = "Error case 1: request <= need_%d failed\n";
const char template_rr_error_2[] = "Error case 2: request <= available\n";
const char template_rr_can_grant[] = "CAN be immediately granted to process %d\n";
const char template_rr_cannot_grant[] = "CANNOT be immediately granted to process %d\n";

// Global variables:
// For simplicity, let's make everything static without using dynamic memory allocation
// The function to parse the input values are given and be invoked in the main() function

char algorithm[100]; // either "Safety" or "Resource-Request"
int num_resource; // m
int num_process; // n
int alloc[MAX_PROCESS][MAX_RESOURCE]; // n x m
int max[MAX_PROCESS][MAX_RESOURCE]; // n x m
int need[MAX_PROCESS][MAX_RESOURCE]; // n x m
int work[MAX_RESOURCE]; // m vector
int finish[MAX_PROCESS]; // n vector
int seq[MAX_PROCESS]; // n vector
int req_process_id = -1; // used in "Resource-Request" algorithm
int request[MAX_RESOURCE]={0}; // m vector, used in "Resource-Request" algorithm

// Function prototypes 
void parse_input(); // Given and Implemented: No need to make any changes 
void print_parsed_values(); // Given and Implemented: No need to make any changes

// Two helper print functions
// print a vector
// Example: 
// 
// print_vec("work", work, num_resource);
void print_vec(char *name, int vec[MAX_RESOURCE], int n);

// print a matrix - it maybe useful for you to debug
void print_mat(char *name, int mat[MAX_PROCESS][MAX_RESOURCE], int num_process, int num_resource);

int print_step(int step, int num_process, int num_resource) {
    printf(template_step_i, step);
    print_mat("work", work, num_process, num_resource);
    print_mat("finish", finish, num_process, num_resource)
}
int process_safe(int process) {
    for (int resource = 0; resource < num_resource; ++resource) {
        if (need[process][resource] > work[resource]) return -1;
    }
    return 1;
}

void solve_banker_algorithm() {
    int head = 0;
    int finished = 0;
    int failed = 0;
    print_step(finished, work, num_process, num_resource);
    for (int i = 0; i < num_process; ++i) {
        int process = (head + i) % num_process;
        if (finish[process] == 1) continue;
        if (process_safe(process) == 1) {
            seq[finished] = process;
            finish[process] = 1;
            head = (process + 1) % num_process;
            finished += 1;
            failed = 0;
            print_step(finished, work, num_process, num_resource);
        } else {
            failed += 1;
        }
        if (failed == num_process - finished) break;
    }


    printf("=== Banker's algorithm result ===\n");


    // TODO: print your Banker's algorithm result
    // if algo == safety
    if (failed == 0) {
        printf(template_safety_safe)
    } else {
        printf(template_safety_not_safe)
    }

}



int main() {

    parse_input(); // parse the input 

    printf("=== Banker's algorithm input values ===\n");
    print_parsed_values(); // print the parsed values to double-check

    printf("=== Banker's algorithm execution ===\n");
    solve_banker_algorithm(); // solve the problem using the banker's algorithm

    return 0;
}


// Check whether the line is a blank line (for input parsing)
int is_blank(char *line) {
    char *ch = line;
    while ( *ch != '\0' ) {
        if ( !isspace(*ch) )
            return 0;
        ch++;
    }
    return 1;
}

// Check whether the input line should be skipped
int is_skip(char *line) {
    if ( is_blank(line) )
        return 1;
    char *ch = line ;
    while ( *ch != '\0' ) {
        if ( !isspace(*ch) && *ch == '#')
            return 1;
        ch++;
    }
    return 0;
}






void print_mat(char *name, int mat[MAX_PROCESS][MAX_RESOURCE], int num_process, int num_resource) {
    int i,j;
    printf("%s = [\n", name);
    for (j=0;j<num_process;j++) {
        for (i=0;i<num_resource;i++) {
            printf("%d", mat[j][i]);
            if ( i<num_resource-1 )
                printf(",");
        }
        printf("\n");
    }
    printf("]\n");
}

void print_vec(char *name, int vec[MAX_RESOURCE], int n) {
    int i;
    printf("%s = [", name);
    for (i=0;i<n;i++) {
        printf("%d", vec[i]);
        if ( i<n-1 )
            printf(",");
    }
    printf("]\n");
}

void parse_input() {

    FILE *fp = stdin;
    char *line = NULL;
    ssize_t  nread;
    size_t len = 0;

    int line_count = 0;
    int i = 0;
    int j = 0;

    while ( (nread = getline(&line, &len, fp)) != -1 ) {
        if ( is_skip(line) == 0)  {

            line_count++;
            line = strtok(line,"\n");

            if ( line_count == 1 ) {
                strcpy(algorithm, line);
            } else if ( line_count == 2 ) {
                sscanf(line, "%d", &num_resource);
            } else if ( line_count == 3 ) {
                sscanf(line, "%d", &num_process);
            } else if ( line_count == 4 ) {
                line = strtok(line, " ");
                i = 0;
                while (line != NULL) {
                    sscanf(line, "%d", &work[i]);
                    i++;
                    line = strtok(NULL," ");
                }
            } else if ( line_count >= 5 && line_count <= (5+num_process-1) ) {
                j = line_count-5;
                line = strtok(line, " ");
                i = 0;
                while (line != NULL) {
                    sscanf(line, "%d", &alloc[j][i]);
                    i++;
                    line = strtok(NULL," ");
                }
            } else if ( line_count >= 5+num_process && line_count <= 5+2*num_process-1 ) {
                j = line_count-5-num_process;
                line = strtok(line, " ");
                i = 0;
                while (line != NULL) {
                    sscanf(line, "%d", &max[j][i]);
                    i++;
                    line = strtok(NULL," ");
                }
            } else if ( line_count == 5+2*num_process && strcmp(algorithm, "Resource-Request")==0) {
                sscanf(line, "%d", &req_process_id);
            } else if ( line_count == 5+2*num_process+1  && strcmp(algorithm, "Resource-Request")==0 ) {
                line = strtok(line, " ");
                i = 0;
                while (line != NULL) {
                    sscanf(line, "%d", &request[i]);
                    i++;
                    line = strtok(NULL," ");
                }
            }

            
        }
    }
}

void print_parsed_values() {

    int i, j;

    printf("algorithm = %s\n", algorithm);
    printf("m = %d\n", num_resource);
    printf("n = %d\n", num_process);
    print_vec("work", work, num_resource);
    print_mat("alloc", alloc, num_process, num_resource);
    print_mat("max", max, num_process, num_resource);

    for (j=0;j<num_process;j++) 
        for (i=0;i<num_resource;i++) 
            need[j][i] = max[j][i] - alloc[j][i];

    print_mat("need", need, num_process, num_resource);

    for (i=0;i<num_process;i++) {
        finish[i] = 0;
        seq[i] = -1;
    }

    print_vec("finish", finish, num_process);

    if ( strcmp(algorithm, "Resource-Request")==0  ) {
        printf("Request process id = %d\n", req_process_id);
        print_vec("request", request, num_resource);
    }

}