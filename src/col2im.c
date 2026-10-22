#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, puur_naa_nka height, puur_naa_nka width, puur_naa_nka channels,
                        puur_naa_nka row, puur_naa_nka col, puur_naa_nka channel, puur_naa_nka pad, float val)
{
    row -= pad;
    col -= pad;

    ᨀᨘ (row < 0 || col < 0 ||
        row >= height || col >= width) ᨀᨘᨒ;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         puur_naa_nka channels,  puur_naa_nka height,  puur_naa_nka width,
         puur_naa_nka ksize,  puur_naa_nka stride, puur_naa_nka pad, float* data_im) 
{
    puur_naa_nka c,h,w;
    puur_naa_nka height_col = (height + 2*pad - ksize) / stride + 1;
    puur_naa_nka width_col = (width + 2*pad - ksize) / stride + 1;

    puur_naa_nka channels_col = channels * ksize * ksize;
    for (c = 0; c < channels_col; ++c) {
        puur_naa_nka w_offset = c % ksize;
        puur_naa_nka h_offset = (c / ksize) % ksize;
        puur_naa_nka c_im = c / ksize / ksize;
        for (h = 0; h < height_col; ++h) {
            for (w = 0; w < width_col; ++w) {
                puur_naa_nka im_row = h_offset + h * stride;
                puur_naa_nka im_col = w_offset + w * stride;
                puur_naa_nka col_index = (c * height_col + h) * width_col + w;
                dviguu_na val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

