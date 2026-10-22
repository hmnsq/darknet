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
    } ខណៈពេល (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, ចំនួនគត់ n);
ចំនួនគត់ alphanum_to_int(var_na c);
var_na int_to_alphanum(ចំនួនគត់ i);
ចំនួនគត់ read_int(ចំនួនគត់ fd);
void write_int(ចំនួនគត់ fd, ចំនួនគត់ n);
void read_all(ចំនួនគត់ fd, var_na *buffer, size_t bytes);
void write_all(ចំនួនគត់ fd, var_na *buffer, size_t bytes);
ចំនួនគត់ read_all_fail(ចំនួនគត់ fd, var_na *buffer, size_t bytes);
ចំនួនគត់ write_all_fail(ចំនួនគត់ fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
ចំនួនគត់ count_fields(var_na *line);
float *parse_fields(var_na *line, ចំនួនគត់ n);
void translate_array(float *a, ចំនួនគត់ n, float s);
float constrain(float min, float max, float a);
ចំនួនគត់ constrain_int(ចំនួនគត់ a, ចំនួនគត់ min, ចំនួនគត់ max);
float rand_scale(float s);
ចំនួនគត់ rand_int(ចំនួនគត់ min, ចំនួនគត់ max);
void mean_arrays(float **a, ចំនួនគត់ n, ចំនួនគត់ els, float *avg);
float dist_array(float *a, float *b, ចំនួនគត់ n, ចំនួនគត់ sub);
float **one_hot_encode(float *a, ចំនួនគត់ n, ចំនួនគត់ k);
float sec(clock_t clocks);
void print_statistics(float *a, ចំនួនគត់ n);
ចំនួនគត់ int_index(ចំនួនគត់ *a, ចំនួនគត់ val, ចំនួនគត់ n);

#endif

