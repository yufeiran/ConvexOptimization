#pragma once
#include<iostream>
#include<initializer_list>

/*
	miniMath
	包含 任意维的向量运算、矩阵运算

*/


namespace mini {

	/*
		矩阵运算 包含
		求逆、矩阵乘法
	*/

	struct Vec
	{
		int n;
		double* _data;

		double& operator[](int index) {
			return _data[index];
		}

		double operator[](int index)const {
			return _data[index];
		}

		Vec(std::initializer_list<double> data)
		{
			n = data.size();
			_data = new double[n];
			auto nowData = data.begin();
			for (int i = 0; i < n; i++)
			{
				_data[i] = *nowData;
				nowData++;
			}
		}

		Vec(int N) :n(N) {
			_data = new double[N];
			for (int i = 0; i < n; i++)
			{
				_data[i] = 0;
			}
		}

		Vec(const Vec& v) {
			n = v.n;
			_data = new double[n];
			for (int i = 0; i < n; i++)
			{
				_data[i] = v._data[i];
			}
		}

		Vec& operator=(const Vec& v) {
			n = v.n;
			delete[]_data;
			_data = new double[n];
			for (int i = 0; i < n; i++)
			{
				_data[i] = v._data[i];
			}
			return *this;
		}

		Vec operator-(const Vec& v)const;
		double value()const;
	};


	struct Mat
	{
		int m, n; //m行n列
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
		Mat getSubMat(int I, int J)const; //获得删除第i行第j列而得到的子矩阵
		Mat operator/(double N)const;
		Mat operator*(double N)const;
		Mat operator*(const Mat& m1)const;
		Vec operator*(const Vec& v1)const ;
		double value()const;
	};

	Mat transposeMat(const Mat& m1);
	Mat inverseMat(const Mat& m1);
	Mat adjMat(const Mat& m1);
	double detMat(const Mat& m1);
	std::ostream& operator<<(std::ostream& os, const Mat& m1);
	std::ostream& operator<<(std::ostream& os, const Vec& v1);


	
}

