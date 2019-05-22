#include"cal_weight.h"

void cal_weight(const cv::Mat &image, const cv::Mat *HSV_channel,cv::Mat &weight_bright, cv::Mat &weight_constract, const double &alpha, const double &fai)
{
	//��������Ȩ�� weight_bright=exp(-(image-0.5)^2/(2*0.25^2))  ���������Ϊ������Ԫ�صĵ�����Ǿ�������
	weight_bright = image - 0.5;
	weight_bright = weight_bright.mul(weight_bright);
	weight_bright = -weight_bright / 0.125;

	//�Ըò��ֲ����ݺ�����ϣ�����Ϻ�Ľ�������������
	cv::Mat wb_3, wb_2;
	cv::pow(weight_bright, 3, wb_3);
	cv::pow(weight_bright, 2, wb_2);
	weight_bright = (0.0571*wb_3) + (0.3699*wb_2) + (0.9395*weight_bright) + 0.9929;

	//���ݶԱȶȺ�ɫ�ʱ��Ͷȼ���Ȩ��
	weight_constract = (alpha*(HSV_channel[0] / 360)) + fai;

	//�Ըò��ֲ����ݺ�����ϣ����ݺ�����Ͻ���������ұ任���
	cv::pow(weight_constract, 3, wb_3);
	cv::pow(weight_constract, 2, wb_2);
	weight_constract = ((-0.1254)*wb_3) + (1.735*wb_2) - (7.002*weight_constract) + 8.58;
	weight_constract = weight_constract.mul(image);
	weight_constract = weight_constract.mul(HSV_channel[1]);
	cv::GaussianBlur(weight_constract, weight_constract, cv::Size(21, 21), 0, 0, cv::BORDER_REFLECT);
	weight_constract = cv::max(weight_constract, 0.000001);

}