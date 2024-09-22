#ifndef FEED_H
#define FEED_H

#include "posts.h"
#include "graph.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, posts_t *posts, list_graph_t *graph);

/**
 * @brief Shows the last <feed_size> posts of a user and its friends
 * @param posts Array of posts
 * @param graph The graph of connections
 */
void show_feed(posts_t *posts, list_graph_t *graph);

/**
 * @brief Displays all the posts and reposts of a user
 * @param posts Array of posts
 */
void view_profile(posts_t *posts);

/**
 * @brief Shows the friends of a user that reposted
 * a post with a given id
 * @param posts Array of posts
 * @param graph The graph of connections
 */
void friends_repost(posts_t *posts, list_graph_t *graph);

/**
 * @brief Finds the maximal clique for with no pivoting
 * @param p The set of potential nodes
 * @param r The current solution for the maximal clique
 * @param x The set of forbidden nodes
 * @param res Pointer that saves the solution
 * @param maxim Maximal length of the solution
 * @param graph The graph of connections
 */
void bron_kerbosch1(int *p, int *r, int *x, int *res,
					list_graph_t *graph, int *maxim);

/**
 * @brief Driver for bron_kerbosch1 function:
 * Displays the users in the maximal clique found
 * @param graph The graph of connections
 */
void common_groups(list_graph_t *graph);

#endif // FEED_H
