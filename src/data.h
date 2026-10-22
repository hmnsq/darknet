#ifndef DATA_H
#define DATA_H
#અન્તર્ભાવ <pthread.h>

#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "matrix.h"
#અન્તર્ભાવ "list.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "tree.h"

સ્થિર અંતઃરેખ દશાંશ distance_from_edge(પૂર્ણાંક x, પૂર્ણાંક max)
{
    પૂર્ણાંક dx = (max/2) - x;
    જો (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    દશાંશ dist = (દશાંશ)dx/max;
    જો (dist > 1) dist = 1;
    પાછા dist;
}
શૂન્ય load_data_blocking(load_args args);


શૂન્ય print_letters(દશાંશ *pred, પૂર્ણાંક n);
data load_data_captcha(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક w, પૂર્ણાંક h);
data load_data_captcha_encode(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h);
data load_data_detection(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક boxes, પૂર્ણાંક classes, દશાંશ jitter, દશાંશ hue, દશાંશ saturation, દશાંશ exposure);
data load_data_tag(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure);
matrix load_image_augment_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure, પૂર્ણાંક center);
data load_data_super(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક scale);
data load_data_augment(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, અક્ષર **labels, પૂર્ણાંક k, વૃક્ષ *hierarchy, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure, પૂર્ણાંક center);
data load_data_regression(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક classes, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure);
data load_go(અક્ષર *filename);


data load_data_writing(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક out_w, પૂર્ણાંક out_h);

શૂન્ય get_random_batch(data d, પૂર્ણાંક n, દશાંશ *X, દશાંશ *y);
data get_data_part(data d, પૂર્ણાંક part, પૂર્ણાંક total);
data get_random_data(data d, પૂર્ણાંક અંક);
data load_categorical_data_csv(અક્ષર *filename, પૂર્ણાંક target, પૂર્ણાંક k);
શૂન્ય normalize_data_rows(data d);
શૂન્ય scale_data_rows(data d, દશાંશ s);
શૂન્ય translate_data_rows(data d, દશાંશ s);
શૂન્ય randomize_data(data d);
data *split_data(data d, પૂર્ણાંક part, પૂર્ણાંક total);
data concat_datas(data *d, પૂર્ણાંક n);
શૂન્ય fill_truth(અક્ષર *path, અક્ષર **labels, પૂર્ણાંક k, દશાંશ *truth);

#endif
