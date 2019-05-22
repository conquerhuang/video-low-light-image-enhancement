#include"multi_fusion.h"

void multi_fusion(const cv::Mat& image_double, cv::Mat &image_enhanced, const double & alpha, const double &fai)
{
	cv::Mat image_hsv;//ԭͼ��hsv��ľ��󣬺���������Ҫ�õ�ͼ������ͨ�������ֵ��HSV_V�����Լ�����Աȶ�Ȩ��ʱ��Ҫ�õ���
	cv::cvtColor(image_double, image_hsv, CV_BGR2HSV);//ת����hsv��

	//�洢HSV����ͨ�� ��image_hsv�з������
	cv::Mat HSV_channel[3];
	//�������ͼ
	cv::split(image_hsv, HSV_channel);//��image_hsv�е�����ͨ�����뵽HSV_channel��ȥ

	cv::Mat I1, I2, I3, reflect;
	get_illuminationmap(image_double, HSV_channel, I1, I2, I3, reflect);


	//����Ȩ��ͼ
	//����������ͼ�ֱ�ӹ���ǿ�ȣ�bright���Աȶ� contract���������������Ӧ��Ȩ�أ�Ȼ��������˵õ�ÿ������ͼ��Ӧ��Ȩ��
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

	//����������ͼ��Ȩ�غ���Ϊ��ĸ����������ͼ��Ȩ�ؽ��м�Ȩƽ��
	cv::Mat w_sum;
	w_sum = w1 + w2 + w3;
	cv::Mat w1_final, w2_final, w3_final;
	w1_final = w1 / w_sum;
	w2_final = w2 / w_sum;
	w3_final = w3 / w_sum;
	//	cv::imshow("w1_final", w1_final);
	//	cv::imshow("w2_final", w2_final);
	//	cv::imshow("w3_final", w3_final);

		//�����ܵĹ���ͼ
	cv::Mat I_fusion;
	I_fusion = I1.mul(w1_final) + I2.mul(w2_final) + I3.mul(w3_final);

	//�����յĹ���ͼ���Ƶ�����ͨ���ϣ���Ϊ�ܵĹ���ͼ
	vector<cv::Mat> I_fusion_3channel;
	I_fusion_3channel.push_back(I_fusion);
	I_fusion_3channel.push_back(I_fusion);
	I_fusion_3channel.push_back(I_fusion);
	cv::merge(I_fusion_3channel, I_fusion);

	//����retinex���ۼ���������ǿ���ͼ����
	image_enhanced = reflect.mul(I_fusion);
	cv::pow(image_enhanced, 0.75, image_enhanced);
}