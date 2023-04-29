/* iaed-23 - ist1106194 - project1 */

/*
 * File:  project1.c
 * Author: Sofia Dinis Pinto Piteira
 * Description: Public transport route management system
 */

/*inclusion of the file containing the structures, constants and libraries being used*/
#include "project1.h"

/*declaration of global variables*/

/*global variable that sees the index at which the last route
was inserted in the vector with the routes*/
int index_route = MINUS_ONE;
/*global variable that sees the index at which the last stop
was inserted in the vector with the stops*/
int index_stop = MINUS_ONE;
/*global variable that sees the index at which the last link
was inserted in the vector with the links*/
int index_link = MINUS_ONE;
/*vector with all the arguments of the instruction being executed*/
char args[MAX_ARGS][MAX_NAME_STOP];
route routes[MAX_ROUTE]; /*vector with the routes*/
stop stops[MAX_STOP]; /*vector with the stops*/
link links[MAX_LINK]; /*vector with the links*/

/*definition of functions that will be necessary to do what the instruction indicates*/

/*routine that put all instruction arguments into an array, except for the command q, c, p, l, or i*/
void arguments(char *instruction)
{
    int i, j, k, args_i, ilen;
    char argument[MAX_NAME_STOP];
    strcpy(argument, ""); /*argument always starts empty*/
    ilen = strlen(instruction); /*find out how many characters the instruction has*/
    args_i = ZERO; /*index of the phrase character to be parsed*/
    k = ZERO;
    /*initialize the vector with nothing*/
    for (i = ZERO; i < MAX_ARGS; i++)
        strcpy(args[i], "");
    /*starts at index 2 of the instruction, because at index 0 is the command and at index 1 is a space*/
    for (j = TWO; j <= ilen && args_i <= MAX_ARGS; j++){
        /*if it finds a character other than a whitespace \n or a \0*/
        if (instruction[j] != ' ' && instruction[j] != '\n' && instruction[j] != '\0'){
            /*if the character found is a quote then all the characters up to the next quote will be stored in the argument
            not including the first or the last quote*/
            if (instruction[j] == '\"'){
                j++; /*so that the quote is not stored in the argument*/
                while (instruction[j] != '\"' && instruction[j] != '\n' && instruction[j] != '\0'){
                    argument[k] = instruction[j];
                    /*j refers to the instruction indexes*/
                    j++;
                    /*k refers to the indices of the argument*/
                    k++;
                }
            }
            /*if the character found is not a quote then all the characters up to the next space will be stored in the argument*/
            else{
                while (instruction[j] != ' ' && instruction[j] != '\n' && instruction[j] != '\0'){
                    argument[k] = instruction[j];
                    j++;
                    k++;
                }
            }
            argument[k] = '\0'; /*as the arguments are strings you have to put \0 at the end of each one after separating them*/
            strcpy(args[args_i], argument); /*put the argument in the vector*/
            args_i++; /*moves to the next position of the vector*/
            /*The next position of the array is initialized with an empty string to which characters are going to be concatenated*/
            strcpy(argument, "");
            k = ZERO; /*k returns to 0 so that the first character is placed in the first position of the argument*/
        }
    }
}

/*routine that returns one if the route passes the stop and zero if it doesn't*/
int route_pass_stop(int c_index, int s_index){
    int i;
    for (i = ZERO; i < stops[s_index].num_route; i++){
        /*checks if in the vector with the indexes of the routes in the stop structure 
        with the index given as the 2nd argument is the index of the route that is given as the 1st argument*/
        if (stops[s_index].routes_index[i] == c_index){
            /*if the index of the route is in the vector, the function returns 1*/
            return ONE; 
            /*as soon as it finds the index the cycle stops so that the rest of the vector does not have to be traveled*/
            break; 
        }
    }
    return ZERO; /*if the index of the route is not in the vector then the function returns 0*/
}

