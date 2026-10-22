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
    } ᨀᨛ (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, puur_naa_nka n);
puur_naa_nka alphanum_to_int(var_na c);
var_na int_to_alphanum(puur_naa_nka i);
puur_naa_nka read_int(puur_naa_nka fd);
void write_int(puur_naa_nka fd, puur_naa_nka n);
void read_all(puur_naa_nka fd, var_na *buffer, size_t bytes);
void write_all(puur_naa_nka fd, var_na *buffer, size_t bytes);
puur_naa_nka read_all_fail(puur_naa_nka fd, var_na *buffer, size_t bytes);
puur_naa_nka write_all_fail(puur_naa_nka fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
puur_naa_nka count_fields(var_na *line);
float *parse_fields(var_na *line, puur_naa_nka n);
void translate_array(float *a, puur_naa_nka n, float s);
float constrain(float min, float max, float a);
puur_naa_nka constrain_int(puur_naa_nka a, puur_naa_nka min, puur_naa_nka max);
float rand_scale(float s);
puur_naa_nka rand_int(puur_naa_nka min, puur_naa_nka max);
void mean_arrays(float **a, puur_naa_nka n, puur_naa_nka els, float *avg);
float dist_array(float *a, float *b, puur_naa_nka n, puur_naa_nka sub);
float **one_hot_encode(float *a, puur_naa_nka n, puur_naa_nka k);
float sec(clock_t clocks);
void print_statistics(float *a, puur_naa_nka n);
puur_naa_nka int_index(puur_naa_nka *a, puur_naa_nka val, puur_naa_nka n);

#endif

