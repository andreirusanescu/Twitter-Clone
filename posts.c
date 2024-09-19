#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

static unsigned int post_counter = 1;

static
void free_post_tree(post_t **tree) {
	if (!*tree) return;

	for (unsigned int i = 0; i < (*tree)->size; ++i)
		free_post_tree(&(*tree)->events[i]);
	free((*tree)->events);
	free((*tree)->v);
	free((*tree)->title);
	free((*tree));
	*tree = NULL;
}

static
void preorder(post_t *root, int cnt) {
	if (!root)
		return;
	if (!cnt)
		printf("\"%s\" - Post by %s\n", root->title, get_user_name(root->user_id));
	else
		printf("Repost #%d by %s\n", root->id, get_user_name(root->user_id));
	for (unsigned int i = 0; i < root->size; ++i)
		preorder(root->events[i], cnt + 1);
}

static
post_t *lca(post_t *root, unsigned int val1, unsigned int val2) {
	if (!root)
		return NULL;

	if (root->id == val1 || root->id == val2)
		return root;
	post_t *res = NULL, *found;
	int cnt = 0;
	for (unsigned int i = 0; i < root->size; ++i) {
		found = lca(root->events[i], val1, val2);
		if (found) {
			++cnt;
			res = found;
		}
	}

	if (cnt == 2)
		return root;
	return res;
}

post_t *make_post(char *title, unsigned int user_id) {
	post_t *post = (post_t *)calloc(1, sizeof(post_t));
	post->id = post_counter;
	post->size = 0;	
	post->user_id = user_id;
	if (title) {
		post->title = malloc(MAX_TITLE_LEN);
		strcpy(post->title, title);
		post->events = calloc(1, sizeof(post_t *));
		post->capacity = 1;
	} else {
		post->events = NULL;
		post->capacity = 0;
	}
	post->v = calloc(550, sizeof(unsigned int));
	post->total_likes = 0;
	return post;
}

void create_post(posts_t **posts) {
	char *name = strtok(NULL, " \n");
	char *title = strtok(NULL, "\"");
	unsigned int id = get_user_id(name);

	post_t *post = make_post(title, id);

	if ((*posts)->size == (*posts)->capacity) {
		(*posts)->capacity <<= 1;
		(*posts)->posts = realloc((*posts)->posts, sizeof(post_t *) * (*posts)->capacity);
	}

	(*posts)->posts[(*posts)->size] = post;
	++(*posts)->size;

	printf("Created \"%s\" for %s\n", title, name);
	++post_counter;
}

unsigned int find_pos(posts_t *posts, unsigned int tree_id) {
	for (unsigned int i = 0; i < posts->size; ++i)
		if (posts->posts[i]->id == tree_id)
			return i;
	return 0;
}

post_t *find_repost(post_t *current_post, unsigned int repost_id,
					post_t **prev, unsigned int *pos) {
	if (current_post->id == repost_id)
		return current_post;

	post_t *found;
	for (unsigned int i = 0; i < current_post->size; ++i) {
		*prev = current_post;
		found = find_repost(current_post->events[i], repost_id, prev, pos);
		if (found) {
			*pos = i;
			return found;
		}
	}
	return NULL;
}

void repost(posts_t **posts) {
	char *name = strtok(NULL, " \n");
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int repost_id = 0;
	char *repost_string = strtok(NULL, " \n");
	if (repost_string)
		repost_id = atoi(repost_string);
	
	/* Find the correct tree */
	unsigned int pos = find_pos(*posts, post_id);
	post_t *current_post = (*posts)->posts[pos];

	/* Resizable array check */
	if (current_post->size == current_post->capacity) {
		current_post->capacity <<= 1;
		current_post->events = realloc(current_post->events,
											current_post->capacity *
											sizeof(post_t *));
	}

	post_t *rep = make_post(NULL, get_user_id(name));

	/* Repost for a post */
	if (!repost_id) {
		current_post->events[current_post->size] = rep;
		++current_post->size;
	} else {
		post_t *dummy = NULL;
		post_t *repp = find_repost(current_post, repost_id, &dummy, &pos);
		if (repp->size == repp->capacity) {
			if (repp->capacity) {
				repp->capacity <<= 1;
				repp->events = realloc(repp->events, repp->capacity * sizeof(post_t *));
			} else {
				repp->events = calloc(1, sizeof(post_t *));
				repp->capacity = 1;
			}
		}

		repp->events[repp->size] = rep;
		++repp->size;
	}
	printf("Created repost #%d for %s\n", post_counter, name);
	++post_counter;
}

void common_repost(posts_t *posts) {
	unsigned int tree_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, tree_id);

	unsigned int rep1 = atoi(strtok(NULL, " \n"));
	unsigned int rep2 = atoi(strtok(NULL, " \n"));

	post_t *current_post = posts->posts[pos];
	post_t *tmp = lca(current_post, rep1, rep2);
	int res = tmp->id;
	printf("The first common repost of %d and %d is %d\n", rep1, rep2, res);
}

