#include "stereo.hpp"
#include <chrono>
#include "visualer.hpp"
#include "filter.hpp"
#include <thread>
#include <mutex>

#define CENTER_X 320
#define CENTER_Y 240

#define HEIGHT 480
#define WEIGHT 640

#define DOPUSK_COLOR_H 9
#define PREDEL_COLOR_S 30
#define PREDEL_COLOR_V 100
#define PREDEL_WHITE_S 25
#define PREDEL_WHITE_V 200
#define PREDEL_BLACK_V 50
#define PREDEL_SMOLL 10

namespace so {
	using namespace cv;

	int result[HEIGHT][WEIGHT]; //test

	struct Res_f1 { //элемент результата обработки frame1
		int cx;//центр
		int cy;
		int x1;//границы
		int x2;
		int y1;
		int y2;
		int av;//средний цвет центра
	};

	std::mutex b_lock;
	std::mutex w_lock;
	std::mutex c_lock;


	void resultClear() {
		for(int dx = 0; dx < WEIGHT; dx++)
				for(int dy = 0; dy < HEIGHT; dy++)
					result[dy][dx] = 0;
	}

	void resultToFrame(cv::Mat& f1) {
		for(int dx = 0; dx < WEIGHT; dx++)
			for(int dy = 0; dy < HEIGHT; dy++) {
				if(result[dy][dx] == 1) {
					f1.at<cv::Vec3b>(dy, dx) = {0, 0, 0};
				}
			}
	}

	void paintBlack(cv::Mat& hsvMap, Res_f1& res) {
				res.x1 = 0;
				res.x2 = 0;
				res.y1 = 0;
				res.y2 = 0;

				for(int dx = -1; dx <=1; dx++)
					for(int dy = -1; dy <= 1; dy++) {
						if(hsvMap.at<Vec3b>(res.cy + dy, res.cx + dx)[2] > PREDEL_BLACK_V) {
							return;
						}
					}

				for(int dx = -1; dx <=1; dx++)
					for(int dy = -1; dy <= 1; dy++) {
						result[res.cy + dy][res.cx + dx] = 1;
					}

				int y;
				int x;
				int x1;
				int x2;
				int y1;
				int y2;
				int count_top = 100;
				int count_down = 100;
				int count_left = 100;
				int count_right = 100;
				int count_bock = 0;

				//цикл кругов
				for(int circl = 2; circl<220; circl++) {
					x1 = res.cx - circl;
					x2 = res.cx + circl;
					y1 = res.cy - circl;
					y2 = res.cy + circl;
					count_bock = 0;

					if(x1 < 0 || y1 < 0 || x2 >= WEIGHT || y2 >= HEIGHT) {
						res.x1 = 0;
						res.x2 = 0;
						res.y1 = 0;
						res.y2 = 0;
		//				std::cout << "over list" << std::endl;
						return;
					}
					if(circl >=215) {
						res.x1 = 0;
						res.x2 = 0;
						res.y1 = 0;
						res.y2 = 0;
		//				std::cout << "over circle" << std::endl;
						return;
					}
					if(count_left > circl || count_left > 5) {
						count_left = 0;
						count_bock++;
						//лево
						for(int d=-1*circl; d <= circl; d++) {
							y = res.cy + d;
							if(hsvMap.at<Vec3b>(y, x1)[2] < PREDEL_BLACK_V) {
								count_left++;
								result[y][x1] = 1;
								res.x1 = x1;
							}
						}
					}

					if(count_right > circl || count_right > 5) {
						count_bock++;
						//право
						count_right = 0;
						for(int d=-1*circl; d <= circl; d++) {
							y = res.cy + d;
							if(hsvMap.at<Vec3b>(y, x2)[2] < PREDEL_BLACK_V) {
								count_right++;
								result[y][x2] = 1;
								res.x2 = x2;
							}
						}
					}

					if(count_top > circl || count_top > 5) {
						count_bock++;
						count_top = 0;
						//верх
						for(int d=-1*circl; d <= circl; d++) {
							x = res.cx + d;
							if(hsvMap.at<Vec3b>(y1, x)[2] < PREDEL_BLACK_V) {
								count_top++;
								result[y1][x] = 1;
								res.y1 = y1;
							}
						}
					}

					if(count_down > circl || count_down > 5) {
						count_bock++;
						count_down = 0;
						//низ
						for(int d=-1*circl; d <= circl; d++) {
							x = res.cx + d;
							if(hsvMap.at<Vec3b>(y2, x)[2] < PREDEL_BLACK_V) {
								count_down++;
								result[y2][x] = 1;
								res.y2 = y2;
							}
						}
					}
					if(count_bock == 0) {
						break;
					}
				}
				if(abs(res.y1 - res.y2) < PREDEL_SMOLL || abs(res.x1 - res.x2) < PREDEL_SMOLL) {
					res.x1 = 0;
					res.x2 = 0;
					res.y1 = 0;
					res.y2 = 0;
				}
				return;
			}

