#ifndef DATA_H
#define DATA_H
#شامل <pthread.h>

#شامل "darknet.h"
#شامل "matrix.h"
#شامل "list.h"
#شامل "image.h"
#شامل "tree.h"

ایستا درونخطی اعشاری distance_from_edge(صحیح x, صحیح max)
{
    صحیح dx = (max/2) - x;
    اگر (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    اعشاری dist = (اعشاری)dx/max;
    اگر (dist > 1) dist = 1;
    بازگشت dist;
}
تهی load_data_blocking(load_args args);


تهی print_letters(اعشاری *pred, صحیح n);
data load_data_captcha(نویسه **paths, صحیح n, صحیح m, صحیح k, صحیح w, صحیح h);
data load_data_captcha_encode(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h);
data load_data_detection(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح boxes, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure);
data load_data_tag(نویسه **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure);
matrix load_image_augment_paths(نویسه **paths, صحیح n, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center);
data load_data_super(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح scale);
data load_data_augment(نویسه **paths, صحیح n, صحیح m, نویسه **labels, صحیح k, درخت *hierarchy, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center);
data load_data_regression(نویسه **paths, صحیح n, صحیح m, صحیح classes, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure);
data load_go(نویسه *filename);


data load_data_writing(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح out_w, صحیح out_h);

تهی get_random_batch(data d, صحیح n, اعشاری *X, اعشاری *y);
data get_data_part(data d, صحیح part, صحیح total);
data get_random_data(data d, صحیح رقم);
data load_categorical_data_csv(نویسه *filename, صحیح target, صحیح k);
تهی normalize_data_rows(data d);
تهی scale_data_rows(data d, اعشاری s);
تهی translate_data_rows(data d, اعشاری s);
تهی randomize_data(data d);
data *split_data(data d, صحیح part, صحیح total);
data concat_datas(data *d, صحیح n);
تهی fill_truth(نویسه *path, نویسه **labels, صحیح k, اعشاری *truth);

#endif
