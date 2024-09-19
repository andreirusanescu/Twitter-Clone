/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "graph.h"

/**
 * Initializes every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	stdout = fopen("test.out", "wt");
	stdin = fopen("test.in", "rt");
	init_users();

	init_tasks();
	list_graph_t *graph = lg_create(MAX_PEOPLE);
	char *input = (char *)malloc(MAX_COMMAND_LEN);
	posts_t *posts = (posts_t *)malloc(sizeof(posts_t));
	posts->size = 0;
	posts->capacity = 1;
	posts->posts = (post_t **)malloc(sizeof(post_t *));

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, posts);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, posts);
		#endif
	}

	lg_free(graph);
	free_posts(&posts);


	free_users();
	free(input);
	fclose(stdin);
	fclose(stdout);
	return 0;
}
