/*
 *  Copyright 2015 Lars Wander (lars.wander@gmail.com)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */ 

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

static char consonants[] = "bcdfghjklmnpqrstvwxyz";
static char vowels[] = "aeiou";

#define NUM_CONSONANTS (sizeof(consonants) - 1)
#define NUM_VOWELS (sizeof(vowels) - 1)
#define YIN_LEN (11)
#define YIN_MASK ((1 << (YIN_LEN)) - 1)

/**
 * @brief yins are numbers with 11 bit digits, each digit being a vowel 
 *        sandwiched between two consonants.
 */
typedef struct _yin {
    /* Highest order sub-digit -- consonant */
    char y;
    /* Vowel */
    char i;
    /* Lowest order sub-digit -- consonant */
    char n;
    /* Pointer to next, lower order, digit */
    struct _yin *next;
} yin;

/**
 * @brief Create a yin digit from an unsigned integer.
 *
 * @param i The integer to be converted. Must be less than 2 ^ 11
 *
 * @return A pointer to the yin digit if successful, otherwise NULL
 */
yin *i2yin_digit(unsigned int i) {
    if (i > YIN_MASK) {
        return NULL;
    }

    yin *res = malloc(sizeof(yin));
    res->next = NULL;

    res->n = i % NUM_CONSONANTS;
    i /= NUM_CONSONANTS;

    res->i = i % NUM_VOWELS;
    i /= NUM_VOWELS;

    res->y = i % NUM_CONSONANTS;

    assert(i / NUM_CONSONANTS == 0);
    return res;
}

/**
 * @brief Create an unsigned integer from a yin digit.
 *
 * @param yd The yin digit to be converted. May not be NULL.
 *
 * @return The integer form i, (0 <= i < 1 << 11), of the yin digit. 
 */
unsigned int yin_digit2i(yin *yd) {
    unsigned int res = yd->y;
    res *= NUM_VOWELS;

    res += yd->i;
    res *= NUM_CONSONANTS;

    res += yd->n;
    return res;
}

/**
 * @brief Print a single yin digit
 *
 * @param yd The yin digit to be printed
 */
void print_yin_digit(yin *yd) {
    printf("%c%c%c", consonants[yd->y], vowels[yd->i], consonants[yd->n]);
}

/**
 * @brief Print a full yin number
 *
 * I opt for iteration over recursion for the sake of a small stack size.
 *
 * @param yd the yin number to be printed
 */
void print_yin_number(yin *yd) {
   while (yd != NULL) {
       print_yin_digit(yd);
       if (yd->next != NULL)
           printf(".");
       yd = yd->next;
   }
   printf("\n");
}

void sanity_check(unsigned int i) {
    printf("i == %u <--> yd == ", i);
    yin *yd = i2yin_digit(i);
    print_yin_digit(yd);
    printf("\n");
    assert(i == yin_digit2i(yd));
    free(yd);
}

/**
 * @brief Free a full yin number
 *
 * @param yd The yin number to be freed
 */
void free_yin_number(yin *yd) {
    yin *next;
    while (yd != NULL) {
        next = yd->next;
        free(yd);
        yd = next;
    }
}

int main(int argc, char *argv[]) {
    size_t size = 0;
    int len;
    char *buf = NULL;
    while ((len = getline(&buf, &size, stdin)) >= 0) {
        unsigned int total = 0;
        yin *yd = NULL;
        for (int i = 0; i < len; i++) {
            if (buf[i] == '\n')
                continue;

            int digit = (int)(buf[i] - '0');
            if (digit < 0 || digit > 10) {
                fprintf(stderr, "Error, %c is not a digit\n", buf[i]);
                exit(-3);
            }

            total *= 10;
            total += (unsigned int)digit;

            /* Have we accumulated more than 11 bits of information? */
            if (total > YIN_MASK) {
                yin *old_yd = yd;
                yd = i2yin_digit(total & YIN_MASK);
                yd->next = old_yd;
                total >>= YIN_LEN;
            }
        }

        yin *old_yd = yd;
        yd = i2yin_digit(total & YIN_MASK);
        yd->next = old_yd;

        print_yin_number(yd);

        free_yin_number(yd);
        free(buf);
        yd = NULL;
        buf = NULL;
        size = 0;
    }

    return 0;
}
