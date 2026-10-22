#ضمن "im2col.h"
#ضمن <stdio.h>
عائم im2col_get_pixel(عائم *im, صحيح اوچائي, صحيح ويڪر, صحيح channels,
                        صحيح صف, صحيح کانو, صحيح channel, صحيح pad)
{
    صف -= pad;
    کانو -= pad;

    إذا (صف < 0 || کانو < 0 ||
        صف >= اوچائي || کانو >= ويڪر) أرجع 0;
    أرجع im[کانو + ويڪر*(صف + اوچائي*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
فراغ im2col_cpu(عائم* معلومات_im,
     صحيح channels,  صحيح اوچائي,  صحيح ويڪر,
     صحيح ksize,  صحيح stride, صحيح pad, عائم* معلومات_کانو) 
{
    صحيح c,h,w;
    صحيح اوچائي_کانو = (اوچائي + 2*pad - ksize) / stride + 1;
    صحيح ويڪر_کانو = (ويڪر + 2*pad - ksize) / stride + 1;

    صحيح channels_کانو = channels * ksize * ksize;
    لكل (c = 0; c < channels_کانو; ++c) {
        صحيح w_وٿي = c % ksize;
        صحيح h_وٿي = (c / ksize) % ksize;
        صحيح c_im = c / ksize / ksize;
        لكل (h = 0; h < اوچائي_کانو; ++h) {
            لكل (w = 0; w < ويڪر_کانو; ++w) {
                صحيح im_صف = h_وٿي + h * stride;
                صحيح im_کانو = w_وٿي + w * stride;
                صحيح کانو_اشاريو = (c * اوچائي_کانو + h) * ويڪر_کانو + w;
                معلومات_کانو[کانو_اشاريو] = im2col_get_pixel(معلومات_im, اوچائي, ويڪر, channels,
                        im_صف, im_کانو, c_im, pad);
            }
        }
    }
}

