#ifndef POSTS_H
#define POSTS_H

#include "graph.h"

#define MAX_TITLE_LEN 281

typedef struct tree_t tree_t;
typedef struct post_t post_t;
typedef struct posts_t posts_t;

struct post_t {
    unsigned int id, user_id;
    char *title;
    unsigned int size, capacity;
    post_t **events;
    unsigned int total_likes;
    unsigned char *v;
};

struct posts_t {
    unsigned int size, capacity;
    post_t **posts;
};

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, posts_t *posts);

void free_posts(posts_t **posts);

#endif // POSTS_H
