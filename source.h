#pragma once
#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<fstream>
#include<thread>
#include<Windows.h>
#include<time.h>
#include<mutex>
#include<CommDlg.h>
#include<tchar.h>
#include"guided_filter.h"
#include"cal_weight.h"
#include"get_illumination_map.h"
#include"multi_fusion.h"

using namespace std;

unsigned int free_space(unsigned int i);
unsigned int ready_to_save(int saved_frams, int *index);
void distribution_frams(void);
unsigned int get_frams(unsigned int i, unsigned int *index);

cv::VideoCapture g_cap;
cv::VideoWriter writer;
int frams;

const double alpha{ 2 }, fai{ 4.3633 };
//����window_1������ͷ�ļ� get_illumination_map.h�У����Զ�����̬ѧ���㴰�ڴ�С��

//������֡��������
unsigned int task_index[8][4]{ 0 };//������֡������������֡��˳���ţ��Լ���������״̬��
cv::Mat proposed_frams[4][4];      //������֡�洢buff

unsigned int pending_index[8][4]{ 0 };
cv::Mat pending_frams[4][4];

void thread1()
{
	//while (frams > 0)
	//{
	//	if (task_index[0][0] == 0)//����ϴδ����Ľ����ȡ��
	//	{
	//		cv::Mat frame;
	//		mu.lock();
	//		{
	//			task_index[1][0] = frams;//��¼��ǰ�������֡
	//			g_cap >> frame;
	//			--frams;
	//		}
	//		mu.unlock();
	//		cv::Mat temp;
	//		frame.convertTo(frame, CV_32F, 1.0 / 255);
	//		multi_fusion(frame, temp, alpha, fai);
	//		cv::vconcat(frame, temp, temp);
	//		proposed_frams[0] = temp;//����ǰ������ɺ��֡��ŵ�֡��������
	//		task_index[0][0] = 1;
	//		cout << "thread1 proposed fram " << task_index[1][0] << endl;
	//	}
	//}
	cv::Mat frame;
	unsigned int save_area{ 0 };
	unsigned int index[2]{ 0,0 };
	while (1)
	{
		save_area = free_space(1);
		if (save_area != 100)//����������п���λ��
		{
			if (get_frams(1, index))//��ѯ�����������Ƿ��д�����֡
			{
				(pending_frams[index[0]][index[1]]).copyTo(frame);
				if (frame.empty())
				{
					cout << "empty frame in thread1" << endl;
					system("pause");
				}
				task_index[1][save_area] = pending_index[2 * index[0] + 1][index[1]];//��ȡ��ǰ��Ƶ֡���
				pending_index[2 * index[0]][index[1]] = 0;

				cv::Mat temp;
				frame.convertTo(frame, CV_32F, 1.0 / 255);
				multi_fusion(frame, temp, alpha, fai);
				cv::vconcat(frame, temp, temp);

				proposed_frams[0][save_area] = temp;//����ǰ������ɺ��֡��ŵ�֡��������
				task_index[0][save_area] = 1;
				cout << "thread1 proposed fram " << task_index[1][save_area] << endl;
			}

		}
	}
}
void thread2()
{
	cv::Mat frame;
	unsigned int save_area{ 0 };
	unsigned int index[2]{ 0,0 };
	while (1)
	{
		save_area = free_space(2);
		if (save_area != 100)//����������п���λ��
		{
			if (get_frams(2, index))//��ѯ�����������Ƿ��д�����֡
			{
				(pending_frams[index[0]][index[1]]).copyTo(frame);
				if (frame.empty())
				{
					cout << "empty frame in thread1" << endl;
					system("pause");
				}
				task_index[3][save_area] = pending_index[2 * index[0] + 1][index[1]];
				pending_index[2 * index[0]][index[1]] = 0;

				cv::Mat temp;
				frame.convertTo(frame, CV_32F, 1.0 / 255);
				multi_fusion(frame, temp, alpha, fai);
				cv::vconcat(frame, temp, temp);

				proposed_frams[1][save_area] = temp;//����ǰ������ɺ��֡��ŵ�֡��������
				task_index[2][save_area] = 1;
				cout << "thread2 proposed fram " << task_index[3][save_area] << endl;
			}
		}
	}
}
void thread3()
{
	cv::Mat frame;
	unsigned int save_area{ 0 };
	unsigned int index[2]{ 0,0 };
	while (1)
	{
		save_area = free_space(3);
		if (save_area != 100)//����������п���λ��
		{
			if (get_frams(3, index))//��ѯ�����������Ƿ��д�����֡
			{
				(pending_frams[index[0]][index[1]]).copyTo(frame);
				if (frame.empty())
				{
					cout << "empty frame in thread1" << endl;
					system("pause");
				}
				task_index[5][save_area] = pending_index[2 * index[0] + 1][index[1]];
				pending_index[2 * index[0]][index[1]] = 0;

				cv::Mat temp;
				frame.convertTo(frame, CV_32F, 1.0 / 255);
				multi_fusion(frame, temp, alpha, fai);
				cv::vconcat(frame, temp, temp);

				proposed_frams[2][save_area] = temp;//����ǰ������ɺ��֡��ŵ�֡��������
				task_index[4][save_area] = 1;
				cout << "thread3 proposed fram " << task_index[5][save_area] << endl;
			}
		}
	}
}
void thread4()
{
	cv::Mat frame;
	unsigned int save_area{ 0 };
	unsigned int index[2]{ 0,0 };
	while (1)
	{
		save_area = free_space(4);
		if (save_area != 100)//����������п���λ��
		{
			if (get_frams(4, index))//��ѯ�����������Ƿ��д�����֡
			{
				(pending_frams[index[0]][index[1]]).copyTo(frame);
				if (frame.empty())
				{
					cout << "empty frame in thread1" << endl;
					system("pause");
				}
				task_index[7][save_area] = pending_index[2 * index[0] + 1][index[1]];
				pending_index[2 * index[0]][index[1]] = 0;

				cv::Mat temp;
				frame.convertTo(frame, CV_32F, 1.0 / 255);
				multi_fusion(frame, temp, alpha, fai);
				cv::vconcat(frame, temp, temp);

				proposed_frams[3][save_area] = temp;//����ǰ������ɺ��֡��ŵ�֡��������
				task_index[6][save_area] = 1;
				cout << "thread4 proposed fram " << task_index[7][save_area] << endl;
			}
		}
	}
}

