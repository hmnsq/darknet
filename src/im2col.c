#अन्तर्भाव "im2col.h"
#अन्तर्भाव <stdio.h>
भिन्न im2col_get_pixel(भिन्न *im, पूर्णाङ्क height, पूर्णाङ्क width, पूर्णाङ्क channels,
                        पूर्णाङ्क row, पूर्णाङ्क col, पूर्णाङ्क channel, पूर्णाङ्क pad)
{
    row -= pad;
    col -= pad;

    यदि (row < 0 || col < 0 ||
        row >= height || col >= width) फर्काऊ 0;
    फर्काऊ im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
शून्य im2col_cpu(भिन्न* data_im,
     पूर्णाङ्क channels,  पूर्णाङ्क height,  पूर्णाङ्क width,
     पूर्णाङ्क ksize,  पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न* data_col) 
{
    पूर्णाङ्क c,h,w;
    पूर्णाङ्क height_col = (height + 2*pad - ksize) / stride + 1;
    पूर्णाङ्क width_col = (width + 2*pad - ksize) / stride + 1;

    पूर्णाङ्क channels_col = channels * ksize * ksize;
    लागि (c = 0; c < channels_col; ++c) {
        पूर्णाङ्क w_offset = c % ksize;
        पूर्णाङ्क h_offset = (c / ksize) % ksize;
        पूर्णाङ्क c_im = c / ksize / ksize;
        लागि (h = 0; h < height_col; ++h) {
            लागि (w = 0; w < width_col; ++w) {
                पूर्णाङ्क im_row = h_offset + h * stride;
                पूर्णाङ्क im_col = w_offset + w * stride;
                पूर्णाङ्क col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

