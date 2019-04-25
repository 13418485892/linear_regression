#include<iostream>
#include<fstream>
#include "line_regression.h"
#include <cmath>

#define TRAIN_NUM 400//�������ݸ���
#define FORECAST_NUM 100//Ԥ�����ݸ���
#define FEATURE_NUM 4//������������
#define ITERATOR_TIME 1000//��������
#define STUDY_A 0.001//ѧϰ��
using namespace std;




double predict(double* w, double* data, int feature_num) {
	double sum = 0;
	for (int i = 0; i < feature_num; i++) {
		sum += w[i] * data[i];
	}
	return sum;
}

// ��ʧ����
double Theta(double **training_set, int featue_num, int training_num, double* w) {
	double sum = 0;
	for (int i = 0; i < training_num; i++) {
		sum += (training_set[i][featue_num] - predict(w, training_set[i], featue_num))*(training_set[i][featue_num] - predict(w, training_set[i], featue_num));
	}
	return sum / (2 * training_num);
}

// �ݶ��½�
void gradient_descent(double** training_set, int feature_num, int training_num, double* w, double a, int iterator_time) {
	while (iterator_time--) {
		//����ǰJ��ֵ
		double temp = Theta(training_set, feature_num, training_num, w);

		double* del_theta = new double[feature_num];
		for (int i = 0; i < feature_num; i++) {
			del_theta[i] = 0;
			for (int j = 0; j < training_num; j++) {
				del_theta[i] += (predict(w, training_set[j], feature_num) - training_set[j][feature_num])*training_set[j][i];
			}
		}
		//w[i]�ĸ��±�������е�del_theta��������˲ſ��ԣ���Ȼ���µĻ�Ӱ��û���µ�
		//���������ڴ����ڱ�ʾ���������forѭ�����ܺ�����ĺϲ���
		for (int i = 0; i < feature_num; i++)
		{
			w[i]-= a * del_theta[i] / (double)training_num;
		}
		
		//printf("%.3lf\n", Theta(training_set, feature_num, training_num, w));
		delete[] del_theta;
		//������J��ֵ
		double temp1 = Theta(training_set, feature_num, training_num, w);
		
		//���ε���J��ֵ�仯С��0.001��ѭ����ֹ
		if (fabs(temp1 - temp) < 0.001)
			break;
	}
	cout << "J_Theta=" << Theta(training_set, feature_num, training_num, w) << endl;
	printf("��������\n");
	for (int i = 0; i < feature_num-1; i++) {
		cout <<"Theta_"<<i<<"="<< w[i] << " ";
	}
	cout << "Theta_"<< feature_num - 1 << "=" << w[feature_num - 1]<<endl;
	return;
}

//�������ݲ���
void forecast(double **forecast_set, double* w, int feature_num,int forecast_num) {
	cout << "J_Theta=" << Theta(forecast_set, feature_num, forecast_num, w) << endl;
	for (int j = 0; j < forecast_num; j++) 
	{
		double y = w[0];
		for (int i = 1; i < feature_num - 1; i++) 
		{
			y = y + w[i] * forecast_set[j][i];
		}
		cout << endl;

		//cout << "Ԥ��ֵ:" << y << "��ʵ��ֵ��" << forecast_set[j][feature_num] << ",���:" << fabs(y - forecast_set[j][feature_num]) * 100 / y << endl;

	}
}


void feature_normalize(double **feature_set, int feature_num, int training_num) {
	//������һ��
	// ����ĳ������ x(i)=(x(i)-average(X))/standard_devistion(X)
	// 1���������X��n�������е�ƽ��ֵaverage��X��
	// 2���������X��n�������еı�׼�� standard_devistion(X)
	// 3��������X��n�������е�ÿ��ֵx��i����ʹ��������ʽ���й�һ��
	double *average = new double[feature_num];
	double  *stanrd_divition = new double[feature_num];
	for (int i = 1; i < feature_num; i++) {
		double sum = 0;
		for (int j = 0; j < training_num; j++) {
			sum += feature_set[j][i];
		}
		average[i] = sum / training_num;
	}
	for (int i = 1; i < feature_num; i++) {
		double sum = 0;
		for (int j = 0; j < training_num; j++) {
			sum += (feature_set[j][i] - average[i])*(feature_set[j][i] - average[i]);
		}
		stanrd_divition[i] = sqrt((sum / (training_num - 1)));
	}
	for (int i = 1; i < feature_num; i++)
		for (int j = 0; j < training_num; j++) {
			feature_set[j][i] = (feature_set[j][i] - average[i]) / (double)stanrd_divition[i];
		}
	delete[] stanrd_divition;
	delete[] average;
}









void main() {
	int i, j;//�м����

	double **training_set = new double*[TRAIN_NUM];//��������
	for (i = 0; i < TRAIN_NUM; i++)
	{
		training_set[i] = new double[FEATURE_NUM + 1];
	}

	double *w = new double[FEATURE_NUM];
	//���ݳ�ʼ��
	for (i = 0; i < FEATURE_NUM; i++)
		w[i] = 1.0;
	
	
	ifstream infile;
	infile.open("train_data.txt");
	for (i = 0; i < TRAIN_NUM; i++)
	{
		for (j = 0; j < FEATURE_NUM + 1; j++)
		{
			infile >> training_set[i][j];
		}
	}
	
	infile.close();
	cout << "��������ɹ�" << endl;
	
	//�ݶ��½�
	gradient_descent(training_set, FEATURE_NUM, TRAIN_NUM, w, STUDY_A, ITERATOR_TIME);
	
	//Ԥ��
	double **forecast_set = new double*[FORECAST_NUM];
	for (i = 0; i < FORECAST_NUM; i++)
	{
		forecast_set[i] = new double[FEATURE_NUM + 1];
	}

	infile.open("test_data.txt");
	for (i = 0; i < TRAIN_NUM; i++)
	{
		for (j = 0; j < FEATURE_NUM + 1; j++)
		{
			infile >> forecast_set[i][j];
		}
	}
	infile.close();
	forecast(forecast_set, w, FEATURE_NUM,FORECAST_NUM);
	feature_normalize(forecast_set, FEATURE_NUM, FORECAST_NUM);

	for (i = 0; i < FORECAST_NUM; i++)
	{
		
		cout <<"��"<<i+1<< "������ Ԥ��ֵ:" << forecast_set[i][FEATURE_NUM] << endl;
	}
	system("pause");
}
