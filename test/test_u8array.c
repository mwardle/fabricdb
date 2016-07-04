#include "test_common.h"

void test_u8array_set_size() {
    u8array arr;

    fdb_assert("Started with unclean memory", fabricdb_mem_used() == 0);

    fdb_assert("Failed to set size", u8array_set_size(&arr, 1) == FABRICDB_OK);
    fdb_assert("Size not set", arr.size == 1);
    fdb_assert("Count is wrong", arr.count == 0);
    fdb_assert("Invalid ptr size", fabricdb_mem_size(arr.data) == sizeof(uint8_t));

    fdb_assert("Failed to set size", u8array_set_size(&arr, 5) == FABRICDB_OK);
    fdb_assert("Size not set", arr.size == 5);
    fdb_assert("Count is wrong", arr.count == 0);
    fdb_assert("Invalid ptr size", fabricdb_mem_size(arr.data) == sizeof(uint8_t) * 5);

    fdb_assert("Could not push value", u8array_push(&arr, 1) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 2) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 4) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 8) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 16) == FABRICDB_OK);

    fdb_assert("Size grew", arr.size == 5);
    fdb_assert("Count was not set", arr.count == 5);
    fdb_assert("Ptr size is wrong", fabricdb_mem_size(arr.data) == sizeof(uint8_t) * 5);

    fdb_assert("Values were not set", u8array_get_or(&arr, 0, 0) == 1);
    fdb_assert("Values were not set", u8array_get_or(&arr, 1, 0) == 2);
    fdb_assert("Values were not set", u8array_get_or(&arr, 2, 0) == 4);
    fdb_assert("Values were not set", u8array_get_or(&arr, 3, 0) == 8);
    fdb_assert("Values were not set", u8array_get_or(&arr, 4, 0) == 16);

    fdb_assert("Could not push value", u8array_push(&arr, 0) == FABRICDB_OK);

    fdb_assert("Size did not grow", arr.size > 5);
    fdb_assert("Count was not set", arr.count == 6);
    fdb_assert("Ptr size is wrong", fabricdb_mem_size(arr.data) > sizeof(uint8_t) * 5);

    fdb_assert("Values were not set", u8array_get_or(&arr, 0, 0) == 1);
    fdb_assert("Values were not set", u8array_get_or(&arr, 1, 0) == 2);
    fdb_assert("Values were not set", u8array_get_or(&arr, 2, 0) == 4);
    fdb_assert("Values were not set", u8array_get_or(&arr, 3, 0) == 8);
    fdb_assert("Values were not set", u8array_get_or(&arr, 4, 0) == 16);
    fdb_assert("Values were not set", u8array_get_or(&arr, 5, 1) == 0);


    fdb_assert("Failed to set size", u8array_set_size(&arr, 3) == FABRICDB_OK);
    fdb_assert("Size not set", arr.size == 3);
    fdb_assert("Count is wrong", arr.count == 3);
    fdb_assert("Ptr size is wrong", fabricdb_mem_size(arr.data) == sizeof(uint8_t) * 3);

    fdb_assert("Values were not copied", u8array_get_or(&arr, 0, 0) == 1);
    fdb_assert("Values were not copied", u8array_get_or(&arr, 1, 0) == 2);
    fdb_assert("Values were not copied", u8array_get_or(&arr, 2, 0) == 4);
    fdb_assert("Out of bounds returned old value", u8array_get_or(&arr, 3, 0) == 0);

    u8array_deinit(&arr);

    fdb_assert("Did not clean up all the memory", fabricdb_mem_used() == 0);
    fdb_passed;
}

void test_u8array_has() {
    u8array arr;
    fdb_assert("Started with unclean memory", fabricdb_mem_used() == 0);

    fdb_assert("Size not initialized to 0", arr.size == 0);
    fdb_assert("Count not initialized to 0", arr.count == 0);
    fdb_assert("Data not initialized to null", arr.data == NULL);

    fdb_assert("Has zero but shouldn't", u8array_has(&arr, 0) == 0);

    fdb_assert("Could not add a value", u8array_set(&arr, 0, 100) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 1, 101) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 2, 102) == FABRICDB_OK);

    fdb_assert("Does not have 0 index", u8array_has(&arr, 0) == 1);
    fdb_assert("Does not have 1 index", u8array_has(&arr, 1) == 1);
    fdb_assert("Does not have 2 index", u8array_has(&arr, 2) == 1);
    fdb_assert("Has 3 index, but should not", u8array_has(&arr, 3) == 0);

    u8array_deinit(&arr);

    fdb_assert("Did not clean up all the memory", fabricdb_mem_used() == 0);
    fdb_passed;
}

