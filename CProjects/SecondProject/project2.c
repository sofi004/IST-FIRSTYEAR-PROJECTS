#include "project2.h" /*Include the project2 header file*/
 
/*auxiliary functions to those that will be used directly in the main function*/

/*This function allocates memory for a string, reallocates memory if needed, and copies the given string to the allocated memory location.*/
void strcpy_alloc(char **ptr, char *str){
    /*Check if the pointer to the character is NULL*/
    if (*ptr == NULL){
        /*If it is NULL, allocate memory for the string and store the address in the pointer*/
        *ptr = (char *)malloc((strlen(str) + ONE) * sizeof(char));
    }
    else{
        /*If the pointer is not NULL, reallocate memory for the string with the new size*/
        *ptr = (char *)realloc(*ptr, (strlen(str) + ONE) * sizeof(char));
    }
    /*Check if the allocation was successful*/
    if (*ptr == NULL){
        /*If not, print an error message and exit the program with an error code*/
        printf("No memory.\n");
        exit(-1);
    }
    /*Copy the content of the string pointed to by str to the memory location pointed to by ptr*/
    strcpy(*ptr, str);
}

/*This function checks if a given stop name exists in a linked list of stops and returns the corresponding node if found, otherwise returns 0.*/
stop* stop_exist(stop *head_stop, char *name){
    while (head_stop){
        /*If the current node's name matches the given name, return the node*/
        if (strcmp(head_stop->name, name) == ZERO){
            return head_stop;
        }
        /*Otherwise, move on to the next node in the linked list*/
        else
            head_stop = head_stop->next;
    }
    /*Return 0 if no node with the given name is found*/
    return ZERO;
}

/*This function checks if a given route name exists in a linked list of routes and returns the corresponding node if found, otherwise returns 0.*/
route* route_exist(route *head_route, char *name){
    while (head_route){
        /*If the current node's name matches the given name, return the node*/
        if (strcmp(head_route->name, name) == ZERO){
            return head_route;
        }
        /*Otherwise, move on to the next node in the linked list*/
        else{
            head_route = head_route->next;
        }
    }
    /* Return 0 if no node with the given name is found*/
    return ZERO;
}

/*This function returns an array of strings that contains the stops in a given route and sets the number of stops as an output argument.*/
char** find_route(route *current_route, int *num_carrear_stops){
    int num_stops = ZERO; /*Initialize the number of stops as 0*/
    struct link *curr_link = current_route->links; /*Get the first link in the linked list of routes*/
    char **route = NULL; /*Initialize the string array that will contain the stops in the route as null*/
    /*Loop through the links in the linked list of routes*/
    while (curr_link != NULL){
        if (route == NULL){
            /*Allocate memory for the first two stops in the route*/
            route = (char **)malloc(TWO * sizeof(char *));
            route[ZERO] = (char *)malloc((strlen(curr_link->origin) + ONE) * sizeof(char)); /*Allocate memory for the origin name*/
            if (route[ZERO] == NULL){
                printf("No memory.\n"); /*In case the memory allocation fails, display an error message and exit the program*/
                exit(-1);
            }
            strcpy(route[ZERO], curr_link->origin); /*Copy the origin name to the string array*/
            route[ONE] = (char *)malloc((strlen(curr_link->destiny) + ONE) * sizeof(char)); /*Allocate memory for the destiny name*/
            if (route[ONE] == NULL){
                printf("No memory.\n"); /*In case the memory allocation fails, display an error message and exit the program*/
                exit(-1);
            }
            strcpy(route[ONE], curr_link->destiny); /*Copy the destiny name to the string array*/
            num_stops = TWO; /*Set the number of stops as 2*/
        }
        else{
            /*Reallocate memory for each additional stop in the route*/
            route = (char **)realloc(route, (num_stops + ONE) * sizeof(char *));
            route[num_stops] = (char *)malloc((strlen(curr_link->destiny) + ONE) * sizeof(char)); /*Allocate memory for the destiny name*/
            if (route[num_stops] == NULL){
                printf("No memory.\n"); /*In case the memory allocation fails, display an error message and exit the program*/
                exit(-1);
            }
            strcpy(route[num_stops], curr_link->destiny); /*Copy the destiny name to the string array*/
            num_stops++; /*Increment the number of stops*/
        }
        curr_link = curr_link->next; /*Move on to the next link in the linked list of routes*/
    }
    *num_carrear_stops = num_stops; /*Set the second input argument to the number of stops in the route*/
    return route; /*Return a pointer to the string array that contains the stops in the route*/
}

