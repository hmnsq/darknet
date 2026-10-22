#جيڪڏهن_وصف_نه_ٿيل معلومات_H
#وصف_ڪريو معلومات_H
#ضمن <pthread.h>

#ضمن "darknet.h"
#ضمن "matrix.h"
#ضمن "list.h"
#ضمن "image.h"
#ضمن "tree.h"

ساكن مضمن عائم مفاصلو_from_ڪنارو(صحيح x, صحيح max)
{
    صحيح dx = (max/2) - x;
    إذا (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    عائم dist = (عائم)dx/max;
    إذا (dist > 1) dist = 1;
    أرجع dist;
}
فراغ ڀريو_معلومات_blocking(ڀريو_دليلون دليلون);


فراغ print_letters(عائم *pred, صحيح n);
معلومات ڀريو_معلومات_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h);
معلومات ڀريو_معلومات_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h);
معلومات ڀريو_معلومات_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure);
معلومات ڀريو_معلومات_ٽيگ(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure);
ميٽرڪس ڀريو_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center);
معلومات ڀريو_معلومات_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale);
معلومات ڀريو_معلومات_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, وڻ *hierarchy, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center);
معلومات ڀريو_معلومات_regression(حرف **paths, صحيح n, صحيح m, صحيح classes, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure);
معلومات ڀريو_go(حرف *filename);


معلومات ڀريو_معلومات_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح ٻاهريون_w, صحيح ٻاهريون_h);

فراغ get_random_batch(معلومات d, صحيح n, عائم *X, عائم *y);
معلومات get_معلومات_part(معلومات d, صحيح part, صحيح ڪل);
معلومات get_random_معلومات(معلومات d, صحيح انگ);
معلومات ڀريو_categorical_معلومات_csv(حرف *filename, صحيح هدف, صحيح k);
فراغ normalize_معلومات_صفون(معلومات d);
فراغ scale_معلومات_صفون(معلومات d, عائم s);
فراغ translate_معلومات_صفون(معلومات d, عائم s);
فراغ randomize_معلومات(معلومات d);
معلومات *ورهايو_معلومات(معلومات d, صحيح part, صحيح ڪل);
معلومات concat_datas(معلومات *d, صحيح n);
فراغ fill_truth(حرف *رستو, حرف **labels, صحيح k, عائم *truth);

#جيڪڏهن_پورو
