#અન્તર્ભાવ "im2col.h"
#અન્તર્ભાવ <stdio.h>
દશાંશ im2col_get_pixel(દશાંશ *im, પૂર્ણાંક height, પૂર્ણાંક width, પૂર્ણાંક channels,
                        પૂર્ણાંક row, પૂર્ણાંક col, પૂર્ણાંક channel, પૂર્ણાંક pad)
{
    row -= pad;
    col -= pad;

    જો (row < 0 || col < 0 ||
        row >= height || col >= width) પાછા 0;
    પાછા im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
શૂન્ય im2col_cpu(દશાંશ* data_im,
     પૂર્ણાંક channels,  પૂર્ણાંક height,  પૂર્ણાંક width,
     પૂર્ણાંક ksize,  પૂર્ણાંક stride, પૂર્ણાંક pad, દશાંશ* data_col) 
{
    પૂર્ણાંક c,h,w;
    પૂર્ણાંક height_col = (height + 2*pad - ksize) / stride + 1;
    પૂર્ણાંક width_col = (width + 2*pad - ksize) / stride + 1;

    પૂર્ણાંક channels_col = channels * ksize * ksize;
    માટે (c = 0; c < channels_col; ++c) {
        પૂર્ણાંક w_offset = c % ksize;
        પૂર્ણાંક h_offset = (c / ksize) % ksize;
        પૂર્ણાંક c_im = c / ksize / ksize;
        માટે (h = 0; h < height_col; ++h) {
            માટે (w = 0; w < width_col; ++w) {
                પૂર્ણાંક im_row = h_offset + h * stride;
                પૂર્ણાંક im_col = w_offset + w * stride;
                પૂર્ણાંક col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

