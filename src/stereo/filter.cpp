#include "filter.hpp"

namespace sfil {

	int vertical_line_filterred(unsigned char (&result)[120][160]) {
		int i = 0;
		for(int x = 1; x >= -80; x--) {
			result[119][i] = 1 - x;
					  i++;
		}

		int count = 0;
//		for(int x = 2; x < 158; x++)
//			for(int y = 0; y < 119; y=y+1){ //todo настроить
//				if(result[y][x] == result[y+1][x] && result[y][x] == result[y+2][x] && result[y][x] == result[y+3][x]){
//					int max = 0;
//					int min = 999;
//					int count_empty = 0;
//					unsigned char d[16];
//					d[0] = result[y-2][x];
//					d[1] = result[y-2][x+1];
//					d[2] = result[y-2][x+2];
//					d[3] = result[y-2][x+3];
//					d[4] = result[y-1][x];
//					d[5] = result[y-1][x+1];
//					d[6] = result[y-1][x+2];
//					d[7] = result[y-1][x+3];
//					d[8] = result[y+1][x];
//					d[9] = result[y+1][x+1];
//					d[10] = result[y+1][x+2];
//					d[11] = result[y+1][x+3];
//					d[12] = result[y+2][x];
//					d[13] = result[y+2][x+1];
//					d[14] = result[y+2][x+2];
//					d[15] = result[y+2][x+3];
//					for(int i = 0; i < 16; i++){
//						if(d[i]!= 255) {
//							if(min > d[i]) {
//							min = d[i];
//							}
//							if(max < d[i]) {
//								max = d[i];
//							}
//						} else {
//							count_empty++;
//						}
//					}
//					if(max - min <3 && count_empty < 3) { //todo настроить
////						result[y][x] = (unsigned char)max;
////						result[y][x] = (unsigned char)max;
////						result[y][x] = (unsigned char)max;
////						result[y][x] = (unsigned char)max;
//						count++;
//					}
//				}
//			}
//		std::cout << count << std::endl;
		return count;
	}

	int filterred(unsigned char (&result)[120][160]) {
		int count = 0;
		for(int x = 1; x < 159; x++)
			for(int y = 1; y < 119; y++){
				unsigned char a1 = result[y-1][x+1];
				unsigned char a2 = result[y][x+1];
				unsigned char a3 = result[y+1][x+1];
				unsigned char a4 = result[y+1][x];
				unsigned char a5 = result[y+1][x-1];
				unsigned char a6 = result[y][x-1];
				unsigned char a7 = result[y-1][x-1];
				unsigned char a8 = result[y-1][x];
				if(a1 == result[y][x] && a5 == result[y][x]) {
					continue;
				}
				if(a5 == a3 && a5 != result[y][x] && a5 != 255) {
					if(a4 == a5) {
						if(a6 == a5) {
							if(a6 == a7) {
								count++;
								result[y][x] = a5;
							} else {
								continue;
							}
						} else {
							if(a2 == a3) {
								if(a1 == a2) {
									count++;
									result[y][x] = a1;
								} else {
									continue;
								}
							} else {
								continue;
							}
						}
					} else {
						continue;
					}
				} else {
					if(a5 == a8 && a5 != result[y][x] && a5 != 255) {
						if(a6 == a5 && a7 == a8) {
							if(a1 == a8 || a4 == a8) {
								count++;
								result[y][x] = a8;
							} else {
								continue;
							}
						} else {
							continue;
						}
					}
					else {
						if(a8 == a3 && a8 != result[y][x] && a8 != 255) {
							if(a3 == a2 && a8 == a1) {
								if(a7 == a8 || a4 == a8) {
									count++;
									result[y][x] = a8;
								} else {
									continue;
								}
							} else {
								continue;
							}
						}
					}
				}
			}
//		std::cout << count << std::endl;
		return count;
	}

}
