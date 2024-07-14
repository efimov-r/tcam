#ifndef STEREO_FILTER_HPP_
#define STEREO_FILTER_HPP_

#include <iostream>
#include <math.h>

namespace sfil {
	int vertical_line_filterred(unsigned char (&result)[120][160]);
	int filterred(unsigned char (&result)[120][160]);
}

#endif
