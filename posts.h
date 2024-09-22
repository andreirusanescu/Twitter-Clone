/*
 * Copyright (c) 2024, Andrei Rusanescu <andreirusanescu154gmail.com>
 */

#ifndef POSTS_H
#define POSTS_H

#include "graph.h"

#define MAX_TITLE_LEN 281

typedef struct tree_t tree_t;
typedef struct post_t post_t;
typedef struct posts_t posts_t;

/* Structure of a post */
struct post_t {
    /* ID of post and User ID of the author */
    unsigned int id, user_id;
    /* Title of the post, repost => NULL */
    char *title;

    /* Holds a resizable array for the reposts */
    unsigned int size, capacity;
    /* Reposts */
    post_t **events;
    /* Counter for total_likes of a post / repost */
    unsigned int total_likes;
    /* People that liked it */
    unsigned int *v;
};

/** Container of all the posts
 *  Resizable array implementation
 */
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

/**
 * @brief Creates a posts called <title> for the user called <name>
 * @param posts Array of all the posts
 */
void create_post(posts_t **posts);

/**
 * @brief Performs binary search for a post
 * @param posts Array of posts
 * @param tree_id ID of the looked up post
 * @return Index of the searched post
 */
unsigned int find_pos(posts_t *posts, unsigned int tree_id);

/**
 * @brief Looks up for the repost with the ID <repost_id> in a post
 * @param current_post Current post
 * @param repost_id ID of the repost to look for in <current_post>
 * @param prev Pointer to a post_t node, saves the node before the
 * looked up repost
 * @param pos Saves the position of the repost in the array of events
 * @return Post found / NULL
 */
post_t *find_repost(post_t *current_post, unsigned int repost_id,
					post_t **prev, unsigned int *pos);

/**
 * @brief Makes a repost to a post / repost with the id
 * <post_id>, <repost_id> respectively, authored by user <name>
 * @param posts Array of posts
 */
void repost(posts_t **posts);

/**
 * @brief Finds the common repost in a post with ID <tree_id>
 * between two reposts with ID's <rep1> and <rep2> using
 * Lowest Common Ancestor approach
 * @param posts Array of posts
 */
void common_repost(posts_t *posts);

/**
 * @brief Likes
 * If a user likes a post that was already liked,
 * it takes away its like
 * @param posts Array of posts
 */
void like(posts_t *posts);

/**
 * @brief Checks if a repost of the post <post_id> has
 * more likes than the original post
 * @param posts Array of posts
 */
void ratio(posts_t *posts);

/**
 * @brief Deletes a post / repost with <post_id>,
 * <repost_id> respectively
 * @param posts Array of posts
 */
void delete(posts_t *posts);

/**
 * @brief Displays the entire hierarchy for a post / repost
 * with <post_id>, <repost_id> respectively
 * @param posts Array of posts
 */
void get_reposts(posts_t *posts);

/**
 * @brief Frees the allocated memory for the array of posts
 * @param posts Pointer to the array of posts
 */
void free_posts(posts_t **posts);

#endif // POSTS_H
