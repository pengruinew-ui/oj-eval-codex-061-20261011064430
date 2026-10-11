#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace sjtu {

// Exception for division by zero in complex arithmetic
class divided_by_zero final : public std::exception {
public:
  divided_by_zero() = default;
  ~divided_by_zero() override = default;
  const char *what() const noexcept override {
    return "complex divided by zero!";
  }
};

// Helper to determine sign with epsilon tolerance
inline int sign(double x) {
  const double eps = 1e-6;
  return (x > eps) - (x < -eps);
}

class complex {
private:
  double a, b; // a + bi
public:
  // Constructors
  complex() : a(0.0), b(0.0) {}
  explicit complex(double a_) : a(a_), b(0.0) {}
  complex(double a_, double b_) : a(a_), b(b_) {}

  // Accessors (return references)
  double &real() { return a; }
  double &imag() { return b; }
  const double &real() const { return a; }
  const double &imag() const { return b; }

  // Unary operators
  complex operator-() const { return complex(-a, -b); }
  complex operator~() const { return complex(a, -b); }

  // Binary arithmetic
  complex operator+(const complex &rhs) const { return complex(a + rhs.a, b + rhs.b); }
  complex operator-(const complex &rhs) const { return complex(a - rhs.a, b - rhs.b); }
  complex operator*(const complex &rhs) const {
    return complex(a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a);
  }
  complex operator/(const complex &rhs) const {
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) {
      throw divided_by_zero();
    }
    double denom = rhs.a * rhs.a + rhs.b * rhs.b;
    return complex((a * rhs.a + b * rhs.b) / denom,
                   (b * rhs.a - a * rhs.b) / denom);
  }

  // Compound assignments
  complex &operator+=(const complex &rhs) { a += rhs.a; b += rhs.b; return *this; }
  complex &operator-=(const complex &rhs) { a -= rhs.a; b -= rhs.b; return *this; }
  complex &operator*=(const complex &rhs) {
    double na = a * rhs.a - b * rhs.b;
    double nb = a * rhs.b + b * rhs.a;
    a = na; b = nb; return *this;
  }
  complex &operator/=(const complex &rhs) {
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) {
      throw divided_by_zero();
    }
    double denom = rhs.a * rhs.a + rhs.b * rhs.b;
    double na = (a * rhs.a + b * rhs.b) / denom;
    double nb = (b * rhs.a - a * rhs.b) / denom;
    a = na; b = nb; return *this;
  }

  // Equality comparison with tolerance
  bool operator==(const complex &rhs) const {
    return std::fabs(a - rhs.a) < 1e-6 && std::fabs(b - rhs.b) < 1e-6;
  }

  // Explicit bool conversion
  explicit operator bool() const {
    return !(std::fabs(a) < 1e-6 && std::fabs(b) < 1e-6);
  }
};

// Output stream operator adhering to formatting rules
inline std::ostream &operator<<(std::ostream &os, const complex &x) {
  const double eps = 1e-6;
  double real_val = std::fabs(x.real()) < eps ? 0.0 : x.real();
  double imag_val = std::fabs(x.imag()) < eps ? 0.0 : x.imag();
  if (real_val < 0) { os << '-'; real_val = -real_val; }
  os << std::fixed << std::setprecision(6) << real_val;
  if (imag_val < 0) { os << '-'; imag_val = -imag_val; }
  else { os << '+'; }
  os << std::fixed << std::setprecision(6) << imag_val << 'i';
  return os;
}

} // namespace sjtu

#endif // COMPLEX_HPP
