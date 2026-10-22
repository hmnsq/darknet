#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, ကိန်းပြည့် height, ကိန်းပြည့် width, ကိန်းပြည့် channels,
                        ကိန်းပြည့် row, ကိန်းပြည့် col, ကိန်းပြည့် channel, ကိန်းပြည့် pad)
{
    row -= pad;
    col -= pad;

    အကယ်၍ (row < 0 || col < 0 ||
        row >= height || col >= width) ပြန်ပေး 0;
    ပြန်ပေး im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     ကိန်းပြည့် channels,  ကိန်းပြည့် height,  ကိန်းပြည့် width,
     ကိန်းပြည့် ksize,  ကိန်းပြည့် stride, ကိန်းပြည့် pad, float* data_col) 
{
    ကိန်းပြည့် c,h,w;
    ကိန်းပြည့် height_col = (height + 2*pad - ksize) / stride + 1;
    ကိန်းပြည့် width_col = (width + 2*pad - ksize) / stride + 1;

    ကိန်းပြည့် channels_col = channels * ksize * ksize;
    အတွက် (c = 0; c < channels_col; ++c) {
        ကိန်းပြည့် w_offset = c % ksize;
        ကိန်းပြည့် h_offset = (c / ksize) % ksize;
        ကိန်းပြည့် c_im = c / ksize / ksize;
        အတွက် (h = 0; h < height_col; ++h) {
            အတွက် (w = 0; w < width_col; ++w) {
                ကိန်းပြည့် im_row = h_offset + h * stride;
                ကိန်းပြည့် im_col = w_offset + w * stride;
                ကိန်းပြည့် col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

