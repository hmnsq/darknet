#شامل <stdio.h>
#شامل <math.h>
تهی col2im_add_pixel(اعشاری *im, صحیح height, صحیح width, صحیح channels,
                        صحیح row, صحیح col, صحیح channel, صحیح pad, اعشاری val)
{
    row -= pad;
    col -= pad;

    اگر (row < 0 || col < 0 ||
        row >= height || col >= width) بازگشت;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
تهی col2im_cpu(اعشاری* data_col,
         صحیح channels,  صحیح height,  صحیح width,
         صحیح ksize,  صحیح stride, صحیح pad, اعشاری* data_im) 
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
                دوگانه val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