//���ҵ�ǰ֡�������Ƿ��п���λ�ã�����з��ؿ���λ��������û�з��ر�־100
//i  ��Ӧ�̱߳�š�
//j  ����λ������
unsigned int free_space(unsigned int i)
{
	for (unsigned int j = 0; j < 4; j++)
	{
		if (task_index[(i - 1) * 2][j] == 0)
		{
			return j;
		}
	}
	return 100;
}

//���ҵ�ǰ˳������Ҫ�洢��֡�Ƿ�����ɡ�
//saved_frams  ��ǰ��Ҫ�洢��֡���
//index        ���ڷ��ص�ǰ��Ҫ�洢��֡��λ��
unsigned int ready_to_save(int saved_frams, int *index)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (task_index[i * 2 + 1][j] == saved_frams)
			{
				if (task_index[i * 2][j] == 1)
				{
					index[0] = i;
					index[1] = j;
					return 1;
				}
			}
		}
	}
	return 0;// ��ǰ��Ҫ�洢֡��δ������ɡ�
}


//�������֡��������������
void distribution_frams(void)
{
	for (unsigned int j = 0; j < 4; j++)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (pending_index[i * 2][j] == 0)//��ѯ�Ƿ��п���ռ��ṩ����
			{
				if (frams > 0)
				{
					pending_index[(i * 2) + 1][j] = frams;//��Ǵ洢֡˳��
					g_cap >> pending_frams[i][j];       //��������֡����������
					--frams;                            //���µ�ǰʣ��֡��
					pending_index[i * 2][j] = 1;        //������������ʾ�û������Լ����Զ�ȡ����
				}
			}
		}
	}
}
//�Ӵ�����֡�������л�ȡ��ǰ������֡
//i     ��ʾ�̱߳��
//index ���ڷ��ش�ȡ֡�ڴ�ȡ֡�������е�������
unsigned int get_frams(unsigned int i, unsigned int *index)
{
	i = i - 1;
	unsigned int max_frams{ 0 };
	for (unsigned int j = 0; j < 4; j++)
	{
		if (pending_index[i * 2][j] == 1)
		{
			if (pending_index[(i * 2) + 1][j] > max_frams)
			{
				max_frams = pending_index[i * 2 + 1][j];
				index[0] = i;
				index[1] = j;
			}
		}
	}
	return max_frams;
}