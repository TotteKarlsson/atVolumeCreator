#ifndef atRenderBoxH
#define atRenderBoxH
#include <cstdlib>
//---------------------------------------------------------------------------

using std::abs;
//!TODO, write code to update the box when something changes..

class RenderBox
{
	public:
	    								RenderBox(double x = 0, double y = 0, double width = 0, double height = 0);

		void							setX1(double x){mX1 = x;}
        double							getX1(){return mX1;}
		void							setX2(double x){mWidth = abs(mX1 - x);}

        double							getX2(){return mX1 + mWidth;}
		void							setWidth(double w){mWidth = w;}
        double							getWidth(){return mWidth;}

		void							setY1(double y){mY1 = y;}
        double							getY1(){return mY1;}
		void							setY2(double y){mHeight = abs(mY1 - y);}
        double							getY2(){return mY1 + mHeight;}
		void							setHeight(double h){mHeight = h;}
        double							getHeight(){return mHeight;}


        void							setZ(double z){mZ = z;}
        double							getZ(){return mZ;}


        void 							zoom(double persent);

	protected:
    	int								mZ;
	    double                          mX1;
    	double                          mY1;
	    double                          mWidth;
    	double                          mHeight;

};

#endif
