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
    } пока (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, целое n);
целое alphanum_to_int(char c);
char int_to_alphanum(целое i);
целое read_int(целое fd);
void write_int(целое fd, целое n);
void read_all(целое fd, char *buffer, size_t bytes);
void write_all(целое fd, char *buffer, size_t bytes);
целое read_all_fail(целое fd, char *buffer, size_t bytes);
целое write_all_fail(целое fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
целое count_fields(char *line);
float *parse_fields(char *line, целое n);
void translate_array(float *a, целое n, float s);
float constrain(float min, float max, float a);
целое constrain_int(целое a, целое min, целое max);
float rand_scale(float s);
целое rand_int(целое min, целое max);
void mean_arrays(float **a, целое n, целое els, float *avg);
float dist_array(float *a, float *b, целое n, целое sub);
float **one_hot_encode(float *a, целое n, целое k);
float sec(clock_t clocks);
void print_statistics(float *a, целое n);
целое int_index(целое *a, целое val, целое n);

#endif

