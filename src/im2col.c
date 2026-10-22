#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, ចំនួនគត់ height, ចំនួនគត់ width, ចំនួនគត់ channels,
                        ចំនួនគត់ row, ចំនួនគត់ col, ចំនួនគត់ channel, ចំនួនគត់ pad)
{
    row -= pad;
    col -= pad;

    ប្រសិនបើ (row < 0 || col < 0 ||
        row >= height || col >= width) ត្រឡប់ 0;
    ត្រឡប់ im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     ចំនួនគត់ channels,  ចំនួនគត់ height,  ចំនួនគត់ width,
     ចំនួនគត់ ksize,  ចំនួនគត់ stride, ចំនួនគត់ pad, float* data_col) 
{
    ចំនួនគត់ c,h,w;
    ចំនួនគត់ height_col = (height + 2*pad - ksize) / stride + 1;
    ចំនួនគត់ width_col = (width + 2*pad - ksize) / stride + 1;

    ចំនួនគត់ channels_col = channels * ksize * ksize;
    សម្រាប់ (c = 0; c < channels_col; ++c) {
        ចំនួនគត់ w_offset = c % ksize;
        ចំនួនគត់ h_offset = (c / ksize) % ksize;
        ចំនួនគត់ c_im = c / ksize / ksize;
        សម្រាប់ (h = 0; h < height_col; ++h) {
            សម្រាប់ (w = 0; w < width_col; ++w) {
                ចំនួនគត់ im_row = h_offset + h * stride;
                ចំនួនគត់ im_col = w_offset + w * stride;
                ចំនួនគត់ col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

