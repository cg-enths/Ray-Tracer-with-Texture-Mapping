#ifndef COLOR_H
#define COLOR_H
#include <math.h> 

class Color 
{
	public:
		double R, G, B;
		Color();
		~Color();
		Color(double R, double G, double B);
		Color Normalize();
		Color Ceil();
		Color Round();
		bool operator==(const Color C);
		Color operator=(const Color C);
		Color operator+(const Color C);
		Color operator-(const Color C);  
		Color operator*(const Color C);
		Color operator/(double number);
		Color operator*(double number);

} ;

Color::Color() {} ;

Color::~Color() {} ;
		
Color::Color(double R, double G, double B)
{
	this->R = R;
	this->G = G;
	this->B = B;
} ;

Color Color::Normalize()
{
	R = R > 255 ? 255 : (R < 0 ? 0 : R) ;
	G = G > 255 ? 255 : (G < 0 ? 0 : G) ;
	B = B > 255 ? 255 : (B < 0 ? 0 : B) ;
	return *this;
}

Color Color::Ceil()
{
	R = ceil(R);
	G = ceil(G);
	B = ceil(B);
	return *this;
}

Color Color::Round()
{
	R = round(R);
	G = round(G);
	B = round(B);
	return *this;
}

bool Color::operator==(const Color C)
{
	return (R == C.R && G == C.G && B == C.B);
} ;
		
Color Color::operator=(const Color C)
{
	this->R = C.R;
	this->G = C.G;
	this->B = C.B;
	return (*this);
} ;

Color Color::operator+(const Color C)
{
	return Color(R + C.R, G + C.G, B + C.B).Ceil().Normalize();
} ;

Color Color::operator-(const Color C)
{
	return Color(R - C.R, G - C.G, B - C.B).Ceil().Normalize();
} ;

Color Color::operator*(const Color C)
{
	return Color(R * C.R, G * C.G, B * C.B).Ceil().Normalize();
} ;

Color Color::operator/(double number)
{
	return Color(R/number, G/number, B/number).Ceil().Normalize();
} ;

Color Color::operator*(double number)
{
	return Color(R * number, G  * number, B * number).Ceil().Normalize();
} ;


#endif
