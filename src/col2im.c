#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, целое height, целое width, целое channels,
                        целое row, целое col, целое channel, целое pad, float val)
{
    row -= pad;
    col -= pad;

    если (row < 0 || col < 0 ||
        row >= height || col >= width) вернуть;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         целое channels,  целое height,  целое width,
         целое ksize,  целое stride, целое pad, float* data_im) 
{
    целое c,h,w;
    целое height_col = (height + 2*pad - ksize) / stride + 1;
    целое width_col = (width + 2*pad - ksize) / stride + 1;

    целое channels_col = channels * ksize * ksize;
    для (c = 0; c < channels_col; ++c) {
        целое w_offset = c % ksize;
        целое h_offset = (c / ksize) % ksize;
        целое c_im = c / ksize / ksize;
        для (h = 0; h < height_col; ++h) {
            для (w = 0; w < width_col; ++w) {
                целое im_row = h_offset + h * stride;
                целое im_col = w_offset + w * stride;
                целое col_index = (c * height_col + h) * width_col + w;
                double val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

