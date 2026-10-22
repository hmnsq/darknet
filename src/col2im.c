#include <stdio.h>
#include <math.h>
void col2im_add_pixel(float *im, ຈຳນວນເຕັມ height, ຈຳນວນເຕັມ width, ຈຳນວນເຕັມ channels,
                        ຈຳນວນເຕັມ row, ຈຳນວນເຕັມ col, ຈຳນວນເຕັມ channel, ຈຳນວນເຕັມ pad, float val)
{
    row -= pad;
    col -= pad;

    ຖ້າ (row < 0 || col < 0 ||
        row >= height || col >= width) ສົ່ງຄືນ;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
void col2im_cpu(float* data_col,
         ຈຳນວນເຕັມ channels,  ຈຳນວນເຕັມ height,  ຈຳນວນເຕັມ width,
         ຈຳນວນເຕັມ ksize,  ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ pad, float* data_im) 
{
    ຈຳນວນເຕັມ c,h,w;
    ຈຳນວນເຕັມ height_col = (height + 2*pad - ksize) / stride + 1;
    ຈຳນວນເຕັມ width_col = (width + 2*pad - ksize) / stride + 1;

    ຈຳນວນເຕັມ channels_col = channels * ksize * ksize;
    ສຳລັບ (c = 0; c < channels_col; ++c) {
        ຈຳນວນເຕັມ w_offset = c % ksize;
        ຈຳນວນເຕັມ h_offset = (c / ksize) % ksize;
        ຈຳນວນເຕັມ c_im = c / ksize / ksize;
        ສຳລັບ (h = 0; h < height_col; ++h) {
            ສຳລັບ (w = 0; w < width_col; ++w) {
                ຈຳນວນເຕັມ im_row = h_offset + h * stride;
                ຈຳນວນເຕັມ im_col = w_offset + w * stride;
                ຈຳນວນເຕັມ col_index = (c * height_col + h) * width_col + w;
                dviguu_na val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

