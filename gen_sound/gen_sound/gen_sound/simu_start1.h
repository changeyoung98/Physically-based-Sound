﻿#pragma once
#include <Eigen\Dense>
#include <iostream>
#include"Sound.h"
#include <string>
#include<stdio.h>
#include<vector>
//scale  - meters
using std::cout;

double dt;   //wood density -400, pine wood young's modulus 9 gpa
double area[4] = { 1e-4,1e-4,1e-4,1e-4 };   //1cm^2    ,areaÃ»Ê²Ã´Ó°Ïì
double rho[4] = { 2.7e3,2.7e3,2.7e3,2.7e3 };  //5 points  gold and wood, right now wood  
double youngs[4] = { 9e10,9e10,9e10,9e10 }; //4 springs
Eigen::Vector3cd positions[5] = { Eigen::Vector3cd(1e-1,2e-1,3e-1),Eigen::Vector3cd(2e-1,3e-1,4e-1),Eigen::Vector3cd(3e-1,4e-1,5e-1) ,Eigen::Vector3cd(4e-1,5e-1,6e-1),Eigen::Vector3cd(5e-1,6e-1,7e-1) };  //m
int forc = 1000;

void change_rho(double rhoD) {
	rho[0] = rhoD;
	rho[1] = rhoD;
	rho[2] = rhoD;
	rho[3] = rhoD;
}
void change_force(int force) {
	forc = force;
}
/*void init(massspringsystem& s)   //add point+spring+fix or not
{
	for (int i = 0; i < 5; i++)
	{
		s.addmass(positions[i].x().real(), positions[i].y().real(), positions[i].z().real(), i);  //use position to get distance
	}
	for (int i = 0; i < 5; i++)   //reset mass based on springs
	{
		if (i + 1 < 5)
		{
			s.addspring(youngs[i], area[i], s.massget(i), s.massget(i + 1));
			s.reset_mass(rho[i], area[i], s.massget(i), s.massget(i + 1));    //3n*3n
		}
	}

}
//currently not used
void init_3d(massspringsystem& s)   //add point+spring+fix or not   
{
	for (int i = 0; i < 5; i++)
	{
		s.addmass(positions[i].x().real(), positions[i].y().real(), positions[i].z().real(), i);  //use position to get distance
	}
	for (int i = 0; i < 5; i++)   //reset mass based on springs
	{
		if (i + 1 < 5)     //should be 3*n  masses
		{
			s.addspring(youngs[i], area[i], s.massget(i), s.massget(i + 1));
			s.reset_mass(rho[i], area[i], s.massget(i), s.massget(i + 1));    //3n*3n
		}
	}

}

void sound_1d_damping()  // difference lay in the stiffmatrix and mass matrix
{
	massspringsystem systems;   dt = 0.1f;
	init(systems);
	cout << "Pre Computing......" << std::endl;
	Eigen::MatrixXd massmatrix;
	build_mass_matrix_1d(systems, massmatrix);
	Eigen::MatrixXd stiffmatrix;
	build_stiff_matrix_1d(systems, stiffmatrix);
	modalmodel mod;
	//Eigen::Matrix3Xd gradient;
	//calculate_gradient(systems, gradient);
	std::vector<double> frequencies;
	Eigen::MatrixXd S;  Eigen::MatrixXd U_T;
	std::vector<double> Force_Ud;
	eigendecomposition(stiffmatrix, massmatrix, S, frequencies, U_T);   // D = frequency^2, //general eigen decomposition
	Eigen::VectorXd force(systems.getmasses().size()); force.setZero();
	cout << "Successfully get all the matrixs!!" << std::endl << std::endl;

	cout << "All the Frequencies in the hearing range: " << std::endl;
	for (int i = 0; i < frequencies.size(); i++)
	{
		if (frequencies[i] != 0)
			cout << frequencies[i] << std::endl;
	}
	mod.load_eigenmodes(frequencies, systems);
	cout << std::endl;
	while (true)   //force input!
	{
		Force_Ud.resize(0);
		cout << "Force: (Assigned)" << std::endl;
		force.setZero();
		force(2) = 1000;   //first
		std::vector<double> amplitude;  double maxamp = 0;
		for (int i = 0; i < U_T.rows(); i++)   //  frequency ->amplitude, 15 dof
		{
			if (frequencies[i] != 0.) {   //cutting mode out of range
				double fi = U_T.row(i) * force;   //fi
				//std::cout << "Size: " << Force_Ud.size() << std::endl;
				Force_Ud.push_back(fi);
			}

			//double tmp = amplitude[i];
			//if (abs(tmp) > maxamp)
			//maxamp = abs(tmp);
		}
		//cout << U_T << std::endl;
		//cout << Force_Ud.size() << std::endl << std::endl;
		cout << "Tap 'Enter' to trigger Sound or '0' to Exit" << std::endl << std::endl;
		std::string cc;
		std::getline(std::cin, cc);
		if (cc._Equal("0")) break;
		//cout << "max" << maxamp << std::endl;
		//amplitude_normalize(amplitude, maxamp);
		sound::soundplay_al_new(Force_Ud, mod, systems);
		//sound::soundplay_sfml(Force_Ud, mod, systems);
	}

}

void sound_1d_no_damping()
{
	massspringsystem systems;   dt = 0.1f;
	init(systems);
	cout << "Pre Computing......" << std::endl;
	Eigen::MatrixXd massmatrix;
	build_mass_matrix_1d(systems, massmatrix);
	Eigen::MatrixXd stiffmatrix;
	build_stiff_matrix_1d(systems, stiffmatrix);
	modalmodel mod;
	//Eigen::Matrix3Xd gradient;
	//calculate_gradient(systems, gradient);
	std::vector<double> frequencies;
	Eigen::MatrixXd S;  Eigen::MatrixXd U_T;
	std::vector<double> Force_Ud;
	eigendecomposition(stiffmatrix, massmatrix, S, frequencies, U_T);   // D = frequency^2, //general eigen decomposition
	Eigen::VectorXd force(systems.getmasses().size()); force.setZero();
	cout << "Successfully get all the matrixs!!" << std::endl << std::endl;

	cout << "All the Frequencies in the hearing range: " << std::endl;
	for (int i = 0; i < frequencies.size(); i++)
	{
		if (frequencies[i] != 0)
			cout << frequencies[i] << std::endl;
	}
	mod.load_eigenmodes(frequencies, systems);
	cout << std::endl;
	while (true)   //force input!
	{
		Force_Ud.resize(0);  //re-counting
		cout << "Force: (Assigned)" << std::endl;
		force.setZero();
		force(1) = 1000;   //first
		std::vector<double> amplitude;  double maxamp = 0;
		for (int i = 0; i < U_T.rows(); i++)   //  frequency ->amplitude, 15 dof
		{
			if (frequencies[i] != 0.) {  //cutting mode out of ranges
				double fi = U_T.row(i) * force;   //fi
				Force_Ud.push_back(fi);
			}

			//double tmp = amplitude[i];
			//if (abs(tmp) > maxamp)
			//maxamp = abs(tmp);
		}
		//cout << Force_Ud.size() << std::endl << std::endl;    //
		cout << "Tap 'Enter' to trigger Sound or '0' to Exit" << std::endl << std::endl;
		std::string cc;
		std::getline(std::cin, cc);
		if (cc._Equal("0")) break;
		//cout << "max" << maxamp << std::endl;
		//amplitude_normalize(amplitude, maxamp);
		sound::soundplay_al_new_nodamping(Force_Ud, mod, systems);
	}
}
*/