/*This function adds a new route to a linked list of routes*/
void add_route(route **head_route, char name[], char origin[], char destiny[], int num_stops, double total_cost, double total_duration){
    /*Allocate memory for a new route*/
    struct route *new_route = (struct route *)malloc(sizeof(struct route));
    /*Check if allocation was successful*/
    if (new_route == NULL){
        printf("No memory.\n");
        exit(-1);
    }
    /*Set initial values for strings to null*/
    new_route->name = NULL;
    new_route->origin = NULL;
    new_route->destiny = NULL;
    /*Copy name, origin, and destiny strings to the new_route struct*/
    strcpy_alloc(&(new_route->name), name);
    strcpy_alloc(&(new_route->origin), origin);
    strcpy_alloc(&(new_route->destiny), destiny);
    /*Assign the other parameters to the new route*/
    new_route->num_stops = num_stops;
    new_route->total_cost = total_cost;
    new_route->total_duration = total_duration;
    new_route->links = NULL;
    new_route->next = NULL;
    /*If the linked list is empty, set new_route as the head*/
    if (*head_route == NULL){
        *head_route = new_route;
    }
    else{
        /*Otherwise, traverse the list to find the last node and append the new route*/
        struct route *last = *head_route;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = new_route;
    }
}

/*This function adds a new stop to a linked list of stops.*/
void add_stop(stop **head_stop, char name[], double latitude, double longitude){
    /* Allocate memory for a new stop */
    struct stop *new_stop = (struct stop *) malloc(sizeof(struct stop));
    if (new_stop == NULL) { /* Check if memory allocation was successful */
        printf("No memory.\n");
        exit(-1);
    }
    new_stop->name = NULL; /* Initialize the name pointer to NULL */
    /* Allocate memory for the name of the stop and copy the name into the new_stop */
    strcpy_alloc(&(new_stop->name), name);
    /* Assign values to the other members of the new_stop */
    new_stop->latitude = latitude;
    new_stop->longitude = longitude;
    new_stop->next = NULL; /* Set the next pointer to NULL, indicating the end of the list */
    /* If head_stop is NULL, set head_stop to point to the new_stop */
    if (*head_stop == NULL) {
        *head_stop = new_stop;
    }
    /* Otherwise, traverse the list until the end and add the new_stop there */
    else {
        struct stop *last = *head_stop;
        while (last->next != NULL) { /* Traverse the list until the last node */
            last = last->next;
        }
        last->next = new_stop; /* Add the new_stop as the next node after the last node */
    }
}

/*This function adds a new link to linked list of links in a route's link list.*/
void add_link(route *current_route, char name_route[], char origin[], char destiny[], double cost, double duration){
    /* Dynamically allocates memory for a new link */
    struct link *new_link = (struct link *)malloc(sizeof(struct link));
    if(new_link == NULL){
        printf("No memory.\n");
        exit(-1);
    }
    new_link->name_route = NULL;
    new_link->origin = NULL;
    new_link->destiny = NULL;
    /* Allocate memory for the name of the route, origin, and destiny of the link, and copy the strings into the new_link */
    strcpy_alloc(&(new_link->name_route), name_route);
    strcpy_alloc(&(new_link->origin), origin);
    strcpy_alloc(&(new_link->destiny), destiny);
    new_link->cost = cost;
    new_link->duration = duration;
    new_link->next = NULL;
    /* If the current route's link list is empty, set the links pointer to point to the new link */
    if(current_route->links == NULL){
        current_route->links = new_link;
    }
    /* Otherwise, traverse the list and add the new link at the end, except if it's the reverse of the first link */
    else {
        if(strcmp(current_route->origin, destiny) == ZERO && strcmp(current_route->destiny, origin) != ZERO){
            new_link->next = current_route->links;
            current_route->links = new_link;
        }
        else{
            /* Traverse the link list until the last node */
            struct link *last = current_route->links;
            while (last->next != NULL) {
                last = last->next;
            }
            /* Add the new link as the next node after the last node */
            last->next = new_link;
        }
    }
}

