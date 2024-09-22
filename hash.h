/*
 * Copyright (c) 2024, Andrei Rusanescu <andreirusanescu154gmail.com>
 */

#ifndef HASH_H
#define HASH_H

#include "graph.h"

#define MAX_STRING_SIZE	256
#define HMAX 100

typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;

	/* Total number of buckets */
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);

	/* Pointer to function that frees a pair (k, v) */
	void (*key_val_free_function)(void*);
	void (*copy_func)(void **, void *, unsigned int);
};

// Comparing functions for keys
int compare_function_ints(void *a, void *b);
int compare_function_strings(void *a, void *b);

/* Frees (info *) data, i.e. key and value and
 * the data itself
 */
void key_val_free_function(void *data);

// creates a hashtable;
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*),
		void (*copy_func)(void **, void*, unsigned int));

// checks if the ht has key
int ht_has_key(hashtable_t *ht, void *key);

// returns a pointer to the value associated with the entry key
void *ht_get(hashtable_t *ht, void *key);

// loads / updates a field in the ht
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

// frees a field from the ht based on the key
void ht_remove_entry(hashtable_t *ht, void *key);

// frees ht
void ht_free(hashtable_t *ht);

unsigned int ht_get_size(hashtable_t *ht);
unsigned int ht_get_hmax(hashtable_t *ht);

// assigns a type node from src to *dst
void node_copy(void **dst, void *src, unsigned int src_size);

// allocs src_size bytes and copies with memcpy from src to dst
void simple_copy(void **dst, void *src, unsigned int src_size);

#endif /* HASH_H */
