#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਅੰਕੜੇ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕੜੇ_H
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "matrix.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ ਦੂਰੀ_from_ਕਿਨਾਰਾ(ਪੂਰਨ x, ਪੂਰਨ max)
{
    ਪੂਰਨ dx = (max/2) - x;
    ਜੇ (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    ਦਸ਼ਮਲਵ dist = (ਦਸ਼ਮਲਵ)dx/max;
    ਜੇ (dist > 1) dist = 1;
    ਵਾਪਸ dist;
}
ਖਾਲੀ ਲੱਦੋ_ਅੰਕੜੇ_blocking(ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ);


ਖਾਲੀ print_letters(ਦਸ਼ਮਲਵ *pred, ਪੂਰਨ n);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_captcha(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ w, ਪੂਰਨ h);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_captcha_encode(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_detection(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ boxes, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ jitter, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_ਟੈਗ(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure);
ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_image_augment_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure, ਪੂਰਨ center);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_super(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ scale);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_augment(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਅੱਖਰ **labels, ਪੂਰਨ k, ਰੁੱਖ *hierarchy, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure, ਪੂਰਨ center);
ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_regression(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ classes, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure);
ਅੰਕੜੇ ਲੱਦੋ_go(ਅੱਖਰ *filename);


ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_writing(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਬਾਹਰਲਾ_w, ਪੂਰਨ ਬਾਹਰਲਾ_h);

ਖਾਲੀ get_random_batch(ਅੰਕੜੇ d, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *y);
ਅੰਕੜੇ get_ਅੰਕੜੇ_part(ਅੰਕੜੇ d, ਪੂਰਨ part, ਪੂਰਨ ਕੁੱਲ);
ਅੰਕੜੇ get_random_ਅੰਕੜੇ(ਅੰਕੜੇ d, ਪੂਰਨ ਅੰਕ);
ਅੰਕੜੇ ਲੱਦੋ_categorical_ਅੰਕੜੇ_csv(ਅੱਖਰ *filename, ਪੂਰਨ ਨਿਸ਼ਾਨਾ, ਪੂਰਨ k);
ਖਾਲੀ normalize_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d);
ਖਾਲੀ scale_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d, ਦਸ਼ਮਲਵ s);
ਖਾਲੀ translate_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d, ਦਸ਼ਮਲਵ s);
ਖਾਲੀ randomize_ਅੰਕੜੇ(ਅੰਕੜੇ d);
ਅੰਕੜੇ *ਵੰਡੋ_ਅੰਕੜੇ(ਅੰਕੜੇ d, ਪੂਰਨ part, ਪੂਰਨ ਕੁੱਲ);
ਅੰਕੜੇ concat_datas(ਅੰਕੜੇ *d, ਪੂਰਨ n);
ਖਾਲੀ fill_truth(ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **labels, ਪੂਰਨ k, ਦਸ਼ਮਲਵ *truth);

#ਜੇ_ਅੰਤ
