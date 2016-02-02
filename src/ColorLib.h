/*
 * ColorLib.h
 *
 *  Created on: 01/02/2016
 *      Author: alvaro
 */

#ifndef COLORLIB_H_
#define COLORLIB_H_

#include<vector>
# include "opencv2/core/core.hpp"
using namespace cv;


class ColorLib {
public:
	ColorLib();
	virtual ~ColorLib();
	void detectColor(int ID);

};

#endif /* COLORLIB_H_ */
