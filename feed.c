#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "graph.h"
#include "hash.h"

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

void bron_kerbosch1(int *p, int *r, int *x, int *res,
					list_graph_t *graph, int uid, int *maxim) {
	int cnt = 0, v;
	for (v = 0; v < 550; ++v) {
		if (p[v] || x[v]) {
			++cnt;
			break;
		}
	}

	/* R is a maximal clique */
	if (!cnt) {
		int counter = 0;
		for (v = 0; v < 550; ++v)
			if (r[v])
				++counter;
		if (counter > *maxim) {
			for (v = 0; v < 550; ++v)
				res[v] = r[v];

			*maxim = counter;
		}
		return;
	}
	
	int *p_intersection, *x_intersection;
	ll_node_t *util;
	int neigh;
	for (v = 0; v < 550; ++v) {
		/* vertex v is in P */
		if (p[v]) {
			// R U v, P intersection with N(v), x intersection with N(v)
			r[v] = 1;
			p_intersection = calloc(550, sizeof(int));
			x_intersection = calloc(550, sizeof(int));

			/* N(v) */
			util = graph->neighbors[v]->head;
			while (util) {
				neigh = *((int *)util->data);
				if (p[neigh])
					p_intersection[neigh] = 1;

				if (x[neigh])
					x_intersection[neigh] = 1;

				util = util->next;
			}
			bron_kerbosch1(p_intersection, r, x_intersection, res, graph, uid, maxim);
			p[v] = 0;
			x[v] = 1;
			r[v] = 0;
			free(p_intersection);
			free(x_intersection);
		}
	}
}

void common_groups(list_graph_t *graph) {
	char *name = strtok(NULL, "\n ");
	int uid = get_user_id(name);

	int p[550] = {0}, r[550] = {0}, x[550] = {0};
	int *res = malloc(550 * sizeof(int));
	ll_node_t *util;

	r[uid] = 1;

	/* Set of all the vertices in the subgraph containing
	   node uid and its neighbors */
	p[uid] = 1;
	util = graph->neighbors[uid]->head;
	while (util) {
		p[*((int *)util->data)] = 1;
		util = util->next;
	}
	
	int maxim = 0;
	bron_kerbosch1(p, r, x, res, graph, uid, &maxim);

	printf("The closest friend group of %s is:\n", name);
	for (int i = 0; i < 550; ++i)
		if (res[i])
			printf("%s\n", get_user_name(i));
	free(res);
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
	else if (!strcmp(cmd, "common-group"))
		common_groups(graph);

	free(commands);
}
