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
    } докато (0)

#define TWO_PI 6.2831853071795864769252866f

double what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, цяло n);
цяло alphanum_to_int(char c);
char int_to_alphanum(цяло i);
цяло read_int(цяло fd);
void write_int(цяло fd, цяло n);
void read_all(цяло fd, char *buffer, size_t bytes);
void write_all(цяло fd, char *buffer, size_t bytes);
цяло read_all_fail(цяло fd, char *buffer, size_t bytes);
цяло write_all_fail(цяло fd, char *buffer, size_t bytes);
void find_replace(char *str, char *orig, char *rep, char *output);
void malloc_error();
void file_error(char *s);
void strip(char *s);
void strip_char(char *s, char bad);
list *split_str(char *s, char delim);
char *fgetl(FILE *fp);
list *parse_csv_line(char *line);
char *copy_string(char *s);
цяло count_fields(char *line);
float *parse_fields(char *line, цяло n);
void translate_array(float *a, цяло n, float s);
float constrain(float min, float max, float a);
цяло constrain_int(цяло a, цяло min, цяло max);
float rand_scale(float s);
цяло rand_int(цяло min, цяло max);
void mean_arrays(float **a, цяло n, цяло els, float *avg);
float dist_array(float *a, float *b, цяло n, цяло sub);
float **one_hot_encode(float *a, цяло n, цяло k);
float sec(clock_t clocks);
void print_statistics(float *a, цяло n);
цяло int_index(цяло *a, цяло val, цяло n);

#endif

