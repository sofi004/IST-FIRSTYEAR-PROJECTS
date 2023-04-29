#include "project2.h" /*Include the project2 header file*/

int main(void){
    char instruction[MAXINPUT]; /*Declare a character array to store user input*/
    char *args[MAX_ARGS]; /*Declare an array of character pointers to store command arguments*/
    char comand; /*Declare a character variable to store the command character*/
    int i; /*Declare an integer variable for looping*/
    stop *head_stop = NULL; /*Declare and initialize a pointer to the head of the stop linked list*/
    route *head_route = NULL; /*Declare and initialize a pointer to the head of the route linked list*/
    /*Dynamically allocate memory for each element of the args array*/
    for (i = 0; i < MAX_ARGS; i++){
        args[i] = malloc(MAXINPUT * sizeof(char));
        /*Check if memory allocation was successful*/
        if (args[i] == NULL){
            printf("No memory.\n");
            return -ONE;
        }
    } 
    /*Loop until the user enters the 'q' command to quit the program*/
    do{
        char *ret = fgets(instruction, MAXINPUT, stdin); /*Read user input from the command line*/
        /*Check if fgets returned NULL (i.e., end-of-file or error)*/
        if (ret == NULL){
            break;
        }
        comand = instruction[ZERO]; /*Extract the command character from the user input*/
        arguments(instruction, args); /*Extract the command arguments from the user input*/
        /*Execute the appropriate command based on the command character*/
        switch (comand){
        case 'q': /*Terminate the program*/
            comand_a(&head_route, &head_stop);/*Free all memory and exit*/
            break;
        case 'c': /*Add and list careers*/
            comand_c(&head_route, args);
            break;
        case 'p': /*Add and list stops*/
            comand_p(&head_stop, &head_route, args);
            break;
        case 'l': /*Add links*/
            comand_l(&head_route, &head_stop, args);
            break;
        case 'i': /*List intersections between careers*/
            comand_i(head_stop, head_route); 
            break;
        case 'r': /*Remove a route*/
            comand_r(&head_route, args[0]); 
            break;
        case 'e': /*Remove a stop from a route*/
            comand_e(&head_stop, &head_route, args[0]); 
            break;
        case 'a': /*Remove all routes and stops*/
            comand_a(&head_route, &head_stop); /*Free all memory*/
            break;
        default: /*Invalid command*/
            printf("This command is not valid\n");
            break;
        }
    } while (comand != 'q'); /*Loop until the user enters the 'q' command to quit the program*/
    /*Free memory for each element of the args array*/
    for (i = 0; i < MAX_ARGS; i++){
        free(args[i]);
    }
    return ZERO; /*Exit the program with a status code of 0 (success)*/
}