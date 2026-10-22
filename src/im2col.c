#ضمن "im2col.h"
#ضمن <stdio.h>
عائم im2col_get_pixel(عائم *im, صحيح اچائی, صحيح چوڑائی, صحيح channels,
                        صحيح صف, صحيح خانہ, صحيح channel, صحيح pad)
{
    صف -= pad;
    خانہ -= pad;

    إذا (صف < 0 || خانہ < 0 ||
        صف >= اچائی || خانہ >= چوڑائی) أرجع 0;
    أرجع im[خانہ + چوڑائی*(صف + اچائی*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
فراغ im2col_cpu(عائم* انکڑے_im,
     صحيح channels,  صحيح اچائی,  صحيح چوڑائی,
     صحيح ksize,  صحيح stride, صحيح pad, عائم* انکڑے_خانہ) 
{
    صحيح c,h,w;
    صحيح اچائی_خانہ = (اچائی + 2*pad - ksize) / stride + 1;
    صحيح چوڑائی_خانہ = (چوڑائی + 2*pad - ksize) / stride + 1;

    صحيح channels_خانہ = channels * ksize * ksize;
    لكل (c = 0; c < channels_خانہ; ++c) {
        صحيح w_وتھ = c % ksize;
        صحيح h_وتھ = (c / ksize) % ksize;
        صحيح c_im = c / ksize / ksize;
        لكل (h = 0; h < اچائی_خانہ; ++h) {
            لكل (w = 0; w < چوڑائی_خانہ; ++w) {
                صحيح im_صف = h_وتھ + h * stride;
                صحيح im_خانہ = w_وتھ + w * stride;
                صحيح خانہ_اشاریہ = (c * اچائی_خانہ + h) * چوڑائی_خانہ + w;
                انکڑے_خانہ[خانہ_اشاریہ] = im2col_get_pixel(انکڑے_im, اچائی, چوڑائی, channels,
                        im_صف, im_خانہ, c_im, pad);
            }
        }
    }
}

