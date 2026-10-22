#اگر_غیر_تعریف_شدہ ڈیٹا_H
#تعریف_کرو ڈیٹا_H
#شامل <pthread.h>

#شامل "darknet.h"
#شامل "matrix.h"
#شامل "list.h"
#شامل "image.h"
#شامل "tree.h"

ساکن اندرونی اعشاری فاصلہ_from_کنارہ(صحیح x, صحیح max)
{
    صحیح dx = (max/2) - x;
    اگر (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    اعشاری dist = (اعشاری)dx/max;
    اگر (dist > 1) dist = 1;
    واپس dist;
}
خالی بھرو_ڈیٹا_blocking(بھرو_دلائل دلائل);


خالی print_letters(اعشاری *pred, صحیح n);
ڈیٹا بھرو_ڈیٹا_captcha(حرف **paths, صحیح n, صحیح m, صحیح k, صحیح w, صحیح h);
ڈیٹا بھرو_ڈیٹا_captcha_encode(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h);
ڈیٹا بھرو_ڈیٹا_detection(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح boxes, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure);
ڈیٹا بھرو_ڈیٹا_لیبل(حرف **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure);
میٹرکس بھرو_image_augment_paths(حرف **paths, صحیح n, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center);
ڈیٹا بھرو_ڈیٹا_super(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح scale);
ڈیٹا بھرو_ڈیٹا_augment(حرف **paths, صحیح n, صحیح m, حرف **labels, صحیح k, درخت *hierarchy, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center);
ڈیٹا بھرو_ڈیٹا_regression(حرف **paths, صحیح n, صحیح m, صحیح classes, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure);
ڈیٹا بھرو_go(حرف *filename);


ڈیٹا بھرو_ڈیٹا_writing(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح برآمد_w, صحیح برآمد_h);

خالی get_random_batch(ڈیٹا d, صحیح n, اعشاری *X, اعشاری *y);
ڈیٹا get_ڈیٹا_part(ڈیٹا d, صحیح part, صحیح کل);
ڈیٹا get_random_ڈیٹا(ڈیٹا d, صحیح عدد);
ڈیٹا بھرو_categorical_ڈیٹا_csv(حرف *filename, صحیح ہدف, صحیح k);
خالی normalize_ڈیٹا_صفیں(ڈیٹا d);
خالی scale_ڈیٹا_صفیں(ڈیٹا d, اعشاری s);
خالی translate_ڈیٹا_صفیں(ڈیٹا d, اعشاری s);
خالی randomize_ڈیٹا(ڈیٹا d);
ڈیٹا *بانٹو_ڈیٹا(ڈیٹا d, صحیح part, صحیح کل);
ڈیٹا concat_datas(ڈیٹا *d, صحیح n);
خالی fill_truth(حرف *راستہ, حرف **labels, صحیح k, اعشاری *truth);

#اگر_ختم
