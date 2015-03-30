#include "linalg.hpp"

// Vectors
//

vec3d::vec3d () { x = y = z = 0.0; }

vec3d::vec3d (double x, double y, double z)
{
    this->x = x; this->y = y; this->z = z;
}

vec3d::vec3d (const vec3d& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void vec3d::set (int who, double what) 
{ 
    if (who == 0)
        x = what;
    else if (who == 1)
        y = what;
    else if (who == 2)
        z = what;
    else
        throw std::out_of_range ("vec3d indexes from 0 to 2");
}

double vec3d::get (int who) 
{ 
    if (who == 0)
        return x;
    else if (who == 1)
        return y;
    else if (who == 2)
        return z;
    else
        throw std::out_of_range ("vec3d indexes from 0 to 2");
}

double vec3d::norm () { return sqrt (this->dot (*this)); }

// Dot and cross products

double vec3d::dot (vec3d other)
{
    return x*other.x + y*other.y + z*other.z;
}

vec3d vec3d::cross (vec3d other)
{
    return vec3d (y*other.z - z*other.y,
                  z*other.x - x*other.z,
                  x*other.y - y*other.x);
}

// Operators and such

double& vec3d::operator[] (const int who) 
{
    if (who == 0)
        return x;
    else if (who == 1)
        return y;
    else if (who == 2)
        return z;
    else
        throw std::out_of_range ("vec3d indexes from 0 to 2");
}

vec3d& vec3d::operator= (const vec3d& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

vec3d vec3d::operator+ (vec3d other)
{
    return vec3d (x + other.x, y + other.y, z + other.z);
}

vec3d vec3d::operator- (vec3d other)
{
    return vec3d (x - other.x, y - other.y, z - other.z);
}

double vec3d::operator* (vec3d other) { return this->dot (other); }

vec3d operator/ (const vec3d &self, double other)
{
    return vec3d (self.x/other, self.y/other, self.z/other);
}

vec3d operator* (const vec3d &self, double other)
{
    return vec3d (self.x*other, self.y*other, self.z*other);
}

vec3d operator* (double other, const vec3d &self)
{
    return self*other;
}

std::ostream& operator<< (std::ostream& stream, const vec3d& vec)
{
    return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
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
        temp[i] = mat.m[i][0]*vec.x + mat.m[i][1]*vec.y + mat.m[i][2]*vec.z;
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