	void paintWhite(cv::Mat& hsvMap, Res_f1& res) {
			res.x1 = 0;
			res.x2 = 0;
			res.y1 = 0;
			res.y2 = 0;

			for(int dx = -1; dx <=1; dx++)
				for(int dy = -1; dy <= 1; dy++) {
					if(hsvMap.at<Vec3b>(res.cy + dy, res.cx + dx)[1] > PREDEL_WHITE_S) {
						return;
					}
					if(hsvMap.at<Vec3b>(res.cy + dy, res.cx + dx)[2] < PREDEL_WHITE_V) {
						return;
					}
				}

			for(int dx = -1; dx <=1; dx++)
				for(int dy = -1; dy <= 1; dy++) {
					result[res.cy + dy][res.cx + dx] = 1;
				}

			int y;
			int x;
			int x1;
			int x2;
			int y1;
			int y2;
			int count_top = 100;
			int count_down = 100;
			int count_left = 100;
			int count_right = 100;
			int count_bock = 0;

			//цикл кругов
			for(int circl = 2; circl<220; circl++) {
				x1 = res.cx - circl;
				x2 = res.cx + circl;
				y1 = res.cy - circl;
				y2 = res.cy + circl;
				count_bock = 0;

				if(x1 < 0 || y1 < 0 || x2 >= WEIGHT || y2 >= HEIGHT) {
					res.x1 = 0;
					res.x2 = 0;
					res.y1 = 0;
					res.y2 = 0;
	//				std::cout << "over list" << std::endl;
					return;
				}
				if(circl >=215) {
					res.x1 = 0;
					res.x2 = 0;
					res.y1 = 0;
					res.y2 = 0;
	//				std::cout << "over circle" << std::endl;
					return;
				}
				if(count_left > circl || count_left > 5) {
					count_left = 0;
					count_bock++;
					//лево
					for(int d=-1*circl; d <= circl; d++) {
						y = res.cy + d;
						if(hsvMap.at<Vec3b>(y, x1)[1] < PREDEL_WHITE_S &&
								hsvMap.at<Vec3b>(y, x1)[2] > PREDEL_WHITE_V) {
							count_left++;
							result[y][x1] = 1;
							res.x1 = x1;
						}
					}
				}

				if(count_right > circl || count_right > 5) {
					count_bock++;
					//право
					count_right = 0;
					for(int d=-1*circl; d <= circl; d++) {
						y = res.cy + d;
						if(hsvMap.at<Vec3b>(y, x2)[1] < PREDEL_WHITE_S &&
								hsvMap.at<Vec3b>(y, x2)[2] > PREDEL_WHITE_V) {
							count_right++;
							result[y][x2] = 1;
							res.x2 = x2;
						}
					}
				}

				if(count_top > circl || count_top > 5) {
					count_bock++;
					count_top = 0;
					//верх
					for(int d=-1*circl; d <= circl; d++) {
						x = res.cx + d;
						if(hsvMap.at<Vec3b>(y1, x)[1] < PREDEL_WHITE_S &&
								hsvMap.at<Vec3b>(y1, x)[2] > PREDEL_WHITE_V) {
							count_top++;
							result[y1][x] = 1;
							res.y1 = y1;
						}
					}
				}

				if(count_down > circl || count_down > 5) {
					count_bock++;
					count_down = 0;
					//низ
					for(int d=-1*circl; d <= circl; d++) {
						x = res.cx + d;
						if(hsvMap.at<Vec3b>(y2, x)[1] < PREDEL_WHITE_S &&
								hsvMap.at<Vec3b>(y2, x)[2] > PREDEL_WHITE_V) {
							count_down++;
							result[y2][x] = 1;
							res.y2 = y2;
						}
					}
				}
				if(count_bock == 0) {
					break;
				}
			}
			if(abs(res.y1 - res.y2) < PREDEL_SMOLL || abs(res.x1 - res.x2) < PREDEL_SMOLL) {
				res.x1 = 0;
				res.x2 = 0;
				res.y1 = 0;
				res.y2 = 0;
			}
			return;
		}

