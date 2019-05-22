#pragma once
#include<opencv2/opencv.hpp>
#include<string>
#include"guided_filter.h"
#include"cal_weight.h"
#include"get_illumination_map.h"
void multi_fusion(const cv::Mat&  , cv::Mat& , const double &  , const double & );
