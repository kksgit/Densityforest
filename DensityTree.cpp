#include "DensityTree.h"
#include <iostream>
using namespace cv;
using namespace std;
DensityTree::DensityTree(unsigned int D, unsigned int n_thresholds, Mat X) 
{
    this-> D=D;
    this-> X=X;
    this-> n_thresholds=n_thresholds;
}
void DensityTree::train()
{
	double minValX, maxValX;
	for (int i = 0; i < 2; i++) {
		minMaxIdx(X.col(i), &minValX, &maxValX, NULL, NULL);

	int N = pow(2, D - 1);//no. of leaf nodes

								  ////bagging: boostrap aggregation.  without index
								  //Mat samplingSet;
								  //for (int i = 0; i < N; i++)
								  //{
								  //	int random = rand() % N;
								  //	Mat R = X.rowRange(random, random + 1).clone();//from 0 to N
								  //	samplingSet.push_back(R);
								  //}

								  //max min
			double minv = 0.0, maxv = 0.0, range = 0.0;
			double* minp = &minv;
			double* maxp = &maxv;
			Mat	X1 = X.col(0).clone();//X.colRange(0, 1).clone();
			minMaxIdx(X1, minp, maxp);
			range = maxv - minv; //around 32.9369
								 //cout << minv << endl;

			Mat leftSet, rightSet;// , leftResult, rightResult;//left set; right set
			double splitPoint, InfoGain = 0.0;
			for (int i = 0; i < n_thresholds; i++)
			{
				InfoGain = 0.0;
				//srand((unsigned)time(NULL));
				double randNo = rand() / double(RAND_MAX);//double from 0 to 1
				double point = minv + range * randNo;

				//get 2 (left,right) sets, clear first
				int countX = 0, countY = 0;
				leftSet.release();
				rightSet.release();
				for (int x = 0; x < X.rows; x++)
				{
					double* data = X.ptr<double>(x);
					//cout << data[0]<<"---------------------"<< point << endl;
					if (data[0] <= point)
					{
						Mat L = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
						leftSet.push_back(L);
						//countX++;//leftSet.rows
					}
					else
					{
						Mat R = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
						rightSet.push_back(R);
						//countY++;
					}
				}//cout << leftSet << countX <<endl;


				if (InfoGain < getInfoGain(X, leftSet, rightSet))
				{
					InfoGain = getInfoGain(X, leftSet, rightSet);//, countX, countY);//leftSet.rows, rightSet.rows
					leftResult = leftSet;
					rightResult = rightSet;
					splitPoint = point;
				}

			}

			cout << "=======================" << endl;


			//store the data
			mean = getMean(leftResult);
			means.push_back(mean);
			variances.push_back(getVariance(leftResult, mean));

			mean = getMean(rightResult);
			means.push_back(mean);
			variances.push_back(getVariance(rightResult, mean));

			cout << "Not implemented " << means.size() << endl;//Temporla
		}
	
	
	
	
	
	
	
	
	cout << "Not implemented" << endl;//Temporla
}
Mat DensityTree::densityXY()
{
    
    return X;//Temporal
}



