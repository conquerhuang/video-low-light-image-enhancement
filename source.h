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
//参数window_1定义在头文件 get_illumination_map.h中，用以定义形态学运算窗口大小。

//处理后的帧缓冲区域
unsigned int task_index[8][4]{ 0 };//处理后的帧索引，包含了帧的顺序标号，以及缓存区的状态。
cv::Mat proposed_frams[4][4];      //处理后的帧存储buff

unsigned int pending_index[8][4]{ 0 };
cv::Mat pending_frams[4][4];

void thread1()
{
	//while (frams > 0)
	//{
	//	if (task_index[0][0] == 0)//如果上次处理后的结果被取出
	//	{
	//		cv::Mat frame;
	//		mu.lock();
	//		{
	//			task_index[1][0] = frams;//记录当前所处理的帧
	//			g_cap >> frame;
	//			--frams;
	//		}
	//		mu.unlock();
	//		cv::Mat temp;
	//		frame.convertTo(frame, CV_32F, 1.0 / 255);
	//		multi_fusion(frame, temp, alpha, fai);
	//		cv::vconcat(frame, temp, temp);
	//		proposed_frams[0] = temp;//将当前处理完成后的帧存放到帧缓冲区中
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
		if (save_area != 100)//如果缓冲区有空闲位置
		{
			if (get_frams(1, index))//查询待处理缓冲区是否有待处理帧
			{
				(pending_frams[index[0]][index[1]]).copyTo(frame);
				if (frame.empty())
				{
					cout << "empty frame in thread1" << endl;
					system("pause");
				}
				task_index[1][save_area] = pending_index[2 * index[0] + 1][index[1]];//读取当前视频帧标号
				pending_index[2 * index[0]][index[1]] = 0;

				cv::Mat temp;
				frame.convertTo(frame, CV_32F, 1.0 / 255);
				multi_fusion(frame, temp, alpha, fai);
				cv::vconcat(frame, temp, temp);

				proposed_frams[0][save_area] = temp;//将当前处理完成后的帧存放到帧缓冲区中
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
		if (save_area != 100)//如果缓冲区有空闲位置
		{
			if (get_frams(2, index))//查询待处理缓冲区是否有待处理帧
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

				proposed_frams[1][save_area] = temp;//将当前处理完成后的帧存放到帧缓冲区中
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
		if (save_area != 100)//如果缓冲区有空闲位置
		{
			if (get_frams(3, index))//查询待处理缓冲区是否有待处理帧
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

				proposed_frams[2][save_area] = temp;//将当前处理完成后的帧存放到帧缓冲区中
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
		if (save_area != 100)//如果缓冲区有空闲位置
		{
			if (get_frams(4, index))//查询待处理缓冲区是否有待处理帧
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

				proposed_frams[3][save_area] = temp;//将当前处理完成后的帧存放到帧缓冲区中
				task_index[6][save_area] = 1;
				cout << "thread4 proposed fram " << task_index[7][save_area] << endl;
			}
		}
	}
}

//查找当前帧缓冲区是否有空闲位置，如果有返回空闲位置索引，没有返回标志100
//i  对应线程标号。
//j  空闲位置索引
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

//查找当前顺序下需要存储的帧是否处理完成。
//saved_frams  当前需要存储的帧标号
//index        用于返回当前需要存储的帧的位置
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
	return 0;// 当前需要存储帧尚未处理完成。
}


//向待处理帧缓冲区分配数据
void distribution_frams(void)
{
	for (unsigned int j = 0; j < 4; j++)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (pending_index[i * 2][j] == 0)//查询是否有空余空间提供分配
			{
				if (frams > 0)
				{
					pending_index[(i * 2) + 1][j] = frams;//标记存储帧顺序
					g_cap >> pending_frams[i][j];       //将待处理帧流出到数组
					--frams;                            //更新当前剩余帧数
					pending_index[i * 2][j] = 1;        //更新索引，表示该缓冲区以及可以读取数据
				}
			}
		}
	}
}
//从待处理帧缓冲区中获取当前待处理帧
//i     表示线程标号
//index 用于返回代取帧在代取帧缓冲区中的索引。
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