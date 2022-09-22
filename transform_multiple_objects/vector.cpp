#include <iostream>
#include <math.h>
#include <assert.h>
using namespace std;
typedef float f;


class Vector{
  public:
    float x, y, z;
    Vector(float x1=0.0, float y1=0.0, float z1=0.0){
      x=x1;
      y=y1;
      z=z1;
    }

    float* addition(float *points, int size){
      for (int i = 0; i < size; i+=3) {
        points[i] += x;
        points[i+1] += y;
        points[i+2] += z; 
      }
      return points;
    }

    float* substraction(float *points, int size){
      for (int i = 0; i < size; i+=3) {
        points[i] -= x;
        points[i+1] -= y;
        points[i+2] -= z; 
      }
      return points;
    }

    float* multiplication(float *points, int size){
      for (int i = 0; i < size; i+=3) {
        points[i] *= x;
        points[i+1] *= y;
        points[i+2] *= z; 
      }
      return points;
    }

    float* division(float *points, int size){
      for (int i = 0; i < size; i+=3) {
        if(x!=0.0){
          points[i] /= x;
        }
        if(y!=0.0){
          points[i+1] /= y;
        }
        if(z!=0.0){
          points[i+2] /= z; 
        }
      }
      return points;
    }

    // float* translation(float *points, int size){
    //  for (int i = 0; i < size; i+=3) {
    //     points[i] = points[i+2] * z - z * points[i+1];
    //     points[i+1] = y;
    //     points[i+2] = z; 
    //   }
    //   return points;

    // }

    Vector update(float x1 = 0.0, float y1=0.0, float z1=0.0){
      x=x1;
      y=y1;
      z=z1;
      return *this;
    }
};

/*

class vector3d
{
public:
  f x, y, z;
  vector3d()
  {
    x = 0;
    y = 0;
    z = 0;
  }

  vector3d(f x1, f y1, f z1 = 0)
  {
    x = x1;
    y = y1;
    z = z1;
  }
  vector3d(const vector3d &vec);             // copy constructor
  vector3d operator+(const vector3d &vec);   // addition
  vector3d &operator+=(const vector3d &vec);
  vector3d operator-(const vector3d &vec);   // substraction
  vector3d &operator-=(const vector3d &vec); // assigning new result to the vector
  vector3d operator*(f value);               // multiplication
  vector3d &operator*=(f value);             // assigning new result to the vector.
  vector3d operator/(f value);               // division
  vector3d &operator/=(f value);             // assigning new result to the vector
  vector3d &operator=(const vector3d &vec);
  f dot_product(const vector3d &vec);          // scalar dot_product
  vector3d cross_product(const vector3d &vec); // cross_product
  f magnitude();                               // magnitude of the vector
  vector3d normalization();                    // nor,malized vector
  f square();                                  // gives square of the vector

  f distance(const vector3d &vec); // gives distance between two vectors
  f show_X();                      // return x
  f show_Y();                      // return y
  f show_Z();                      // return z
  void disp();                     // display value of vectors
};

vector3d::vector3d(const vector3d &vec)
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
}

// addition

vector3d vector3d ::operator+(const vector3d &vec)
{
  return vector3d(x + vec.x, y + vec.y, z + vec.z);
}

vector3d &vector3d ::operator+=(const vector3d &vec)
{
  x += vec.x;
  y += vec.y;
  z += vec.z;
  return *this;
}
// substraction//
vector3d vector3d ::operator-(const vector3d &vec)
{
  return vector3d(x - vec.x, y - vec.y, z - vec.z);
}

vector3d &vector3d::operator-=(const vector3d &vec)
{
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  return *this;
}

// scalar multiplication

vector3d vector3d ::operator*(f value)
{
  return vector3d(x * value, y * value, z * value);
}

vector3d &vector3d::operator*=(f value)
{
  x *= value;
  y *= value;
  z *= value;
  return *this;
}

// scalar division
vector3d vector3d ::operator/(f value)
{
  assert(value != 0);
  return vector3d(x / value, y / value, z / value);
}

vector3d &vector3d ::operator/=(f value)
{
  assert(value != 0);
  x /= value;
  y /= value;
  z /= value;
  return *this;
}

vector3d &vector3d::operator=(const vector3d &vec)
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
  return *this;
}

// Dot product
f vector3d::dot_product(const vector3d &vec)
{
  return x * vec.x + vec.y * y + vec.z * z;
}

// cross product
vector3d vector3d ::cross_product(const vector3d &vec)
{
  f ni = y * vec.z - z * vec.y;
  f nj = z * vec.x - x * vec.z;
  f nk = x * vec.y - y * vec.x;
  return vector3d(ni, nj, nk);
}

f vector3d::magnitude()
{
  return sqrt(square());
}

f vector3d::square()
{
  return x * x + y * y + z * z;
}

vector3d vector3d::normalization()
{
  assert(magnitude() != 0);
  *this /= magnitude();
  return *this;
}

f vector3d::distance(const vector3d &vec)
{
  vector3d dist = *this - vec;
  return dist.magnitude();
}

f vector3d::show_X()
{
  return x;
}

f vector3d::show_Y()
{
  return y;
}

f vector3d::show_Z()
{
  return z;
}

void vector3d::disp()
{
  cout << x << " " << y << " " << z << endl;
}*/