/*This function copies route information to the passed variables.*/
void info_route(route *head_route, char **origin, char **destiny, int *num_stops, double *total_cost, double *total_duration){
    /* Copy the origin string using dynamic memory allocation */
    strcpy_alloc(origin, head_route->origin);
    /* Copy the destiny string using dynamic memory allocation */
    strcpy_alloc(destiny, head_route->destiny);
    /* Copy the num_stops integer */
    *num_stops = head_route->num_stops;
    /* Copy the total_cost double */
    *total_cost = head_route->total_cost;
    /* Copy the total_duration double */
    *total_duration = head_route->total_duration;
}

/*This function updates the information of a given route.*/
void update_route(route *head_route, char *origin, char *destiny, int num_stops, double total_cost, double total_duration){
    /* Dynamically allocates memory for the new origin and copies the string */
    strcpy_alloc(&(head_route->origin), origin);
    /* Dynamically allocates memory for the new destiny and copies the string */
    strcpy_alloc(&(head_route->destiny), destiny);
    /* Assigns the number of stops to the corresponding member of the route */
    head_route->num_stops = num_stops;
    /* Assigns the total cost to the corresponding member of the route */
    head_route->total_cost = total_cost;
    /* Assigns the total duration to the corresponding member of the route */
    head_route->total_duration = total_duration;
}

/*This function updates the information of a given link.*/
void update_link(link *head_link, char origin[], char destiny[], double cost, double duration){
    /* Copy the new origin value into the origin field of the link struct */
    strcpy_alloc(&(head_link->origin), origin);
    /* Copy the new destiny value into the destiny field of the link struct */
    strcpy_alloc(&(head_link->destiny), destiny);
    /* Update the cost field of the link struct with the new cost value */
    head_link->cost = cost;
    /* Update the duration field of the link struct with the new duration value */
    head_link->duration = duration;
}

/*This function removes links whith a given stop from a linked list of routes*/
void remove_links_stops_in_routes(struct route *head_route, char stop[]){
    int r_num_stops, num_link, found; 
    double r_total_cost, r_total_duration, r_total_cost_aux, r_total_duration_aux;
    char *r_origin = NULL, *r_destiny = NULL;
    struct link* current_link = NULL;
    struct link* previous_link = NULL;
    struct link* next_link = NULL;
    /*Loop through all routes in the linked list*/
    while(head_route != NULL){
        /*Get information about the current route*/
        info_route(head_route, &r_origin, &r_destiny, &r_num_stops, &r_total_cost, &r_total_duration);
        /*Initialize variables for looping through the links in the route*/
        previous_link = NULL;
        num_link = ZERO; found = ZERO;
        current_link = head_route->links;
        next_link = current_link;
        /*Loop through all links in the route*/
        while (current_link != NULL){
            num_link++;
            if (num_link == 1){
                /*If this is the first link in the route*/
                if(strcmp(current_link->origin, stop) == ZERO){
                    r_num_stops--;
                    found = 1;
                    /*If the link contains the stop to be removed as its origin*/
                    if(current_link->next == NULL){
                        /*Update the route's information and set next_link to point to the next link in the route*/
                        update_route(head_route, "", "", ZERO, ZERO, ZERO); 
                    }
                    else{
                        num_link = 0;
                        update_route(head_route, current_link->destiny, r_destiny, r_num_stops, r_total_cost-current_link->cost, r_total_duration-current_link->duration);
                    }
                    next_link = current_link->next;
                    /*Free memory for the current link and set current_link to point to the next link in the route*/
                    free(current_link);
                }
                else{
                    if(strcmp(current_link->destiny, stop) == ZERO){
                        r_num_stops--;
                        found = 1;
                        if(current_link->next == NULL){
                            update_route(head_route, "", "", ZERO, ZERO, ZERO);
                            next_link = current_link->next;
                            free(current_link);
                        }
                        else{
                            previous_link = current_link;
                            r_total_cost_aux = previous_link->cost;
                            r_total_duration_aux = previous_link->duration;
                        }
                    }
                }
            }
            else{
                if( current_link->next != NULL){
                    if(previous_link!=NULL){
                        if(strcmp(current_link->destiny, stop) != ZERO){
                            update_link(previous_link ,previous_link->origin, current_link->destiny, (previous_link->cost + r_total_cost_aux), (previous_link->duration+r_total_duration_aux));
                            head_route->num_stops=r_num_stops;
                            previous_link->next = current_link->next;
                            next_link = previous_link;
                            previous_link = NULL;
                            free(current_link);
                        }
                        else{
                            r_num_stops--;
                            r_total_cost_aux += previous_link->cost;
                            r_total_duration_aux += previous_link->duration;
                            previous_link->next = current_link->next;
                            next_link->next = previous_link;
                            /*Free memory for the current link and set current_link to point to the next link in the route*/
                            free(current_link);
                        }
                    }
                    else{
                        if(strcmp(current_link->destiny, stop) == 0){
                            found = 1;
                            previous_link = current_link;
                            r_total_cost_aux = previous_link->cost;
                            r_total_duration_aux = previous_link->duration;
                        }
                    }
                }
                else{ 
                    if(previous_link!=NULL){    
                        if(strcmp(current_link->destiny, stop) != ZERO){
                            if(strcmp(current_link->origin, stop) != ZERO){
                                found = 1;           
                                update_link(previous_link ,previous_link->origin, current_link->destiny, (previous_link->cost + r_total_cost_aux), (previous_link->duration+r_total_duration_aux));
                                head_route->num_stops--;
                                previous_link->next = NULL;
                                next_link->next = previous_link;
                                previous_link = NULL;
                                r_total_cost_aux = 0;
                                r_total_duration_aux = 0;
                                free(current_link);
                            }
                            else{
                                head_route->num_stops--;
                                update_link(previous_link ,previous_link->origin, current_link->destiny, (previous_link->cost + r_total_cost_aux), (previous_link->duration+r_total_duration_aux));
                                previous_link->next = NULL;
                                free(current_link);
                            }
                        }
                    }
                    else{
                        if(strcmp(current_link->destiny, stop) == ZERO){
                            found = 1;
                            r_num_stops--;
                            update_route(head_route, r_origin, current_link->origin, r_num_stops, r_total_cost-current_link->cost, r_total_duration-current_link->duration);
                            next_link->next = NULL;
                            free(current_link);
                        }
                    }
                }
            }
            
            current_link = current_link->next;
        }
        if(found == 1 ){
           head_route->links = next_link; 
        }
        head_route = head_route->next; 
    }
    free(r_origin);
    free(r_destiny);
    while (head_route != NULL) {
        current_link = head_route->links;
        while (current_link != NULL) {
            struct link* next_link = current_link->next;
            free(current_link);
            current_link = next_link;
        }
        head_route = head_route->next;
    }
}

