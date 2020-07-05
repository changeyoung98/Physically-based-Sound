# define _DLLExport __declspec (dllexport)
#include<iostream>
#include<stdio.h>
#include <math.h>
#include<SFML/Graphics.hpp>
#include<SFML\Audio.hpp>
#include <alc.h>
#include <al.h>
#include <Windows.h>
#include <iomanip>
#include "simu_start.hpp"
#include <time.h>
//#include "Sound.h"
#define maxforce 3000
//scale  - meters
// five Points, 4 Spring
// alloyed steel 2e-7 young's modulus
using std::cin; using std::cout;
//K= y/r_restlength   

//double masses[5] = { 400,400,400,400,400 };  //5 points  gold and wood  
//double youngs[4] = { 9e9,9e9,9e9,9e9}; //4 springs

//double restlength[4] = { 1,1,1,1};   //shold be very small



//void simpleui();


/*extern "C" int _DLLExport USound(int obj, int mat, double force) {
	//change_rho(rho);
//	change_force(force);
	damping_sound(obj,mat,force);
	return 0;
}*/

int main()
{
	//simpleui();
	while (true) {
		double amp;
		int obj, mat;
		cin >> obj >> mat >> amp;
		clock_t t1 = clock();
		damping_sound(obj, mat, amp);
		clock_t t2 = clock();
		cout << t2 - t1 << std::endl;
	}
	return 0;
}

/*void simpleui()
{
	while (true)
	{
		std::cout << "Choose Modals:  " << std::endl << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "1. 1D - No Damping" << std::endl;
		std::cout << "2. 1D - Damping" << std::endl;
		std::cout << "3. 3D - No Damping" << std::endl;
		std::cout << "4. 3D - Damping" << std::endl << std::endl;
		int cnt = 0;
		std::cin >> cnt;
		std::string cc;
		std::getline(std::cin, cc);   //delete enter
		switch (cnt)
		{
		case 1: sound_1d_no_damping(); break;
		case 2: sound_1d_damping(); break;
		case 3: no_damping_sound(); break;
		case 4: damping_sound(); break;
		default: break;

		}
		if (cnt <= 0) break;
		std::cout << std::endl << "Play the Sound Successfully!" << std::endl;
	}


}

*/