#include "box.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

ကိန်းပြည့် nms_comparator(const void *pa, const void *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    float diff = 0;
    အကယ်၍(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } မဟုတ်ပါက {
        diff = a.objectness - b.objectness;
    }
    အကယ်၍(diff < 0) ပြန်ပေး 1;
    မဟုတ်ပါက အကယ်၍(diff > 0) ပြန်ပေး -1;
    ပြန်ပေး 0;
}

void do_nms_obj(detection *dets, ကိန်းပြည့် total, ကိန်းပြည့် classes, float thresh)
{
    ကိန်းပြည့် i, j, k;
    k = total-1;
    အတွက်(i = 0; i <= k; ++i){
        အကယ်၍(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    အတွက်(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, parimaa_na(detection), nms_comparator);
    အတွက်(i = 0; i < total; ++i){
        အကယ်၍(dets[i].objectness == 0) continue;
        box a = dets[i].bbox;
        အတွက်(j = i+1; j < total; ++j){
            အကယ်၍(dets[j].objectness == 0) continue;
            box b = dets[j].bbox;
            အကယ်၍ (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                အတွက်(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


void do_nms_sort(detection *dets, ကိန်းပြည့် total, ကိန်းပြည့် classes, float thresh)
{
    ကိန်းပြည့် i, j, k;
    k = total-1;
    အတွက်(i = 0; i <= k; ++i){
        အကယ်၍(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    အတွက်(k = 0; k < classes; ++k){
        အတွက်(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, parimaa_na(detection), nms_comparator);
        အတွက်(i = 0; i < total; ++i){
            အကယ်၍(dets[i].prob[k] == 0) continue;
            box a = dets[i].bbox;
            အတွက်(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                အကယ်၍ (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(float *f, ကိန်းပြည့် stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    ပြန်ပေး b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    float l1 = a.x - a.w/2;
    float l2 = b.x - b.w/2;
    အကယ်၍ (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    float r1 = a.x + a.w/2;
    float r2 = b.x + b.w/2;
    အကယ်၍(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    အကယ်၍ (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    အကယ်၍ (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    float t1 = a.y - a.h/2;
    float t2 = b.y - b.h/2;
    အကယ်၍ (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    float b1 = a.y + a.h/2;
    float b2 = b.y + b.h/2;
    အကယ်၍(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    အကယ်၍ (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    အကယ်၍ (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    ပြန်ပေး d;
}

float overlap(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1/2;
    float l2 = x2 - w2/2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1/2;
    float r2 = x2 + w2/2;
    float right = r1 < r2 ? r1 : r2;
    ပြန်ပေး right - left;
}

float box_intersection(box a, box b)
{
    float w = overlap(a.x, a.w, b.x, b.w);
    float h = overlap(a.y, a.h, b.y, b.h);
    အကယ်၍(w < 0 || h < 0) ပြန်ပေး 0;
    float area = w*h;
    ပြန်ပေး area;
}

float box_union(box a, box b)
{
    float i = box_intersection(a, b);
    float u = a.w*a.h + b.w*b.h - i;
    ပြန်ပေး u;
}

float box_iou(box a, box b)
{
    ပြန်ပေး box_intersection(a, b)/box_union(a, b);
}

float box_rmse(box a, box b)
{
    ပြန်ပေး sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    float w = overlap(a.x, a.w, b.x, b.w);
    float h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    ပြန်ပေး di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    ပြန်ပေး du;
}


void test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    float inter =  box_union(a, b);
    float xinter = box_union(dxa, b);
    float yinter = box_union(dya, b);
    float winter = box_union(dwa, b);
    float hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
void test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    float inter =  box_intersection(a, b);
    float xinter = box_intersection(dxa, b);
    float yinter = box_intersection(dya, b);
    float winter = box_intersection(dwa, b);
    float hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

void test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    float iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    float xiou = box_iou(dxa, b);
    float yiou = box_iou(dya, b);
    float wiou = box_iou(dwa, b);
    float hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    float u = box_union(a,b);
    float i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    အကယ်၍(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        ပြန်ပေး dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    ပြန်ပေး dd;
}


void do_nms(box *boxes, float **probs, ကိန်းပြည့် total, ကိန်းပြည့် classes, float thresh)
{
    ကိန်းပြည့် i, j, k;
    အတွက်(i = 0; i < total; ++i){
        ကိန်းပြည့် any = 0;
        အတွက်(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        အကယ်၍(!any) {
            continue;
        }
        အတွက်(j = i+1; j < total; ++j){
            အကယ်၍ (box_iou(boxes[i], boxes[j]) > thresh){
                အတွက်(k = 0; k < classes; ++k){
                    အကယ်၍ (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    မဟုတ်ပါက probs[j][k] = 0;
                }
            }
        }
    }
}

box encode_box(box b, box anchor)
{
    box encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = log2(b.w / anchor.w);
    encode.h = log2(b.h / anchor.h);
    ပြန်ပေး encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    ပြန်ပေး decode;
}