/*This function removes a stop from a linked list of stops, as well as any links to that stop from a linked list of routes.*/
void remove_stop(struct stop **head_stop, struct route **head_route, char name[]){
    struct stop *current_stop = *head_stop;
    struct stop *previous_stop = NULL;
    int found = ZERO;
    /*search for the stop with the given name in the linked list of stops*/
    while (current_stop != NULL){
        if (strcmp(current_stop->name, name) == ZERO){
            found = ONE;
            break;
        }
        previous_stop = current_stop;
        current_stop = current_stop->next;
    }
    /*if the stop is found, remove it from the linked list of stops*/
    if (found == ONE){
        if (previous_stop == NULL){
            /*the stop to be removed is the head of the linked list of stops*/
            *head_stop = current_stop->next;
        }
        else{
            /*the stop to be removed is not the head of the linked list of stops*/
            previous_stop->next = current_stop->next;
        }

        /*remove the links that contain the stop to be removed from the linked list of routes*/
        remove_links_stops_in_routes(*head_route, current_stop->name);

        /*free the memory allocated for the current stop*/
        free(current_stop);
    }
    else{
        /*the stop to be removed was not found in the linked list of stops*/
        printf("%s: no such stop.\n", name);
    }
}

/*This function removes all stops from a linked list of stops by iterating over each stop and freeing the memory allocated for it.*/
void remove_all_stops(struct stop **head_stop){
    struct stop *current_stop = *head_stop;
    struct stop *next_stop = NULL;
    /*iterate over all stops in the linked list of stops*/
    while (current_stop != NULL){
        /*store the next stop before deleting the current stop*/
        next_stop = current_stop->next;
        /*free the memory allocated for the current stop*/
        free(current_stop);
        current_stop = next_stop;
    }
    /*set the head of the linked list of stops to NULL*/
    *head_stop = NULL;
}

