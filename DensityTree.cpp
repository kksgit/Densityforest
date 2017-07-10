#include "DensityTree.h" 
#include <stdlib.h> 
#include <iostream>
#include <random>
#include <cmath>
# define PI 3.14159265358979323846
using namespace cv;
using namespace std;

double getinfoGain(Mat& LS, Mat& RS, Mat& S);


DensityTree::DensityTree(unsigned int D, unsigned int n_thresholds, Mat X) 
{
    this-> D=D;
    this-> X=X;
    this-> n_thresholds=n_thresholds;
}


void DensityTree::train()
{
	double minVal, maxVal;
	/* iterate twice for X and Y */
	for (int i = 0; i < 2; i++) {
		minMaxIdx(X.col(i), &minVal, &maxVal);// get the max and min value in col(0,1)
	//	cout << minVal << endl << maxVal << endl;
		/*----- iterate n times to get n thetas, choose the largest infoGain---*/
		Mat LS, RS, LS_final, RS_final;
		double max_infoGain = 0 , theta_split = 0, theta = 0;
		for (int j = 0; j < n_thresholds; j++){
			theta=rand()/(double)(RAND_MAX)*(maxVal-minVal)+minVal;  // get the splitpoint
			//cout << theta << endl;
			LS.release();
			RS.release();
		//	cout << j << ";"<< theta << endl;
			//sperate X into LS and RS
			for(int p = 0; p < X.rows ; p++ ){
				double temp = X.at<double>(p, i);
				if(temp <= theta )
				LS.push_back(temp);
				else RS.push_back(temp);
			   	}                
			double  infoGain = getinfoGain(LS, RS, X);//这个地方这么赋值有没有问题？？
			if (infoGain > max_infoGain) {
				max_infoGain = infoGain;
				theta_split = theta;
				LS_final = LS.clone();
				RS_final = RS.clone();
			}
			 }
		
		//compute the Gaussian parameter of LS and RS.
	
		//二维数组，第一维分别X==》0，Y==》1，第二维分别存放mean==>0，variance==>1，比例系数==>2,theta==>3。
		//cout << LS.rows << ";" << RS.rows << ";" <<theta_split <<endl;
		//system("pause");
		Mat temp_m, temp_sd;
		meanStdDev(LS_final,temp_m, temp_sd);
		LSG[i][0]=temp_m.at<double>(0,0);
		LSG[i][1]= temp_sd.at<double>(0,0)*temp_sd.at<double>(0,0);
		LSG[i][2]=(double)LS.rows/(double)X.rows;
		LSG[i][3] = theta_split;
		meanStdDev(RS_final,temp_m, temp_sd);
		RSG[i][0]=temp_m.at<double>(0,0);
		RSG[i][1]= temp_sd.at<double>(0,0)*temp_sd.at<double>(0,0);
		RSG[i][2]=(double)RS.rows/(double)X.rows;
		RSG[i][3] = theta_split;

		//cout << theta_split << endl;
		//system("pause");
	//	for (int c = 0; c <LS_final.rows; c++) {
	//		cout << c << ";" << LS_final.at<double>(c, 0) << endl;
			
	//		}
		//system("pause");
	}			

	
	
	
	
	
	
	//cout << "Not implemented" << endl;//Temporla
}
Mat DensityTree::densityXY()
{
    	// add double LSG[4],RSG[4]  in .h public
	//二维数组，第一维分别X==》0，Y==》1，第二维分别存放mean==>0，variance==>1，比例系数==>2,theta==>3。
	Mat density = Mat(1000, 2, CV_64F);
	double val;
	for (int i = 0; i < X.rows; i++) {
		
		for (int j = 0; j < 2; j++) {
			val = X.at<double>(i, j);
			//cout << val << endl<<LSG[j][3]<<endl;
		if (val <= LSG[j][3])
			density.at<double>(i, j) = LSG[j][2] * 1.0 / sqrt(2.0*PI*LSG[j][1])*exp(-0.5*pow(val - LSG[j][0], 2) / LSG[j][1]);
		else
			density.at<double>(i, j) = RSG[j][2] * 1.0 / sqrt(2.0*PI*RSG[j][1])*exp(-0.5*pow(val - RSG[j][0], 2) / RSG[j][1]);
	}
	}
	
	
	
	
	
	
	
    return density;//Temporal
}


/*------get infoGain------*/
double getinfoGain(Mat& LS, Mat& RS, Mat& S) {
	
	Mat mean,LCovar, RCovar, SCovar;
	
	double infoGain = 0;
	if (LS.rows == 0) return infoGain;
	if (LS.rows == 1) return infoGain;
	if (RS.rows == 0) return infoGain;
	if (RS.rows == 1) return infoGain;
	// calculate the covariance matrix
	calcCovarMatrix(LS, LCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);  //we only want covariance.
	calcCovarMatrix(RS, RCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);
	calcCovarMatrix(S, SCovar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);
		
	// calculate the determinant and infoGain
	infoGain = log(determinant(SCovar)) - (double)LS.rows / (double)S.rows*log(determinant(LCovar)) - (double)RS.rows / (double)S.rows*log(determinant(RCovar));
	
	
	return infoGain;

}


