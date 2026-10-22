#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdlib.h>

પૂર્ણાંક nms_comparator(અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    દશાંશ diff = 0;
    જો(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } નહીંતર {
        diff = a.objectness - b.objectness;
    }
    જો(diff < 0) પાછા 1;
    નહીંતર જો(diff > 0) પાછા -1;
    પાછા 0;
}

શૂન્ય do_nms_obj(detection *dets, પૂર્ણાંક total, પૂર્ણાંક classes, દશાંશ thresh)
{
    પૂર્ણાંક i, j, k;
    k = total-1;
    માટે(i = 0; i <= k; ++i){
        જો(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    માટે(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, કદ(detection), nms_comparator);
    માટે(i = 0; i < total; ++i){
        જો(dets[i].objectness == 0) ચાલુ;
        box a = dets[i].bbox;
        માટે(j = i+1; j < total; ++j){
            જો(dets[j].objectness == 0) ચાલુ;
            box b = dets[j].bbox;
            જો (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                માટે(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


શૂન્ય do_nms_sort(detection *dets, પૂર્ણાંક total, પૂર્ણાંક classes, દશાંશ thresh)
{
    પૂર્ણાંક i, j, k;
    k = total-1;
    માટે(i = 0; i <= k; ++i){
        જો(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    માટે(k = 0; k < classes; ++k){
        માટે(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, કદ(detection), nms_comparator);
        માટે(i = 0; i < total; ++i){
            જો(dets[i].prob[k] == 0) ચાલુ;
            box a = dets[i].bbox;
            માટે(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                જો (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(દશાંશ *f, પૂર્ણાંક stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    પાછા b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    દશાંશ l1 = a.x - a.w/2;
    દશાંશ l2 = b.x - b.w/2;
    જો (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    દશાંશ r1 = a.x + a.w/2;
    દશાંશ r2 = b.x + b.w/2;
    જો(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    જો (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    જો (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    દશાંશ t1 = a.y - a.h/2;
    દશાંશ t2 = b.y - b.h/2;
    જો (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    દશાંશ b1 = a.y + a.h/2;
    દશાંશ b2 = b.y + b.h/2;
    જો(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    જો (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    જો (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    પાછા d;
}

દશાંશ overlap(દશાંશ x1, દશાંશ w1, દશાંશ x2, દશાંશ w2)
{
    દશાંશ l1 = x1 - w1/2;
    દશાંશ l2 = x2 - w2/2;
    દશાંશ ડાબું = l1 > l2 ? l1 : l2;
    દશાંશ r1 = x1 + w1/2;
    દશાંશ r2 = x2 + w2/2;
    દશાંશ જમણું = r1 < r2 ? r1 : r2;
    પાછા જમણું - ડાબું;
}

દશાંશ box_intersection(box a, box b)
{
    દશાંશ w = overlap(a.x, a.w, b.x, b.w);
    દશાંશ h = overlap(a.y, a.h, b.y, b.h);
    જો(w < 0 || h < 0) પાછા 0;
    દશાંશ area = w*h;
    પાછા area;
}

દશાંશ box_union(box a, box b)
{
    દશાંશ i = box_intersection(a, b);
    દશાંશ u = a.w*a.h + b.w*b.h - i;
    પાછા u;
}

દશાંશ box_iou(box a, box b)
{
    પાછા box_intersection(a, b)/box_union(a, b);
}

દશાંશ box_rmse(box a, box b)
{
    પાછા sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    દશાંશ w = overlap(a.x, a.w, b.x, b.w);
    દશાંશ h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    પાછા di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    પાછા du;
}


શૂન્ય test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    દશાંશ inter =  box_union(a, b);
    દશાંશ xinter = box_union(dxa, b);
    દશાંશ yinter = box_union(dya, b);
    દશાંશ winter = box_union(dwa, b);
    દશાંશ hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
શૂન્ય test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    દશાંશ inter =  box_intersection(a, b);
    દશાંશ xinter = box_intersection(dxa, b);
    દશાંશ yinter = box_intersection(dya, b);
    દશાંશ winter = box_intersection(dwa, b);
    દશાંશ hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

શૂન્ય test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    દશાંશ iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    દશાંશ xiou = box_iou(dxa, b);
    દશાંશ yiou = box_iou(dya, b);
    દશાંશ wiou = box_iou(dwa, b);
    દશાંશ hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    દશાંશ u = box_union(a,b);
    દશાંશ i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    જો(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        પાછા dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    પાછા dd;
}


શૂન્ય do_nms(box *boxes, દશાંશ **probs, પૂર્ણાંક total, પૂર્ણાંક classes, દશાંશ thresh)
{
    પૂર્ણાંક i, j, k;
    માટે(i = 0; i < total; ++i){
        પૂર્ણાંક any = 0;
        માટે(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        જો(!any) {
            ચાલુ;
        }
        માટે(j = i+1; j < total; ++j){
            જો (box_iou(boxes[i], boxes[j]) > thresh){
                માટે(k = 0; k < classes; ++k){
                    જો (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    નહીંતર probs[j][k] = 0;
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
    પાછા encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    પાછા decode;
}
