// key offset
// element size
// key type (int, double...)




#include <malloc.h>
#include <string.h>


void qsort_points (void *a, int p, int r, int key_offset, int element_size, double THIS_IS_THE_TYPE);
int qs_partition (void *a, int p, int r, int key_offset, int element_size, double THIS_IS_THE_TYPE);
void exchange_element (void *i, void *j, int element_size);




// use quicksort to sort array a
void qsort_points (void *a, int p, int r, int key_offset, int element_size, double THIS_IS_THE_TYPE) {
	if (p < r) {
		int q = qs_partition (a, p, r, key_offset, element_size, THIS_IS_THE_TYPE);

		qsort_points (a, p, q - 1, key_offset, element_size, THIS_IS_THE_TYPE);
		qsort_points (a, q + 1, r, key_offset, element_size, THIS_IS_THE_TYPE);
	}
}


// partition function for quicksort
int qs_partition (void *a, int p, int r, int key_offset, int element_size, double THIS_IS_THE_TYPE) {
	double v = (double)*(a + (r * element_size) + key_offset); // load the pivot key
	int i = p - 1;

	for (int j = p; j < r; j++) {
		if ((double)*(a + (j * element_size) + key_offset) <= v) {
			i++;
			exchange_element (a + (i * element_size), a + (j * element_size), element_size);
		}
	}

	exchange_element (&a[i + 1], &a[r]);

	return i + 1;
}


// exchange 2 double elements
void exchange_element (void *i, void *j, int element_size) {
	void *temp = (void *)malloc (element_size);
	
	memcpy (temp, i, element_size);
	memcpy (i, j, element_size);
	memcpy (j, temp, element_size);
	
	free (temp);
}
