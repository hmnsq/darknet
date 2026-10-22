#समावेश "im2col.h"
#समावेश <stdio.h>
भग्न im2col_get_pixel(भग्न *im, पूर्णांक height, पूर्णांक width, पूर्णांक channels,
                        पूर्णांक row, पूर्णांक col, पूर्णांक channel, पूर्णांक pad)
{
    row -= pad;
    col -= pad;

    अगर (row < 0 || col < 0 ||
        row >= height || col >= width) वापस 0;
    वापस im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
व्योम im2col_cpu(भग्न* तथ्य_im,
     पूर्णांक channels,  पूर्णांक height,  पूर्णांक width,
     पूर्णांक ksize,  पूर्णांक stride, पूर्णांक pad, भग्न* तथ्य_col) 
{
    पूर्णांक c,h,w;
    पूर्णांक height_col = (height + 2*pad - ksize) / stride + 1;
    पूर्णांक width_col = (width + 2*pad - ksize) / stride + 1;

    पूर्णांक channels_col = channels * ksize * ksize;
    क्रम (c = 0; c < channels_col; ++c) {
        पूर्णांक w_offset = c % ksize;
        पूर्णांक h_offset = (c / ksize) % ksize;
        पूर्णांक c_im = c / ksize / ksize;
        क्रम (h = 0; h < height_col; ++h) {
            क्रम (w = 0; w < width_col; ++w) {
                पूर्णांक im_row = h_offset + h * stride;
                पूर्णांक im_col = w_offset + w * stride;
                पूर्णांक col_सूचकांक = (c * height_col + h) * width_col + w;
                तथ्य_col[col_सूचकांक] = im2col_get_pixel(तथ्य_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

