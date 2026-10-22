#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, จำนวนเต็ม height, จำนวนเต็ม width, จำนวนเต็ม channels,
                        จำนวนเต็ม row, จำนวนเต็ม col, จำนวนเต็ม channel, จำนวนเต็ม pad, float val)
{
    row -= pad;
    col -= pad;

    ถ้า (row < 0 || col < 0 ||
        row >= height || col >= width) คืน;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         จำนวนเต็ม channels,  จำนวนเต็ม height,  จำนวนเต็ม width,
         จำนวนเต็ม ksize,  จำนวนเต็ม stride, จำนวนเต็ม pad, float* data_im) 
{
    จำนวนเต็ม c,h,w;
    จำนวนเต็ม height_col = (height + 2*pad - ksize) / stride + 1;
    จำนวนเต็ม width_col = (width + 2*pad - ksize) / stride + 1;

    จำนวนเต็ม channels_col = channels * ksize * ksize;
    สำหรับ (c = 0; c < channels_col; ++c) {
        จำนวนเต็ม w_offset = c % ksize;
        จำนวนเต็ม h_offset = (c / ksize) % ksize;
        จำนวนเต็ม c_im = c / ksize / ksize;
        สำหรับ (h = 0; h < height_col; ++h) {
            สำหรับ (w = 0; w < width_col; ++w) {
                จำนวนเต็ม im_row = h_offset + h * stride;
                จำนวนเต็ม im_col = w_offset + w * stride;
                จำนวนเต็ม col_index = (c * height_col + h) * width_col + w;
                dviguu_na val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

