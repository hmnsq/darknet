#ifndef DATA_H
#define DATA_H
#ضمن <pthread.h>

#ضمن "darknet.h"
#ضمن "matrix.h"
#ضمن "list.h"
#ضمن "image.h"
#ضمن "tree.h"

ساكن مضمن عائم distance_from_edge(صحيح x, صحيح max)
{
    صحيح dx = (max/2) - x;
    אם (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    عائم dist = (عائم)dx/max;
    אם (dist > 1) dist = 1;
    החזר dist;
}
فراغ load_data_blocking(load_args args);


فراغ print_letters(عائم *pred, صحيح n);
data load_data_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h);
data load_data_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h);
data load_data_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure);
data load_data_tag(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure);
matrix load_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center);
data load_data_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale);
data load_data_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, tree *hierarchy, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center);
data load_data_regression(حرف **paths, صحيح n, صحيح m, صحيح classes, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure);
data load_go(حرف *filename);


data load_data_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح out_w, صحيح out_h);

فراغ get_random_batch(data d, صحيح n, عائم *X, عائم *y);
data get_data_part(data d, صحيح part, صحيح total);
data get_random_data(data d, صحيح num);
data load_categorical_data_csv(حرف *filename, صحيح target, صحيح k);
فراغ normalize_data_rows(data d);
فراغ scale_data_rows(data d, عائم s);
فراغ translate_data_rows(data d, عائم s);
فراغ randomize_data(data d);
data *split_data(data d, صحيح part, صحيح total);
data concat_datas(data *d, صحيح n);
فراغ fill_truth(حرف *path, حرف **labels, صحيح k, عائم *truth);

#endif
