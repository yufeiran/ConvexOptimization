#include"miniMath.h"
#include<assert.h>
using namespace mini;


Vec Vec::operator-(const Vec& v) const
{
	assert(this->n == v.n);
	Vec ansV(*this);
	for (int i = 0; i < n; i++)
	{
		ansV[i] -= v[i];
	}
	return ansV;
}

Vec Vec::operator+(const Vec& v)const
{
	assert(this->n == v.n);
	Vec ansV(*this);
	for (int i = 0; i < n; i++)
	{
		ansV[i] += v[i];
	}
	return ansV;
}

double Vec::value()const
{
	assert(n == 1);
	return _data[0];
}

Mat Mat::operator/(double N)const
{
	Mat ansMat(* this);
	for (int i = 0; i < m*n; i++)
	{
		ansMat._data[i] /= N;
	}
	return ansMat;
}
Mat Mat::operator*(double N)const
{
	Mat ansMat(*this);
	for (int i = 0; i < m * n; i++)
	{
		ansMat._data[i] *= N;
	}
	return ansMat;
}
Mat Mat::operator*(const Mat& m1)const
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

Vec Mat::operator*(const Vec& v1)const
{


	assert(this->n == v1.n);

	Vec ansVec(this->m);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ansVec[i] += d(i, j) * v1[j];
		}
	}

	return ansVec;
}

Mat Mat::operator+(const Mat& m1)const
{
	assert(this->n == m1.n && this->m == m1.m);

	Mat ansMat(m, n);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ansMat.d(i, j) = d(i, j) + m1.d(i, j);
		}
	}
	return ansMat;
}

double Mat::value()const
{
	assert(n == 1 && m == 1);
	return d(0, 0);
}

Mat mini::operator*(double N, const Mat& m)
{
	return m * N;
}

Mat mini::MakeIdentityMat(int N)
{
	Mat IM(N, N);
	for (int i = 0; i < N; i++)
	{
		IM.d(i, i) = 1;
	}
	return IM;
}


const int MAX_N = 200;
double W[MAX_N][MAX_N], W_n[MAX_N][MAX_N], L[MAX_N][MAX_N], U[MAX_N][MAX_N], L_n[MAX_N][MAX_N], U_n[MAX_N][MAX_N];
double arr[MAX_N][MAX_N];
void LU_decomposition(int N);
Mat mini::inverseMatLU_decomposition(const Mat& m1)
{
	for (int i = 0; i < m1.m; i++)
	{
		for (int j = 0; j < m1.n; j++)
		{
			arr[i][j] = m1.d(i, j);
		}
	}
	LU_decomposition(m1.m);

	Mat newM(m1.m, m1.n);
	for (int i = 0; i < newM.m; i++)
	{
		for (int j = 0; j < newM.n; j++)
		{
			newM.d(i, j) = W_n[i][j];
		}
	}
	return newM;	
}


//https://blog.csdn.net/weixin_46207279/article/details/120374064
// 采用LU分解法来对矩阵求逆
void LU_decomposition(int N)
{

	int i, j, k, d;
	float s;

	// 赋初值
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			W[i][j] = (float)arr[i][j];
			L[i][j] = 0;
			U[i][j] = 0;
			L_n[i][j] = 0;
			U_n[i][j] = 0;
			W_n[i][j] = 0;
		}
	}

	for (i = 0; i < N; i++)  // L对角置1
	{
		L[i][i] = 1.0;
	}

	for (j = 0; j < N; j++)
	{
		U[0][j] = W[0][j];
	}

	for (i = 1; i < N; i++)
	{
		L[i][0] = W[i][0] / U[0][0];
	}

	for (i = 1; i < N; i++)
	{
		for (j = i; j < N; j++) // 求U
		{
			s = 0;
			for (k = 0; k < i; k++)
			{
				s += L[i][k] * U[k][j];
			}
			U[i][j] = W[i][j] - s;
		}

		for (d = i; d < N; d++) // 求L
		{
			s = 0;
			for (k = 0; k < i; k++)
			{
				s += L[d][k] * U[k][i];
			}
			L[d][i] = (W[d][i] - s) / U[i][i];
		}
	}

	for (j = 0; j < N; j++)  //求L的逆
	{
		for (i = j; i < N; i++)
		{
			if (i == j)
				L_n[i][j] = 1 / L[i][j];
			else if (i < j)
				L_n[i][j] = 0;
			else
			{
				s = 0.;
				for (k = j; k < i; k++)
				{
					s += L[i][k] * L_n[k][j];
				}
				L_n[i][j] = -L_n[j][j] * s;
			}
		}
	}

	for (i = 0; i < N; i++)  //求U的逆
	{
		for (j = i; j >= 0; j--)
		{
			if (i == j)
				U_n[j][i] = 1 / U[j][i];
			else if (j > i)
				U_n[j][i] = 0;
			else
			{
				s = 0.;
				for (k = j + 1; k <= i; k++)
				{
					s += U[j][k] * U_n[k][i];
				}
				U_n[j][i] = -1 / U[j][j] * s;
			}
		}
	}


	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				W_n[i][j] += U_n[i][k] * L_n[k][j];
			}
		}
	}

	/*
		printf("使用LU分解法求逆的结果为：\n");
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				printf("%7.2f", W_n[i][j]);
			}
			printf("\n");
		}
	*/

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
	if (m1.m == 1 && m1.n == 1)return m1.d(0, 0);

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

std::ostream& mini::operator<<(std::ostream& os, const Vec& v1)
{
	for (int i = 0; i < v1.n; i++)
	{
		os << "| " << v1[i] << " |" << std::endl;
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