	void paintColor(cv::Mat& hsvMap, Res_f1& res) {
		long av = hsvMap.at<Vec3b>(res.cy, res.cx)[0];
		long count_p = 1;
		res.x1 = 0;
		res.x2 = 0;
		res.y1 = 0;
		res.y2 = 0;

		int vdx, vdy;
		for(int dx = -1; dx <=1; dx++)
			for(int dy = -1; dy <= 1; dy++) {
				if(dx == 0 && dy == 0) {
					continue;
				}
				vdx = res.cx + dx;
				vdy = res.cy + dy;
				if(abs(av - hsvMap.at<Vec3b>(vdy, vdx)[0]) > DOPUSK_COLOR_H) {
					return;
				}
				if(hsvMap.at<Vec3b>(vdy, vdx)[1] < PREDEL_COLOR_S) {
					return;
				}
				if(hsvMap.at<Vec3b>(vdy, vdx)[2] < PREDEL_COLOR_V) {
					return;
				}

				av = ((av*count_p) + hsvMap.at<Vec3b>(vdy, vdx)[0]);
				count_p++;
				av = av/count_p;
			}
		res.av = av;

		for(int dx = -1; dx <=1; dx++)
			for(int dy = -1; dy <= 1; dy++) {
				result[res.cy + dy][res.cx + dx] = 1;
			}

		int y;
		int x;
		int x1;
		int x2;
		int y1;
		int y2;
		int count_top = 100;
		int count_down = 100;
		int count_left = 100;
		int count_right = 100;
		int count_bock = 0;

		//цикл кругов
		for(int circl = 2; circl<220; circl++) {
			x1 = res.cx - circl;
			x2 = res.cx + circl;
			y1 = res.cy - circl;
			y2 = res.cy + circl;
			count_bock = 0;

			if(x1 < 0 || y1 < 0 || x2 >= WEIGHT || y2 >= HEIGHT) {
				res.x1 = 0;
				res.x2 = 0;
				res.y1 = 0;
				res.y2 = 0;
//				std::cout << "over list" << std::endl;
				return;
			}
			if(circl >=215) {
				res.x1 = 0;
				res.x2 = 0;
				res.y1 = 0;
				res.y2 = 0;
//				std::cout << "over circle" << std::endl;
				return;
			}
			if(count_left > circl || count_left > 5) {
				count_left = 0;
				count_bock++;
				//лево
				for(int d=-1*circl; d <= circl; d++) {
					y = res.cy + d;
					if(abs(av - hsvMap.at<Vec3b>(y, x1)[0]) < DOPUSK_COLOR_H &&
							hsvMap.at<Vec3b>(y, x1)[1] > PREDEL_COLOR_S &&
							hsvMap.at<Vec3b>(y, x1)[2] > PREDEL_COLOR_V) {
						count_left++;
						result[y][x1] = 1;
						res.x1 = x1;
					}
				}
			}

			if(count_right > circl || count_right > 5) {
				count_bock++;
				//право
				count_right = 0;
				for(int d=-1*circl; d <= circl; d++) {
					y = res.cy + d;
					if(abs(av - hsvMap.at<Vec3b>(y, x2)[0]) < DOPUSK_COLOR_H &&
							hsvMap.at<Vec3b>(y, x2)[1] > PREDEL_COLOR_S &&
							hsvMap.at<Vec3b>(y, x2)[2] > PREDEL_COLOR_V) {
						count_right++;
						result[y][x2] = 1;
						res.x2 = x2;
					}
				}
			}

			if(count_top > circl || count_top > 5) {
				count_bock++;
				count_top = 0;
				//верх
				for(int d=-1*circl; d <= circl; d++) {
					x = res.cx + d;
					if(abs(av - hsvMap.at<Vec3b>(y1, x)[0]) < DOPUSK_COLOR_H &&
							hsvMap.at<Vec3b>(y1, x)[1] > PREDEL_COLOR_S &&
							hsvMap.at<Vec3b>(y1, x)[2] > PREDEL_COLOR_V) {
						count_top++;
						result[y1][x] = 1;
						res.y1 = y1;
					}
				}
			}

			if(count_down > circl || count_down > 5) {
				count_bock++;
				count_down = 0;
				//низ
				for(int d=-1*circl; d <= circl; d++) {
					x = res.cx + d;
					if(abs(av - hsvMap.at<Vec3b>(y2, x)[0]) < DOPUSK_COLOR_H &&
							hsvMap.at<Vec3b>(y2, x)[1] > PREDEL_COLOR_S &&
							hsvMap.at<Vec3b>(y2, x)[2] > PREDEL_COLOR_V) {
						count_down++;
						result[y2][x] = 1;
						res.y2 = y2;
					}
				}
			}
			if(count_bock == 0) {
				break;
			}
		}
		if(abs(res.y1 - res.y2) < PREDEL_SMOLL || abs(res.x1 - res.x2) < PREDEL_SMOLL) {
			res.x1 = 0;
			res.x2 = 0;
			res.y1 = 0;
			res.y2 = 0;
		}
		return;
	}


