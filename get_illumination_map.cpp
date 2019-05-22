#include"get_illumination_map.h"

void get_illuminationmap(const cv::Mat& image_double,const cv::Mat*HSV_channel,cv::Mat &I1,cv::Mat &I2,cv::Mat &I3,cv::Mat &reflect)
{
	//初步评估光照图
	cv::Mat I;
	//对开运算闭运算需要用到的原型模板，原型模板直径大小为 window_1
	cv::Matx<unsigned char, window_1, window_1> element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(window_1, window_1));
	cv::morphologyEx(HSV_channel[2], I, cv::MORPH_CLOSE, element);

	//0.013s
	I1 = fastGuidedFilter(HSV_channel[2], I, 15, 0.001, 1);//采用HSV_V进行导向滤波得到光照图I1
	//0.222s 这个时间正常
//	cv::imshow("illumation map 1", I1);//显示光照图1
//计算反射图，通过retinex理论，由原图按照元素除以光照图即可得到反射图
	//假设三个色彩通道的光照图一致，将光照图I1复制到三个通道进而计算反射图
	cv::Mat I1_3channel;
	vector<cv::Mat> illumation_map_temp;
	illumation_map_temp.push_back(I1);
	illumation_map_temp.push_back(I1);
	illumation_map_temp.push_back(I1);
	cv::merge(illumation_map_temp, I1_3channel);
	reflect = image_double / I1_3channel;
	//0.364s
	//计算光照图I2，光照图I2是为了增强全局光照让图像的暗部区域显示得更加清楚，作者采用了arctan变换来计算光照图I2
	double I_mean = mean(I1)[0];
	double lamuda = 10 + ((1 - I_mean) / I_mean);//lamuda是自适应参数，用于进行arctan变换时归一化效果
	I2 = lamuda * I1;
	I2 = (-1) / (I2 + 1) + 1;
	//0.385s
	//光照图I3是为了增强图像对比度，将光照图I1进行CLAHE变换后得到的
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(10, cv::Size(10, 10));//第一个参数为分块对比度限制，第二个参数为分块多少
	//OpenCV子带的直方图均衡化采用的是unsigned int 8类型的数据，此处需要将浮点型的光照图转换到uint8类型
	cv::Mat temp;
	I1.convertTo(temp, CV_8UC1, 255.0);
	clahe->apply(temp, I3);
	I3.convertTo(I3, CV_32F, 1.0 / 255);
}