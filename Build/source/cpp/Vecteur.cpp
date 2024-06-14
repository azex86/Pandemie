#include "Vecteur.hpp"

    double Vecteur::norme() const
    {
        return pow(x * x + y * y, 0.5);
    }

	//norme squared
    double Vecteur::magnitude() const
    {
        return x * x + y * y;
    }

    Vecteur::Vecteur() {
        x = 0;
        y = 0;
    }
    Vecteur::Vecteur(U x) {
        this->x = x;
        y = 0;
    }

    Vecteur::Vecteur(U x, U y) {
        this->x = x;
        this->y = y;
    }


    Vecteur Vecteur::operator+(const Vecteur& v)const {
        return Vecteur(x + v.x, y + v.y);
    }

    void Vecteur::operator+=(const Vecteur& v)
    {
        x += v.x;
        y += v.y;

    }

    Vecteur Vecteur::operator-() const
    {
        return Vecteur(-x,-y);
    }

    Vecteur Vecteur::operator-(const Vecteur& v)const {
        return Vecteur(x - v.x, y - v.y);
    }
    void Vecteur::operator-=(const Vecteur& v)
    {
        x -= v.x;
        y -= v.y;
    }
    Vecteur Vecteur::operator*(const double k)const {
        return Vecteur(x * k, y * k);
    }

    void Vecteur::operator*=(double i)
    {
        x *= i;
        y *= i;
    }

    Vecteur Vecteur::operator/(const double k)const {
        return Vecteur(x / k, y / k);
    }

    void Vecteur::operator/=(double i)
    {
        x /= i;
        y /= i;
    }

    bool Vecteur::operator==(const Vecteur& v)const
    {
        if (v.x == x && v.y == y)
            return true;
        else
            return false;
    }

    Vecteur::operator sf::Vector2f() const
    {
        return sf::Vector2f(x,y);
    }

    std::ostream& operator<<(std::ostream& out, const Vecteur& _vec)
    {
        // TODO: insérer une instruction return ici
        out << '(' + std::to_string(_vec.x) + ';' + std::to_string(_vec.y) +(std::string)")";

        return out;
    }
    Vecteur randVecteur(const Vecteur& max)
    {
        return Vecteur(rand()%(int)max.x,rand()%(int)max.y);
    }
