#অন্তর্ভাৱ "im2col.h"
#অন্তর্ভাৱ <stdio.h>
ভিন্ন im2col_get_pixel(ভিন্ন *im, পূর্ণাঙ্ক height, পূর্ণাঙ্ক width, পূর্ণাঙ্ক channels,
                        পূর্ণাঙ্ক row, পূর্ণাঙ্ক col, পূর্ণাঙ্ক channel, পূর্ণাঙ্ক pad)
{
    row -= pad;
    col -= pad;

    যদি (row < 0 || col < 0 ||
        row >= height || col >= width) ঘূৰাই 0;
    ঘূৰাই im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
শূন্য im2col_cpu(ভিন্ন* data_im,
     পূর্ণাঙ্ক channels,  পূর্ণাঙ্ক height,  পূর্ণাঙ্ক width,
     পূর্ণাঙ্ক ksize,  পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ভিন্ন* data_col) 
{
    পূর্ণাঙ্ক c,h,w;
    পূর্ণাঙ্ক height_col = (height + 2*pad - ksize) / stride + 1;
    পূর্ণাঙ্ক width_col = (width + 2*pad - ksize) / stride + 1;

    পূর্ণাঙ্ক channels_col = channels * ksize * ksize;
    বাবে (c = 0; c < channels_col; ++c) {
        পূর্ণাঙ্ক w_offset = c % ksize;
        পূর্ণাঙ্ক h_offset = (c / ksize) % ksize;
        পূর্ণাঙ্ক c_im = c / ksize / ksize;
        বাবে (h = 0; h < height_col; ++h) {
            বাবে (w = 0; w < width_col; ++w) {
                পূর্ণাঙ্ক im_row = h_offset + h * stride;
                পূর্ণাঙ্ক im_col = w_offset + w * stride;
                পূর্ণাঙ্ক col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

