#include "linalg.hpp"

// Vectors
//

vec3d::vec3d () { x[0] = x[1] = x[2] = 0.0; }

vec3d::vec3d (const double x, const double y, const double z)
{
    this->x[0] = x; this->x[1] = y; this->x[2] = z;
}

vec3d::vec3d (const vec3d& v)
{
    x[0] = v.x[0]; x[1] = v.x[1]; x[2] = v.x[2];
}

void vec3d::set (const unsigned int who, const double what)
{ 
    if (who >= d)
        throw std::out_of_range ("vec3d indexes from 0 to 2");
    else
        x[who] = what;
}

double vec3d::get (const unsigned int who) const
{ 
    if (who >= d)
        throw std::out_of_range ("vec3d indexes from 0 to 2");
    return x[who];
}

double vec3d::norm () { return sqrt (this->dot (*this)); }

std::vector<double> vec3d::to_vector () const
{ 
    return std::vector<double> ({ x[0], x[1], x[2] }); 
}

// Dot and cross products

double vec3d::dot (const vec3d other) const
{
    return x[0]*other.x[0] + x[1]*other.x[1] + x[2]*other.x[2];
}

vec3d vec3d::cross (const vec3d other) const
{
    return vec3d (x[1]*other.x[2] - x[2]*other.x[1],
                  x[2]*other.x[0] - x[0]*other.x[2],
                  x[0]*other.x[1] - x[1]*other.x[0]);
}

// Operators and such

double& vec3d::operator[] (const unsigned int who)
{
    if (who >= d)
        throw std::out_of_range ("vec3d indexes from 0 to 2");
    return x[who];
}

vec3d& vec3d::operator= (const vec3d& other)
{
    x[0] = other.x[0];
    x[1] = other.x[1];
    x[2] = other.x[2];
    return *this;
}

vec3d vec3d::operator+ (vec3d other)
{
    return vec3d (x[0] + other.x[0], x[1] + other.x[1], x[2] + other.x[2]);
}

vec3d vec3d::operator- (vec3d other)
{
    return vec3d (x[0] - other.x[0], x[1] - other.x[1], x[2] - other.x[2]);
}

double vec3d::operator* (vec3d other) { return this->dot (other); }

vec3d operator/ (const vec3d &self, double other)
{
    return vec3d (self.x[0]/other, self.x[1]/other, self.x[2]/other);
}

vec3d operator* (const vec3d &self, double other)
{
    return vec3d (self.x[0]*other, self.x[1]*other, self.x[2]*other);
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
    for (unsigned int i=0; i<d; ++i)
        for (unsigned int j=0; j<d; ++j)
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
    for (unsigned int i=0; i<d; ++i)
        for (unsigned int j=0; j<d; ++j)
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
    for (unsigned int i=0; i<d; ++i)
        for (unsigned int j=0; j<d; ++j)
            temp.m[i][j] = m[j][i];
    return temp;
}

sqm3d sqm3d::inv ()
{
    sqm3d temp;
    double determinant = det ();
    if (fabs (determinant) < constants::eps)
        throw std::invalid_argument ("attempted to invert singular matrix");
    for (unsigned int i=0; i<d; ++i)
        for (unsigned int j=0; j<d; ++j) 
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
    for (unsigned int i=0; i<d; ++i)
        for (unsigned int j=0; j<d; ++j)
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
    for (unsigned int i=0; i<d; ++i)
    {
        for (unsigned int j=0; j<d; ++j)
        {
            temp.m[i][j] = 0.0;
            for (unsigned int k=0; k<d; ++k)
                temp.m[i][j] += m[i][k]*other.m[k][j];
        }
    }
    return temp;
}

sqm3d operator* (double other, const sqm3d& mat)
{
    sqm3d temp;
    for (unsigned int i=0; i<sqm3d::d; ++i)
        for (unsigned int j=0; j<sqm3d::d; ++j)
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
    for (unsigned int i=0; i<sqm3d::d; ++i)
    {
        temp[i] = mat.m[i][0]*vec.x[0] + mat.m[i][1]*vec.x[1] + mat.m[i][2]*vec.x[2];
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
    for (unsigned int i=0; i<sqm3d::d; ++i)
    {
        stream << "(";
        for (unsigned int j=0; j<sqm3d::d; ++j)
        {
            stream << mat.m[i][j];
            if (j < sqm3d::d-1) stream << ",\t";
        }
        stream << ")" << std::endl;
    }
    return stream;
}