/*routine that checks if a stop with the name given as an argument already exists*/
int stop_exist(char stop[MAX_NAME_STOP]){
    /*takes the stop name as an input argument*/
    int k;
    int stop_i = MINUS_ONE; /*if the stop does not exist, the function returns -1*/
    for (k = ZERO; k <= index_stop; k++){
        if (strcmp(stops[k].name, stop) == ZERO){
            stop_i = k; /*if the stop exists the function returns the index of the stop*/
            break;
        }
    }
    return stop_i;
}

/*routine that checks if there is already a route with the given name as an argument*/
int route_exist(char route[MAX_NAME_ROUTE]){
    /*takes the route name as an input argument*/
    int k;
    int route_index = MINUS_ONE; /*if the route does not exist, the function returns -1*/
    for (k = ZERO; k <= index_route; k++){
        if (strcmp(routes[k].name, route) == ZERO){
            route_index = k; /*if the route exists the function returns the index of the route*/
            break;
        }
    }
    return route_index;
}

/*routine that prints the routes in alphabetical order*/
void order_stop_routes_(int stop_index){
    int i, j;
    char aux[MAX_NAME_ROUTE]; /*temporary variable*/
    /*two-dimensional table whose rows correspond to
    the indexes of the routes and the columns to their names*/
    char vetor[MAX_ROUTE][MAX_NAME_ROUTE];
    for (i = ZERO; i < stops[stop_index].num_route; i++){
        strcpy(vetor[i], routes[stops[stop_index].routes_index[i]].name);
    }

    for (i = ZERO; i < stops[stop_index].num_route; i++){
        /*comparison made through ascii table*/
        for (j = i + ONE; j < stops[stop_index].num_route; j++){
            if (strcmp(vetor[i], vetor[j]) > ZERO){
                /*when it finds a word whose name is alphabetically bigger than
                the previous one, it copies the name of the previous one to the
                auxiliary variable*/
                strcpy(aux, vetor[i]);
                /*copies the alphabetically bigger word to the place of the
                previous word that was smaller*/
                strcpy(vetor[i], vetor[j]);
                /*then copy what's in the auxiliary variable to the place where
                the alphabetically bigger word was*/
                strcpy(vetor[j], aux);
            }
        }
    }

    for (i = ZERO; i < stops[stop_index].num_route; i++){
        /*prints all words sorted alphabetically on the same line and separated by spaces*/
        if (i == stops[stop_index].num_route - ONE){
            /*the /n is only placed after the last word*/
            printf(" %s\n", vetor[i]);
        }
        else{
            printf(" %s", vetor[i]);
        }
    }
}

/*insert information into the link structure*/
void insert_information_links(char origin[MAX_NAME_STOP], char destiny[MAX_NAME_STOP], char cost[BUFSIZ], char duration[BUFSIZ], int route_index){
    /*increments by 1 the global variable that sees the index at which the last link was inserted in the vector with the links*/
    index_link++;
    /*adds to the link structure the cost of traversing the link*/
    links[index_link].cost = atof(cost);
    /*adds to the link structure the duration of traversing the link*/
    links[index_link].duration = atof(duration);
    /*adds to the link structure the index of the route where the link was inserted*/
    links[index_link].i_route = route_index;
    /*adds the origin stop index to the link structure*/
    strcpy(links[index_link].origin, origin);
    /*adds the destiny stop index to the link structure*/
    strcpy(links[index_link].destiny, destiny);
    /*adds to the cost counter in the route structure the cost of the connection in that route*/
    routes[route_index].total_cost += atof(cost);
    /*adds to the duration counter in the route structure the duration of the link in that route*/
    routes[route_index].total_duration += atof(duration);
}

/*insert information into the stop structure*/
void insert_information_stops(int route_index, int stop_origin_index, int stop_destiny_index){
    /*If the route does not pass through the origin stop of the connection*/
    if (!route_pass_stop(route_index, stop_origin_index)){
        /*adds to a vector of the stop structure the index of the route to which the stop was added*/
        stops[stop_origin_index].routes_index[stops[stop_origin_index].num_route] = route_index;
        /*increments by 1 the number of routes passing through the stop*/
        stops[stop_origin_index].num_route++;
    }
    /*If the route does not pass through destination stop of the connection*/
    if (!route_pass_stop(route_index, stop_destiny_index)){
        /*adds to a vector of the stop structure the index of the route to which the stop was added*/
        stops[stop_destiny_index].routes_index[stops[stop_destiny_index].num_route] = route_index;
        /*increments by 1 the number of routes passing through the stop*/
        stops[stop_destiny_index].num_route++;
    }
}

