#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UTILS_H
#ವ್ಯಾಖ್ಯಾನಿಸು UTILS_H
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "list.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಸಮಯ_2(a) \
    ಮಾಡು { \
    ದ್ವಿಗುಣ ಆರಂಭ = what_ಸಮಯ_is_it_now(); \
    a; \
    ಬರೆ("%s took: %f seconds\n", #a, what_ಸಮಯ_is_it_now() - ಆರಂಭ); \
    } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು TWO_PI 6.2831853071795864769252866f

ದ್ವಿಗುಣ what_ಸಮಯ_is_it_now();
ಶೂನ್ಯ shuffle(ಶೂನ್ಯ *arr, size_t n, size_t ಗಾತ್ರ_2);
ಶೂನ್ಯ sorta_shuffle(ಶೂನ್ಯ *arr, size_t n, size_t ಗಾತ್ರ_2, size_t sections);
ಶೂನ್ಯ free_ptrs(ಶೂನ್ಯ **ptrs, ಪೂರ್ಣಾಂಕ n);
ಪೂರ್ಣಾಂಕ alphanum_ಗಮ್ಯ_int(ಅಕ್ಷರ c);
ಅಕ್ಷರ int_ಗಮ್ಯ_alphanum(ಪೂರ್ಣಾಂಕ i);
ಪೂರ್ಣಾಂಕ read_int(ಪೂರ್ಣಾಂಕ fd);
ಶೂನ್ಯ write_int(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ n);
ಶೂನ್ಯ read_all(ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *ಬಫರ್, size_t ಬೈಟುಗಳು);
ಶೂನ್ಯ write_all(ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *ಬಫರ್, size_t ಬೈಟುಗಳು);
ಪೂರ್ಣಾಂಕ read_all_fail(ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *ಬಫರ್, size_t ಬೈಟುಗಳು);
ಪೂರ್ಣಾಂಕ write_all_fail(ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *ಬಫರ್, size_t ಬೈಟುಗಳು);
ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_replace(ಅಕ್ಷರ *str, ಅಕ್ಷರ *orig, ಅಕ್ಷರ *rep, ಅಕ್ಷರ *output);
ಶೂನ್ಯ malloc_error();
ಶೂನ್ಯ file_error(ಅಕ್ಷರ *s);
ಶೂನ್ಯ strip(ಅಕ್ಷರ *s);
ಶೂನ್ಯ strip_char(ಅಕ್ಷರ *s, ಅಕ್ಷರ bad);
list *ವಿಭಜಿಸು_str(ಅಕ್ಷರ *s, ಅಕ್ಷರ delim);
ಅಕ್ಷರ *fgetl(FILE *fp);
list *parse_csv_ಸಾಲು(ಅಕ್ಷರ *ಸಾಲು);
ಅಕ್ಷರ *copy_string(ಅಕ್ಷರ *s);
ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_fields(ಅಕ್ಷರ *ಸಾಲು);
ದಶಮಾಂಶ *parse_fields(ಅಕ್ಷರ *ಸಾಲು, ಪೂರ್ಣಾಂಕ n);
ಶೂನ್ಯ translate_array(ದಶಮಾಂಶ *a, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ s);
ದಶಮಾಂಶ constrain(ದಶಮಾಂಶ min, ದಶಮಾಂಶ max, ದಶಮಾಂಶ a);
ಪೂರ್ಣಾಂಕ constrain_int(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max);
ದಶಮಾಂಶ rand_scale(ದಶಮಾಂಶ s);
ಪೂರ್ಣಾಂಕ rand_int(ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max);
ಶೂನ್ಯ ಸರಾಸರಿ_arrays(ದಶಮಾಂಶ **a, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ els, ದಶಮಾಂಶ *avg);
ದಶಮಾಂಶ dist_array(ದಶಮಾಂಶ *a, ದಶಮಾಂಶ *b, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ sub);
ದಶಮಾಂಶ **one_hot_encode(ದಶಮಾಂಶ *a, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k);
ದಶಮಾಂಶ sec(clock_t clocks);
ಶೂನ್ಯ print_statistics(ದಶಮಾಂಶ *a, ಪೂರ್ಣಾಂಕ n);
ಪೂರ್ಣಾಂಕ int_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ *a, ಪೂರ್ಣಾಂಕ val, ಪೂರ್ಣಾಂಕ n);

#ಆದರೆ_ಅಂತ್ಯ

