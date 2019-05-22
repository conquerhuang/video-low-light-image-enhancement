#include"get_illumination_map.h"

void get_illuminationmap(const cv::Mat& image_double,const cv::Mat*HSV_channel,cv::Mat &I1,cv::Mat &I2,cv::Mat &I3,cv::Mat &reflect)
{
	//������������ͼ
	cv::Mat I;
	//�Կ������������Ҫ�õ���ԭ��ģ�壬ԭ��ģ��ֱ����СΪ window_1
	cv::Matx<unsigned char, window_1, window_1> element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(window_1, window_1));
	cv::morphologyEx(HSV_channel[2], I, cv::MORPH_CLOSE, element);

	//0.013s
	I1 = fastGuidedFilter(HSV_channel[2], I, 15, 0.001, 1);//����HSV_V���е����˲��õ�����ͼI1
	//0.222s ���ʱ������
//	cv::imshow("illumation map 1", I1);//��ʾ����ͼ1
//���㷴��ͼ��ͨ��retinex���ۣ���ԭͼ����Ԫ�س��Թ���ͼ���ɵõ�����ͼ
	//��������ɫ��ͨ���Ĺ���ͼһ�£�������ͼI1���Ƶ�����ͨ���������㷴��ͼ
	cv::Mat I1_3channel;
	vector<cv::Mat> illumation_map_temp;
	illumation_map_temp.push_back(I1);
	illumation_map_temp.push_back(I1);
	illumation_map_temp.push_back(I1);
	cv::merge(illumation_map_temp, I1_3channel);
	reflect = image_double / I1_3channel;
	//0.364s
	//�������ͼI2������ͼI2��Ϊ����ǿȫ�ֹ�����ͼ��İ���������ʾ�ø�����������߲�����arctan�任���������ͼI2
	double I_mean = mean(I1)[0];
	double lamuda = 10 + ((1 - I_mean) / I_mean);//lamuda������Ӧ���������ڽ���arctan�任ʱ��һ��Ч��
	I2 = lamuda * I1;
	I2 = (-1) / (I2 + 1) + 1;
	//0.385s
	//����ͼI3��Ϊ����ǿͼ��Աȶȣ�������ͼI1����CLAHE�任��õ���
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(10, cv::Size(10, 10));//��һ������Ϊ�ֿ�Աȶ����ƣ��ڶ�������Ϊ�ֿ����
	//OpenCV�Ӵ���ֱ��ͼ���⻯���õ���unsigned int 8���͵����ݣ��˴���Ҫ�������͵Ĺ���ͼת����uint8����
	cv::Mat temp;
	I1.convertTo(temp, CV_8UC1, 255.0);
	clahe->apply(temp, I3);
	I3.convertTo(I3, CV_32F, 1.0 / 255);
}