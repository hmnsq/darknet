#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>
শূন্য col2im_add_pixel(দশমিক *im, পূর্ণসংখ্যা height, পূর্ণসংখ্যা width, পূর্ণসংখ্যা channels,
                        পূর্ণসংখ্যা row, পূর্ণসংখ্যা col, পূর্ণসংখ্যা channel, পূর্ণসংখ্যা pad, দশমিক val)
{
    row -= pad;
    col -= pad;

    যদি (row < 0 || col < 0 ||
        row >= height || col >= width) ফেরত;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
শূন্য col2im_cpu(দশমিক* তথ্য_col,
         পূর্ণসংখ্যা channels,  পূর্ণসংখ্যা height,  পূর্ণসংখ্যা width,
         পূর্ণসংখ্যা ksize,  পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, দশমিক* তথ্য_im) 
{
    পূর্ণসংখ্যা c,h,w;
    পূর্ণসংখ্যা height_col = (height + 2*pad - ksize) / stride + 1;
    পূর্ণসংখ্যা width_col = (width + 2*pad - ksize) / stride + 1;

    পূর্ণসংখ্যা channels_col = channels * ksize * ksize;
    জন্য (c = 0; c < channels_col; ++c) {
        পূর্ণসংখ্যা w_offset = c % ksize;
        পূর্ণসংখ্যা h_offset = (c / ksize) % ksize;
        পূর্ণসংখ্যা c_im = c / ksize / ksize;
        জন্য (h = 0; h < height_col; ++h) {
            জন্য (w = 0; w < width_col; ++w) {
                পূর্ণসংখ্যা im_row = h_offset + h * stride;
                পূর্ণসংখ্যা im_col = w_offset + w * stride;
                পূর্ণসংখ্যা col_সূচক = (c * height_col + h) * width_col + w;
                দ্বিগুণ val = তথ্য_col[col_সূচক];
                col2im_add_pixel(তথ্য_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

