#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, ཧྲིལ height, ཧྲིལ width, ཧྲིལ channels,
                        ཧྲིལ row, ཧྲིལ col, ཧྲིལ channel, ཧྲིལ pad)
{
    row -= pad;
    col -= pad;

    གལ་ཏེ (row < 0 || col < 0 ||
        row >= height || col >= width) ཕྱིར 0;
    ཕྱིར im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     ཧྲིལ channels,  ཧྲིལ height,  ཧྲིལ width,
     ཧྲིལ ksize,  ཧྲིལ stride, ཧྲིལ pad, float* data_col) 
{
    ཧྲིལ c,h,w;
    ཧྲིལ height_col = (height + 2*pad - ksize) / stride + 1;
    ཧྲིལ width_col = (width + 2*pad - ksize) / stride + 1;

    ཧྲིལ channels_col = channels * ksize * ksize;
    ཆེད་དུ (c = 0; c < channels_col; ++c) {
        ཧྲིལ w_offset = c % ksize;
        ཧྲིལ h_offset = (c / ksize) % ksize;
        ཧྲིལ c_im = c / ksize / ksize;
        ཆེད་དུ (h = 0; h < height_col; ++h) {
            ཆེད་དུ (w = 0; w < width_col; ++w) {
                ཧྲིལ im_row = h_offset + h * stride;
                ཧྲིལ im_col = w_offset + w * stride;
                ཧྲིལ col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

