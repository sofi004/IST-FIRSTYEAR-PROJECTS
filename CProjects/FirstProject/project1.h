#ifndef PROJECT1

/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*definition of constants*/
#define MAX_NAME_ROUTE 21 /*20 characters + \0 = 21 bytes*/
#define MAX_NAME_STOP 51 /*50 characters + \0 = 21 bytes*/
#define MAX_ROUTE 200 /*limit of routes that can exist*/
#define MAX_STOP 10000 /*limit of stops that can exist*/
#define MAX_LINK 30000 /*limit of connections that can exist*/
#define MAX_ARGS 5 /*maximum number of arguments a instruction will have*/
/*because a command can be q, c, p, l or i and as these are
received as strings we have to consider the /0 at the end*/
#define INVERSE_SIZE 8 /*because inverse has 7 characters plus the \0*/
#define FOUR 4
#define THREE 3
#define TWO 2
#define ONE 1
#define ZERO 0
#define MINUS_ONE -1

/*definition of global structures*/

typedef struct route {
    char name[MAX_NAME_ROUTE];  /*route name*/
    char origin[MAX_NAME_STOP]; /*origin stop*/
    char destiny[MAX_NAME_STOP]; /*destination stop*/
    int num_stops; /*number of stops in the route*/
    double total_cost; /*total cost to go through all the links in this route*/
    double total_duration; /*total duration to go through all the links in this route*/
    int stops_index[MAX_STOP]; /*vector with the indices of the stops where that route passes*/
} route;

typedef struct stop {
    char name[MAX_NAME_STOP]; /*stop name*/
    /*type double (64 bits) because it has greater precision than
    float (32 bits), which in the case of latitude and longitude
    is important*/
    double latitude;  /*horizontal line number that measures the distance north or south of the equator*/
    double longitude; /*vertical line number that measures the distance east or west of the Greenwich meridian*/
    int num_route; /*number of carrers passing through the stop*/
    int routes_index[MAX_ROUTE]; /*vector with the indices of the routes that pass at the stop*/
} stop;

typedef struct link {
    int i_route; /*route index found through the function*/  
    char origin[MAX_NAME_STOP]; /*origin stop*/
    char destiny[MAX_NAME_STOP]; /*destination stop*/
    double cost; /*cost of going from the origin stop of the connection to the destination stop*/
    double duration; /*duration of going from the origin stop of the connection to the destination stop*/
} link;

#endif