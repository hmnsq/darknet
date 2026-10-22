#ضمن <stdio.h>
#ضمن <math.h>
فراغ col2im_جمع_کړه_pixel(عائم *im, صحيح لوړوالی, صحيح پلنوالی, صحيح channels,
                        صحيح لیکه, صحيح ستون, صحيح channel, صحيح pad, عائم val)
{
    لیکه -= pad;
    ستون -= pad;

    إذا (لیکه < 0 || ستون < 0 ||
        لیکه >= لوړوالی || ستون >= پلنوالی) أرجع;
    im[ستون + پلنوالی*(لیکه + لوړوالی*channel)] += val;
}
//This one might be too, can't remember.
فراغ col2im_cpu(عائم* معلومات_ستون,
         صحيح channels,  صحيح لوړوالی,  صحيح پلنوالی,
         صحيح ksize,  صحيح stride, صحيح pad, عائم* معلومات_im) 
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
                مزدوج val = معلومات_ستون[ستون_شاخص];
                col2im_جمع_کړه_pixel(معلومات_im, لوړوالی, پلنوالی, channels,
                        im_لیکه, im_ستون, c_im, pad, val);
            }
        }
    }
}

