#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <math.h>
வெறுமை col2im_add_pixel(தசம *im, முழு height, முழு width, முழு channels,
                        முழு row, முழு col, முழு channel, முழு pad, தசம val)
{
    row -= pad;
    col -= pad;

    எனில் (row < 0 || col < 0 ||
        row >= height || col >= width) திரும்பு;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
வெறுமை col2im_cpu(தசம* data_col,
         முழு channels,  முழு height,  முழு width,
         முழு ksize,  முழு stride, முழு pad, தசம* data_im) 
{
    முழு c,h,w;
    முழு height_col = (height + 2*pad - ksize) / stride + 1;
    முழு width_col = (width + 2*pad - ksize) / stride + 1;

    முழு channels_col = channels * ksize * ksize;
    க்கு (c = 0; c < channels_col; ++c) {
        முழு w_offset = c % ksize;
        முழு h_offset = (c / ksize) % ksize;
        முழு c_im = c / ksize / ksize;
        க்கு (h = 0; h < height_col; ++h) {
            க்கு (w = 0; w < width_col; ++w) {
                முழு im_row = h_offset + h * stride;
                முழு im_col = w_offset + w * stride;
                முழு col_index = (c * height_col + h) * width_col + w;
                இரட்டை val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