void damping_sound()
{
	massspringsystem systems;   dt = 0.1f;
	init(systems);
	cout << "Pre Computing......" << std::endl;
	Eigen::MatrixXd massmatrix;
	build_mass_matrix(systems, massmatrix);
	Eigen::MatrixXd stiffmatrix;
	stiffness_build_3d(systems, stiffmatrix);
	modalmodel mod;
	//Eigen::Matrix3Xd gradient;
	//calculate_gradient(systems, gradient);
	std::vector<double> frequencies;
	Eigen::MatrixXd S;  Eigen::MatrixXd U_T;
	std::vector<double> Force_Ud;
	eigendecomposition(stiffmatrix, massmatrix, S, frequencies, U_T);   // D = frequency^2, //general eigen decomposition
	Eigen::VectorXd force(systems.getmasses().size() * 3); force.setZero();
	cout << "Successfully get all the matrixs!!" << std::endl << std::endl;

	cout << "All the Frequencies in the hearing range: " << std::endl;
	for (int i = 0; i < frequencies.size(); i++)
	{
		if (frequencies[i] != 0)
			cout << frequencies[i] << std::endl;
	}
	mod.load_eigenmodes(frequencies, systems);
	cout << std::endl;
	//	while (true)   //force input!
		//{
	Force_Ud.resize(0);
	cout << "Force: (Assigned)" << std::endl;
	force.setZero();
	force(12) = forc;   //first
	std::vector<double> amplitude;  double maxamp = 0;
	for (int i = 0; i < U_T.rows(); i++)   //  frequency ->amplitude, 15 dof
	{
		if (frequencies[i] != 0.) {
			double fi = U_T.row(i) * force;   //fi
			Force_Ud.push_back(fi);
			//double tmp = amplitude[i];
			//if (abs(tmp) > maxamp)
			//maxamp = abs(tmp);
		}
	}
	//std::cout << U_T << std::endl;
	//std::cout << Force_Ud.size() << std::endl;
//	cout << "Tap 'Enter' to trigger Sound or '0' to Exit" << std::endl << std::endl;
//	std::string cc;
//	std::getline(std::cin, cc);
//	if (cc._Equal("0")) break;
	//cout << "max" << maxamp << std::endl;
	//amplitude_normalize(amplitude, maxamp);
	//sound::soundplay_al_new(Force_Ud, mod, systems);
	sound::soundplay_sfml(Force_Ud, mod, systems);
	//sound::soundplay_al_one_frequency(Force_Ud, mod, systems, 3);
	//sound::soundplay_al_one_frequency_nodamping(Force_Ud, mod, systems, 3);
	//sound::sound_test();
//}
}
/*
void no_damping_sound()
{
	massspringsystem systems;   dt = 0.1f;
	init(systems);
	cout << "Pre Computing......" << std::endl;
	Eigen::MatrixXd massmatrix;
	build_mass_matrix(systems, massmatrix);
	Eigen::MatrixXd stiffmatrix;
	stiffness_build_3d(systems, stiffmatrix);
	modalmodel mod;
	//Eigen::Matrix3Xd gradient;
	//calculate_gradient(systems, gradient);
	std::vector<double> frequencies;
	Eigen::MatrixXd S;  Eigen::MatrixXd U_T;
	std::vector<double> Force_Ud;
	eigendecomposition(stiffmatrix, massmatrix, S, frequencies, U_T);   // D = frequency^2, //general eigen decomposition
	Eigen::VectorXd force(systems.getmasses().size() * 3); force.setZero();
	cout << "Successfully get all the matrixs!!" << std::endl << std::endl;

	cout << "All the Frequencies in the hearing range: " << std::endl;
	for (int i = 0; i < frequencies.size(); i++)
	{
		if (frequencies[i] != 0)
			cout << frequencies[i] << std::endl;
	}
	mod.load_eigenmodes(frequencies, systems);
	cout << std::endl;
	while (true)   //force input!
	{
		Force_Ud.resize(0);
		cout << "Force: (Assigned)" << std::endl;
		force.setZero();
		force(0) = 1000;   //first
		std::vector<double> amplitude;  double maxamp = 0;
		for (int i = 0; i < U_T.rows(); i++)   //  frequency ->amplitude, 15 dof
		{
			if (frequencies[i] != 0.) {
				double fi = U_T.row(i) * force;   //fi
				Force_Ud.push_back(fi);
				//double tmp = amplitude[i];
				//if (abs(tmp) > maxamp)
				//maxamp = abs(tmp);
			}
		}
		//cout << Force_Ud[0] << std::endl << std::endl;
	//	cout << "Tap 'Enter' to trigger Sound or '0' to Exit" << std::endl << std::endl;
	//	std::string cc;
	//	std::getline(std::cin, cc);
	//	if (cc._Equal("0")) break;
		//cout << "max" << maxamp << std::endl;
		//amplitude_normalize(amplitude, maxamp);
		sound::soundplay_al_new_nodamping(Force_Ud, mod, systems);

	}*/
}