/*
 * A lot of this class implementation is based on how Vector2 is implemented in Geometry Central.
 */

#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

struct Vector2 {
    double x, y;

    // Access-by-index
    double& operator[](int index) { return (&x)[index]; }
    double operator[](int index) const { return (&x)[index]; };

    // Overloaded operators
    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(double s) const;
    Vector2 operator/(double s) const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const double& s);
    Vector2& operator/=(const double& s);
    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;
    const Vector2 operator-() const;

    // Does not modify in place
    Vector2 normalize() const;

    double norm() const;
    double norm2() const;
};

template <typename T>
Vector2 operator*(const T s, const Vector2& v);

::std::ostream& operator<<(std::ostream& output, const Vector2& v);
::std::istream& operator<<(std::istream& input, Vector2& v);

#include "msfit/utilities/vector2.ipp"