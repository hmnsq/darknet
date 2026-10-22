#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, ཧྲིལ height, ཧྲིལ width, ཧྲིལ channels,
                        ཧྲིལ row, ཧྲིལ col, ཧྲིལ channel, ཧྲིལ pad, float val)
{
    row -= pad;
    col -= pad;

    གལ་སྲིད (row < 0 || col < 0 ||
        row >= height || col >= width) ལོག;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         ཧྲིལ channels,  ཧྲིལ height,  ཧྲིལ width,
         ཧྲིལ ksize,  ཧྲིལ stride, ཧྲིལ pad, float* data_im) 
{
    ཧྲིལ c,h,w;
    ཧྲིལ height_col = (height + 2*pad - ksize) / stride + 1;
    ཧྲིལ width_col = (width + 2*pad - ksize) / stride + 1;

    ཧྲིལ channels_col = channels * ksize * ksize;
    དོན་ལུ (c = 0; c < channels_col; ++c) {
        ཧྲིལ w_offset = c % ksize;
        ཧྲིལ h_offset = (c / ksize) % ksize;
        ཧྲིལ c_im = c / ksize / ksize;
        དོན་ལུ (h = 0; h < height_col; ++h) {
            དོན་ལུ (w = 0; w < width_col; ++w) {
                ཧྲིལ im_row = h_offset + h * stride;
                ཧྲིལ im_col = w_offset + w * stride;
                ཧྲིལ col_index = (c * height_col + h) * width_col + w;
                dviguu_na val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

