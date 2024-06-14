#pragma once
#include<cmath>
#include <iostream>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>

#ifdef _DEBUG
typedef double U;
#else
typedef float U;
#endif // _DEBUG



//template<typename U>
class Vecteur {
	public:
		U x;
		U y;

		Vecteur(U x, U y);
		Vecteur(U x);
		Vecteur();

		double norme()const;
		double magnitude()const;
		Vecteur operator+(const Vecteur &v)const;
		void operator+=(const Vecteur& v);
		Vecteur operator-()const;
		Vecteur operator-(const Vecteur& v)const;
		void operator-=(const Vecteur& v);
		Vecteur operator*(const double i)const;
		void operator*=(const double i);
		Vecteur operator/(const double i)const;
		void operator/=(double i);
		bool operator==(const Vecteur& v)const;
		operator sf::Vector2f() const;
};

std::ostream& operator<< (std::ostream& out, const Vecteur& _vec);
	
Vecteur randVecteur(const Vecteur& max);
	


