#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
ਖਾਲੀ col2im_ਜੋੜੋ_pixel(ਦਸ਼ਮਲਵ *im, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ channels,
                        ਪੂਰਨ ਪੰਕਤੀ, ਪੂਰਨ ਖਾਨਾ, ਪੂਰਨ channel, ਪੂਰਨ pad, ਦਸ਼ਮਲਵ val)
{
    ਪੰਕਤੀ -= pad;
    ਖਾਨਾ -= pad;

    ਜੇ (ਪੰਕਤੀ < 0 || ਖਾਨਾ < 0 ||
        ਪੰਕਤੀ >= ਉਚਾਈ || ਖਾਨਾ >= ਚੌੜਾਈ) ਵਾਪਸ;
    im[ਖਾਨਾ + ਚੌੜਾਈ*(ਪੰਕਤੀ + ਉਚਾਈ*channel)] += val;
}
//This one might be too, can't remember.
ਖਾਲੀ col2im_cpu(ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_ਖਾਨਾ,
         ਪੂਰਨ channels,  ਪੂਰਨ ਉਚਾਈ,  ਪੂਰਨ ਚੌੜਾਈ,
         ਪੂਰਨ ksize,  ਪੂਰਨ stride, ਪੂਰਨ pad, ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_im) 
{
    ਪੂਰਨ c,h,w;
    ਪੂਰਨ ਉਚਾਈ_ਖਾਨਾ = (ਉਚਾਈ + 2*pad - ksize) / stride + 1;
    ਪੂਰਨ ਚੌੜਾਈ_ਖਾਨਾ = (ਚੌੜਾਈ + 2*pad - ksize) / stride + 1;

    ਪੂਰਨ channels_ਖਾਨਾ = channels * ksize * ksize;
    ਲਈ (c = 0; c < channels_ਖਾਨਾ; ++c) {
        ਪੂਰਨ w_ਵਿੱਥ = c % ksize;
        ਪੂਰਨ h_ਵਿੱਥ = (c / ksize) % ksize;
        ਪੂਰਨ c_im = c / ksize / ksize;
        ਲਈ (h = 0; h < ਉਚਾਈ_ਖਾਨਾ; ++h) {
            ਲਈ (w = 0; w < ਚੌੜਾਈ_ਖਾਨਾ; ++w) {
                ਪੂਰਨ im_ਪੰਕਤੀ = h_ਵਿੱਥ + h * stride;
                ਪੂਰਨ im_ਖਾਨਾ = w_ਵਿੱਥ + w * stride;
                ਪੂਰਨ ਖਾਨਾ_ਸੂਚਕਾਂਕ = (c * ਉਚਾਈ_ਖਾਨਾ + h) * ਚੌੜਾਈ_ਖਾਨਾ + w;
                ਦੁੱਗਣਾ val = ਅੰਕੜੇ_ਖਾਨਾ[ਖਾਨਾ_ਸੂਚਕਾਂਕ];
                col2im_ਜੋੜੋ_pixel(ਅੰਕੜੇ_im, ਉਚਾਈ, ਚੌੜਾਈ, channels,
                        im_ਪੰਕਤੀ, im_ਖਾਨਾ, c_im, pad, val);
            }
        }
    }
}

