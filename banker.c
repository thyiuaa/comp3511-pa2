/* 
    Implementation of solve_banker_algorithm
    
    Student Name: YIU, Tung Hon
    ITSC email: thyiuaa@connect.ust.hk
    
    (A). Safety Algorithm:
        (1). if number of failed processes equals to number of unfinished processes
        (2). loop through every process from the beginning
            (2.1). if the process if finished, go to (2)
            (2.2). if the resouces can be granted to the process
                (2.2.1). add the process into sequence, mark it as finished, and sum up work and alloc
                (2.2.2). increase # finished process by 1, and reset failed to 0
                (2.2.3). goto (1)
                (2.2.4). otherwise, increase # failed process by 1
            (2.4). goto (1)
        (3). end

    (B). Resource-Request Algorithm:
        (1). loop throught the request
            (1.1). if the request of the process is greater than the needed resources of the process
                (1.1.1). mark the error and goto (4)
            (1.2). if the request of the process is greater than the work of the process
                (1.2.1). mark the error and goto (4)
        (2). loop throught the request
            (2.1). decrease the work of the process by the requested amount
            (2.2). increase the allocated resources of the process by the requested amount
            (2.3). decrease the needed resources of the process by the requested amount
        (3). run Safety Algorithm
        (4). end

    (C). Output:
        For Safety Algorithm:
            (1). print error if there are any, otherwise print the process sequence
        For Resource-Request Algorithm:
            (1). print error if there are any and print the requested resources
            (2). print the message to tell if the resouce can be granted immediately
            (3). print the process sequence

    (D). Software:
        (1). if the input file is for safety algorihtm, run (A)
            (1.1). otherwise, run (B)
        (2) run (C)

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

void print_step(int step, int num_process, int num_resource) {
    printf(template_step_i, step);
    print_vec("work", work, num_resource);
    print_vec("finish", finish, num_process);
}

int process_safe(int process) {
    for (int resource = 0; resource < num_resource; ++resource) {
        if (need[process][resource] > work[resource]) return -1;
    }
    return 1;
}

int safety_finished = 0;
int safety_failed = 0;
void safety_algorithm() {
    int finished = 0;
    int failed = 0;
    print_step(finished, num_process, num_resource);
    while (failed != num_process - finished){
        for (int process = 0; process < num_process; ++process) {
            if (finish[process] == 1) continue;
            if (process_safe(process) == 1) {
                seq[finished] = process;
                finish[process] = 1;
                for (int j = 0; j < num_resource; ++j) {
                    work[j] += alloc[process][j];
                }
                finished += 1;
                failed = 0;
                print_step(finished, num_process, num_resource);
                break;
            } else {
                failed += 1;
            }
        }
    }
    safety_finished = finished;
    safety_failed = failed;
}

int error_need = 0;
int error_ava = 0;
void rr_algorithm() {
    for (int i = 0; i < num_resource; ++i) {
        if (request[i] > need[req_process_id][i]) {
            error_need = 1;
            return;
        }

        if (request[i] > work[i]) {
            error_ava = 1;
            return;
        }
    }

    for (int i = 0; i < num_resource; ++i) {
        work[i] -= request[i];
        alloc[req_process_id][i] += request[i];
        need[req_process_id][i] -= request[i];
    }
    safety_algorithm();
}

void solve_banker_algorithm() {
    if (strcmp(algorithm, "Resource-Request") != 0) {
        safety_algorithm();
    } else {
        rr_algorithm();
    }

    printf("=== Banker's algorithm result ===\n");

    if (strcmp(algorithm, "Resource-Request") != 0) {
        if (safety_failed == 0) {
            printf(template_safety_safe);
        } else {
            printf(template_safety_not_safe);
        }
        print_vec("seq", seq, safety_finished);
    } else {
        if (error_need) printf(template_rr_error_1, req_process_id);
        if (error_ava) printf(template_rr_error_2);
        print_vec("request", request, num_resource);
        if (safety_failed == 0) {
            printf(template_rr_can_grant, req_process_id);
        } else {
            printf(template_rr_cannot_grant, req_process_id);
        }
        print_vec("seq", seq, safety_finished);
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