#அந்தர்பாவ "box.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdlib.h>

முழு nms_comparator(மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    தசம diff = 0;
    எனில்(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } இல்லையெனில் {
        diff = a.objectness - b.objectness;
    }
    எனில்(diff < 0) திரும்பு 1;
    இல்லையெனில் எனில்(diff > 0) திரும்பு -1;
    திரும்பு 0;
}

வெறுமை do_nms_obj(detection *dets, முழு total, முழு classes, தசம thresh)
{
    முழு i, j, k;
    k = total-1;
    க்கு(i = 0; i <= k; ++i){
        எனில்(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    க்கு(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, அளவு(detection), nms_comparator);
    க்கு(i = 0; i < total; ++i){
        எனில்(dets[i].objectness == 0) தொடர்;
        box a = dets[i].bbox;
        க்கு(j = i+1; j < total; ++j){
            எனில்(dets[j].objectness == 0) தொடர்;
            box b = dets[j].bbox;
            எனில் (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                க்கு(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


வெறுமை do_nms_sort(detection *dets, முழு total, முழு classes, தசம thresh)
{
    முழு i, j, k;
    k = total-1;
    க்கு(i = 0; i <= k; ++i){
        எனில்(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    க்கு(k = 0; k < classes; ++k){
        க்கு(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, அளவு(detection), nms_comparator);
        க்கு(i = 0; i < total; ++i){
            எனில்(dets[i].prob[k] == 0) தொடர்;
            box a = dets[i].bbox;
            க்கு(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                எனில் (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(தசம *f, முழு stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    திரும்பு b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    தசம l1 = a.x - a.w/2;
    தசம l2 = b.x - b.w/2;
    எனில் (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    தசம r1 = a.x + a.w/2;
    தசம r2 = b.x + b.w/2;
    எனில்(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    எனில் (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    எனில் (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    தசம t1 = a.y - a.h/2;
    தசம t2 = b.y - b.h/2;
    எனில் (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    தசம b1 = a.y + a.h/2;
    தசம b2 = b.y + b.h/2;
    எனில்(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    எனில் (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    எனில் (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    திரும்பு d;
}

தசம overlap(தசம x1, தசம w1, தசம x2, தசம w2)
{
    தசம l1 = x1 - w1/2;
    தசம l2 = x2 - w2/2;
    தசம இடது = l1 > l2 ? l1 : l2;
    தசம r1 = x1 + w1/2;
    தசம r2 = x2 + w2/2;
    தசம வலது = r1 < r2 ? r1 : r2;
    திரும்பு வலது - இடது;
}

தசம box_intersection(box a, box b)
{
    தசம w = overlap(a.x, a.w, b.x, b.w);
    தசம h = overlap(a.y, a.h, b.y, b.h);
    எனில்(w < 0 || h < 0) திரும்பு 0;
    தசம area = w*h;
    திரும்பு area;
}

தசம box_union(box a, box b)
{
    தசம i = box_intersection(a, b);
    தசம u = a.w*a.h + b.w*b.h - i;
    திரும்பு u;
}

தசம box_iou(box a, box b)
{
    திரும்பு box_intersection(a, b)/box_union(a, b);
}

தசம box_rmse(box a, box b)
{
    திரும்பு sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    தசம w = overlap(a.x, a.w, b.x, b.w);
    தசம h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    திரும்பு di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    திரும்பு du;
}


வெறுமை test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    தசம inter =  box_union(a, b);
    தசம xinter = box_union(dxa, b);
    தசம yinter = box_union(dya, b);
    தசம winter = box_union(dwa, b);
    தசம hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
வெறுமை test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    தசம inter =  box_intersection(a, b);
    தசம xinter = box_intersection(dxa, b);
    தசம yinter = box_intersection(dya, b);
    தசம winter = box_intersection(dwa, b);
    தசம hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

வெறுமை test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    தசம iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    தசம xiou = box_iou(dxa, b);
    தசம yiou = box_iou(dya, b);
    தசம wiou = box_iou(dwa, b);
    தசம hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    தசம u = box_union(a,b);
    தசம i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    எனில்(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        திரும்பு dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    திரும்பு dd;
}


வெறுமை do_nms(box *boxes, தசம **probs, முழு total, முழு classes, தசம thresh)
{
    முழு i, j, k;
    க்கு(i = 0; i < total; ++i){
        முழு any = 0;
        க்கு(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        எனில்(!any) {
            தொடர்;
        }
        க்கு(j = i+1; j < total; ++j){
            எனில் (box_iou(boxes[i], boxes[j]) > thresh){
                க்கு(k = 0; k < classes; ++k){
                    எனில் (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    இல்லையெனில் probs[j][k] = 0;
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
    திரும்பு encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    திரும்பு decode;
}
