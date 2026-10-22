#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTILS_H
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਮਾਂ_2(a) \
    ਕਰੋ { \
    ਦੁੱਗਣਾ ਸ਼ੁਰੂ = what_ਸਮਾਂ_is_it_now(); \
    a; \
    ਲਿਖੋ("%s took: %f seconds\n", #a, what_ਸਮਾਂ_is_it_now() - ਸ਼ੁਰੂ); \
    } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TWO_PI 6.2831853071795864769252866f

ਦੁੱਗਣਾ what_ਸਮਾਂ_is_it_now();
ਖਾਲੀ shuffle(ਖਾਲੀ *arr, size_t n, size_t ਮਾਪ);
ਖਾਲੀ sorta_shuffle(ਖਾਲੀ *arr, size_t n, size_t ਮਾਪ, size_t sections);
ਖਾਲੀ free_ptrs(ਖਾਲੀ **ptrs, ਪੂਰਨ n);
ਪੂਰਨ alphanum_ਮੰਜ਼ਿਲ_int(ਅੱਖਰ c);
ਅੱਖਰ int_ਮੰਜ਼ਿਲ_alphanum(ਪੂਰਨ i);
ਪੂਰਨ read_int(ਪੂਰਨ fd);
ਖਾਲੀ write_int(ਪੂਰਨ fd, ਪੂਰਨ n);
ਖਾਲੀ read_all(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ);
ਖਾਲੀ write_all(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ);
ਪੂਰਨ read_all_fail(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ);
ਪੂਰਨ write_all_fail(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ);
ਖਾਲੀ ਲੱਭੋ_replace(ਅੱਖਰ *str, ਅੱਖਰ *orig, ਅੱਖਰ *rep, ਅੱਖਰ *output);
ਖਾਲੀ malloc_error();
ਖਾਲੀ file_error(ਅੱਖਰ *s);
ਖਾਲੀ strip(ਅੱਖਰ *s);
ਖਾਲੀ strip_char(ਅੱਖਰ *s, ਅੱਖਰ bad);
list *ਵੰਡੋ_str(ਅੱਖਰ *s, ਅੱਖਰ delim);
ਅੱਖਰ *fgetl(FILE *fp);
list *parse_csv_ਸਤਰ(ਅੱਖਰ *ਸਤਰ);
ਅੱਖਰ *copy_string(ਅੱਖਰ *s);
ਪੂਰਨ ਗਿਣਤੀ_fields(ਅੱਖਰ *ਸਤਰ);
ਦਸ਼ਮਲਵ *parse_fields(ਅੱਖਰ *ਸਤਰ, ਪੂਰਨ n);
ਖਾਲੀ translate_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਦਸ਼ਮਲਵ s);
ਦਸ਼ਮਲਵ constrain(ਦਸ਼ਮਲਵ min, ਦਸ਼ਮਲਵ max, ਦਸ਼ਮਲਵ a);
ਪੂਰਨ constrain_int(ਪੂਰਨ a, ਪੂਰਨ min, ਪੂਰਨ max);
ਦਸ਼ਮਲਵ rand_scale(ਦਸ਼ਮਲਵ s);
ਪੂਰਨ rand_int(ਪੂਰਨ min, ਪੂਰਨ max);
ਖਾਲੀ ਔਸਤ_arrays(ਦਸ਼ਮਲਵ **a, ਪੂਰਨ n, ਪੂਰਨ els, ਦਸ਼ਮਲਵ *avg);
ਦਸ਼ਮਲਵ dist_array(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਪੂਰਨ n, ਪੂਰਨ sub);
ਦਸ਼ਮਲਵ **one_hot_encode(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਪੂਰਨ k);
ਦਸ਼ਮਲਵ sec(clock_t clocks);
ਖਾਲੀ print_statistics(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n);
ਪੂਰਨ int_ਸੂਚਕਾਂਕ(ਪੂਰਨ *a, ਪੂਰਨ val, ਪੂਰਨ n);

#ਜੇ_ਅੰਤ

