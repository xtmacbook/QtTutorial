#include <iostream>
#include <iomanip>

#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<algorithm>
#include<vector>
#include<cmath>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;


/********�ú���������ȷ��ƥ���ԣ������ͼ��֮��ı任��ϵ********/
/*match1_xy��ʾ�ο�ͼ�����������꼯��,[M x 2]����M��ʾ�����ĸ���
 match2_xy��ʾ����׼ͼ�������㼯�ϣ�[M x 2]����M��ʾ�����㼯��
 model��ʾ�任���ͣ������Ʊ任��,"����任","͸�ӱ任"
 rmse��ʾ���������
 ����ֵΪ����õ���3 x 3�������
 */
Mat LMS(const Mat&match1_xy, const Mat &match2_xy,  float &rmse)
{

	if (match1_xy.rows != match2_xy.rows)
		CV_Error(CV_StsBadArg, "LMSģ������������Ը�����һ�£�");


	const int N = match1_xy.rows;//���������
	
	Mat match2_xy_trans, match1_xy_trans;//����ת��
	transpose(match1_xy, match1_xy_trans);
	transpose(match2_xy, match2_xy_trans);

	Mat change = Mat::zeros(3, 3, CV_32FC1);

	//A*X=B,���������ַ���任��͸�ӱ任һ��,��������������M����A=[2*M,6]����
	//A=[x1,y1,0,0,1,0;0,0,x1,y1,0,1;.....xn,yn,0,0,1,0;0,0,xn,yn,0,1]
	Mat A = Mat::zeros(2*N,6,CV_32FC1);
	for (int i = 0; i < N; ++i)
	{
		A.at<float>(2 * i, 0) = match2_xy.at<float>(i, 0);//x
		A.at<float>(2 * i, 1) = match2_xy.at<float>(i, 1);//y
		A.at<float>(2*i, 4) = 1.f;

		A.at<float>(2 * i + 1, 2) = match2_xy.at<float>(i, 0);
		A.at<float>(2 * i + 1, 3) = match2_xy.at<float>(i, 1);
		A.at<float>(2 * i+1, 5) = 1.f;
	}

	//��������������M,�Ǹ�B=[2*M,1]����
	//B=[u1,v1,u2,v2,.....,un,vn]
	Mat B;
	B.create(2 * N, 1, CV_32FC1);
	for (int i = 0; i < N; ++i)
	{
		B.at<float>(2 * i, 0) = match1_xy.at<float>(i, 0);//x
		B.at<float>(2 * i + 1, 0) = match1_xy.at<float>(i, 1);//y
	}

	//����任
	
	Vec6f values;
	solve(A, B, values, DECOMP_QR);
	change = (Mat_<float>(3,3)<<values(0), values(1), values(4),
		values(2), values(3), values(5),
		+0.0f, +0.0f, 1.0f);

	Mat temp_1 = change(Range(0, 2), Range(0, 2));//�߶Ⱥ���ת��
	Mat temp_2 = change(Range(0, 2), Range(2, 3));//ƽ����
	
	Mat match2_xy_change = temp_1 * match2_xy_trans + repeat(temp_2, 1, N);
	Mat diff = match2_xy_change - match1_xy_trans;//���
	
	pow(diff,2.f,diff);

	Mat diff_trans;
	transpose(diff, diff_trans);
	//rmse = (float)sqrt(sum(diff)(0)*1.0/N);//sum����Ǹ���ͨ���ĺ�
	//

	return diff_trans;
}






