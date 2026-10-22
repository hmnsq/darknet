#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

ପୂର୍ଣ୍ଣସଂଖ୍ୟା nms_comparator(ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    ଦଶମିକ diff = 0;
    ଯଦି(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } ଅନ୍ୟଥା {
        diff = a.objectness - b.objectness;
    }
    ଯଦି(diff < 0) ଫେରନ୍ତୁ 1;
    ଅନ୍ୟଥା ଯଦି(diff > 0) ଫେରନ୍ତୁ -1;
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ do_nms_obj(detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    k = total-1;
    ପାଇଁ(i = 0; i <= k; ++i){
        ଯଦି(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    ପାଇଁ(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, ଆକାର(detection), nms_comparator);
    ପାଇଁ(i = 0; i < total; ++i){
        ଯଦି(dets[i].objectness == 0) ଜାରି;
        box a = dets[i].bbox;
        ପାଇଁ(j = i+1; j < total; ++j){
            ଯଦି(dets[j].objectness == 0) ଜାରି;
            box b = dets[j].bbox;
            ଯଦି (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                ପାଇଁ(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


ଶୂନ୍ୟ do_nms_sort(detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    k = total-1;
    ପାଇଁ(i = 0; i <= k; ++i){
        ଯଦି(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    ପାଇଁ(k = 0; k < classes; ++k){
        ପାଇଁ(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, ଆକାର(detection), nms_comparator);
        ପାଇଁ(i = 0; i < total; ++i){
            ଯଦି(dets[i].prob[k] == 0) ଜାରି;
            box a = dets[i].bbox;
            ପାଇଁ(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                ଯଦି (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(ଦଶମିକ *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    ଫେରନ୍ତୁ b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    ଦଶମିକ l1 = a.x - a.w/2;
    ଦଶମିକ l2 = b.x - b.w/2;
    ଯଦି (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    ଦଶମିକ r1 = a.x + a.w/2;
    ଦଶମିକ r2 = b.x + b.w/2;
    ଯଦି(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    ଯଦି (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    ଯଦି (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    ଦଶମିକ t1 = a.y - a.h/2;
    ଦଶମିକ t2 = b.y - b.h/2;
    ଯଦି (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    ଦଶମିକ b1 = a.y + a.h/2;
    ଦଶମିକ b2 = b.y + b.h/2;
    ଯଦି(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    ଯଦି (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    ଯଦି (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    ଫେରନ୍ତୁ d;
}

ଦଶମିକ overlap(ଦଶମିକ x1, ଦଶମିକ w1, ଦଶମିକ x2, ଦଶମିକ w2)
{
    ଦଶମିକ l1 = x1 - w1/2;
    ଦଶମିକ l2 = x2 - w2/2;
    ଦଶମିକ ବାମ = l1 > l2 ? l1 : l2;
    ଦଶମିକ r1 = x1 + w1/2;
    ଦଶମିକ r2 = x2 + w2/2;
    ଦଶମିକ ଡାହାଣ = r1 < r2 ? r1 : r2;
    ଫେରନ୍ତୁ ଡାହାଣ - ବାମ;
}

ଦଶମିକ box_intersection(box a, box b)
{
    ଦଶମିକ w = overlap(a.x, a.w, b.x, b.w);
    ଦଶମିକ h = overlap(a.y, a.h, b.y, b.h);
    ଯଦି(w < 0 || h < 0) ଫେରନ୍ତୁ 0;
    ଦଶମିକ area = w*h;
    ଫେରନ୍ତୁ area;
}

ଦଶମିକ box_union(box a, box b)
{
    ଦଶମିକ i = box_intersection(a, b);
    ଦଶମିକ u = a.w*a.h + b.w*b.h - i;
    ଫେରନ୍ତୁ u;
}

ଦଶମିକ box_iou(box a, box b)
{
    ଫେରନ୍ତୁ box_intersection(a, b)/box_union(a, b);
}

ଦଶମିକ box_rmse(box a, box b)
{
    ଫେରନ୍ତୁ sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    ଦଶମିକ w = overlap(a.x, a.w, b.x, b.w);
    ଦଶମିକ h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    ଫେରନ୍ତୁ di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    ଫେରନ୍ତୁ du;
}


ଶୂନ୍ୟ test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ଦଶମିକ inter =  box_union(a, b);
    ଦଶମିକ xinter = box_union(dxa, b);
    ଦଶମିକ yinter = box_union(dya, b);
    ଦଶମିକ winter = box_union(dwa, b);
    ଦଶମିକ hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
ଶୂନ୍ୟ test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ଦଶମିକ inter =  box_intersection(a, b);
    ଦଶମିକ xinter = box_intersection(dxa, b);
    ଦଶମିକ yinter = box_intersection(dya, b);
    ଦଶମିକ winter = box_intersection(dwa, b);
    ଦଶମିକ hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

ଶୂନ୍ୟ test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    ଦଶମିକ iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    ଦଶମିକ xiou = box_iou(dxa, b);
    ଦଶମିକ yiou = box_iou(dya, b);
    ଦଶମିକ wiou = box_iou(dwa, b);
    ଦଶମିକ hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    ଦଶମିକ u = box_union(a,b);
    ଦଶମିକ i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    ଯଦି(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        ଫେରନ୍ତୁ dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    ଫେରନ୍ତୁ dd;
}


ଶୂନ୍ୟ do_nms(box *boxes, ଦଶମିକ **probs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(i = 0; i < total; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା any = 0;
        ପାଇଁ(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        ଯଦି(!any) {
            ଜାରି;
        }
        ପାଇଁ(j = i+1; j < total; ++j){
            ଯଦି (box_iou(boxes[i], boxes[j]) > thresh){
                ପାଇଁ(k = 0; k < classes; ++k){
                    ଯଦି (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    ଅନ୍ୟଥା probs[j][k] = 0;
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
    ଫେରନ୍ତୁ encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    ଫେରନ୍ତୁ decode;
}
