#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
ଶୂନ୍ୟ col2im_add_pixel(ଦଶମିକ *im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row, ପୂର୍ଣ୍ଣସଂଖ୍ୟା col, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ଦଶମିକ val)
{
    row -= pad;
    col -= pad;

    ଯଦି (row < 0 || col < 0 ||
        row >= height || col >= width) ଫେରନ୍ତୁ;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
ଶୂନ୍ୟ col2im_cpu(ଦଶମିକ* data_col,
         ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels,  ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
         ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize,  ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ଦଶମିକ* data_im) 
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c,h,w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା height_col = (height + 2*pad - ksize) / stride + 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା width_col = (width + 2*pad - ksize) / stride + 1;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels_col = channels * ksize * ksize;
    ପାଇଁ (c = 0; c < channels_col; ++c) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_offset = c % ksize;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_offset = (c / ksize) % ksize;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_im = c / ksize / ksize;
        ପାଇଁ (h = 0; h < height_col; ++h) {
            ପାଇଁ (w = 0; w < width_col; ++w) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା im_row = h_offset + h * stride;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା im_col = w_offset + w * stride;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା col_index = (c * height_col + h) * width_col + w;
                ଦ୍ୱିଗୁଣ val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

