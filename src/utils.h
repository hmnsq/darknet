#ifndef UTILS_H
#define UTILS_H
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ "darknet.h"
#அந்தர்பாவ "list.h"

#define TIME(a) \
    செய் { \
    இரட்டை start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } போது (0)

#define TWO_PI 6.2831853071795864769252866f

இரட்டை what_time_is_it_now();
வெறுமை shuffle(வெறுமை *arr, size_t n, size_t size);
வெறுமை sorta_shuffle(வெறுமை *arr, size_t n, size_t size, size_t sections);
வெறுமை free_ptrs(வெறுமை **ptrs, முழு n);
முழு alphanum_to_int(எழுத்து c);
எழுத்து int_to_alphanum(முழு i);
முழு read_int(முழு fd);
வெறுமை write_int(முழு fd, முழு n);
வெறுமை read_all(முழு fd, எழுத்து *buffer, size_t bytes);
வெறுமை write_all(முழு fd, எழுத்து *buffer, size_t bytes);
முழு read_all_fail(முழு fd, எழுத்து *buffer, size_t bytes);
முழு write_all_fail(முழு fd, எழுத்து *buffer, size_t bytes);
வெறுமை find_replace(எழுத்து *str, எழுத்து *orig, எழுத்து *rep, எழுத்து *output);
வெறுமை malloc_error();
வெறுமை file_error(எழுத்து *s);
வெறுமை strip(எழுத்து *s);
வெறுமை strip_char(எழுத்து *s, எழுத்து bad);
list *split_str(எழுத்து *s, எழுத்து delim);
எழுத்து *fgetl(FILE *fp);
list *parse_csv_வரி(எழுத்து *வரி);
எழுத்து *copy_string(எழுத்து *s);
முழு மொத்தம்_fields(எழுத்து *வரி);
தசம *parse_fields(எழுத்து *வரி, முழு n);
வெறுமை translate_array(தசம *a, முழு n, தசம s);
தசம constrain(தசம min, தசம max, தசம a);
முழு constrain_int(முழு a, முழு min, முழு max);
தசம rand_scale(தசம s);
முழு rand_int(முழு min, முழு max);
வெறுமை mean_arrays(தசம **a, முழு n, முழு els, தசம *avg);
தசம dist_array(தசம *a, தசம *b, முழு n, முழு sub);
தசம **one_hot_encode(தசம *a, முழு n, முழு k);
தசம sec(clock_t clocks);
வெறுமை print_statistics(தசம *a, முழு n);
முழு int_index(முழு *a, முழு val, முழு n);

#endif

