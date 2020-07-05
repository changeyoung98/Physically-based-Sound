#pragma once
#pragma once
#pragma once
#include <Eigen\Dense>
#include <iostream>
#include"Sound.h"
#include <string>
#include <fstream>
#include<stdio.h>
#include<vector>
#include "ModalModel.h"
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



void damping_sound(int obj, int mat, double amp)
{
	
 soundplay_sfml(obj,mat,amp);
	
}


