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

#endif // FEED_H
