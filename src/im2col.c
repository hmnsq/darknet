#ضمن "im2col.h"
#ضمن <stdio.h>
عائم im2col_get_pixel(عائم *im, صحيح ارتفاع, صحيح عرض, صحيح channels,
                        صحيح ردیف, صحيح ستون, صحيح channel, صحيح pad)
{
    ردیف -= pad;
    ستون -= pad;

    إذا (ردیف < 0 || ستون < 0 ||
        ردیف >= ارتفاع || ستون >= عرض) أرجع 0;
    أرجع im[ستون + عرض*(ردیف + ارتفاع*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
فراغ im2col_cpu(عائم* داده_im,
     صحيح channels,  صحيح ارتفاع,  صحيح عرض,
     صحيح ksize,  صحيح stride, صحيح pad, عائم* داده_ستون) 
{
    صحيح c,h,w;
    صحيح ارتفاع_ستون = (ارتفاع + 2*pad - ksize) / stride + 1;
    صحيح عرض_ستون = (عرض + 2*pad - ksize) / stride + 1;

    صحيح channels_ستون = channels * ksize * ksize;
    لكل (c = 0; c < channels_ستون; ++c) {
        صحيح w_جابه_جایی = c % ksize;
        صحيح h_جابه_جایی = (c / ksize) % ksize;
        صحيح c_im = c / ksize / ksize;
        لكل (h = 0; h < ارتفاع_ستون; ++h) {
            لكل (w = 0; w < عرض_ستون; ++w) {
                صحيح im_ردیف = h_جابه_جایی + h * stride;
                صحيح im_ستون = w_جابه_جایی + w * stride;
                صحيح ستون_شاخص = (c * ارتفاع_ستون + h) * عرض_ستون + w;
                داده_ستون[ستون_شاخص] = im2col_get_pixel(داده_im, ارتفاع, عرض, channels,
                        im_ردیف, im_ستون, c_im, pad);
            }
        }
    }
}

