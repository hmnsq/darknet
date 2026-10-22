#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, 整数 height, 整数 width, 整数 channels,
                        整数 row, 整数 col, 整数 channel, 整数 pad)
{
    row -= pad;
    col -= pad;

    如果 (row < 0 || col < 0 ||
        row >= height || col >= width) 返回 0;
    返回 im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     整数 channels,  整数 height,  整数 width,
     整数 ksize,  整数 stride, 整数 pad, float* data_col) 
{
    整数 c,h,w;
    整数 height_col = (height + 2*pad - ksize) / stride + 1;
    整数 width_col = (width + 2*pad - ksize) / stride + 1;

    整数 channels_col = channels * ksize * ksize;
    为 (c = 0; c < channels_col; ++c) {
        整数 w_offset = c % ksize;
        整数 h_offset = (c / ksize) % ksize;
        整数 c_im = c / ksize / ksize;
        为 (h = 0; h < height_col; ++h) {
            为 (w = 0; w < width_col; ++w) {
                整数 im_row = h_offset + h * stride;
                整数 im_col = w_offset + w * stride;
                整数 col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