/*insert the stop indexes in the route vector correctly so that the first index of that vector is the index
of the origin stop and the last one is the index of the destination stop and that in the middle of the vector
the indexes are all sorted according to the route of the route*/
void insertions_of_stops_in_the_route(int route_index, int stop_origin_index, int stop_destiny_index,
int route_destiny_index, int route_origin_index, char *origin, char *destiny){
    int i;
    /*when the route does not yet have associated stops*/
    if (routes[route_index].num_stops == ZERO){
        routes[route_index].stops_index[ZERO] = stop_origin_index;
        routes[route_index].stops_index[ONE] = stop_destiny_index;
        strcpy(routes[route_index].origin, origin);
        strcpy(routes[route_index].destiny, destiny);
        /*increments the route stop counter by two*/
        routes[route_index].num_stops += TWO;
    }
    else{
        /*when the connection is between the origin and destination stops of a route, making this a circular route*/
        if ((route_destiny_index == stop_origin_index) &&
            (route_origin_index == stop_destiny_index)){
            routes[route_index].stops_index[routes[route_index].num_stops] = routes[route_index].stops_index[ZERO];
            strcpy(routes[route_index].destiny, destiny);
        }
        /*when you want to add a connection with the destination stop*/
        else if (routes[route_index].stops_index[routes[route_index].num_stops - ONE] == stop_origin_index){
            routes[route_index].stops_index[routes[route_index].num_stops] = stop_destiny_index;
            strcpy(routes[route_index].destiny, destiny);
        }
        /*when you want to add a connection with the origin stop*/
        else if (routes[route_index].stops_index[ZERO] == stop_destiny_index){
            for (i = routes[route_index].num_stops - ONE; i >= ZERO; i--){
                routes[route_index].stops_index[i + ONE] = routes[route_index].stops_index[i];
            }
            routes[route_index].stops_index[ZERO] = stop_origin_index;
            strcpy(routes[route_index].origin, origin);
        }
        routes[route_index].num_stops++;
    }
}

/*definition of possible commands*/

void comand_c(){
    char route[MAX_NAME_ROUTE] = "", order_by[INVERSE_SIZE] = "";
    int i, route_index;
    /*just to make the code more readable*/
    strcpy(route, args[ZERO]);
    strcpy(order_by, args[ONE]);

    if (strcmp(route, "") == ZERO){
        for (i = ZERO; i <= index_route; i++){
            if (strcmp(routes[i].origin, "") == ZERO)
                /*the total cost and total duration of the route path are printed with only 2 decimal places*/
                printf("%s %d %.2f %.2f\n", routes[i].name, routes[i].num_stops, routes[i].total_cost, routes[i].total_duration);
            else
                /*the total cost and total duration of the route path are printed with only 2 decimal places*/
                printf("%s %s %s %d %.2f %.2f\n", routes[i].name, routes[i].origin, routes[i].destiny,
                routes[i].num_stops, routes[i].total_cost, routes[i].total_duration);
        }
    }
    else{
        route_index = route_exist(route);
        /*if route does not belong*/
        if (route_index == MINUS_ONE){
            /*increment by one the number of indexes of the
            route vector that already have a route*/
            index_route++;
            /*create a route*/
            strcpy(routes[index_route].name, route);
        }
        else{
            /*The c command followed by an existing route in the system
            allows you to list all the stops on the route from origin to destination*/
            if (strcmp(order_by, "") == ZERO){
                if (routes[route_index].num_stops > ZERO){
                    for (i = ZERO; i < routes[route_index].num_stops; i++){
                        if (i != routes[route_index].num_stops - ONE){
                            printf("%s, ", stops[routes[route_index].stops_index[i]].name);
                        }
                        else{
                            /*the \n is only placed after the last printed stop*/
                            printf("%s\n", stops[routes[route_index].stops_index[i]].name);
                        }
                    }
                }
            }
            /*inverso or one of its abbreviations up to 3 characters*/
            else if (strcmp(order_by, "inv") == ZERO || strcmp(order_by, "inve") == ZERO ||
                     strcmp(order_by, "inver") == ZERO || strcmp(order_by, "invers") == ZERO ||
                     strcmp(order_by, "inverso") == ZERO){
                if (routes[route_index].num_stops > ZERO){
                    for (i = routes[route_index].num_stops - ONE; i >= ZERO; i--){
                        /*The command c followed by an existing route and the word "inverso" or one of its abbreviations
                        up to 3 characters, allows you to list all the stops on the route from the destination to the origin*/
                        if (i != ZERO){
                            printf("%s, ", stops[routes[route_index].stops_index[i]].name);
                        }
                        else{
                            /*the \n is only placed after the last printed stop*/
                            printf("%s\n", stops[routes[route_index].stops_index[i]].name);
                        }
                    }
                }
            }
            else{
                /*if after the name of the route there is a word that is not inverse or one of its abbreviations up to 3 characters*/
                printf("incorrect sort option.\n");
            }
        }
    }
}

