#شامل "im2col.h"
#شامل <stdio.h>
اعشاری im2col_get_pixel(اعشاری *im, صحیح اونچائی, صحیح چوڑائی, صحیح channels,
                        صحیح صف, صحیح ستون, صحیح channel, صحیح pad)
{
    صف -= pad;
    ستون -= pad;

    اگر (صف < 0 || ستون < 0 ||
        صف >= اونچائی || ستون >= چوڑائی) واپس 0;
    واپس im[ستون + چوڑائی*(صف + اونچائی*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
خالی im2col_cpu(اعشاری* ڈیٹا_im,
     صحیح channels,  صحیح اونچائی,  صحیح چوڑائی,
     صحیح ksize,  صحیح stride, صحیح pad, اعشاری* ڈیٹا_ستون) 
{
    صحیح c,h,w;
    صحیح اونچائی_ستون = (اونچائی + 2*pad - ksize) / stride + 1;
    صحیح چوڑائی_ستون = (چوڑائی + 2*pad - ksize) / stride + 1;

    صحیح channels_ستون = channels * ksize * ksize;
    برائے (c = 0; c < channels_ستون; ++c) {
        صحیح w_انحراف = c % ksize;
        صحیح h_انحراف = (c / ksize) % ksize;
        صحیح c_im = c / ksize / ksize;
        برائے (h = 0; h < اونچائی_ستون; ++h) {
            برائے (w = 0; w < چوڑائی_ستون; ++w) {
                صحیح im_صف = h_انحراف + h * stride;
                صحیح im_ستون = w_انحراف + w * stride;
                صحیح ستون_اشاریہ = (c * اونچائی_ستون + h) * چوڑائی_ستون + w;
                ڈیٹا_ستون[ستون_اشاریہ] = im2col_get_pixel(ڈیٹا_im, اونچائی, چوڑائی, channels,
                        im_صف, im_ستون, c_im, pad);
            }
        }
    }
}

