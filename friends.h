#ifndef FRIENDS_H
#define FRIENDS_H

#include "graph.h"
#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *graph);

void add_friend(list_graph_t *graph);

void remove_friend(list_graph_t *graph);

void get_distance(list_graph_t *graph);

void suggestions(list_graph_t *graph);

void common(list_graph_t *graph);

void friends(list_graph_t *graph);

void popular(list_graph_t *graph);

void common(list_graph_t *graph);

#endif // FRIENDS_H
