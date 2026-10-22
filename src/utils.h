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
    } 동안 (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, 정수 n);
정수 alphanum_to_int(char c);
char int_to_alphanum(정수 i);
정수 read_int(정수 fd);
void write_int(정수 fd, 정수 n);
void read_all(정수 fd, char *buffer, size_t bytes);
void write_all(정수 fd, char *buffer, size_t bytes);
정수 read_all_fail(정수 fd, char *buffer, size_t bytes);
정수 write_all_fail(정수 fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
정수 count_fields(char *line);
float *parse_fields(char *line, 정수 n);
void translate_array(float *a, 정수 n, float s);
float constrain(float min, float max, float a);
정수 constrain_int(정수 a, 정수 min, 정수 max);
float rand_scale(float s);
정수 rand_int(정수 min, 정수 max);
void mean_arrays(float **a, 정수 n, 정수 els, float *avg);
float dist_array(float *a, float *b, 정수 n, 정수 sub);
float **one_hot_encode(float *a, 정수 n, 정수 k);
float sec(clock_t clocks);
void print_statistics(float *a, 정수 n);
정수 int_index(정수 *a, 정수 val, 정수 n);

#endif

