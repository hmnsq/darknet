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
    } док (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, цео n);
цео alphanum_to_int(char c);
char int_to_alphanum(цео i);
цео read_int(цео fd);
void write_int(цео fd, цео n);
void read_all(цео fd, char *buffer, size_t bytes);
void write_all(цео fd, char *buffer, size_t bytes);
цео read_all_fail(цео fd, char *buffer, size_t bytes);
цео write_all_fail(цео fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
цео count_fields(char *line);
float *parse_fields(char *line, цео n);
void translate_array(float *a, цео n, float s);
float constrain(float min, float max, float a);
цео constrain_int(цео a, цео min, цео max);
float rand_scale(float s);
цео rand_int(цео min, цео max);
void mean_arrays(float **a, цео n, цео els, float *avg);
float dist_array(float *a, float *b, цео n, цео sub);
float **one_hot_encode(float *a, цео n, цео k);
float sec(clock_t clocks);
void print_statistics(float *a, цео n);
цео int_index(цео *a, цео val, цео n);

#endif

