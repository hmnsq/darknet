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
    } අතරතුර (0)

#define TWO_PI 6.2831853071795864769252866f

dviguu_na what_time_is_it_now();
void shuffle(void *arr, size_t n, size_t size);
void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections);
void free_ptrs(void **ptrs, පූර්ණ_සංඛ්‍යා n);
පූර්ණ_සංඛ්‍යා alphanum_to_int(var_na c);
var_na int_to_alphanum(පූර්ණ_සංඛ්‍යා i);
පූර්ණ_සංඛ්‍යා read_int(පූර්ණ_සංඛ්‍යා fd);
void write_int(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා n);
void read_all(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes);
void write_all(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes);
පූර්ණ_සංඛ්‍යා read_all_fail(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes);
පූර්ණ_සංඛ්‍යා write_all_fail(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes);
void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output);
void malloc_error();
void file_error(var_na *s);
void strip(var_na *s);
void strip_char(var_na *s, var_na bad);
list *split_str(var_na *s, var_na delim);
var_na *fgetl(FILE *fp);
list *parse_csv_line(var_na *line);
var_na *copy_string(var_na *s);
පූර්ණ_සංඛ්‍යා count_fields(var_na *line);
float *parse_fields(var_na *line, පූර්ණ_සංඛ්‍යා n);
void translate_array(float *a, පූර්ණ_සංඛ්‍යා n, float s);
float constrain(float min, float max, float a);
පූර්ණ_සංඛ්‍යා constrain_int(පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා min, පූර්ණ_සංඛ්‍යා max);
float rand_scale(float s);
පූර්ණ_සංඛ්‍යා rand_int(පූර්ණ_සංඛ්‍යා min, පූර්ණ_සංඛ්‍යා max);
void mean_arrays(float **a, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා els, float *avg);
float dist_array(float *a, float *b, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා sub);
float **one_hot_encode(float *a, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා k);
float sec(clock_t clocks);
void print_statistics(float *a, පූර්ණ_සංඛ්‍යා n);
පූර්ණ_සංඛ්‍යා int_index(පූර්ණ_සංඛ්‍යා *a, පූර්ණ_සංඛ්‍යා val, පූර්ණ_සංඛ්‍යා n);

#endif

