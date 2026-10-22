#ضمن <stdio.h>
#ضمن <math.h>
فراغ col2im_جوڑو_pixel(عائم *im, صحيح اچائی, صحيح چوڑائی, صحيح channels,
                        صحيح صف, صحيح خانہ, صحيح channel, صحيح pad, عائم val)
{
    صف -= pad;
    خانہ -= pad;

    إذا (صف < 0 || خانہ < 0 ||
        صف >= اچائی || خانہ >= چوڑائی) أرجع;
    im[خانہ + چوڑائی*(صف + اچائی*channel)] += val;
}
//This one might be too, can't remember.
فراغ col2im_cpu(عائم* انکڑے_خانہ,
         صحيح channels,  صحيح اچائی,  صحيح چوڑائی,
         صحيح ksize,  صحيح stride, صحيح pad, عائم* انکڑے_im) 
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
                مزدوج val = انکڑے_خانہ[خانہ_اشاریہ];
                col2im_جوڑو_pixel(انکڑے_im, اچائی, چوڑائی, channels,
                        im_صف, im_خانہ, c_im, pad, val);
            }
        }
    }
}

