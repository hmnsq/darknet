#ifndef DATA_H
#define DATA_H
#ଅନ୍ତର୍ଭାଵ <pthread.h>

#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "matrix.h"
#ଅନ୍ତର୍ଭାଵ "list.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ distance_from_edge(ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx = (max/2) - x;
    ଯଦି (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    ଦଶମିକ dist = (ଦଶମିକ)dx/max;
    ଯଦି (dist > 1) dist = 1;
    ଫେରନ୍ତୁ dist;
}
ଶୂନ୍ୟ load_data_blocking(load_args args);


ଶୂନ୍ୟ print_letters(ଦଶମିକ *pred, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
data load_data_captcha(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
data load_data_captcha_encode(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
data load_data_detection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ jitter, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure);
data load_data_tag(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure);
matrix load_image_augment_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure, ପୂର୍ଣ୍ଣସଂଖ୍ୟା center);
data load_data_super(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale);
data load_data_augment(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ବୃକ୍ଷ *hierarchy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure, ପୂର୍ଣ୍ଣସଂଖ୍ୟା center);
data load_data_regression(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure);
data load_go(ଅକ୍ଷର *filename);


data load_data_writing(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h);

ଶୂନ୍ୟ get_random_batch(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *X, ଦଶମିକ *y);
data get_data_part(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା part, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total);
data get_random_data(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ);
data load_categorical_data_csv(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା target, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k);
ଶୂନ୍ୟ normalize_data_rows(data d);
ଶୂନ୍ୟ scale_data_rows(data d, ଦଶମିକ s);
ଶୂନ୍ୟ translate_data_rows(data d, ଦଶମିକ s);
ଶୂନ୍ୟ randomize_data(data d);
data *split_data(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା part, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total);
data concat_datas(data *d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ଶୂନ୍ୟ fill_truth(ଅକ୍ଷର *path, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦଶମିକ *truth);

#endif
