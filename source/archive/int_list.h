#ifndef INT_LIST_H_
#define INT_LIST_H_

#include <malloc.h>


// typedefs * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typedef struct {
	int size;
	int *data;
} int_list_t;


// prototypes * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int_list_t create_int_list (int size);
void destroy_int_list (int_list_t list);
int is_element_of (int x, int_list_t list);
int is_element_of (int x, int_list_t list, int l, int r);


// functions * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// create a new int_list
int_list_t create_int_list (int size) {
	int_list_t ret;

	ret.size = size;
	ret.data = (int *)malloc (size * sizeof (int));

	return ret;
}

// free the memory used by an int_list
void destroy_int_list (int_list_t list) {
	if (list.data != NULL) free (list.data);
}

// checks to see if an int is in an int_list_t
int is_element_of (int x, int_list_t list) {
	for (int i = 0; i < list.size; i++) {
		if (list.data[i] == x) return 1;
	}

	return 0;
}

// checks to see if an int is in a subarray of int_list_t
int is_element_of (int x, int_list_t list, int l, int r) {
	for (; l <= r; l++) {
		if (list.data[l] == x) return 1;
	}

	return 0;
}


#endif