void test_u8array_get_or() {
    u8array arr;
    fdb_assert("Started with unclean memory", fabricdb_mem_used() == 0);

    fdb_assert("Size not initialized to 0", arr.size == 0);
    fdb_assert("Count not initialized to 0", arr.count == 0);
    fdb_assert("Data not initialized to null", arr.data == NULL);

    fdb_assert("Has zero but shouldn't", u8array_get_or(&arr, 0, 1) == 1);

    fdb_assert("Could not add a value", u8array_set(&arr, 0, 100) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 1, 101) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 2, 102) == FABRICDB_OK);

    fdb_assert("Does not have 0 index", u8array_get_or(&arr, 0, 12) == 100);
    fdb_assert("Does not have 1 index", u8array_get_or(&arr, 1, 12) == 101);
    fdb_assert("Does not have 2 index", u8array_get_or(&arr, 2, 12) == 102);
    fdb_assert("Has 3 index, but should not", u8array_get_or(&arr, 3, 12) == 12);

    u8array_deinit(&arr);

    fdb_assert("Did not clean up all the memory", fabricdb_mem_used() == 0);
    fdb_passed;
}

void test_u8array_get_ref() {
    u8array arr;
    uint8_t* v;
    fdb_assert("Started with unclean memory", fabricdb_mem_used() == 0);

    fdb_assert("Size not initialized to 0", arr.size == 0);
    fdb_assert("Count not initialized to 0", arr.count == 0);
    fdb_assert("Data not initialized to null", arr.data == NULL);

    fdb_assert("Has zero but shouldn't", u8array_get_ref(&arr, 0) == NULL);

    fdb_assert("Could not add a value", u8array_set(&arr, 0, 100) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 1, 101) == FABRICDB_OK);
    fdb_assert("Could not add a value", u8array_set(&arr, 2, 102) == FABRICDB_OK);

    v = u8array_get_ref(&arr, 0);
    fdb_assert("Does not have 0 index", *v == 100);
    v = u8array_get_ref(&arr, 1);
    fdb_assert("Does not have 1 index", *v == 101);
    v = u8array_get_ref(&arr, 2);
    fdb_assert("Does not have 2 index", *v == 102);
    *v = 13;
    fdb_assert("Does not have 2 index", u8array_get_or(&arr, 2, 0) == 13);
    v = u8array_get_ref(&arr, 3);
    fdb_assert("Has 3 index, but should not", v == NULL);

    u8array_deinit(&arr);

    fdb_assert("Did not clean up all the memory", fabricdb_mem_used() == 0);
    fdb_passed;
}

void test_u8array_pop_or() {
    u8array arr;
    fdb_assert("Started with unclean memory", fabricdb_mem_used() == 0);

    fdb_assert("Could not push value", u8array_push(&arr, 1) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 2) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 4) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 8) == FABRICDB_OK);
    fdb_assert("Could not push value", u8array_push(&arr, 16) == FABRICDB_OK);

    fdb_assert("Count was not set", arr.count == 5);

    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 16);
    fdb_assert("Count not updated", arr.count == 4);
    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 8);
    fdb_assert("Count not updated", arr.count == 3);
    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 4);
    fdb_assert("Count not updated", arr.count == 2);
    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 2);
    fdb_assert("Count not updated", arr.count == 1);
    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 1);
    fdb_assert("Count not updated", arr.count == 0);
    fdb_assert("Did not pop value", u8array_pop_or(&arr, 0) == 0);
    fdb_assert("Count not updated", arr.count == 0);

    u8array_deinit(&arr);
    fdb_assert("Did not clean up all the memory", fabricdb_mem_used() == 0);
    fdb_passed;

}

void test_u8array() {
    fdb_runtest("u8array set size", test_u8array_set_size);
    fdb_runtest("u8array has", test_u8array_has);
    fdb_runtest("u8array get or", test_u8array_get_or);
    fdb_runtest("u8array get ref", test_u8array_get_ref);
    fdb_runtest("u8array pop or", test_u8array_pop_or);
}