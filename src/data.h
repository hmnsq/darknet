#ifndef তথ্য_H
#define তথ্য_H
#অন্তর্ভাব <pthread.h>

#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "matrix.h"
#অন্তর্ভাব "list.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "tree.h"

স্থির অন্তঃরেখ দশমিক distance_from_edge(পূর্ণসংখ্যা x, পূর্ণসংখ্যা max)
{
    পূর্ণসংখ্যা dx = (max/2) - x;
    যদি (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    দশমিক dist = (দশমিক)dx/max;
    যদি (dist > 1) dist = 1;
    ফেরত dist;
}
শূন্য load_তথ্য_blocking(load_args args);


শূন্য print_letters(দশমিক *pred, পূর্ণসংখ্যা n);
তথ্য load_তথ্য_captcha(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
তথ্য load_তথ্য_captcha_encode(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
তথ্য load_তথ্য_detection(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা boxes, পূর্ণসংখ্যা classes, দশমিক jitter, দশমিক hue, দশমিক saturation, দশমিক exposure);
তথ্য load_তথ্য_tag(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure);
matrix load_image_augment_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure, পূর্ণসংখ্যা center);
তথ্য load_তথ্য_super(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা scale);
তথ্য load_তথ্য_augment(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, অক্ষর **labels, পূর্ণসংখ্যা k, বৃক্ষ *hierarchy, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure, পূর্ণসংখ্যা center);
তথ্য load_তথ্য_regression(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure);
তথ্য load_go(অক্ষর *filename);


তথ্য load_তথ্য_writing(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা out_w, পূর্ণসংখ্যা out_h);

শূন্য get_random_batch(তথ্য d, পূর্ণসংখ্যা n, দশমিক *X, দশমিক *y);
তথ্য get_তথ্য_part(তথ্য d, পূর্ণসংখ্যা part, পূর্ণসংখ্যা যোগ_2);
তথ্য get_random_তথ্য(তথ্য d, পূর্ণসংখ্যা অঙ্ক);
তথ্য load_categorical_তথ্য_csv(অক্ষর *filename, পূর্ণসংখ্যা target, পূর্ণসংখ্যা k);
শূন্য normalize_তথ্য_rows(তথ্য d);
শূন্য scale_তথ্য_rows(তথ্য d, দশমিক s);
শূন্য translate_তথ্য_rows(তথ্য d, দশমিক s);
শূন্য randomize_তথ্য(তথ্য d);
তথ্য *split_তথ্য(তথ্য d, পূর্ণসংখ্যা part, পূর্ণসংখ্যা যোগ_2);
তথ্য concat_datas(তথ্য *d, পূর্ণসংখ্যা n);
শূন্য fill_truth(অক্ষর *path, অক্ষর **labels, পূর্ণসংখ্যা k, দশমিক *truth);

#endif
