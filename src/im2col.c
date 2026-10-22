#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, පූර්ණ_සංඛ්‍යා height, පූර්ණ_සංඛ්‍යා width, පූර්ණ_සංඛ්‍යා channels,
                        පූර්ණ_සංඛ්‍යා row, පූර්ණ_සංඛ්‍යා col, පූර්ණ_සංඛ්‍යා channel, පූර්ණ_සංඛ්‍යා pad)
{
    row -= pad;
    col -= pad;

    නම් (row < 0 || col < 0 ||
        row >= height || col >= width) ආපසු 0;
    ආපසු im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
void im2col_cpu(float* data_im,
     පූර්ණ_සංඛ්‍යා channels,  පූර්ණ_සංඛ්‍යා height,  පූර්ණ_සංඛ්‍යා width,
     පූර්ණ_සංඛ්‍යා ksize,  පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා pad, float* data_col) 
{
    පූර්ණ_සංඛ්‍යා c,h,w;
    පූර්ණ_සංඛ්‍යා height_col = (height + 2*pad - ksize) / stride + 1;
    පූර්ණ_සංඛ්‍යා width_col = (width + 2*pad - ksize) / stride + 1;

    පූර්ණ_සංඛ්‍යා channels_col = channels * ksize * ksize;
    සඳහා (c = 0; c < channels_col; ++c) {
        පූර්ණ_සංඛ්‍යා w_offset = c % ksize;
        පූර්ණ_සංඛ්‍යා h_offset = (c / ksize) % ksize;
        පූර්ණ_සංඛ්‍යා c_im = c / ksize / ksize;
        සඳහා (h = 0; h < height_col; ++h) {
            සඳහා (w = 0; w < width_col; ++w) {
                පූර්ණ_සංඛ්‍යා im_row = h_offset + h * stride;
                පූර්ණ_සංඛ්‍යා im_col = w_offset + w * stride;
                පූර්ණ_සංඛ්‍යා col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

