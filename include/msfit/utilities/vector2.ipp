inline Vector2 Vector2::operator+(const Vector2& v) const { return Vector2{x + v.x, y + v.y}; }

inline Vector2 Vector2::operator-(const Vector2& v) const { return Vector2{x - v.x, y - v.y}; }

inline Vector2 Vector2::operator*(double s) const { return Vector2{x * s, y * s}; }

inline Vector2 Vector2::operator/(double s) const {
    const double r = 1. / s;
    return Vector2{x * r, y * r};
}

inline const Vector2 Vector2::operator-() const { return Vector2{-x, -y}; }

template <typename T>
inline Vector2 operator*(const T s, const Vector2& v) {
    return Vector2{s * v.x, s * v.y};
}

inline Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

inline Vector2& Vector2::operator*=(const double& s) {
    x *= s;
    y *= s;
    return *this;
}

inline Vector2& Vector2::operator/=(const double& s) {
    x /= s;
    y /= s;
    return *this;
}

inline bool Vector2::operator==(const Vector2& other) const { return x == other.x && y == other.y; }

inline bool Vector2::operator!=(const Vector2& other) const { return !(*this == other); }

inline Vector2 Vector2::normalize() const {
    double r = 1. / std::sqrt(x * x + y * y);
    return *this * r;
}

inline double Vector2::norm() const { return std::sqrt(x * x + y * y); }
inline double Vector2::norm2() const { return x * x + y * y; }

inline std::ostream& operator<<(std::ostream& output, const Vector2& v) {
    output << "<" << v.x << ", " << v.y << ">";
    return output;
}

inline std::istream& operator>>(std::istream& input, Vector2& v) {
    double x, y;
    input >> x >> y;
    v = Vector2{x, y};
    return input;
}