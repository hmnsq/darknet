#അന്തര്ഭാവ "im2col.h"
#അന്തര്ഭാവ <stdio.h>
ദശാംശം im2col_get_pixel(ദശാംശം *im, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ channels,
                        പൂർണ്ണസംഖ്യ row, പൂർണ്ണസംഖ്യ col, പൂർണ്ണസംഖ്യ channel, പൂർണ്ണസംഖ്യ pad)
{
    row -= pad;
    col -= pad;

    എങ്കിൽ (row < 0 || col < 0 ||
        row >= height || col >= width) തിരികെ 0;
    തിരികെ im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
ശൂന്യം im2col_cpu(ദശാംശം* data_im,
     പൂർണ്ണസംഖ്യ channels,  പൂർണ്ണസംഖ്യ height,  പൂർണ്ണസംഖ്യ width,
     പൂർണ്ണസംഖ്യ ksize,  പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ദശാംശം* data_col) 
{
    പൂർണ്ണസംഖ്യ c,h,w;
    പൂർണ്ണസംഖ്യ height_col = (height + 2*pad - ksize) / stride + 1;
    പൂർണ്ണസംഖ്യ width_col = (width + 2*pad - ksize) / stride + 1;

    പൂർണ്ണസംഖ്യ channels_col = channels * ksize * ksize;
    വേണ്ടി (c = 0; c < channels_col; ++c) {
        പൂർണ്ണസംഖ്യ w_offset = c % ksize;
        പൂർണ്ണസംഖ്യ h_offset = (c / ksize) % ksize;
        പൂർണ്ണസംഖ്യ c_im = c / ksize / ksize;
        വേണ്ടി (h = 0; h < height_col; ++h) {
            വേണ്ടി (w = 0; w < width_col; ++w) {
                പൂർണ്ണസംഖ്യ im_row = h_offset + h * stride;
                പൂർണ്ണസംഖ്യ im_col = w_offset + w * stride;
                പൂർണ്ണസംഖ്യ col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