/*This function removes a route and its associated links from a linked list of routes, given the name of the route.*/
void remove_route(struct route **head_route, char name[]){
    struct route *current_route = *head_route;
    struct link *current_link;
    struct link *next_link;
    struct route *previous_route = NULL;
    int found = ZERO;
    /* iterate over all routes in the linked list of routes */
    while (current_route != NULL){
        /* check if current route is the one to remove */
        if (strcmp(current_route->name, name) == ZERO){
            found = ONE;
            break;
        }
        previous_route = current_route;
        current_route = current_route->next;
    }
    if (found == ONE){
        /* remove the route from the linked list */
        if (previous_route == NULL)
        {
            *head_route = current_route->next;
        }
        else
        {
            previous_route->next = current_route->next;
        }
        /* iterate over all links in the removed route */
        current_link = current_route->links;
        while (current_link != NULL){
            next_link = current_link->next;
            /* free memory allocated for link fields */
            free(current_link->name_route);
            free(current_link->origin);
            free(current_link->destiny);
            free(current_link);
            current_link = next_link;
        }
        /* free memory allocated for route fields */
        free(current_route->name);
        free(current_route->origin);
        free(current_route->destiny);
        free(current_route);
    }
    /* route not found */
    if (found == ZERO){
        printf("%s: no such line.\n", name);
    }
}

/*This function removes all routes and their associated links from a linked list of routes.*/
void remove_all_routes(struct route **head_route){
    struct route *current_route = *head_route;
    struct route *next_route = NULL;
    struct link *current_link = NULL;
    struct link *next_link = NULL;
    /*iterate over all routes in the linked list of routes*/
    while (current_route != NULL){
        /*store the next route before deleting the current route*/
        next_route = current_route->next;
        /*iterate over all links in the current route*/
        current_link = current_route->links;
        while (current_link != NULL){
            /*store the next link before deleting the current link*/
            next_link = current_link->next;
            free(current_link);
            current_link = next_link;
        }
        /*free the memory allocated for the current route*/
        free(current_route);
        current_route = next_route;
    }
    /*set the head of the linked list of routes to NULL*/
    *head_route = NULL;
}

/*This function sorts and prints routes that pass through a given stop.*/
void sort_stop_routes(route *head_route, char stop_name[]){
    int i, j, min_index, num_routes = ZERO, route_belongs = ZERO;
    char *temp;
    char **routes = NULL;
    struct route *current_route = head_route;
    /*iterate over all the routes in the linked list*/
    while (current_route != NULL){
        route_belongs = ZERO;
        /*Check if the stop is the origin or destiny of the current route*/
        if (strcmp(current_route->origin, stop_name) == ZERO || strcmp(current_route->destiny, stop_name) == ZERO){
            route_belongs = ONE;
        }
        else{
            /*Check if the stop is in any of the links of the current route*/
            struct link *current_link = current_route->links;
            while (current_link != NULL){
                if (strcmp(current_link->origin, stop_name) == ZERO || strcmp(current_link->destiny, stop_name) == ZERO){
                    route_belongs = ONE;
                    break;
                }
                current_link = current_link->next;
            }
        }
        /*If the stop belongs to the current route, add the route name to the array of routes*/
        if(route_belongs){

            if (routes == NULL){
                /*Allocate memory for the first route name*/
                routes = (char **)malloc(sizeof(char *));
                if (routes == NULL){
                    printf("No memory.\n");
                    exit(-1);
                }
            }
            else
                /* Resize the array to fit one more route name*/
                routes = (char **)realloc(routes, (num_routes + ONE) * sizeof(char *));
            /* Allocate memory for the route name */
            routes[num_routes] = (char *)malloc((strlen(current_route->name) + ONE) * sizeof(char));
            if (routes[num_routes] == NULL){
                printf("No memory.\n");
                exit(-1);
            }
            /*Copy the route name to the allocated memory*/
            strcpy_alloc(&(routes[num_routes]), current_route->name);
            num_routes++;
        }
        current_route = current_route->next;
    }
    /*If there are routes that belong to the stop, sort and print them*/
    if (routes != NULL){
        if(num_routes > ONE){
            /*the strchr() function finds the first occurrence of a character in a string*/
            /* Check if the stop name contains spaces or tabs */
            if (strchr(stop_name, ' ') != NULL || strchr(stop_name, '\t') != NULL){
                /*\" because stops whose name has spaces must be enclosed in quotes*/
                printf("\"%s\" %d:", stop_name, num_routes);
            }
            else{
                /*prints the name of the stop and the number of routes that pass through that stop*/
                printf("%s %d:", stop_name, num_routes);
            }
            /*Sort the array of route names using selection sort*/
            for (i = ZERO; i < num_routes - ONE; i++){
                min_index = i;
                for (j = i + ONE; j < num_routes; j++){
                    if (strcmp(routes[j], routes[min_index]) < ZERO)
                    {
                        min_index = j;
                    }
                }
                if (min_index != i)
                {
                    temp = routes[i];
                    routes[i] = routes[min_index];
                    routes[min_index] = temp;
                }
                /* Print the sorted array of route names*/
                for (i = ZERO; i < num_routes; i++){
                    if (i == num_routes - ONE){
                        /*The /n is only placed after the last word*/
                        printf(" %s\n", routes[i]);
                    }
                    else
                    {
                        printf(" %s", routes[i]);
                    }
                }
            }
            /*Free the allocated memory for the route name */
            for (i = ZERO; i < num_routes; i++){
                free(routes[i]);
            }
        }
        free(routes);
    }
}

