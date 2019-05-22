#pragma once
#include"guided_filter.h"
#include<opencv2/opencv.hpp>
#define _HSV_V_

constexpr unsigned int window_1{ 7 };

using namespace std;

void get_illuminationmap(const cv::Mat&  , const cv::Mat*, cv::Mat & , cv::Mat & , cv::Mat &,cv::Mat & );
