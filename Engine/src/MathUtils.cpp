#include "MathUtils.h"
#include <math.h>
#include "MathConstants.h"

const Vector2 Vector2::Zero = { 0.0f, 0.0f };
const Vector3 Vector3::Zero = { 0.0f, 0.0f, 0.0f };
const Vector4 Vector4::Zero = { 0.0f, 0.0f, 0.0f, 0.0f };

Vector2 Vector2::operator+(const Vector2& rhs) { return { x + rhs.x, y + rhs.y }; }
Vector2 Vector2::operator-(const Vector2& rhs) { return { x - rhs.x, y - rhs.y }; }
Vector2 Vector2::operator*(const float& rhs) { return { x * rhs, y * rhs }; }
Vector2 Vector2::operator/(const float& rhs) { return { x / rhs, y / rhs }; }

Vector3 Vector3::operator+(const Vector3& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
Vector3 Vector3::operator-(const Vector3& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
Vector3 Vector3::operator*(const float& rhs) { return { x * rhs, y * rhs, z * rhs }; }
Vector3 Vector3::operator/(const float& rhs) { return { x / rhs, y / rhs, z / rhs }; }

Vector4 Vector4::operator+(const Vector4& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
Vector4 Vector4::operator-(const Vector4& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
Vector4 Vector4::operator*(const float& rhs) { return { x * rhs, y * rhs, z * rhs, w * rhs }; }
Vector4 Vector4::operator/(const float& rhs) { return { x / rhs, y / rhs, z / rhs, w / rhs }; }

float MathUtils::ToDegrees(float radians)
{
  float radiansToDegrees = 180.0f / MATH_PI;
  return radians * radiansToDegrees;
}

float MathUtils::ToRadians(float degrees)
{
  float degreesToRadians = MATH_PI / 180.0f;
  return degrees * degreesToRadians;
}

float MathUtils::Magnitude(Vector2 vector)
{
  return sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f));
}

float MathUtils::Magnitude(Vector3 vector)
{
  return sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f) + powf(vector.z, 2.0f));
}

float MathUtils::Magnitude(Vector4 vector)
{
  return sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f) + powf(vector.z, 2.0f) + powf(vector.w, 2.0f));
}