/*this function prints the elements from the find_route() function, in reverse order.*/
void print_route_invers(route *current_route){
    int i = ZERO;
    int num_stops = ZERO;
    /*find the stops in the current route and store them in a char pointer array*/
    char **route = find_route(current_route, &num_stops);
    /*iterate over the stops in the route backwards and print them to the console*/
    for (i = num_stops - ONE; i + ONE != ZERO; i--){
        printf("%s", route[i]);
        if (i != ZERO)
            printf(", "); /*print a comma if the current stop is not the first stop in the route*/
        else
            printf("\n"); /*print a newline character if the current stop is the first stop in the route*/
    }
    for (i = ZERO; i < num_stops; i++){
        free(route[i]);
    }
    free(route);
}

/*this function prints the elements from the find_route() function*/
void print_route(route *current_route){
    int i = ZERO;
    int num_stops = ZERO;
    /*find the stops in the current route and store them in a char pointer array*/
    char **route = find_route(current_route, &num_stops);
    /*iterate over the stops in the route and print them to the console*/
    while (i < num_stops){
        printf("%s", route[i]);
        if (i + ONE != num_stops)
            printf(", "); /*print a comma if the current stop is not the last stop in the route*/
        else
            printf("\n"); /*print a newline character if the current stop is the last stop in the route*/
        i++;
    }
    for (i = ZERO; i < num_stops; i++){
        free(route[i]);
    }
    free(route);
}

/*This function checks if a given stop name exists in an array of stops and returns 1 if it does and 0 if it doesn't.*/
int route_pass_stop(char name_stop[], char **vector_stops, int num_stops){
    int i;
    /*check if the array of stops is not empty*/
    if (vector_stops != NULL){
        /*iterate over the stops in the list*/
        for (i = ZERO; i < num_stops; i++){
            /*compare the current stop name with the target stop name*/
            if (strcmp(name_stop, vector_stops[i]) == ZERO){
                /*stop name is found in the list, return 1*/
                return ONE;
            }
        }
    }
    /*stop name not found in the list, return 0*/
    return ZERO;
}

/*This function counts how many routes pass through a given stop.*/
int count_route_pass_stop(struct route **head_route, char stop[]) {
    int count = ZERO, i; /*Counter for the number of routes that pass through the stop*/
    struct route *current_route = *head_route; /*Pointer to the current route being examined, starting at the head of the list*/
    /*Iterate over each route in the linked list*/
    while (current_route != NULL){
        /*Check if the current route passes through the specified stop*/
        char **route = find_route(current_route, &(current_route->num_stops));
        if (route_pass_stop(stop, route, current_route->num_stops)) {
            /*If the current route passes through the specified stop, increment the counter*/
            count++;
        }
        /*Deallocate the memory for the route array*/
        for (i = 0; i < current_route->num_stops; i++) {
            free(route[i]);
        }
        free(route);
        /*Move on to the next route in the linked list*/
        current_route = current_route->next;
    }
    /*Return the final count of routes that pass through the specified stop*/
    return count;
}


/*From here on down are the functions that are used directly in the main function*/

