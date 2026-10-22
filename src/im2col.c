#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, цео height, цео width, цео channels,
                        цео row, цео col, цео channel, цео pad)
{
    row -= pad;
    col -= pad;

    ако (row < 0 || col < 0 ||
        row >= height || col >= width) врати 0;
    врати im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     цео channels,  цео height,  цео width,
     цео ksize,  цео stride, цео pad, float* data_col) 
{
    цео c,h,w;
    цео height_col = (height + 2*pad - ksize) / stride + 1;
    цео width_col = (width + 2*pad - ksize) / stride + 1;

    цео channels_col = channels * ksize * ksize;
    за (c = 0; c < channels_col; ++c) {
        цео w_offset = c % ksize;
        цео h_offset = (c / ksize) % ksize;
        цео c_im = c / ksize / ksize;
        за (h = 0; h < height_col; ++h) {
            за (w = 0; w < width_col; ++w) {
                цео im_row = h_offset + h * stride;
                цео im_col = w_offset + w * stride;
                цео col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

