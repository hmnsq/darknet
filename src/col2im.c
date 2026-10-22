#ضمن <stdio.h>
#ضمن <math.h>
فراغ col2im_جمع_کن_pixel(عائم *im, صحيح ارتفاع, صحيح عرض, صحيح channels,
                        صحيح ردیف, صحيح ستون, صحيح channel, صحيح pad, عائم val)
{
    ردیف -= pad;
    ستون -= pad;

    إذا (ردیف < 0 || ستون < 0 ||
        ردیف >= ارتفاع || ستون >= عرض) أرجع;
    im[ستون + عرض*(ردیف + ارتفاع*channel)] += val;
}
//This one might be too, can't remember.
فراغ col2im_cpu(عائم* داده_ستون,
         صحيح channels,  صحيح ارتفاع,  صحيح عرض,
         صحيح ksize,  صحيح stride, صحيح pad, عائم* داده_im) 
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
                مزدوج val = داده_ستون[ستون_شاخص];
                col2im_جمع_کن_pixel(داده_im, ارتفاع, عرض, channels,
                        im_ردیف, im_ستون, c_im, pad, val);
            }
        }
    }
}

