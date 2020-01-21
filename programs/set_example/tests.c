#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTSet.h"


void strings() {
	char* s1 = "FOO";
	char* s2 = "BAR";

	Set set = set_create((CompareFunc)strcmp);
	set_insert(set, s1);
	set_insert(set, s2);

	TEST_CHECK(set_size(set) == 2);

	char* value1 = set_get(set, "FOO");
	char* value2 = set_get(set, "BAR");

	TEST_ASSERT(s1 == value1);
	TEST_ASSERT(s2 == value2);

	set_destroy(set);
}

int compare_ints(int* a, int* b) {
	return *a - *b;
}

void integers() {
	int a1 = 5;
	int a2 = 42;

	Set set = set_create((CompareFunc)compare_ints);

	// ΠΡΟΣΟΧΗ: προσθέτουμε στο set pointers προς τοπικές μεταβλητές! Αυτό είναι ok αν το set
	// χρησιμοποιείται ΜΟΝΟ όσο οι μεταβλητές βρίσκονται στο scope! (δλαδή μέχρι το τέλος της κλήσης της συνάρτησης)
	set_insert(set, &a1);
	set_insert(set, &a2);

	TEST_CHECK(set_size(set) == 2);

	int b1 = 5;
	int b2 = 42;
	int* value1 = set_get(set, &b1);
	int* value2 = set_get(set, &b2);

	TEST_ASSERT(value1 == &a1);
	TEST_ASSERT(value2 == &a2);

	set_destroy(set);
}

void integers_loop() {
	Set set = set_create((CompareFunc)compare_ints);

	// Για να αποθηκεύσουμε 100 διαφορετικούς ακεραίους
	// πρέπει κάθε φορά να δημιουργήσουμε έναν νέο ακέραιο.
	int i;
	for(i = 0; i < 100; i++) {
		int *p = malloc(sizeof(int));
		*p = i;						// αντιγραφή του i στον νέο ακέραιο
		set_insert(set, p);
	}

	// set_min and set_next
	i = 0;
	for(int* value = set_min(set); value != NULL; value = set_next(set, value)) {
		TEST_ASSERT(*value == i++);
	}

	// set_max and set_previous
	i = 99;
	for(int* value = set_max(set); value != NULL; value = set_previous(set, value)) {
		TEST_ASSERT(*value == i--);
	}

	// get, remove, free
	for(i = 0; i < 100; i++) {
		int* value = set_get(set, &i);
		set_remove(set, &i);
		TEST_ASSERT(*value == i);
		free(value);
	}

	set_destroy(set);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "strings", strings },
	{ "integers", integers },
	{ "integers_loop", integers_loop },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};