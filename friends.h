#ifndef FRIENDS_H
#define FRIENDS_H

#include "graph.h"
#define MAX_COMMAND_LEN 500

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *graph);

/**
 * @brief Makes a connection between two users
 * @param graph The graph of connections
 */
void add_friend(list_graph_t *graph);

/**
 * @brief Removes the connection between two users
 * @param graph The graph of connections
 */
void remove_friend(list_graph_t *graph);

/**
 * @brief Shows the minimal number of connections
 *  between two users if there is a path
 * @param graph The graph of connections
 */
void get_distance(list_graph_t *graph);

/**
 * @brief Finds the friends of all the friends of
 * a user that are not yet connected with the user.
 * Displays them in ascending order of the user id.
 * @param graph The graph of connections
 */
void suggestions(list_graph_t *graph);

/**
 * @brief Finds the common friends of two users
 * Displays them in ascending order of the user id.
 * @param graph The graph of connections
 */
void common(list_graph_t *graph);

/**
 * @brief Displays the number of connections of a user
 * @param graph The graph of connections
 */
void friends(list_graph_t *graph);

/**
 * @brief Displays the most popular user between
 * a user with name <name> and its friends
 * @param graph The graph of connections
 */
void popular(list_graph_t *graph);

#endif // FRIENDS_H
