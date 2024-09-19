#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "graph.h"

void show_feed(posts_t *posts, list_graph_t *graph) {
	char *name = strtok(NULL, "\n ");
	int feed_size = atoi(strtok(NULL, "\n "));
	int cnt = 0;
	unsigned int user_id = get_user_id(name);
	for (int i = posts->size - 1; i >= 0 && cnt < feed_size; --i) {
		if (lg_has_edge(graph, user_id, posts->posts[i]->user_id) ||
			user_id == posts->posts[i]->user_id) {
			printf("%s: \"%s\"\n", get_user_name(posts->posts[i]->user_id),
								   posts->posts[i]->title);
			++cnt;
		}
	}
}

static
void check_post_for_profile(post_t *post, unsigned int uid, char *title) {
	if (!post) return;

	if (post->user_id == uid) {
		if (post->title)
			printf("Posted: \"%s\"\n", title);
		else
			printf("Reposted: \"%s\"\n", title);
	}

	for (unsigned int i = 0; i < post->size; ++i)
		check_post_for_profile(post->events[i], uid, title);
}

void view_profile(posts_t *posts) {
	char *name = strtok(NULL, "\n ");
	unsigned int user_id = get_user_id(name);
	for (unsigned int i = 0; i < posts->size; ++i)
		check_post_for_profile(posts->posts[i], user_id, posts->posts[i]->title);
}

static
void find_friends(post_t *post, unsigned int uid, list_graph_t *graph) {
	if (!post) return;

	if (lg_has_edge(graph, uid, post->user_id) && !post->title)
		printf("%s\n", get_user_name(post->user_id));
	
	for (unsigned int i = 0; i < post->size; ++i)
		find_friends(post->events[i], uid, graph);
}

void friends_repost(posts_t *posts, list_graph_t *graph) {
	char *name = strtok(NULL, " \n");
	unsigned int post_id = atoi(strtok(NULL, " \n"));
	unsigned int pos = find_pos(posts, post_id);
	unsigned int uid = get_user_id(name);

	post_t *current_post = posts->posts[pos];
	find_friends(current_post, uid, graph);
}

void handle_input_feed(char *input, posts_t *posts, list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	(void)posts;
	if (!cmd)
		return;

	if (!strcmp(cmd, "feed"))
		show_feed(posts, graph);
	else if (!strcmp(cmd, "view-profile"))
		view_profile(posts);
	else if (!strcmp(cmd, "friends-repost"))
		friends_repost(posts, graph);
	else if (!strcmp(cmd, "common-groups"))
		(void)cmd;
		// TODO: Add function

	free(commands);
}
