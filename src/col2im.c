#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>
शून्य col2im_add_pixel(भिन्न *im, पूर्णाङ्क height, पूर्णाङ्क width, पूर्णाङ्क channels,
                        पूर्णाङ्क row, पूर्णाङ्क col, पूर्णाङ्क channel, पूर्णाङ्क pad, भिन्न val)
{
    row -= pad;
    col -= pad;

    यदि (row < 0 || col < 0 ||
        row >= height || col >= width) प्रत्यागम;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
शून्य col2im_cpu(भिन्न* data_col,
         पूर्णाङ्क channels,  पूर्णाङ्क height,  पूर्णाङ्क width,
         पूर्णाङ्क ksize,  पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न* data_im) 
{
    पूर्णाङ्क c,h,w;
    पूर्णाङ्क height_col = (height + 2*pad - ksize) / stride + 1;
    पूर्णाङ्क width_col = (width + 2*pad - ksize) / stride + 1;

    पूर्णाङ्क channels_col = channels * ksize * ksize;
    प्रति (c = 0; c < channels_col; ++c) {
        पूर्णाङ्क w_offset = c % ksize;
        पूर्णाङ्क h_offset = (c / ksize) % ksize;
        पूर्णाङ्क c_im = c / ksize / ksize;
        प्रति (h = 0; h < height_col; ++h) {
            प्रति (w = 0; w < width_col; ++w) {
                पूर्णाङ्क im_row = h_offset + h * stride;
                पूर्णाङ्क im_col = w_offset + w * stride;
                पूर्णाङ्क col_index = (c * height_col + h) * width_col + w;
                द्विगुण val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

