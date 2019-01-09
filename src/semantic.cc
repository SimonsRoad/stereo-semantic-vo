//
// Created by zss on 19-1-3.
//

#include <Tracking.h>
#include "semantic.h"

Semantic::Semantic(vector<string> &imleft,vector<string> &imright){
    left=imleft;
    right=imright;
}

void Semantic::Run()
{
    std::vector<cv::Scalar> colors;
    for (int i = 0; i < 80; i++) {
        colors.push_back(cv::Scalar(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
    }
    detector.Create( "/home/zss/stereo-semantic-vo/bin/kitti/yolov2-tiny-kitti.weights","/home/zss/stereo-semantic-vo/bin/kitti/yolov2-tiny-kitti.cfg", "/home/zss/stereo-semantic-vo/bin/kitti/kitti.names");

    while (1)
    {
        if(Checknewframe())
        {
            for(list<frame*>::iterator sit=NewFrames.begin(); sit!=NewFrames.end();)
            {
                frame* cur=*sit;
                cv::Mat img=cur->leftimg;
                std::vector<BoxSE> boxes = detector.Detect(img, 0.6F);
//                cout<<boxes.size()<<endl;
                cur->boxes=boxes;
                cur->have_detected=true;
                int n = boxes.size();
                for (int i = 0; i < n; i++) {
//                    cout<<boxes[i].x<<"~"<<boxes[i].y<<"~"<<boxes[i].width<<"~"<<boxes[i].height<<endl;
                    cv::rectangle(img, boxes[i].tl(), boxes[i].br(), colors[boxes[i].m_class],2);

                    putText(img, boxes[i].m_class_name, boxes[i].tl(), cv::FONT_HERSHEY_PLAIN, 1.25, colors[boxes[i].m_class], 2);
                }
                cv::imshow("11",img);
                cv::waitKey(100);
                NewFrames.erase(sit++);
            }

        }
    }
}

void Semantic::SetTracker(Tracking *pTracker)
{
    mpTracker=pTracker;
}

void Semantic::Insertframe(frame *cur_frame)
{
    NewFrames.push_back(cur_frame);
}

bool Semantic::Checknewframe()
{
    return(!NewFrames.empty());/////机智的写法

}

