#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
ಭಿನ್ನ im2col_get_pixel(ಭಿನ್ನ *im, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ channels,
                        ಪೂರ್ಣಾಙ್ಕ row, ಪೂರ್ಣಾಙ್ಕ col, ಪೂರ್ಣಾಙ್ಕ channel, ಪೂರ್ಣಾಙ್ಕ pad)
{
    row -= pad;
    col -= pad;

    ಯದಿ (row < 0 || col < 0 ||
        row >= height || col >= width) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
ಶೂನ್ಯ im2col_cpu(ಭಿನ್ನ* data_im,
     ಪೂರ್ಣಾಙ್ಕ channels,  ಪೂರ್ಣಾಙ್ಕ height,  ಪೂರ್ಣಾಙ್ಕ width,
     ಪೂರ್ಣಾಙ್ಕ ksize,  ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ಭಿನ್ನ* data_col) 
{
    ಪೂರ್ಣಾಙ್ಕ c,h,w;
    ಪೂರ್ಣಾಙ್ಕ height_col = (height + 2*pad - ksize) / stride + 1;
    ಪೂರ್ಣಾಙ್ಕ width_col = (width + 2*pad - ksize) / stride + 1;

    ಪೂರ್ಣಾಙ್ಕ channels_col = channels * ksize * ksize;
    ಪ್ರತಿ (c = 0; c < channels_col; ++c) {
        ಪೂರ್ಣಾಙ್ಕ w_offset = c % ksize;
        ಪೂರ್ಣಾಙ್ಕ h_offset = (c / ksize) % ksize;
        ಪೂರ್ಣಾಙ್ಕ c_im = c / ksize / ksize;
        ಪ್ರತಿ (h = 0; h < height_col; ++h) {
            ಪ್ರತಿ (w = 0; w < width_col; ++w) {
                ಪೂರ್ಣಾಙ್ಕ im_row = h_offset + h * stride;
                ಪೂರ್ಣಾಙ್ಕ im_col = w_offset + w * stride;
                ಪೂರ್ಣಾಙ್ಕ col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

