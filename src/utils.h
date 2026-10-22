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
    } поки (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, ціле n);
ціле alphanum_to_int(char c);
char int_to_alphanum(ціле i);
ціле read_int(ціле fd);
void write_int(ціле fd, ціле n);
void read_all(ціле fd, char *buffer, size_t bytes);
void write_all(ціле fd, char *buffer, size_t bytes);
ціле read_all_fail(ціле fd, char *buffer, size_t bytes);
ціле write_all_fail(ціле fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
ціле count_fields(char *line);
float *parse_fields(char *line, ціле n);
void translate_array(float *a, ціле n, float s);
float constrain(float min, float max, float a);
ціле constrain_int(ціле a, ціле min, ціле max);
float rand_scale(float s);
ціле rand_int(ціле min, ціле max);
void mean_arrays(float **a, ціле n, ціле els, float *avg);
float dist_array(float *a, float *b, ціле n, ціле sub);
float **one_hot_encode(float *a, ціле n, ціле k);
float sec(clock_t clocks);
void print_statistics(float *a, ціле n);
ціле int_index(ціле *a, ціле val, ціле n);

#endif