	void thf(cv::Mat& hsvMap, std::vector<Res_f1>& vw, std::vector<Res_f1>& vb, std::vector<Res_f1>& vc, int h_start, int h_stop)
		{
			Res_f1 res;
			for(int yc = h_start; yc < h_stop; yc = yc + 10) {
				for(int xc = 15; xc < WEIGHT; xc = xc + 10) {
					res.cx = xc;
					res.cy = yc;
					paintWhite(hsvMap, res);
					if(res.y1 != 0 && res.y2 != 0 && res.x1 != 0 && res.x2 != 0) {
						w_lock.lock();
						vw.push_back(res);
						w_lock.unlock();
					}
					paintBlack(hsvMap, res);
					if(res.y1 != 0 && res.y2 != 0 && res.x1 != 0 && res.x2 != 0) {
						b_lock.lock();
						vb.push_back(res);
						b_lock.unlock();
					}
					paintColor(hsvMap, res);
					if(res.y1 != 0 && res.y2 != 0 && res.x1 != 0 && res.x2 != 0) {
						c_lock.lock();
						vc.push_back(res);
						c_lock.unlock();
					}
				}
			}
		}


	bool stereo_work(cv::Mat& f1, cv::Mat& f2) {
			resultClear();
			Mat hsvMap;
			cvtColor(f1, hsvMap, COLOR_BGR2HSV);
			auto start = std::chrono::high_resolution_clock::now(); // сохраняем время начала выполнения функции


			std::vector<Res_f1> vb;
			std::vector<Res_f1> vc;
			std::vector<Res_f1> vw;

			std::thread tho1(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 10, 48);
			std::thread tho2(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48, 48*2);
			std::thread tho3(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*2, 48*3);
			std::thread tho4(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*3, 48*4);
			std::thread tho5(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*4, 48*5);
			std::thread tho6(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*5, 48*6);
			std::thread tho7(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*6, 48*7);
			std::thread tho8(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*7, 48*8);
			std::thread tho9(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*8, 48*9);
			std::thread tho10(thf, std::ref(hsvMap), std::ref(vw), std::ref(vb), std::ref(vc), 48*9, 48*10);

			tho1.join();
			tho2.join();
			tho3.join();
			tho4.join();
			tho5.join();
			tho6.join();
			tho7.join();
			tho8.join();
			tho9.join();
			tho10.join();

			std::vector<Res_f1> vb_r;
			std::vector<Res_f1> vc_r;
			std::vector<Res_f1> vw_r;

			for(long unsigned int i = 0; i != vb.size(); ++i) {
				bool err_el = false;
				for(long unsigned int j = i; j != vb.size(); ++j) {
					if(i == j) {
						continue;
					}
					Res_f1 a = vb[i];
					Res_f1 b = vb[j];
					if( abs(a.x1 - b.x1) < 5 &&
							abs(a.x2 - b.x2) < 5 &&
							abs(a.y1 - b.y1) < 5 &&
							abs(a.y2 - b.y2) < 5 ) {
						err_el = true;
					}
				}
				if(!err_el) {
					vb_r.push_back(vb[i]);
				}
			}

			for(long unsigned int i = 0; i != vc.size(); ++i) {
				bool err_el = false;
				for(long unsigned int j = i; j != vc.size(); ++j) {
					if(i == j) {
						continue;
					}
					Res_f1 a = vc[i];
					Res_f1 b = vc[j];
					if( abs(a.x1 - b.x1) < 5 &&
							abs(a.x2 - b.x2) < 5 &&
							abs(a.y1 - b.y1) < 5 &&
							abs(a.y2 - b.y2) < 5 &&
							abs(a.av - b.av) < 3) {
						err_el = true;
					}
				}
				if(!err_el) {
					vc_r.push_back(vc[i]);
				}
			}

			for(long unsigned int i = 0; i != vw.size(); ++i) {
				bool err_el = false;
				for(long unsigned int j = i; j != vw.size(); ++j) {
					if(i == j) {
						continue;
					}
					Res_f1 a = vw[i];
					Res_f1 b = vw[j];
					if( abs(a.x1 - b.x1) < 5 &&
							abs(a.x2 - b.x2) < 5 &&
							abs(a.y1 - b.y1) < 5 &&
							abs(a.y2 - b.y2) < 5 ) {
						err_el = true;
					}
				}
				if(!err_el) {
					vw_r.push_back(vw[i]);
				}
			}

//			int bilo = vc.size() + vw.size() + vb.size();
//			int stalo = vc_r.size() + vw_r.size() + vb_r.size();
//			std::cout << "Count err: " << bilo << " - " << stalo << " = " << bilo - stalo << std::endl;

			int count_rec = 0;

			for(Res_f1 res_b : vb_r) {
				if(res_b.y1 != 0 && res_b.y2 != 0 && res_b.x1 != 0 && res_b.x2 != 0) {
					cv::rectangle(f1, cv::Point(res_b.x1, res_b.y1), cv::Point(res_b.x2, res_b.y2), cv::Scalar(0, 255, 0), 3);
					count_rec++;
				}
			}
			for(Res_f1 res_c : vc_r) {
				if(res_c.y1 != 0 && res_c.y2 != 0 && res_c.x1 != 0 && res_c.x2 != 0) {
					cv::rectangle(f1, cv::Point(res_c.x1, res_c.y1), cv::Point(res_c.x2, res_c.y2), cv::Scalar(255, 0, 0), 3);
					count_rec++;
				}
			}
			for(Res_f1 res_w: vw_r) {
				if(res_w.y1 != 0 && res_w.y2 != 0 && res_w.x1 != 0 && res_w.x2 != 0) {
					cv::rectangle(f1, cv::Point(res_w.x1, res_w.y1), cv::Point(res_w.x2, res_w.y2), cv::Scalar(0, 0, 255), 3);
					count_rec++;
				}
			}
//			std::cout << "Count req: " << count_rec << std::endl;



//			if(res_y1_c != 0 && res_y2_c != 0 && res_x1_c != 0 && res_x2_c != 0) {
//				resultToFrame(f1);
//			}



			auto end = std::chrono::high_resolution_clock::now(); // \u0441\u043e\u0445\u0440\u0430\u043d\u044f\u0435\u043c \u0432\u0440\u0435\u043c\u044f \u043e\u043a\u043e\u043d\u0447\u0430\u043d\u0438\u044f \u0432\u044b\u043f\u043e\u043b\u043d\u0435\u043d\u0438\u044f \u0444\u0443\u043d\u043a\u0446\u0438\u0438
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // \u0432\u044b\u0447\u0438\u0441\u043b\u044f\u0435\u043c \u043f\u0440\u043e\u0434\u043e\u043b\u0436\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c \u0432\u044b\u043f\u043e\u043b\u043d\u0435\u043d\u0438\u044f \u0444\u0443\u043d\u043a\u0446\u0438\u0438 \u0432 \u043c\u0438\u043a\u0440\u043e\u0441\u0435\u043a\u0443\u043d\u0434\u0430\u0445
					std::cout << "Duration: " << duration.count() << " milllisec" << std::endl;


			std::string rc = "hsv: " + std::to_string(hsvMap.at<Vec3b>(CENTER_Y, CENTER_X)[0]) +
					" ," + std::to_string(hsvMap.at<Vec3b>(CENTER_Y, CENTER_X)[1]) +
					" ," + std::to_string(hsvMap.at<Vec3b>(CENTER_Y, CENTER_X)[2]);
			svis::so_drow(f1, rc);


			int k = cv::waitKey(1);
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
