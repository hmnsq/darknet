#ضمن "im2col.h"
#ضمن <stdio.h>
عائم im2col_get_pixel(عائم *im, صحيح لوړوالی, صحيح پلنوالی, صحيح channels,
                        صحيح لیکه, صحيح ستون, صحيح channel, صحيح pad)
{
    لیکه -= pad;
    ستون -= pad;

    إذا (لیکه < 0 || ستون < 0 ||
        لیکه >= لوړوالی || ستون >= پلنوالی) أرجع 0;
    أرجع im[ستون + پلنوالی*(لیکه + لوړوالی*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
فراغ im2col_cpu(عائم* معلومات_im,
     صحيح channels,  صحيح لوړوالی,  صحيح پلنوالی,
     صحيح ksize,  صحيح stride, صحيح pad, عائم* معلومات_ستون) 
{
    صحيح c,h,w;
    صحيح لوړوالی_ستون = (لوړوالی + 2*pad - ksize) / stride + 1;
    صحيح پلنوالی_ستون = (پلنوالی + 2*pad - ksize) / stride + 1;

    صحيح channels_ستون = channels * ksize * ksize;
    لكل (c = 0; c < channels_ستون; ++c) {
        صحيح w_دننه_واټن = c % ksize;
        صحيح h_دننه_واټن = (c / ksize) % ksize;
        صحيح c_im = c / ksize / ksize;
        لكل (h = 0; h < لوړوالی_ستون; ++h) {
            لكل (w = 0; w < پلنوالی_ستون; ++w) {
                صحيح im_لیکه = h_دننه_واټن + h * stride;
                صحيح im_ستون = w_دننه_واټن + w * stride;
                صحيح ستون_شاخص = (c * لوړوالی_ستون + h) * پلنوالی_ستون + w;
                معلومات_ستون[ستون_شاخص] = im2col_get_pixel(معلومات_im, لوړوالی, پلنوالی, channels,
                        im_لیکه, im_ستون, c_im, pad);
            }
        }
    }
}

