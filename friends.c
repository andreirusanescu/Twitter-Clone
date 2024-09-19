#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

void add_friend(list_graph_t *graph) {
	char *name1 = strtok(NULL, " \n");
	char *name2 = strtok(NULL, " \n");
	int id1 = get_user_id(name1), id2 = get_user_id(name2);
	lg_add_edge(graph, id1, id2);
	lg_add_edge(graph, id2, id1);
	printf("Added connection %s - %s\n", name1, name2);
}

void remove_friend(list_graph_t *graph) {
	char *name1 = strtok(NULL, " \n");
	char *name2 = strtok(NULL, " \n");
	int id1 = get_user_id(name1), id2 = get_user_id(name2);
	lg_remove_edge(graph, id1, id2);
	lg_remove_edge(graph, id2, id1);
	printf("Removed connection %s - %s\n", name1, name2);
}

void get_distance(list_graph_t *graph) {
	char *name1 = strtok(NULL, " \n");
	char *name2 = strtok(NULL, " \n");
	int id1 = get_user_id(name1), id2 = get_user_id(name2);
	int distance = print_min_path(graph, id1, id2);
	if (distance)
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
	else
		printf("There is no way to get from %s to %s\n", name1, name2);
}

void suggestions(list_graph_t *graph) {
	char *name = strtok(NULL, " \n");
	int id = get_user_id(name), id2, id3;
	ll_node_t *nei = graph->neighbors[id]->head;
	ll_node_t *nei_of_nei;
	char ok = 0;
	int visited[MAX_PEOPLE] = {0};
	while (nei) {
		id2 = *((int *)nei->data);
		nei_of_nei = graph->neighbors[id2]->head;
		while (nei_of_nei) {
			id3 = *((int *)nei_of_nei->data);
			if (!lg_has_edge(graph, id, id3) && id3 != id && !visited[id3]) {
				ok = 1;
				visited[id3] = 1;
			}
			nei_of_nei = nei_of_nei->next;
		}
		nei = nei->next;
	}

	if (!ok) {
		printf("There are no suggestions for %s\n", name);
		return;
	}
	
	printf("Suggestions for %s:\n", name);
	for (int i = 0; i < MAX_PEOPLE; ++i)
		if (visited[i])
			printf("%s\n", get_user_name(i));
}

void common(list_graph_t *graph) {
	char *name1 = strtok(NULL, " \n");
	char *name2 = strtok(NULL, " \n");
	int id1 = get_user_id(name1), id2 = get_user_id(name2);
	int common_nei;
	int visited[MAX_PEOPLE] = {0};
	char ok = 0;
	ll_node_t *nei = graph->neighbors[id1]->head;
	while (nei) {
		common_nei = *((int *)nei->data);
		if (lg_has_edge(graph, common_nei, id2) && common_nei != id2 &&
			!visited[common_nei]) {
			visited[common_nei] = 1;
			ok = 1;
		}
		nei = nei->next;
	}

	if (!ok) {
		printf("No common friends for %s and %s\n", name1, name2);
		return;
	}
	printf("The common friends between %s and %s are:\n", name1, name2);
	for (int i = 0; i < MAX_PEOPLE; ++i)
		if (visited[i])
			printf("%s\n", get_user_name(i));
}

void friends(list_graph_t *graph) {
	char *name = strtok(NULL, " \n");
	int id = get_user_id(name);
	printf("%s has %d friends\n", name, graph->neighbors[id]->size);
}

void popular(list_graph_t *graph) {
	char *name = strtok(NULL, " \n");
	unsigned int id = get_user_id(name), id2;
	unsigned int max_friends = graph->neighbors[id]->size, max_id = id;
	ll_node_t *nei = graph->neighbors[id]->head;
	while (nei) {
		id2 = *((int *)nei->data);
		if (graph->neighbors[id2]->size > max_friends) {
			max_friends = graph->neighbors[id2]->size;
			max_id = id2;
		} else if (graph->neighbors[id2]->size == max_friends && max_id != id) {
			if (max_id > id2)
				max_id = id2;
		}
		nei = nei->next;
	}

	if (max_id != id)
		printf("%s is the most popular friend of %s\n", get_user_name(max_id), get_user_name(id));
	else
		printf("%s is the most popular\n", get_user_name(max_id));
}

void handle_input_friends(char *input, list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add"))
		add_friend(graph);
	else if (!strcmp(cmd, "remove"))
		remove_friend(graph);
	else if (!strcmp(cmd, "suggestions"))
		suggestions(graph);
	else if (!strcmp(cmd, "distance"))
		get_distance(graph);
	else if (!strcmp(cmd, "common"))
		common(graph);
	else if (!strcmp(cmd, "friends"))
		friends(graph);
	else if (!strcmp(cmd, "popular"))
		popular(graph);

	free(commands);
}
