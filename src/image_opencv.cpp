#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    целое x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    целое step = disp->widthStep;
    для(y = 0; y < im.h; ++y){
        для(x = 0; x < im.w; ++x){
            для(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    вернуть disp;
}

image ipl_to_image(IplImage* src)
{
    целое h = src->height;
    целое w = src->width;
    целое c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    целое step = src->widthStep;
    целое i, j, k;

    для(i = 0; i < h; ++i){
        для(k= 0; k < c; ++k){
            для(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    вернуть im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    если(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    вернуть m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    вернуть im;
}

void *open_video_stream(const char *f, целое c, целое w, целое h, целое fps)
{
    VideoCapture *cap;
    если(f) cap = new VideoCapture(f);
    иначе cap = new VideoCapture(c);
    если(!cap->isOpened()) вернуть 0;
    если(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    если(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    если(fps) cap->set(CV_CAP_PROP_FPS, w);
    вернуть (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    если(m.empty()) вернуть make_empty_image(0,0,0);
    вернуть mat_to_image(m);
}

image load_image_cv(char *filename, целое channels)
{
    целое flag = -1;
    если (channels == 0) flag = -1;
    иначе если (channels == 1) flag = 0;
    иначе если (channels == 3) flag = 1;
    иначе {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    если(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        вернуть make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    вернуть im;
}

целое show_image_cv(image im, const char* name, целое ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    целое c = waitKey(ms);
    если (c != -1) c = c%256;
    вернуть c;
}

void make_window(char *name, целое w, целое h, целое fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    если (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } иначе {
        resizeWindow(name, w, h);
        если(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
