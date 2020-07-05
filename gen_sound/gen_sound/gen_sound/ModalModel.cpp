#include "ModalModel.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
#include "Eigen/Dense"
#define M_PI        3.14159265358979323846264338327950288   /* pi */
#   define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi */
//#include "utils/term_msg.h"
//#include "utils/macros.h"


using namespace std;

const double CUTTING_FREQ = 16000.;
const double CUTTING_OMEGA = CUTTING_FREQ * 2. * M_PI;
//const double MIN_AUD_OMEGA = 20. * 2. * M_PI;

ModalModel::ModalModel(const std::string& modalFile, double density, double alpha, double beta) :
	density_(density), invDensity_(1. / density), alpha_(alpha), beta_(beta)
{
	if (density < 1E-8 || alpha < 0. || beta < 0.)
	{
	//	PRINT_ERROR("Invalid modal model parameters\n");
	//	SHOULD_NEVER_HAPPEN(-1);
	}

//	PRINT_MSG("Load eigenmodes [%s] ...\n", modalFile.c_str());
	load_eigenmodes(modalFile.c_str());
}

/*!
 * Load eigen modes of rigid object from file
 */
void ModalModel::load_eigenmodes(const char* file)
{
	ifstream fin(file, ios::binary);

	fin.read((char*)& n3_, sizeof(int));       // size of eigen problem
	fin.read((char*)& numModes_, sizeof(int)); // number of modes
	if (fin.fail())
	{
		cout << "fail" << endl;
	//	PRINT_ERROR("Cannot read file: %s\n", file);
	//	SHOULD_NEVER_HAPPEN(2);
	}
	eigenmodes_.resize(numModes_);
//	PRINT_MSG("Load %d eigenmodes\n", numModes_);
	fin.read((char*)& eigenmodes_[0], sizeof(double) * numModes_);
//	PRINT_MSG("Compute eigenmodes' frequencies ...\n");
	int nmds = 0;
	for (; nmds < numModes_; ++nmds)
	{
		//// Here we divide by the density, because the eigenvalue analysis were done assuming a unit density
		eigenmodes_[nmds] *= invDensity_;
		if (eigenmodes_[nmds] > CUTTING_OMEGA * CUTTING_OMEGA) break;
	}
//	PRINT_MSG("%d modes in audible range\n", nmds);
	numModes_ = nmds;

	//// all the eigen vectors are stored in a n3 x nModes matrix
	//// it is stored as v1 v2 ...
	eigenvec_.resize(n3_ * numModes_);
	fin.read((char*)& eigenvec_[0], sizeof(double) * eigenvec_.size());
	if (fin.fail())
	{
	//	PRINT_ERROR("Cannot read file: %s\n", file);
	//	SHOULD_NEVER_HAPPEN(2);
	}

	//// compute modal parameters
	omega_.resize(numModes_);
	omegaD_.resize(numModes_);
	freqs_.resize(numModes_);
	c_.resize(numModes_);
	for (int i = 0; i < numModes_; ++i)
	{
		omega_[i] = sqrt(eigenmodes_[i]);
		freqs_[i] = omega_[i] * 0.5 * M_1_PI;       // freq. = w / (2*pi)
		c_[i] = alpha_ * eigenmodes_[i] + beta_;
		double xi = c_[i] / (2. * omega_[i]);
		if (xi >= 1.)
		{
	//		PRINT_ERROR("c[%d] should always be in the range [0, 1]: %lf\n", i, xi);
	//		SHOULD_NEVER_HAPPEN(2);
		}
		omegaD_[i] = omega_[i] * sqrt(1. - xi * xi);  // damped frequency
	}
}

void ModalModel::accum_modal_impulse(int vid, const Eigen::Vector3d imp, vector<double> &vec) const
{
	// out += U'*imp/rho  n*3 3*1 n*1
//	cblas_dgemv(CblasColMajor, CblasTrans, 3, numModes_, invDensity_,
//		&eigenvec_[vid * 3], n3_, (const double*)imp, 1, 1., out, 1);
///	Eigen::MatrixXd ev(numModes_,3);
	//ev(0,0)=eigenvec_[vid * 3];
//	cout <<"dir: "<< imp << endl;
	double* sta = (double*)&eigenvec_[vid * 3];
	Eigen::MatrixXd ev=Eigen::Map<Eigen::MatrixXd> (sta, numModes_, 3);
//	cout<<"ev: " << ev << endl;
	Eigen::VectorXd tmp;
	tmp = ev * imp * invDensity_;
	vec.resize(tmp.size());
	Eigen::VectorXd::Map(&vec[0], tmp.size()) = tmp;
//	cout <<"out: " << tmp << endl;
}
