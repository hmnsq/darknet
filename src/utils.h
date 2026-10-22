#ifndef UTILS_H
#define UTILS_H
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "list.h"

#define TIME(a) \
    কুরু { \
    দ্বিগুণ start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } যাবত্ (0)

#define TWO_PI 6.2831853071795864769252866f

দ্বিগুণ what_time_is_it_now();
শূন্য shuffle(শূন্য *arr, size_t n, size_t size);
শূন্য sorta_shuffle(শূন্য *arr, size_t n, size_t size, size_t sections);
শূন্য free_ptrs(শূন্য **ptrs, পূর্ণাঙ্ক n);
পূর্ণাঙ্ক alphanum_to_int(বর্ণ c);
বর্ণ int_to_alphanum(পূর্ণাঙ্ক i);
পূর্ণাঙ্ক read_int(পূর্ণাঙ্ক fd);
শূন্য write_int(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক n);
শূন্য read_all(পূর্ণাঙ্ক fd, বর্ণ *buffer, size_t bytes);
শূন্য write_all(পূর্ণাঙ্ক fd, বর্ণ *buffer, size_t bytes);
পূর্ণাঙ্ক read_all_fail(পূর্ণাঙ্ক fd, বর্ণ *buffer, size_t bytes);
পূর্ণাঙ্ক write_all_fail(পূর্ণাঙ্ক fd, বর্ণ *buffer, size_t bytes);
শূন্য find_replace(বর্ণ *str, বর্ণ *orig, বর্ণ *rep, বর্ণ *output);
শূন্য malloc_error();
শূন্য file_error(বর্ণ *s);
শূন্য strip(বর্ণ *s);
শূন্য strip_char(বর্ণ *s, বর্ণ bad);
list *split_str(বর্ণ *s, বর্ণ delim);
বর্ণ *fgetl(FILE *fp);
list *parse_csv_সারি(বর্ণ *সারি);
বর্ণ *copy_string(বর্ণ *s);
পূর্ণাঙ্ক গুনতি_fields(বর্ণ *সারি);
ভিন্ন *parse_fields(বর্ণ *সারি, পূর্ণাঙ্ক n);
শূন্য translate_array(ভিন্ন *a, পূর্ণাঙ্ক n, ভিন্ন s);
ভিন্ন constrain(ভিন্ন min, ভিন্ন max, ভিন্ন a);
পূর্ণাঙ্ক constrain_int(পূর্ণাঙ্ক a, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max);
ভিন্ন rand_scale(ভিন্ন s);
পূর্ণাঙ্ক rand_int(পূর্ণাঙ্ক min, পূর্ণাঙ্ক max);
শূন্য mean_arrays(ভিন্ন **a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক els, ভিন্ন *avg);
ভিন্ন dist_array(ভিন্ন *a, ভিন্ন *b, পূর্ণাঙ্ক n, পূর্ণাঙ্ক sub);
ভিন্ন **one_hot_encode(ভিন্ন *a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k);
ভিন্ন sec(clock_t clocks);
শূন্য print_statistics(ভিন্ন *a, পূর্ণাঙ্ক n);
পূর্ণাঙ্ক int_index(পূর্ণাঙ্ক *a, পূর্ণাঙ্ক val, পূর্ণাঙ্ক n);

#endif

