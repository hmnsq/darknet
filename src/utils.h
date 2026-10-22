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
    } ขณะ (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, จำนวนเต็ม n);
จำนวนเต็ม alphanum_to_int(var_na c);
var_na int_to_alphanum(จำนวนเต็ม i);
จำนวนเต็ม read_int(จำนวนเต็ม fd);
void write_int(จำนวนเต็ม fd, จำนวนเต็ม n);
void read_all(จำนวนเต็ม fd, var_na *buffer, size_t bytes);
void write_all(จำนวนเต็ม fd, var_na *buffer, size_t bytes);
จำนวนเต็ม read_all_fail(จำนวนเต็ม fd, var_na *buffer, size_t bytes);
จำนวนเต็ม write_all_fail(จำนวนเต็ม fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
จำนวนเต็ม count_fields(var_na *line);
float *parse_fields(var_na *line, จำนวนเต็ม n);
void translate_array(float *a, จำนวนเต็ม n, float s);
float constrain(float min, float max, float a);
จำนวนเต็ม constrain_int(จำนวนเต็ม a, จำนวนเต็ม min, จำนวนเต็ม max);
float rand_scale(float s);
จำนวนเต็ม rand_int(จำนวนเต็ม min, จำนวนเต็ม max);
void mean_arrays(float **a, จำนวนเต็ม n, จำนวนเต็ม els, float *avg);
float dist_array(float *a, float *b, จำนวนเต็ม n, จำนวนเต็ม sub);
float **one_hot_encode(float *a, จำนวนเต็ม n, จำนวนเต็ม k);
float sec(clock_t clocks);
void print_statistics(float *a, จำนวนเต็ม n);
จำนวนเต็ม int_index(จำนวนเต็ม *a, จำนวนเต็ม val, จำนวนเต็ม n);

#endif

