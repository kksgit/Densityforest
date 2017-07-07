#include "DensityTree.h"
#include <iostream>
#include <random>
#include <cmath>
using namespace cv;
using namespace std;

double getinfoGain()(Mat& LS, Mat& RS, Mat& S)


DensityTree::DensityTree(unsigned int D, unsigned int n_thresholds, Mat X) 
{
    this-> D=D;
    this-> X=X;
    this-> n_thresholds=n_thresholds;
}


// generate random value from 3 to 7 ===> rand()/(float)(RAND_MAX)*4+3
void DensityTree::train()
{
	double minVal, maxVal,theta;
	/* iterate twice for X and Y */
	for (int i = 0; i < 2; i++) {
		minMaxIdx(X.col(i), &minVal, &maxVal);// get the max and min value in col(0,1)
		
		/*----- iterate n times to get n thetas, choose the largest infoGain---*/
		Mat LS, RS, LS_final, RS_final;
		double max_infoGain = 0, theta_split;
		for (int j = 0; j < n_thersholds; j++){
			theta=rand()/(double)(RAND_MAX)*(maxVal-minVal)+minVal;  // get the splitpoint
			LS.release();
			RS.release();
			
			//sperate X into LS and RS
			for(int p = 0; p < X.rows ; p++ ){
				double temp = x.at<double>(p,i)
				if(temp <= theta )
				LS.push_back(temp);
				else RS.push_back(temp);
			   	}                
			double  infoGain = getinfoGain(LS, RS, X);
			if (infoGain > max_infoGain) 
					max_infoGain = infoGain;
					theta_split = theta;
					LS_final = LS.clone();		
					RS_final = RS.clone();
			 }
			 //meanStdDev(M,temp_m,temp_sd)
			 
		
		
		
		}

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
			//double minv = 0.0, maxv = 0.0, range = 0.0;
			//double* minp = &minv;
			//double* maxp = &maxv;
			//Mat	X1 = X.col(0).clone();//X.colRange(0, 1).clone();
			//minMaxIdx(X1, minp, maxp);
			//range = maxv - minv; //around 32.9369
								 //cout << minv << endl;

			//Mat leftSet, rightSet;// , leftResult, rightResult;//left set; right set
			//double splitPoint, InfoGain = 0.0;
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
		
	
	
	
	
	
	
	
	
	cout << "Not implemented" << endl;//Temporla
}
Mat DensityTree::densityXY()
{
    
    return X;//Temporal
}


/*------get infoGain------*/
double getinfoGain()(Mat& LS, Mat& RS, Mat& S) {
	
	Mat mean,LCovar, RCovar, SCovar;
	
	// calculate the covariance matrix
	calcCovarMatrix(LS, LCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);  //we only want covariance.
	calcCovarMatrix(RS, RCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);
	calcCovarMatrix(S, SCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);
		
	// calculate the determinant and infoGain
	double infoGain = log(determinant(SCovar)) - (double)LS.rows / (double)S.rows*log(determinant(LCovar)) - (double)RS.rows / (double)S.rows*log(determinant(RCovar));
	
	
	return infoGain;

}


