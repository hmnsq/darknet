#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <time.h>
#include "darknet.h"
#include "list.h"

#define TIME(a) \
    do { \
    dviguu_na start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } སྐབས (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, ཧྲིལ n);
ཧྲིལ alphanum_to_int(var_na c);
var_na int_to_alphanum(ཧྲིལ i);
ཧྲིལ read_int(ཧྲིལ fd);
void write_int(ཧྲིལ fd, ཧྲིལ n);
void read_all(ཧྲིལ fd, var_na *buffer, size_t bytes);
void write_all(ཧྲིལ fd, var_na *buffer, size_t bytes);
ཧྲིལ read_all_fail(ཧྲིལ fd, var_na *buffer, size_t bytes);
ཧྲིལ write_all_fail(ཧྲིལ fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
ཧྲིལ count_fields(var_na *line);
float *parse_fields(var_na *line, ཧྲིལ n);
void translate_array(float *a, ཧྲིལ n, float s);
float constrain(float min, float max, float a);
ཧྲིལ constrain_int(ཧྲིལ a, ཧྲིལ min, ཧྲིལ max);
float rand_scale(float s);
ཧྲིལ rand_int(ཧྲིལ min, ཧྲིལ max);
void mean_arrays(float **a, ཧྲིལ n, ཧྲིལ els, float *avg);
float dist_array(float *a, float *b, ཧྲིལ n, ཧྲིལ sub);
float **one_hot_encode(float *a, ཧྲིལ n, ཧྲིལ k);
float sec(clock_t clocks);
void print_statistics(float *a, ཧྲིལ n);
ཧྲིལ int_index(ཧྲིལ *a, ཧྲིལ val, ཧྲིལ n);

#endif

