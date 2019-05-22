#include"Source.h"



int main(void)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };//�洢�û������õ����ļ���Ϣ
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("mp4�ļ�(*.mp4)\0*.mp4\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = _T("E:\\graduation_project\\via C++\\test_video");//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

	g_cap.open(szBuffer);

	frams = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_COUNT));     //��Ƶ֡��
	int tmpw = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_WIDTH));  //��Ƶ֡���
	int tmph = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //��Ƶ֡����
	cout << "video has " << frams << " frams" << " wideth tmpw " << tmpw << " height " << tmph << endl;//��ʾ��Ƶ��Ϣ
	writer.open("enhanced_video.avi", -1, 30, cv::Size(tmpw, tmph * 2));
	int nframs{ 0 };//���ڼ�����Ƶ�����ٶȵı�����
	nframs = frams;

	int saved_frams;
	saved_frams = frams;

	distribution_frams();//Ϊ�߳�Ԥ���䴦��֡

	//�����߳�
	thread task1(thread1);
	thread task2(thread2);
	thread task3(thread3);
	thread task4(thread4);
	task1.detach();
	task2.detach();
	task3.detach();
	task4.detach();

	clock_t start, finish;
	start = clock();//���̴߳򿪿�ʼ��ʱ��ͳ�Ƴ��������ٶȡ�
	//������Ƶ˳��洢��Ƶ֡
	int index[2] = { 0,0 };
	cv::Mat frame;
	while (saved_frams > 0)
	{
		if (ready_to_save(saved_frams, index))//����Ƿ�ǰ��Ҫ�洢��֡�Ѿ�ת�����
		{
			frame = proposed_frams[index[0]][index[1]];
			writer << frame;                  //��ת���õ�֡������Ƶ��ȥ
			cv::imshow("output", frame);       //ʵʱˢ���Ѿ�ת���õ�֡
			task_index[2 * index[0]][index[1]] = 0;                         //����֡��Ӧ�ռ���Ϊ������ȡ��
			cv::waitKey(1);                                                 //�ȴ�1ms���˴�������ȴ�1ms��ת�������Ƶͼ�񲻻��Զ�ˢ��
			saved_frams--;                                                  //
		}
		distribution_frams();
	}

	finish = clock();
	//��ʾ��������ʱ��
	cout << "totally used time is " << static_cast<double>(finish - start) / CLOCKS_PER_SEC << " S" << endl;
	cout << "proposed speed " << nframs / (static_cast<double>(finish - start) / CLOCKS_PER_SEC) << " fps" << endl;
	//��ʾ����ͼ��
	//cv::namedWindow("input image", cv::WINDOW_NORMAL);
	////��ʾȥ��ͼ��
	//cv::namedWindow("dehazed image", cv::WINDOW_NORMAL);
	//cv::imshow("input image", input_image);
	//cv::imshow("dehazed image", radiance);
	system("pause");
	cv::destroyAllWindows();
	return 0;
}