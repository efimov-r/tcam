#include "stereo.hpp"
#include <chrono>
#include "visualer.hpp"
#include "filter.hpp"

namespace st {
	using namespace cv;

	Mat grame1, grame2;

	int menu_delta=10;
	int menu_predel=400;

	int count = 0;


	unsigned char result[120][160];

	//todo если в сером точка плохая, попробовать просчитать ее в rgb, вдруг удасться найти пару!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int delta_c(int yp, int yp1, int yp2, int yp3, int xp, int xp1, int xp2, int xp3) {
		int max = 0;
		int min = 999;
		uchar m[16];
		m[0]=grame1.at<uchar>(yp, xp);
		m[1]=grame1.at<uchar>(yp1, xp);
		m[2]=grame1.at<uchar>(yp2, xp);
		m[3]=grame1.at<uchar>(yp3, xp);
		m[4]=grame1.at<uchar>(yp, xp1);
		m[5]=grame1.at<uchar>(yp1, xp1);
		m[6]=grame1.at<uchar>(yp2, xp1);
		m[7]=grame1.at<uchar>(yp3, xp1);
		m[8]=grame1.at<uchar>(yp, xp2);
		m[9]=grame1.at<uchar>(yp1, xp2);
		m[10]=grame1.at<uchar>(yp2, xp2);
		m[11]=grame1.at<uchar>(yp3, xp2);
		m[12]=grame1.at<uchar>(yp, xp3);
		m[13]=grame1.at<uchar>(yp1, xp3);
		m[14]=grame1.at<uchar>(yp2, xp3);
		m[15]=grame1.at<uchar>(yp3, xp3);
		for(int i = 0; i < 16; i++){
			if(min > m[i]) {
				min = m[i];
			}
			if(max < m[i]) {
				max = m[i];
			}
		}
		return max - min;
	}

	void pixel_work(int y_n, int x_n) {
		int min = 99000;
		int rx=-555;

		int xp = x_n*4;
		int xp1 = xp + 1;
    	int xp2 = xp + 2;
		int xp3 = xp + 3;
		int yp = y_n*4;
		int yp1 = yp + 1;
		int yp2 = yp + 2;
		int yp3 = yp + 3;
		int ypp = yp+3;
		int ypp1 = yp1+3;
		int ypp2 = yp2+3;
		int ypp3 = yp3+3;

		int xxd;
		int r;

		if(result[y_n][x_n] != 255) {
			return;
		}

		//проверка значимости точки
		if(delta_c(yp, yp1, yp2, yp3, xp, xp1, xp2, xp3) < 10) {
			return;
		}

		//пердсказания
		if(y_n != 0 && result[y_n-1][x_n] != 255) {
			unsigned char topp = result[y_n-1][x_n];
			topp = 1 - topp;
			xxd = xp + topp;
			r =  abs(grame1.at<uchar>(yp, xp) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r +abs(grame1.at<uchar>(yp, xp1) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp1) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp1) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp1) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r + abs(grame1.at<uchar>(yp, xp2) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp2) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp2) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp2) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r + abs(grame1.at<uchar>(yp, xp3) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp3) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp3) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp3) - grame2.at<uchar>(ypp3, xxd));
			if(r < 600) {
				result[y_n][x_n] = 1-topp;
				return;
			}
		}

		for(int x = 1; x >= -80; x--) {
			xxd = xp + x;
			r =  abs(grame1.at<uchar>(yp, xp) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r +abs(grame1.at<uchar>(yp, xp1) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp1) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp1) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp1) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r + abs(grame1.at<uchar>(yp, xp2) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp2) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp2) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp2) - grame2.at<uchar>(ypp3, xxd));
			xxd++;
			r = r + abs(grame1.at<uchar>(yp, xp3) - grame2.at<uchar>(ypp, xxd))+
					abs(grame1.at<uchar>(yp1, xp3) - grame2.at<uchar>(ypp1, xxd))+
					abs(grame1.at<uchar>(yp2, xp3) - grame2.at<uchar>(ypp2, xxd))+
					abs(grame1.at<uchar>(yp3, xp3) - grame2.at<uchar>(ypp3, xxd));
			if(r < 50) {
				result[y_n][x_n] = 1-x;
				return;
			}
			if(r < min) {
				min = r;
				rx=x;
			}
		}
		result[y_n][x_n] = 1-rx;
	}

	bool stereo_work(cv::Mat f1, cv::Mat f2) {
		count = 0;
		for(int y = 0; y<120; y++)
			for(int x=0; x<160; x++)
				result[y][x] = 255;

		auto start = std::chrono::high_resolution_clock::now(); // сохраняем время начала выполнения функции
		cvtColor(f1, grame1, COLOR_BGR2GRAY);
		cvtColor(f2, grame2, COLOR_BGR2GRAY);


//		frame1 = Mat(Size(640, 480), CV_8UC3, Scalar(0, 0, 255));
//		frame2 = Mat(Size(640, 480), CV_8UC3, Scalar(0, 0, 255));
//		svis::stereo_drow(f1, f2);
//		svis::roi_drow(f1, f2);


		for(int x = 0; x < 160; x++)
			for(int y = 0; y < 120; y++)
				pixel_work(y, x);
//
//		//todo vertical line filter
//		sfil::filterred(result);
		sfil::vertical_line_filterred(result);

		auto end = std::chrono::high_resolution_clock::now(); // сохраняем время окончания выполнения функции
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // вычисляем продолжительность выполнения функции в микросекундах
				std::cout << "Duration: " << duration.count() << " milllisec" << std::endl; // выводим продолжительность выполнения функции в консоль


//		std::cout << count << std::endl;


		svis::stereo_drow(f1, f2);
		svis::depth_drow(result, ("predel" + (std::to_string(menu_predel))), ("delta" + (std::to_string(menu_delta))));


		int k = cv::waitKey(10);
		if(k == 93) {
			menu_predel = menu_predel + 300;
		}
		if(k == 91) {
			menu_predel = menu_predel - 300;
		}
		if(k == 39) {
			menu_delta = menu_delta + 1;
		}
		if(k == 59) {
			menu_delta = menu_delta - 1;
		}
		if(k == 112) {
			if(cv::waitKey(500000) == 112) {
				return false;
			}
		}
		if(k == 113) {
			return true;
		}
		return false;
	}
}
