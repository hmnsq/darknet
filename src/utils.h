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
    } ながら (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, 整数 n);
整数 alphanum_to_int(char c);
char int_to_alphanum(整数 i);
整数 read_int(整数 fd);
void write_int(整数 fd, 整数 n);
void read_all(整数 fd, char *buffer, size_t bytes);
void write_all(整数 fd, char *buffer, size_t bytes);
整数 read_all_fail(整数 fd, char *buffer, size_t bytes);
整数 write_all_fail(整数 fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
整数 count_fields(char *line);
float *parse_fields(char *line, 整数 n);
void translate_array(float *a, 整数 n, float s);
float constrain(float min, float max, float a);
整数 constrain_int(整数 a, 整数 min, 整数 max);
float rand_scale(float s);
整数 rand_int(整数 min, 整数 max);
void mean_arrays(float **a, 整数 n, 整数 els, float *avg);
float dist_array(float *a, float *b, 整数 n, 整数 sub);
float **one_hot_encode(float *a, 整数 n, 整数 k);
float sec(clock_t clocks);
void print_statistics(float *a, 整数 n);
整数 int_index(整数 *a, 整数 val, 整数 n);

#endif

