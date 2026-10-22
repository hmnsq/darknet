#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, số_nguyên height, số_nguyên width, số_nguyên channels,
                        số_nguyên row, số_nguyên col, số_nguyên channel, số_nguyên pad)
{
    row -= pad;
    col -= pad;

    nếu (row < 0 || col < 0 ||
        row >= height || col >= width) trả_về 0;
    trả_về im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     số_nguyên channels,  số_nguyên height,  số_nguyên width,
     số_nguyên ksize,  số_nguyên stride, số_nguyên pad, float* data_col) 
{
    số_nguyên c,h,w;
    số_nguyên height_col = (height + 2*pad - ksize) / stride + 1;
    số_nguyên width_col = (width + 2*pad - ksize) / stride + 1;

    số_nguyên channels_col = channels * ksize * ksize;
    cho (c = 0; c < channels_col; ++c) {
        số_nguyên w_offset = c % ksize;
        số_nguyên h_offset = (c / ksize) % ksize;
        số_nguyên c_im = c / ksize / ksize;
        cho (h = 0; h < height_col; ++h) {
            cho (w = 0; w < width_col; ++w) {
                số_nguyên im_row = h_offset + h * stride;
                số_nguyên im_col = w_offset + w * stride;
                số_nguyên col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