void comand_p(){
    char stop[MAX_NAME_STOP] = "", latitude[MAX_NAME_ROUTE] = "", longitude[MAX_NAME_ROUTE] = "";
    int i, stop_i;
    /*just to make the code more readable*/
    strcpy(stop, args[ZERO]);
    strcpy(latitude, args[ONE]);
    strcpy(longitude, args[TWO]);

    if (strcmp(stop, "") == ZERO){
        for (i = ZERO; i <= index_stop; i++){
            /*Coordinates are printed in 16 positions with 12 decimal digits*/
            printf("%s: %16.12f %16.12f %d\n", stops[i].name, stops[i].latitude, stops[i].longitude, stops[i].num_route);
        }
    }
    else{
        /*see if the stop has already been created*/
        stop_i = stop_exist(stop);

        if (strcmp(latitude, "") != ZERO){
            if (stop_i != MINUS_ONE){
                /*in the case of trying to create a stop and there is already a stop with the indicated name.*/
                printf("%s: stop already exists.\n", stop);
            }
            else{
                index_stop++;
                strcpy(stops[index_stop].name, stop);
                /*atof() function passes strings to numbers of the defined type*/
                stops[index_stop].latitude = atof(latitude);
                stops[index_stop].longitude = atof(longitude);
            }
        }
        else{
            if (stop_i == MINUS_ONE){
                /*in the case of an attempt to list a stop and there is no stop with the indicated name.*/
                printf("%s: no such stop.\n", stop);
            }
            else{
                for (i = ZERO; i <= index_stop; i++){
                    if (strcmp(stops[i].name, stop) == ZERO){
                        /*Coordinates are printed in 16 positions with 12 decimal digits*/
                        printf("%16.12f %16.12f\n", stops[i].latitude, stops[i].longitude);
                    }
                }
            }
        }
    }
}