/*This function extract the command arguments from the user input*/
void arguments(char *instruction, char **args){
    int i, j, k = ZERO, args_i, ilen, argument_size;
    char *argument = malloc(TWO * sizeof(char)); /*allocate memory for the argument string*/
    if (argument == NULL){
        printf("No memory.\n");
        exit(-1);
    }
    argument_size = TWO;
    ilen = strlen(instruction);
    args_i = ZERO;
    for (i = ZERO; i < MAX_ARGS; i++)
        strcpy(args[i], ""); /*initialize args array with empty strings*/
    for (j = TWO; j <= ilen && args_i <= MAX_ARGS; j++){ /*iterate over the instruction string*/
        if (instruction[j] != ' ' && instruction[j] != '\n' && instruction[j] != '\0'){ /*check for non-delimiter characters*/
            if (instruction[j] == '\"'){ /*handle quotes*/
                j++;
                while (instruction[j] != '\"' && instruction[j] != '\n' && instruction[j] != '\0'){
                    if (k + ONE >= argument_size){ /*expand argument string if necessary*/
                        argument_size *= TWO;
                        argument = realloc(argument, argument_size * sizeof(char));
                    }
                    argument[k] = instruction[j]; /*add character to argument string*/
                    j++;
                    k++;
                }
            }
            else{ /*handle normal words*/
                while (instruction[j] != ' ' && instruction[j] != '\n' && instruction[j] != '\0'){
                    if (k + ONE >= argument_size)
                    {
                        argument_size *= TWO;
                        argument = realloc(argument, argument_size * sizeof(char));
                    }
                    argument[k] = instruction[j];
                    j++;
                    k++;
                }
            }
            argument[k] = '\0'; /*terminate argument string*/
            strcpy(args[args_i], argument); /*copy argument to args array*/
            args_i++;
            argument_size = TWO; /*reset argument size and string*/
            argument = realloc(argument, argument_size * sizeof(char));
            argument[ZERO] = '\0';
            k = ZERO;
        }
    }
    free(argument); /*free argument string memory*/
}

/*This function dd and list careers*/
void comand_c(route **head_route, char **args){
    /*Create a pointer to the head of the linked list of routes.*/
    route *current_route = *head_route;
    /*Check if the first argument is empty, which means that the user wants to print all routes.*/
    if (strcmp(args[ZERO], "") == ZERO){
        /*Iterate through all the routes in the linked list, printing their information.*/
        while (current_route != NULL){
            if (strcmp(current_route->origin, "") == ZERO)
                printf("%s %d %.2f %.2f\n", current_route->name, current_route->num_stops, current_route->total_cost, current_route->total_duration);
            else
                printf("%s %s %s %d %.2f %.2f\n", current_route->name, current_route->origin, current_route->destiny,
                       current_route->num_stops, current_route->total_cost, current_route->total_duration);
            current_route = current_route->next;
        }
    }
    /*If the first argument is not empty, it means the user wants to perform an operation on a specific route.*/
    else{
        /*Check if the route already exists.*/
        current_route = route_exist(*head_route, args[ZERO]);
        if (current_route == NULL){
            /*If the route doesn't exist, add it to the linked list.*/
            add_route(head_route, args[ZERO], "", "", ZERO, ZERO, ZERO);
        }
        else{
            /*If the route already exists, check the second argument to see what operation the user wants to perform.*/
            if (strcmp(args[ONE], "") == ZERO){
                /*If the second argument is empty, print the information for the route.*/
                print_route(current_route);
            }
            else if (strcmp(args[ONE], "inv") == ZERO || strcmp(args[ONE], "inve") == ZERO ||
                     strcmp(args[ONE], "inver") == ZERO || strcmp(args[ONE], "invers") == ZERO ||
                     strcmp(args[ONE], "inverso") == ZERO){
                /*If the second argument is one of the valid sort options, print the information for the route in reverse order.*/
                print_route_invers(current_route);
            }
            else{
                /*If the second argument is not empty and not one of the valid sort options, print an error message.*/
                printf("incorrect sort option.\n");
            }
        }
    }
}

