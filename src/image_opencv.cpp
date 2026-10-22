#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    整数 x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    整数 step = disp->widthStep;
    为(y = 0; y < im.h; ++y){
        为(x = 0; x < im.w; ++x){
            为(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    返回 disp;
}

image ipl_to_image(IplImage* src)
{
    整数 h = src->height;
    整数 w = src->width;
    整数 c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    整数 step = src->widthStep;
    整数 i, j, k;

    为(i = 0; i < h; ++i){
        为(k= 0; k < c; ++k){
            为(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    返回 im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    如果(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    返回 m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    返回 im;
}

void *open_video_stream(const char *f, 整数 c, 整数 w, 整数 h, 整数 fps)
{
    VideoCapture *cap;
    如果(f) cap = new VideoCapture(f);
    否则 cap = new VideoCapture(c);
    如果(!cap->isOpened()) 返回 0;
    如果(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    如果(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    如果(fps) cap->set(CV_CAP_PROP_FPS, w);
    返回 (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    如果(m.empty()) 返回 make_empty_image(0,0,0);
    返回 mat_to_image(m);
}

image load_image_cv(char *filename, 整数 channels)
{
    整数 flag = -1;
    如果 (channels == 0) flag = -1;
    否则 如果 (channels == 1) flag = 0;
    否则 如果 (channels == 3) flag = 1;
    否则 {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    如果(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        返回 make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    返回 im;
}

整数 show_image_cv(image im, const char* name, 整数 ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    整数 c = waitKey(ms);
    如果 (c != -1) c = c%256;
    返回 c;
}

void make_window(char *name, 整数 w, 整数 h, 整数 fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    如果 (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } 否则 {
        resizeWindow(name, w, h);
        如果(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
