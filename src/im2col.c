#شامل "im2col.h"
#شامل <stdio.h>
اعشاری im2col_get_pixel(اعشاری *im, صحیح height, صحیح width, صحیح channels,
                        صحیح row, صحیح col, صحیح channel, صحیح pad)
{
    row -= pad;
    col -= pad;

    اگر (row < 0 || col < 0 ||
        row >= height || col >= width) بازگشت 0;
    بازگشت im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
تهی im2col_cpu(اعشاری* data_im,
     صحیح channels,  صحیح height,  صحیح width,
     صحیح ksize,  صحیح stride, صحیح pad, اعشاری* data_col) 
{
    صحیح c,h,w;
    صحیح height_col = (height + 2*pad - ksize) / stride + 1;
    صحیح width_col = (width + 2*pad - ksize) / stride + 1;

    صحیح channels_col = channels * ksize * ksize;
    برای (c = 0; c < channels_col; ++c) {
        صحیح w_offset = c % ksize;
        صحیح h_offset = (c / ksize) % ksize;
        صحیح c_im = c / ksize / ksize;
        برای (h = 0; h < height_col; ++h) {
            برای (w = 0; w < width_col; ++w) {
                صحیح im_row = h_offset + h * stride;
                صحیح im_col = w_offset + w * stride;
                صحیح col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

