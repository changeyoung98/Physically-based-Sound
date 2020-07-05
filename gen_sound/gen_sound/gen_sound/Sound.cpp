#include "Sound.h"
#include<SFML\Audio.hpp>
#include <Eigen\Dense>
#include <algorithm>
#include <iostream>
#include <fstream>
#include<time.h>
#include "ModalModel.h"
using namespace std;

#define SECOND 3
#define SAMPLING_HZ 44100
#define BUFFER_LENGTH (SECOND * SAMPLING_HZ)
#define SOUND_HZ 14706

static const int SR = 44100;
static const int TS = 3;
double normalizeScale_damping = -1.0;
double normalizeScale_no_dampin = -1.0;
std::vector<int>    vidS2T_;        // vertex map for surface VID to tet VID
int                 numFixed_;
void load_vertex_map(const std::string& filename)
{
	int id1, id2;

	std::ifstream fin(filename);
	if (fin.fail())
	{
		//	PRINT_ERROR("Cannot read file: %s\n", filename.toStdString().c_str());
		//	SHOULD_NEVER_HAPPEN(2);
	}
	fin >> numFixed_ >> id1;    // # of fixed vertices in tet mesh
								// & total number of surface vertices
	
//	PRINT_MSG("  # of fixed vertices: %d\n", numFixed_);
	vidS2T_.resize(id1);
	cout << "num" << id1 << endl;
	for (size_t i = vidS2T_.size(); i > 0; --i)
	{
		fin >> id1 >> id2;
		if (id2 >= vidS2T_.size())
		{
			//	PRINT_ERROR("Id2 is out of range in geometry file\n");
			//	SHOULD_NEVER_HAPPEN(3);
		}
		vidS2T_[id2] = id1;
	}
	if (fin.fail())
	{
		//	PRINT_ERROR("Error occurred while reading file: %s\n", filename.toStdString().c_str());
		//	SHOULD_NEVER_HAPPEN(2);
	}
	fin.close();
}
//namespace sound {
	
	void soundplay_sfml(int obj, int mat ,double amplitude)
	{
		
		clock_t t1 = clock();
		//std::vector<double> const &force, modalmodel &modal, massspringsystem &sys
		const unsigned SAMPLES = 44100;
		const unsigned SAMPLE_RATE = 44100;
		const unsigned AMPLITUDE = 32767;

		sf::Int16 raw[SR * TS] = { 0 };
		const int totticks = SR * TS;
		string pre = "D:/ready/" + to_string(obj) + "/mat-" + to_string(mat);
		load_vertex_map(pre+"/obj-"+to_string(obj)+".vmap");
		//load_moments(filename);
		string filename = "D:/ready/mat/mat-" + to_string(mat) + ".txt";
		std::ifstream fin(filename);
		double dense, alpha,beta;
		fin >> dense >> alpha >> beta;
		cout << dense << alpha << beta << endl;
		fin.close();
		ModalModel* modal;
		modal = new ModalModel(
			pre+"/obj-"+to_string(obj)+ ".ev" ,
			dense,//2700,//7850.000000,//615,750
			alpha,//1e-7,//3.00000e-08,//5e-6,2e-6,
			beta);//5 5 35  60
		std::vector<double> Force_Ud;
		Force_Ud.resize(modal->num_modes());
		memset(Force_Ud.data(), 0, sizeof(double) * Force_Ud.size());
		cout << "Force: (Assigned)" << std::endl;

		//int selTriId = 0;

		Eigen::Vector3d dir;
		dir <<-0.11294752, -0.31060568, -0.028931; // 【TODO】:目前的力默认施加在模型的node 0 处，相当于将模型的碰撞视为一个整体，目前均以magic number形式处理
		//参考modalsound项目，需要将模型碰撞点对应起来，这里的dir应该是对应顶点的偏移位置，vidS2T_[n]中的n应为碰撞顶点的标号
		modal->accum_modal_impulse(vidS2T_[0] - numFixed_, dir,Force_Ud);
		
		clock_t t2 = clock();
		cout << t2 - t1 << endl;
		const std::vector<double>& omegaD = modal->damped_omega();  //no damping
		const std::vector<double>& c = modal->damping_vector();
	//	const std::vector<pointmass*> mass = sys.getmasses();
		for (int i = 0; i < modal->num_modes(); i++)
		{
			//cout << "Force:" << Force_Ud[i] << endl;
			const double SS = Force_Ud[i] *470000/ omegaD[i]*0.4; /// 【TODO】: 这里的470000也是一个magic number，后续如果加入声音传递波动方程，这里的470000要替换为方程计算值
			for (int ti = 0; ti < totticks; ti++)
			{
				const double ts = static_cast<double>(ti) / static_cast<double>(SR);
				const double amp = exp(-c[i] * 0.5 * ts);
				if (amp < 1e-3) 
				{
					//cout <<amp<< " break" << endl;
					break;  //cut amplitude
				}
				//cout << amp << endl;
				raw[ti] += sin(omegaD[i] * ts) * amp * SS * amplitude * 1000; // SHRT_MAX; //

			}
		}
		clock_t t3 = clock();
		cout << t3 - t2 << endl;
		sf::SoundBuffer Buffer;

		if (!Buffer.loadFromSamples(raw, totticks, 1, SAMPLE_RATE)) {
			std::cerr << "Loading failed!" << std::endl;
			return;
		}

		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.setLoop(false);
		clock_t t4 = clock();
		Sound.play();
		
		cout << t4 - t3 << std::endl;
		sf::sleep(sf::seconds(0.5));
		Sound.stop();
		//while (1) {
		//	sf::sleep(sf::milliseconds(100));
		//}
	}

