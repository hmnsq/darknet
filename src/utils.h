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
    } နေစဉ် (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, ကိန်းပြည့် n);
ကိန်းပြည့် alphanum_to_int(var_na c);
var_na int_to_alphanum(ကိန်းပြည့် i);
ကိန်းပြည့် read_int(ကိန်းပြည့် fd);
void write_int(ကိန်းပြည့် fd, ကိန်းပြည့် n);
void read_all(ကိန်းပြည့် fd, var_na *buffer, size_t bytes);
void write_all(ကိန်းပြည့် fd, var_na *buffer, size_t bytes);
ကိန်းပြည့် read_all_fail(ကိန်းပြည့် fd, var_na *buffer, size_t bytes);
ကိန်းပြည့် write_all_fail(ကိန်းပြည့် fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
ကိန်းပြည့် count_fields(var_na *line);
float *parse_fields(var_na *line, ကိန်းပြည့် n);
void translate_array(float *a, ကိန်းပြည့် n, float s);
float constrain(float min, float max, float a);
ကိန်းပြည့် constrain_int(ကိန်းပြည့် a, ကိန်းပြည့် min, ကိန်းပြည့် max);
float rand_scale(float s);
ကိန်းပြည့် rand_int(ကိန်းပြည့် min, ကိန်းပြည့် max);
void mean_arrays(float **a, ကိန်းပြည့် n, ကိန်းပြည့် els, float *avg);
float dist_array(float *a, float *b, ကိန်းပြည့် n, ကိန်းပြည့် sub);
float **one_hot_encode(float *a, ကိန်းပြည့် n, ကိန်းပြည့် k);
float sec(clock_t clocks);
void print_statistics(float *a, ကိန်းပြည့် n);
ကိန်းပြည့် int_index(ကိန်းပြည့် *a, ကိန်းပြည့် val, ကိန်းပြည့် n);

#endif

