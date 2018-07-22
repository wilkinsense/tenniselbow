#pragma once

// A structure to hold two floats. Similar to SDL_Point (though it contains ints).
struct Vector2
{
  float x;
  float y;

  static const Vector2 Zero;
  Vector2 operator+(const Vector2& rhs);
  Vector2 operator-(const Vector2& rhs);
  Vector2 operator*(const float& rhs);
  Vector2 operator/(const float& rhs);
};

struct Vector3
{
  float x;
  float y;
  float z;

  static const Vector3 Zero;
  Vector3 operator+(const Vector3& rhs);
  Vector3 operator-(const Vector3& rhs);
  Vector3 operator*(const float& rhs);
  Vector3 operator/(const float& rhs);
};

struct Vector4
{
  float x;
  float y;
  float z;
  float w;

  static const Vector4 Zero;
  Vector4 operator+(const Vector4& rhs);
  Vector4 operator-(const Vector4& rhs);
  Vector4 operator*(const float& rhs);
  Vector4 operator/(const float& rhs);
};

struct Transform
{
  Vector3 position;
  Vector3 rotation;
  Vector3 scale;
};

class MathUtils
{
public:
  static float ToRadians(float degrees);
  static float ToDegrees(float radians);

  template<typename Vector>
  static Vector Normalize(Vector vector) {
    return vector / Magnitude(vector);
  }

  static float Magnitude(Vector2 vector);
  static float Magnitude(Vector3 vector);
  static float Magnitude(Vector4 vector);
};