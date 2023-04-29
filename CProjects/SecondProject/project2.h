#ifndef PROJECT2
#define PROJECT2

/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*definition of constants*/
#define MAXINPUT 65535
#define MAX_ARGS 5
#define FOUR 4
#define THREE 3
#define TWO 2 
#define ONE 1
#define ZERO 0

/*definition of structures/ linked lists*/
typedef struct route {
    char *name;  
    char *origin;
    char *destiny; 
    int num_stops;
    double total_cost; 
    double total_duration;
    struct link *links;
    struct route *next;
} route;

typedef struct stop {
    char *name; 
    double latitude;  
    double longitude; 
    struct stop *next;
} stop;

typedef struct link {
    char *name_route;   
    char *origin;
    char *destiny; 
    double cost; 
    double duration;
    struct link *next;
} link;
 
void strcpy_alloc(char **ptr, char *str);

stop* stop_exist(stop* head_stop, char *name);

route* route_exist(route* head_route, char *name);

char** find_route(route *current_route, int *num_carrear_stops);

void add_route(route **head_route, char name[], char origin[], char destiny[], int num_stops, double total_cost, double total_duration);

void add_stop(stop **head_stop, char name[], double latitude, double longitude);

void add_link(route *current_route, char name_route[], char origin[], char destiny[], double cost, double duration);

void comand_p(stop **head_stop, route **head_route ,char **args);

void update_route(route *head_route, char origin[], char destiny[], int num_stops, double total_cost, double total_duration);

void update_link(link *head_link, char origin[], char destiny[], double cost, double duration);

void info_route(route *head_route, char **origin, char **destiny, int *num_stops, double *total_cost, double *total_duration);

void remove_links_stops_in_routes(struct route *head_route, char stop[]);

void remove_stop(struct stop** head_stop, struct route** head_route, char name[]);

void remove_all_stops(struct stop** head_stop);

int word_exist_vetor(char *vetor[], int num_elements, char word[]);

void remove_route(struct route** head_route, char name[]);

void remove_all_routes(struct route** head_route);

void comand_r(struct route** head_route, char name[]);

void comand_e(struct stop** head_stop, struct route** head_route, char name[]);

void comand_a(struct route** head_route, struct stop** head_stop);

void arguments(char *instruction, char **args);

void sort_stop_routes(route *head_route, char stop_name[]);

void print_route_invers(route *current_route);

void print_route(route *current_route);

void comand_c(route **head_route, char **args);

int route_pass_stop(char name_stop[],char **vector_stops, int num_stops);

int count_route_pass_stop(struct route **head_route, char stop[]);

void comand_l(route **head_route, stop** head_stop, char **args);

void comand_i(stop *head_stop, route *head_route);

#endif