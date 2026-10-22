#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, цяло height, цяло width, цяло channels,
                        цяло row, цяло col, цяло channel, цяло pad, float val)
{
    row -= pad;
    col -= pad;

    ако (row < 0 || col < 0 ||
        row >= height || col >= width) върни;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         цяло channels,  цяло height,  цяло width,
         цяло ksize,  цяло stride, цяло pad, float* data_im) 
{
    цяло c,h,w;
    цяло height_col = (height + 2*pad - ksize) / stride + 1;
    цяло width_col = (width + 2*pad - ksize) / stride + 1;

    цяло channels_col = channels * ksize * ksize;
    за (c = 0; c < channels_col; ++c) {
        цяло w_offset = c % ksize;
        цяло h_offset = (c / ksize) % ksize;
        цяло c_im = c / ksize / ksize;
        за (h = 0; h < height_col; ++h) {
            за (w = 0; w < width_col; ++w) {
                цяло im_row = h_offset + h * stride;
                цяло im_col = w_offset + w * stride;
                цяло col_index = (c * height_col + h) * width_col + w;
                double val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

