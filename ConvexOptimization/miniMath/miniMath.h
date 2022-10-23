#pragma once
#include<iostream>

/*
	miniMath
	���� ����ά���������㡢��������

*/


namespace mini {

	/*
		�������� ����
		���桢����˷�
	*/
	struct Mat
	{
		int m, n; //m��n��
		double*_data; 
		Mat(int M, int N) :m(M), n(N) {
			_data = new double[M*N];
			for (int i = 0; i < M*N; i++) {
				_data[i] = 0;
			}
		}
		Mat(const Mat& m1) {
			m = m1.m;
			n = m1.n;
			_data = new double[m * n];
			for (int i = 0; i < m * n; i++) {
				_data[i] = m1._data[i];
			}
		}
		
		~Mat() {
			if(_data!=nullptr)delete[]_data;
		}

		Mat& operator=(const Mat& m1) {
			if (m != m1.m || n != m1.n) {
				delete[]this->_data;
				_data = new double[m1.m * m1.n];
			}
			m = m1.m;
			n = m1.n;

			
			for (int i = 0; i < m * n; i++) {
				_data[i] = m1._data[i];
			}
			return *this;
		}

		bool isSquare()const { if (m != 0 && m == n)return true; return false; }
		double& d(int M, int N) { return _data[M * n + N]; };
		const double d(int M, int N)const { return _data[M * n + N]; }
		Mat getSubMat(int startM, int startN, int sizeM, int sizeN)const;
		Mat getSubMat(int I, int J)const; //���ɾ����i�е�j�ж��õ����Ӿ���
		Mat operator/(double N);
		Mat operator*(double N);
		Mat operator*(const Mat& m1);
	};

	Mat transposeMat(const Mat& m1);
	Mat inverseMat(const Mat& m1);
	Mat adjMat(const Mat& m1);
	double detMat(const Mat& m1);
	std::ostream& operator<<(std::ostream& os, const Mat& m1);
}

