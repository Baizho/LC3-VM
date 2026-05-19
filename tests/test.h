#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT_EQ_U16(expected, actual)                                                    \
    do {                                                                                   \
        uint16_t expected_value = (uint16_t)(expected);                                    \
        uint16_t actual_value = (uint16_t)(actual);                                        \
        if (expected_value != actual_value) {                                              \
            fprintf(stderr, "%s:%d expected 0x%04X, got 0x%04X\n",                        \
                    __FILE__, __LINE__, expected_value, actual_value);                     \
            return EXIT_FAILURE;                                                          \
        }                                                                                  \
    } while (0)

#define ASSERT_SUCCESS(expr)                                                               \
    do {                                                                                   \
        if ((expr) != EXIT_SUCCESS) {                                                      \
            fprintf(stderr, "%s:%d expected success\n", __FILE__, __LINE__);              \
            return EXIT_FAILURE;                                                          \
        }                                                                                  \
    } while (0)

#endif // TEST_H