/*This function add and list stops*/
void comand_p(stop **head_stop, route **head_route, char **args){
    /*check if no arguments were given*/
    if (strcmp(args[ZERO], "") == ZERO){
        /*print information about all stops*/
        stop *current_stop = *head_stop;
        while (current_stop != NULL){
            printf("%s: %16.12f %16.12f %d\n", current_stop->name, current_stop->latitude, current_stop->longitude, count_route_pass_stop(head_route, current_stop->name));
            current_stop = current_stop->next;
        }
    }
    else{
        /*check if second argument was given*/
        if (strcmp(args[ONE], "") != ZERO){
            /*check if stop already exists*/
            if (stop_exist(*head_stop, args[ZERO])){
                printf("%s: stop already exists.\n", args[ZERO]);
            }
            else{
                /*add a new stop*/
                add_stop(head_stop, args[ZERO], atof(args[ONE]), atof(args[TWO]));
            }
        }
        else{
            /*check if stop exists*/
            if (!stop_exist(*head_stop, args[ZERO])){
                printf("%s: no such stop.\n", args[ZERO]);
            }
            else{
                /*print information about a specific stop*/
                stop *current_stop = *head_stop;
                while (current_stop != NULL){
                    if (strcmp(current_stop->name, args[ZERO]) == ZERO){
                        printf("%16.12f %16.12f\n", current_stop->latitude, current_stop->longitude);
                    }
                    current_stop = current_stop->next;
                }
            }
        }
    }
}

/*This function add links*/
void comand_l(route **head_route, stop **head_stop, char **args){
    struct route *current_route = *head_route;
    struct stop *current_stop_origin = *head_stop;
    struct stop *current_stop_destiny = *head_stop;
    current_route = route_exist(*head_route, args[ZERO]);
    if (current_route == NULL){
        /*in case there is no route with the indicated name*/
        printf("%s: no such line.\n", args[ZERO]);
    }
    else{
        current_stop_origin = stop_exist(*head_stop, args[ONE]);
        if (current_stop_origin == NULL){
            /*in case a stop with the indicated name has not been created*/
            printf("%s: no such stop.\n", args[ONE]);
        }
        else{
            current_stop_destiny = stop_exist(*head_stop, args[TWO]);
            if (current_stop_destiny == NULL){
                /*in case a stop with the indicated name has not been created*/
                printf("%s: no such stop.\n", args[TWO]);
            }
            else{
                if (current_route->links != NULL && !(strcmp(current_route->origin, args[ONE]) == ZERO || strcmp(current_route->origin, args[TWO]) == ZERO || strcmp(current_route->destiny, args[ONE]) == ZERO || strcmp(current_route->destiny, args[TWO]) == ZERO)){
                    printf("link cannot be associated with bus line.\n");
                }
                else{
                    if (atof(args[THREE]) < ZERO || atof(args[FOUR]) < ZERO){
                        /*in case the cost or duration on the link are negative values.*/
                        printf("negative cost or duration.\n");
                    }
                    else{
                        add_link(current_route, args[ZERO], args[ONE], args[TWO], atof(args[THREE]), atof(args[FOUR]));
                        if (current_route->num_stops == ZERO){
                            strcpy_alloc(&(current_route->origin), args[ONE]);
                            strcpy_alloc(&(current_route->destiny), args[TWO]);
                            current_route->num_stops += TWO;
                        }
                        else{
                            /*when the connection is between the origin and destination stops of a route, making this a circular route*/
                            if((strcmp(current_route->destiny, current_stop_origin->name) == ZERO) &&
                                (strcmp(current_route->origin, current_stop_destiny->name) == ZERO)){
                                strcpy_alloc(&(current_route->destiny), args[TWO]);
                            }
                            /*when you want to add a connection with the destination stop*/
                            else if(strcmp(current_route->destiny, current_stop_origin->name) == ZERO){
                                strcpy_alloc(&(current_route->destiny), args[TWO]);
                            }
                            /*when you want to add a connection with the origin stop*/
                            else if(strcmp(current_route->origin, current_stop_destiny->name) == ZERO){
                                strcpy_alloc(&(current_route->origin), args[ONE]);
                            }
                            current_route->num_stops++;
                        }
                        current_route->total_cost += atof(args[THREE]);
                        current_route->total_duration += atof(args[FOUR]);
                    }
                }
            }
        }
    }
}

/*This function list intersections between careers*/
void comand_i(stop *head_stop, route *head_route){
    /*lista os nós de interligação*/
    while (head_stop != NULL){
        sort_stop_routes(head_route, head_stop->name);
        head_stop = head_stop->next;
    }
}

/*This function removes a route*/
void comand_r(struct route **head_route, char name[]){
    remove_route(head_route, name);
}

/*This function removes a stop*/
void comand_e(struct stop **head_stop, struct route **head_route, char name[]){
    remove_stop(head_stop, head_route, name);
}

/*This function removes all routes and stops*/
void comand_a(struct route **head_route, struct stop **head_stop){
    remove_all_routes(head_route);
    remove_all_stops(head_stop);
}