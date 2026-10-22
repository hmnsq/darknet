#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>
शून्य col2im_add_pixel(दशांश *im, पूर्णांक height, पूर्णांक width, पूर्णांक channels,
                        पूर्णांक row, पूर्णांक col, पूर्णांक channel, पूर्णांक pad, दशांश val)
{
    row -= pad;
    col -= pad;

    जर (row < 0 || col < 0 ||
        row >= height || col >= width) परत;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
शून्य col2im_cpu(दशांश* data_col,
         पूर्णांक channels,  पूर्णांक height,  पूर्णांक width,
         पूर्णांक ksize,  पूर्णांक stride, पूर्णांक pad, दशांश* data_im) 
{
    पूर्णांक c,h,w;
    पूर्णांक height_col = (height + 2*pad - ksize) / stride + 1;
    पूर्णांक width_col = (width + 2*pad - ksize) / stride + 1;

    पूर्णांक channels_col = channels * ksize * ksize;
    साठी (c = 0; c < channels_col; ++c) {
        पूर्णांक w_offset = c % ksize;
        पूर्णांक h_offset = (c / ksize) % ksize;
        पूर्णांक c_im = c / ksize / ksize;
        साठी (h = 0; h < height_col; ++h) {
            साठी (w = 0; w < width_col; ++w) {
                पूर्णांक im_row = h_offset + h * stride;
                पूर्णांक im_col = w_offset + w * stride;
                पूर्णांक col_index = (c * height_col + h) * width_col + w;
                द्विगुण val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

