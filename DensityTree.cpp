#include "DensityTree.h" // add double LSG[2][4],RSG[2][4]  in .h public 
//二维数组，第一维分别X==》0，Y==》1，第二维分别存放mean==>0，variance==>1，比例系数==>2,theta==>3。
#include <iostream>
#include <random>
#include <cmath>
# define PI 3.14159265358979323846
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
				double temp = X.at<double>(p,i)
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
		/*Scalar meanX;
			Scalar meanY;
			Scalar sdX;
			Scalar sdY;
			meanStdDev(S.col(0),meanX,sdX);
			meanStdDev(S.col(1),meanY,sdY); 
			WeakLearner node;
                 node.leafNode(meanX[0],sdX[0]*sdX[0],meanY[0],sdY[0]*sdY[0],S.rows);
                *///问流畅这段代码的含义
		//compute the Gaussian parameter of LS and RS.
		// add double LSG[4],RSG[4]  in .h public 
		//二维数组，第一维分别X==》0，Y==》1，第二维分别存放mean==>0，variance==>1，比例系数==>2,theta==>3。
		        double temp_m, temp_sd;/// temp is a mat or double????????????????
		
		meanStdDev(LS,temp_m, temp_sd);
		LSG[i][0]=temp_m.at<double>(0,0);
		LSG[i][1]= temp_sd.at<double>(0,0)*temp_sd.at<double>(0,0);
		LSG[i][2]=(double)LS.rows/(double)X.rows;
		LSG[i][3] = theta_split;
		meanStdDev(RS,temp_m, temp_sd);
		RSG[i][0]=temp_m.at<double>(0,0);
		RSG[i][1]= temp_sd.at<double>(0,0)*temp_sd.at<double>(0,0);
		RSG[i][2]=(double)RS.rows/(double)X.rows;
		RSG[i][3] = theta_split;
	}			

	
	
	
	
	
	
	cout << "Not implemented" << endl;//Temporla
}
Mat DensityTree::densityXY()
{
    	// add double LSG[4],RSG[4]  in .h public
	//二维数组，第一维分别X==》0，Y==》1，第二维分别存放mean==>0，variance==>1，比例系数==>2,theta==>3。
	Mat density;
	double val;
	for(int i=0; i < X.rows; i++){
		val = X.at<double>(i,0);
		if(val<=LSG[i][3])
	     density.at<double>(i,0)=LSG[i][2]*1.0/sqrt(2.0*PI*LSG[i][1])*exp(-0.5*pow(val-LSG[i][0],2)/LSG[i][1]);
		else  
	     density.at<double>(i,0)=RSG[i][2]*1.0/sqrt(2.0*PI*RSG[i][1])*exp(-0.5*pow(X.at<double>(i,0)-RSG[i][0],2)/RSG[i][1]);
	
	}
	
	
	
	
	
	
	
    return density;//Temporal
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


