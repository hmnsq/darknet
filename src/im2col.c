#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, ціле height, ціле width, ціле channels,
                        ціле row, ціле col, ціле channel, ціле pad)
{
    row -= pad;
    col -= pad;

    якщо (row < 0 || col < 0 ||
        row >= height || col >= width) повернути 0;
    повернути im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     ціле channels,  ціле height,  ціле width,
     ціле ksize,  ціле stride, ціле pad, float* data_col) 
{
    ціле c,h,w;
    ціле height_col = (height + 2*pad - ksize) / stride + 1;
    ціле width_col = (width + 2*pad - ksize) / stride + 1;

    ціле channels_col = channels * ksize * ksize;
    для (c = 0; c < channels_col; ++c) {
        ціле w_offset = c % ksize;
        ціле h_offset = (c / ksize) % ksize;
        ціле c_im = c / ksize / ksize;
        для (h = 0; h < height_col; ++h) {
            для (w = 0; w < width_col; ++w) {
                ціле im_row = h_offset + h * stride;
                ціле im_col = w_offset + w * stride;
                ціле col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