void like(posts_t *posts) {
	char *name = strtok(NULL, " \n");
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);

	char *tmp = strtok(NULL, " \n");
	unsigned int repost_id = 0;

	post_t *current_post = posts->posts[pos];
	if (!tmp) {
		if (!current_post->v[get_user_id(name)]) {
			current_post->v[get_user_id(name)] = 1;
			current_post->total_likes++;
			printf("User %s liked post \"%s\"\n", name, current_post->title);
		} else {
			current_post->v[get_user_id(name)] = 0;
			current_post->total_likes--;
			printf("User %s unliked post \"%s\"\n", name, current_post->title);
		}
	} else {
		repost_id = atoi(tmp);
		post_t *dummy = NULL;
		post_t *repp = find_repost(current_post, repost_id, &dummy, &pos);
		if (!repp->v[get_user_id(name)]) {
			repp->v[get_user_id(name)] = 1;
			++repp->total_likes;
			printf("User %s liked repost \"%s\"\n", name, current_post->title);
		} else {
			repp->v[get_user_id(name)] = 0;
			--repp->total_likes;
			printf("User %s unliked repost \"%s\"\n", name, current_post->title);
		}
	}	
}

void check_ratio(post_t *root, unsigned int *max_likes, unsigned int *max_id) {
	if (!root)
		return;
	if (root->total_likes > *max_likes) {
		*max_likes = root->total_likes;
		*max_id = root->id;
	} else if (root->total_likes == *max_likes &&
			   root->id != *max_id) {
		if (*max_id > root->id)
			*max_id = root->id;
	}

	for (unsigned int i = 0; i < root->size; ++i)
		check_ratio(root->events[i], max_likes, max_id);

}

void ratio(posts_t *posts) {
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);
	post_t *current_post = posts->posts[pos];
	unsigned int max_likes = current_post->total_likes, max_id = current_post->id;
	check_ratio(current_post, &max_likes, &max_id);

	if (max_likes != current_post->total_likes)
		printf("Post %d got ratio'd by repost %d\n", post_id, max_id);
	else
		printf("The original post is the highest rated\n");
}

void delete(posts_t *posts) {
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);
	char *tmp = strtok(NULL, " \n");

	post_t *current_post = posts->posts[pos];
	if (tmp) {
		unsigned int rep_id = atoi(tmp);
		post_t *prev = NULL;
		post_t *rep = find_repost(current_post, rep_id, &prev, &pos);
		for (unsigned int i = pos; i < prev->size - 1; ++i)
			prev->events[i] = prev->events[i + 1];
		--prev->size;
		printf("Deleted repost #%d of post \"%s\"\n", rep_id, current_post->title);
		free_post_tree(&rep);
	} else {
		for (unsigned int i = pos; i < posts->size - 1; ++i)
			posts->posts[i] = posts->posts[i + 1];
		--posts->size;
		printf("Deleted \"%s\"\n", current_post->title);
		free_post_tree(&current_post);
	}
}

void get_likes(posts_t *posts) {
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);
	char *tmp = strtok(NULL, " \n");
	post_t *current_post = posts->posts[pos];
	if (!tmp) {
		printf("Post \"%s\" has %d likes\n", current_post->title, current_post->total_likes);
	} else {
		post_t *prev;
		post_t *rep = find_repost(current_post, atoi(tmp), &prev, &pos);
		printf("Repost #%d has %d likes\n", rep->id, rep->total_likes);
	}
}

void get_reposts(posts_t *posts) {
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);
	char *tmp = strtok(NULL, " \n");
	post_t *current_post = posts->posts[pos];
	if (!tmp) {
		preorder(current_post, 0);
	} else {
		post_t *prev;
		post_t *rep = find_repost(current_post, atoi(tmp), &prev, &pos);
		preorder(rep, 1);
	}
}

void free_posts(posts_t **posts) {
	unsigned int i;
	for (i = 0; i < (*posts)->size; ++i)
		free_post_tree(&(*posts)->posts[i]);
	free((*posts)->posts);
	free(*posts);
}

void handle_input_posts(char *input, posts_t *posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "create"))
		create_post(&posts);
	else if (!strcmp(cmd, "repost"))
		repost(&posts);
	else if (!strcmp(cmd, "common-repost"))
		common_repost(posts);
	else if (!strcmp(cmd, "like"))
		like(posts);
	else if (!strcmp(cmd, "ratio"))
		ratio(posts);
	else if (!strcmp(cmd, "delete"))
		delete(posts);
	else if (!strcmp(cmd, "get-likes"))
		get_likes(posts);
	else if (!strcmp(cmd, "get-reposts"))
		get_reposts(posts);

	free(commands);
}

