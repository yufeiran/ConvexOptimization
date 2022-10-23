#include"miniMath.h"
#include<assert.h>
using namespace mini;

Mat Mat::operator/(double N)
{
	Mat ansMat(* this);
	for (int i = 0; i < m*n; i++)
	{
		ansMat._data[i] /= N;
	}
	return ansMat;
}
Mat Mat::operator*(double N)
{
	Mat ansMat(*this);
	for (int i = 0; i < m * n; i++)
	{
		ansMat._data[i] *= N;
	}
	return ansMat;
}
Mat Mat::operator*(const Mat& m1)
{
	/*
					  + +
	 + + + + + +      + +    + +
	 + + + + + +  *   + +  = + +
					  + +
					  + +
	*/
	assert(n == m1.m);
	//矩阵相乘的结果是 左边矩阵的行数m 和 右边矩阵的列数 n
	Mat newM(m, m1.n);


	for (int i = 0; i < newM.m; i++)
	{
		for (int j = 0; j < newM.n; j++)
		{
			newM.d(i, j) = 0;
			for (int k = 0; k < n; k++) {
				newM.d(i, j) += this->d(i, k) * m1.d(k, j);
			}
			
		}
	}
	return newM;
}

Mat Mat::getSubMat(int startM, int startN, int sizeM, int sizeN) const
{
	assert(startM >= 0 && sizeM > 0 && startN >= 0 && sizeN > 0);
	assert(startM + sizeM <= this->m&& startN + sizeN <= this->n);
	Mat subMat(sizeM, sizeN);
	for (int i = 0; i < sizeM; i++)
	{
		for (int j = 0; j < sizeN; j++)
		{
			subMat.d(i, j) = this->d(startM + i, startN + j);
		}
	}

	return subMat;
}

Mat Mat::getSubMat(int I, int J) const//获得删除第i行第j列而得到的子矩阵
{
	assert(I >= 0 && I < m&& J>=0 && J < n);
	assert(m > 1 && n > 1);
	Mat subMat(m - 1, n - 1);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i < I) {
				if (j < J) {
					subMat.d(i, j) = this->d(i, j);
				}
				else if (j > J) {
					subMat.d(i, j - 1) = this->d(i, j);
				}
			}
			else if (i > I) {
				if (j < J) {
					subMat.d(i - 1, j) = this->d(i, j);
				}
				else if (j > J) {
					subMat.d(i - 1, j - 1) = this->d(i, j);
				}
			}
		}
	}
	return subMat;
}

Mat mini::inverseMat(const Mat& m1)
{
	// A-1=A*/det(A)
	// A*就是Adj(A)
	Mat invM(m1.m, m1.n);
	invM = adjMat(m1) / detMat(m1);
	return invM;

}

Mat mini::adjMat(const Mat& m1)
{
	Mat adjMat(m1.m, m1.n);
	for (int i = 0; i < m1.m; i++)
	{
		for (int j = 0; j < m1.n; j++)
		{
			Mat nowSubMat = m1.getSubMat(i, j);
			//std::cout << "===nowSubMat===" << std::endl;
			//std::cout << nowSubMat << std::endl;
			double oneSubDet = detMat(nowSubMat);
			if ((i + j) % 2 != 0)oneSubDet = -oneSubDet;
			//std::cout << "oneSubDet:" << oneSubDet << std::endl;
			adjMat.d(j, i) = oneSubDet;
		}
	}
	return adjMat;
}


double mini::detMat(const Mat& m1)
{
	assert(m1.isSquare() == true);
	//从任选一行展开
	//det(A)=a11*det(A11)+a12*det(A12)+...+a1n*det(A1n)
	//这里选择从第一行展开,是递归算法，在方阵变成2x2方阵的时候结束递归
	if (m1.n == 2) {
		return m1.d(0, 0) * m1.d(1, 1) - m1.d(0, 1) * m1.d(1, 0);
	}
	else {
		double ans = 0;
		for (int i = 0; i < m1.n; i++)
		{
			Mat nowSubMat = m1.getSubMat(0, i);
			double oneSubDet = m1.d(0, i) * detMat(nowSubMat);
			if (i % 2 != 0) {
				oneSubDet = -oneSubDet;
			}
			ans += oneSubDet;
		}
		return ans;
	}

}


std::ostream& mini::operator<<(std::ostream& os, const Mat& m1)
{
	for (int i = 0; i < m1.m; i++)
	{
		for (int j = 0; j < m1.n; j++)
		{
			os << m1.d(i, j) << " ";
		}
		os << std::endl;
	}

	return os;
}

Mat mini::transposeMat(const Mat& m1)
{
	Mat tMat(m1.n, m1.m);

	for (int i = 0; i < tMat.m; i++)
	{
		for (int j = 0; j < tMat.n; j++)
		{
			tMat.d(i, j) = m1.d(j, i);
		}

	}
	return tMat;
}