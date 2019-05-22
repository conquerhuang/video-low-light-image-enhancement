#include"multi_fusion.h"

void multi_fusion(const cv::Mat& image_double, cv::Mat &image_enhanced, const double & alpha, const double &fai)
{
	cv::Mat image_hsv;//原图在hsv域的矩阵，后续计算需要用到图像三个通道的最大值（HSV_V），以及计算对比度权重时需要用到。
	cv::cvtColor(image_double, image_hsv, CV_BGR2HSV);//转换到hsv域

	//存储HSV三个通道 从image_hsv中分离出来
	cv::Mat HSV_channel[3];
	//计算光照图
	cv::split(image_hsv, HSV_channel);//将image_hsv中的三个通道分离到HSV_channel中去

	cv::Mat I1, I2, I3, reflect;
	get_illuminationmap(image_double, HSV_channel, I1, I2, I3, reflect);


	//计算权重图
	//对三幅光照图分别从光照强度，bright，对比度 contract两个方面来计算对应的权重，然后将两者相乘得到每幅光照图对应的权重
	cv::Mat w1_b, w1_c;
	cal_weight(I1, HSV_channel, w1_b, w1_c, alpha, fai);
	cv::Mat w2_b, w2_c;
	cal_weight(I2, HSV_channel, w2_b, w2_c, alpha, fai);
	cv::Mat w3_b, w3_c;
	cal_weight(I3, HSV_channel, w3_b, w3_c, alpha, fai);

	cv::Mat w1, w2, w3;
	w1 = w1_b.mul(w1_c);
	w2 = w2_b.mul(w2_c);
	w3 = w3_b.mul(w3_c);

	//以三幅光照图的权重和作为分母对三幅光照图的权重进行加权平均
	cv::Mat w_sum;
	w_sum = w1 + w2 + w3;
	cv::Mat w1_final, w2_final, w3_final;
	w1_final = w1 / w_sum;
	w2_final = w2 / w_sum;
	w3_final = w3 / w_sum;
	//	cv::imshow("w1_final", w1_final);
	//	cv::imshow("w2_final", w2_final);
	//	cv::imshow("w3_final", w3_final);

		//计算总的光照图
	cv::Mat I_fusion;
	I_fusion = I1.mul(w1_final) + I2.mul(w2_final) + I3.mul(w3_final);

	//将最终的光照图复制到三个通道上，作为总的光照图
	vector<cv::Mat> I_fusion_3channel;
	I_fusion_3channel.push_back(I_fusion);
	I_fusion_3channel.push_back(I_fusion);
	I_fusion_3channel.push_back(I_fusion);
	cv::merge(I_fusion_3channel, I_fusion);

	//采用retinex理论计算最终增强后的图像结果
	image_enhanced = reflect.mul(I_fusion);
	cv::pow(image_enhanced, 0.75, image_enhanced);
}