#ifndef UTILS_H
#define UTILS_H
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ "darknet.h"
#അന്തര്ഭാവ "list.h"

#define TIME(a) \
    ചെയ്യുക { \
    ഇരട്ട start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } ആകുമ്പോൾ (0)

#define TWO_PI 6.2831853071795864769252866f

ഇരട്ട what_time_is_it_now();
ശൂന്യം shuffle(ശൂന്യം *arr, size_t n, size_t size);
ശൂന്യം sorta_shuffle(ശൂന്യം *arr, size_t n, size_t size, size_t sections);
ശൂന്യം free_ptrs(ശൂന്യം **ptrs, പൂർണ്ണസംഖ്യ n);
പൂർണ്ണസംഖ്യ alphanum_to_int(അക്ഷരം c);
അക്ഷരം int_to_alphanum(പൂർണ്ണസംഖ്യ i);
പൂർണ്ണസംഖ്യ read_int(പൂർണ്ണസംഖ്യ fd);
ശൂന്യം write_int(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ n);
ശൂന്യം read_all(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes);
ശൂന്യം write_all(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes);
പൂർണ്ണസംഖ്യ read_all_fail(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes);
പൂർണ്ണസംഖ്യ write_all_fail(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes);
ശൂന്യം find_replace(അക്ഷരം *str, അക്ഷരം *orig, അക്ഷരം *rep, അക്ഷരം *output);
ശൂന്യം malloc_error();
ശൂന്യം file_error(അക്ഷരം *s);
ശൂന്യം strip(അക്ഷരം *s);
ശൂന്യം strip_char(അക്ഷരം *s, അക്ഷരം bad);
list *split_str(അക്ഷരം *s, അക്ഷരം delim);
അക്ഷരം *fgetl(FILE *fp);
list *parse_csv_വരി(അക്ഷരം *വരി);
അക്ഷരം *copy_string(അക്ഷരം *s);
പൂർണ്ണസംഖ്യ എണ്ണം_fields(അക്ഷരം *വരി);
ദശാംശം *parse_fields(അക്ഷരം *വരി, പൂർണ്ണസംഖ്യ n);
ശൂന്യം translate_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, ദശാംശം s);
ദശാംശം constrain(ദശാംശം min, ദശാംശം max, ദശാംശം a);
പൂർണ്ണസംഖ്യ constrain_int(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max);
ദശാംശം rand_scale(ദശാംശം s);
പൂർണ്ണസംഖ്യ rand_int(പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max);
ശൂന്യം mean_arrays(ദശാംശം **a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ els, ദശാംശം *avg);
ദശാംശം dist_array(ദശാംശം *a, ദശാംശം *b, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ sub);
ദശാംശം **one_hot_encode(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k);
ദശാംശം sec(clock_t clocks);
ശൂന്യം print_statistics(ദശാംശം *a, പൂർണ്ണസംഖ്യ n);
പൂർണ്ണസംഖ്യ int_index(പൂർണ്ണസംഖ്യ *a, പൂർണ്ണസംഖ്യ val, പൂർണ്ണസംഖ്യ n);

#endif