void comand_l(){
    char route[MAX_NAME_ROUTE] = "", origin[MAX_NAME_STOP] = "", destiny[MAX_NAME_STOP] = "", cost[BUFSIZ] = "", duration[BUFSIZ] = "";
    int i, route_index = ZERO, stop_destiny_index = ZERO, stop_origin_index = ZERO,
    stop_belongs = ZERO, route_origin_index = ZERO, route_destiny_index = ZERO;
    /*just to make the code more readable*/
    strcpy(route, args[ZERO]);
    strcpy(origin, args[ONE]);
    strcpy(destiny, args[TWO]);
    strcpy(cost, args[THREE]);
    strcpy(duration, args[FOUR]);
    /*find the route index where a link is supposed to be inserted*/
    route_index = route_exist(route);
    if (route_index == MINUS_ONE){
        /*in case there is no route with the indicated name*/
        printf("%s: no such line.\n", route);
    }
    else{
        route_origin_index = routes[route_index].stops_index[ZERO];
        route_destiny_index = routes[route_index].stops_index[routes[route_index].num_stops - ONE];
        stop_origin_index = stop_exist(origin);
        if (stop_origin_index == MINUS_ONE){
            /*in case a stop with the indicated name has not been created*/
            printf("%s: no such stop.\n", origin);
        }
        else{
            stop_destiny_index = stop_exist(destiny);
            if (stop_destiny_index == MINUS_ONE){
                /*in case a stop with the indicated name has not been created*/
                printf("%s: no such stop.\n", destiny);
            }
            else{
                for (i = ONE; i < routes[route_index].num_stops - ONE; i++){
                    if (stop_origin_index == routes[route_index].stops_index[i] ||
                    stop_destiny_index == routes[route_index].stops_index[i]){
                        stop_belongs = ONE;
                        break;
                    }
                }
                /*in case one of the stops of the l command is already inside the vector
                with the stops and it isn't the origin or destination stop*/
                if (stop_belongs || (!(strcmp(routes[route_index].origin, "") == ZERO) && !(strcmp(routes[route_index].origin, origin) == ZERO
                || strcmp(routes[route_index].origin, destiny) == ZERO || strcmp(routes[route_index].destiny, origin) == ZERO ||
                strcmp(routes[route_index].destiny, destiny) == ZERO)) || strcmp(routes[route_index].origin, origin) == ZERO ||
                strcmp(routes[route_index].destiny, destiny) == ZERO){
                    /*in the case of stops on the link, origin or destination,
                    do not correspond to one of the ends of the route*/
                    printf("link cannot be associated with bus line.\n");
                }
                else{
                    if (atof(cost) < ZERO || atof(duration) < ZERO){
                        /*in case the cost or duration on the link are negative values.*/
                        printf("negative cost or duration.\n");
                    }
                    else{
                        insertions_of_stops_in_the_route(route_index, stop_origin_index, stop_destiny_index,
                        route_destiny_index, route_origin_index, origin, destiny);
                        insert_information_stops(route_index, stop_origin_index, stop_destiny_index);
                        insert_information_links(origin, destiny, cost, duration, route_index);
                    }
                }
            }
        }
    }
}

void comand_i(){
    int i;
    for (i = ZERO; i < MAX_STOP; i++){
        if (stops[i].num_route > ONE){
            /*the strchr() function finds the first occurrence of a character in a string*/
            if (strchr(stops[i].name, ' ') != NULL || strchr(stops[i].name, '\t') != NULL){
                /*\" because stops whose name has spaces must be enclosed in quotes */
                printf("\"%s\" %d:", stops[i].name, stops[i].num_route);
            }
            else{
                /*prints the name of the stop and the number of routes that pass through that stop*/
                printf("%s %d:", stops[i].name, stops[i].num_route);
            }
            /*in front of the name of the stop and the number of routes that pass through that stop
            prints in a straight line and in alphabetical order all the routes that pass through the stop*/
            order_stop_routes_(i);
        }
    }
}

int main(void){
    char instruction[BUFSIZ]; /*BUFSIZ bytes (8192 bytes at most of systems)*/
    char comand;
    do{
        fgets(instruction, BUFSIZ, stdin); /*takes input and reads up to /n*/
        comand = instruction[ZERO]; /*q, c, p, l  or i*/
        arguments(instruction); /*invoca a função que vai ler os argumentos*/
        /*as the arguments go to a vector it's no longer necessary to pass the instruction to the functions*/
        switch (comand){
        case 'q': /*end the program*/
            break;
        case 'c': /*add and list routes*/
            comand_c();
            break;
        case 'p': /*add and list stops*/
            comand_p();
            break;
        case 'l': /*add and list connections between stops*/
            comand_l();
            break;
        case 'i': /*list the interconnection nodes*/
            comand_i();
            break;
        default:
            /*in case the beginning of the input phrase is not q, c, p, l or i*/
            printf("This command is not valid\n");
            break;
        }
    } while (comand != 'q');
    return ZERO;
}