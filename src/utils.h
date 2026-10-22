#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <time.h>
#include "darknet.h"
#include "list.h"

#define TIME(a) \
    do { \
    double start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } trong_khi (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, số_nguyên n);
số_nguyên alphanum_to_int(char c);
char int_to_alphanum(số_nguyên i);
số_nguyên read_int(số_nguyên fd);
void write_int(số_nguyên fd, số_nguyên n);
void read_all(số_nguyên fd, char *buffer, size_t bytes);
void write_all(số_nguyên fd, char *buffer, size_t bytes);
số_nguyên read_all_fail(số_nguyên fd, char *buffer, size_t bytes);
số_nguyên write_all_fail(số_nguyên fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
số_nguyên count_fields(char *line);
float *parse_fields(char *line, số_nguyên n);
void translate_array(float *a, số_nguyên n, float s);
float constrain(float min, float max, float a);
số_nguyên constrain_int(số_nguyên a, số_nguyên min, số_nguyên max);
float rand_scale(float s);
số_nguyên rand_int(số_nguyên min, số_nguyên max);
void mean_arrays(float **a, số_nguyên n, số_nguyên els, float *avg);
float dist_array(float *a, float *b, số_nguyên n, số_nguyên sub);
float **one_hot_encode(float *a, số_nguyên n, số_nguyên k);
float sec(clock_t clocks);
void print_statistics(float *a, số_nguyên n);
số_nguyên int_index(số_nguyên *a, số_nguyên val, số_nguyên n);

#endif

