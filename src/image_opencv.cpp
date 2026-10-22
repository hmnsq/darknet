#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    정수 x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    정수 step = disp->widthStep;
    위해(y = 0; y < im.h; ++y){
        위해(x = 0; x < im.w; ++x){
            위해(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    반환 disp;
}

image ipl_to_image(IplImage* src)
{
    정수 h = src->height;
    정수 w = src->width;
    정수 c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    정수 step = src->widthStep;
    정수 i, j, k;

    위해(i = 0; i < h; ++i){
        위해(k= 0; k < c; ++k){
            위해(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    반환 im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    만약(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    반환 m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    반환 im;
}

void *open_video_stream(const char *f, 정수 c, 정수 w, 정수 h, 정수 fps)
{
    VideoCapture *cap;
    만약(f) cap = new VideoCapture(f);
    아니면 cap = new VideoCapture(c);
    만약(!cap->isOpened()) 반환 0;
    만약(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    만약(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    만약(fps) cap->set(CV_CAP_PROP_FPS, w);
    반환 (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    만약(m.empty()) 반환 make_empty_image(0,0,0);
    반환 mat_to_image(m);
}

image load_image_cv(char *filename, 정수 channels)
{
    정수 flag = -1;
    만약 (channels == 0) flag = -1;
    아니면 만약 (channels == 1) flag = 0;
    아니면 만약 (channels == 3) flag = 1;
    아니면 {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    만약(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        반환 make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    반환 im;
}

정수 show_image_cv(image im, const char* name, 정수 ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    정수 c = waitKey(ms);
    만약 (c != -1) c = c%256;
    반환 c;
}

void make_window(char *name, 정수 w, 정수 h, 정수 fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    만약 (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } 아니면 {
        resizeWindow(name, w, h);
        만약(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
