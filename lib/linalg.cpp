#include <cmath>
#include <cassert>
#include "linalg.hpp"

// Vectors
//

vec3d::vec3d () { x[0] = x[1] = x[2] = 0.0; }

vec3d::vec3d (double x, double y, double z)
{
    this->x[0] = x; this->x[1] = y; this->x[2] = z;
}

vec3d::vec3d (const vec3d& v)
{
    for (int i=0; i<d; ++i)
        x[i] = v.x[i];
}

void vec3d::set (int who, double what) { x[who] = what; }

double vec3d::get (int who) { return x[who]; }

double vec3d::norm () { return sqrt (this->dot (*this)); }

int vec3d::getDim () { return d; }

// Dot and cross products

double vec3d::dot (vec3d other)
{
    double temp = 0.0;
    for (int i=0; i<d; ++i)
        temp += x[i]*other.x[i];
    return temp;
}

vec3d vec3d::cross (vec3d other)
{
    return vec3d (x[1]*other.x[2] - x[2]*other.x[1],
                  x[2]*other.x[0] - x[0]*other.x[2],
                  x[0]*other.x[1] - x[1]*other.x[0]);
}

// Operators and such

double& vec3d::operator[] (const int i) 
{
    return x[i]; 
}

vec3d& vec3d::operator= (const vec3d& other)
{
    for (int i=0; i<d; ++i)
        x[i] = other.x[i];
    return *this;
}

vec3d vec3d::operator+ (vec3d other)
{
    vec3d temp;
    for (int i=0; i<d; ++i)
        temp.x[i] = x[i] + other.x[i];
    return temp;
}

vec3d vec3d::operator- (vec3d other)
{
    vec3d temp;
    for (int i=0; i<d; ++i)
        temp.x[i] = x[i] - other.x[i];
    return temp;
}

double vec3d::operator* (vec3d other) { return this->dot (other); }

vec3d operator/ (const vec3d &self, double other)
{
    vec3d temp;
    for (int i=0; i<d; ++i)
        temp.x[i] = self.x[i]/other;
    return temp;
}

vec3d operator* (const vec3d &self, double other)
{
    vec3d temp;
    for (int i=0; i<d; ++i)
        temp.x[i] = other*self.x[i];
    return temp;
}

vec3d operator* (double other, const vec3d &self)
{
    return self*other;
}

std::ostream& operator<< (std::ostream& stream, const vec3d& vec)
{
    return stream << "(" << vec.x[0] << ", " << vec.x[1] << ", " << vec.x[2] << ")";
}

// End vectors
//
// Matrices
//

sqm3d::sqm3d ()
{
    double m[d][d];
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j)
            m[i][j] = 0.0;
    set (m);
}

sqm3d::sqm3d (double m[][d])
{
    set (m);
}

sqm3d::sqm3d (const sqm3d& other)
{
    set (other.m);
}

void sqm3d::set (const double m[][d])
{
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j)
            this->m[i][j] = m[i][j];
} 

void sqm3d::set (int i, int j, double val)
{
    m[i][j] = val;
}

double sqm3d::det ()
{
    return 
        m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] +
        m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] -
        m[0][1]*m[1][0]*m[2][2] - m[0][2]*m[1][1]*m[2][0];
}

sqm3d sqm3d::transpose ()
{
    sqm3d temp;
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j)
            temp.m[i][j] = m[j][i];
    return temp;
}

sqm3d sqm3d::inv ()
{
    sqm3d temp;
    double determinant = det ();
    assert (fabs (determinant) > eps);
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j) 
            temp.m[j][i] = (m[(i+1)%d][(j+1)%d]*m[(i+2)%d][(j+2)%d]
                - m[(i+1)%d][(j+2)%d]*m[(i+2)%d][(j+1)%d])/determinant;
    return temp;    
}

// Operators and such

sqm3d& sqm3d::operator= (const sqm3d& other)
{
    set (other.m);
    return *this;
}

sqm3d sqm3d::operator+ (sqm3d other)
{
    sqm3d temp;
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j)
            temp.m[i][j] = this->m[i][j] + other.m[i][j];
    return temp;
}

sqm3d sqm3d::operator- (sqm3d other)
{
    return (*this) + (-1.0*other);
}

sqm3d sqm3d::operator* (sqm3d other)
{
    sqm3d temp;
    for (int i=0; i<d; ++i)
    {
        for (int j=0; j<d; ++j)
        {
            temp.m[i][j] = 0.0;
            for (int k=0; k<d; ++k)
                temp.m[i][j] += m[i][k]*other.m[k][j];
        }
    }
    return temp;
}

sqm3d operator* (double other, const sqm3d& mat)
{
    sqm3d temp;
    for (int i=0; i<d; ++i)
        for (int j=0; j<d; ++j)
            temp.m[i][j] = other*mat.m[i][j];
    return temp;
}

sqm3d operator* (const sqm3d& mat, double other)
{
    return other*mat;
}

sqm3d operator/ (const sqm3d& mat, double other)
{
    return mat*(1/other);
}

sqm3d operator/ (double other, const sqm3d& mat)
{
    return mat*(1/other);
}

vec3d operator* (const sqm3d& mat, const vec3d& vec)
{
    vec3d temp;
    for (int i=0; i<d; ++i)
    {
        temp.x[i] = 0;
        for (int j=0; j<d; ++j)
            temp.x[i] += mat.m[i][j]*vec.x[j];
    }
    return temp;
}

vec3d operator* (const vec3d& vec, const sqm3d& mat)
{
    sqm3d temp = mat; // so we can use non-const transpose function
    return (temp.transpose ())*vec;
}

std::ostream& operator<< (std::ostream& stream, const sqm3d& mat)
{
    stream << std::endl;
    for (int i=0; i<d; ++i)
    {
        stream << "(";
        for (int j=0; j<d; ++j)
        {
            stream << mat.m[i][j];
            if (j < d-1) stream << ",\t";
        }
        stream << ")" << std::endl;
    }
    return stream;
}

