#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, 정수 height, 정수 width, 정수 channels,
                        정수 row, 정수 col, 정수 channel, 정수 pad)
{
    row -= pad;
    col -= pad;

    만약 (row < 0 || col < 0 ||
        row >= height || col >= width) 반환 0;
    반환 im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     정수 channels,  정수 height,  정수 width,
     정수 ksize,  정수 stride, 정수 pad, float* data_col) 
{
    정수 c,h,w;
    정수 height_col = (height + 2*pad - ksize) / stride + 1;
    정수 width_col = (width + 2*pad - ksize) / stride + 1;

    정수 channels_col = channels * ksize * ksize;
    위해 (c = 0; c < channels_col; ++c) {
        정수 w_offset = c % ksize;
        정수 h_offset = (c / ksize) % ksize;
        정수 c_im = c / ksize / ksize;
        위해 (h = 0; h < height_col; ++h) {
            위해 (w = 0; w < width_col; ++w) {
                정수 im_row = h_offset + h * stride;
                정수 im_col = w_offset + w * stride;
                정수 col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

