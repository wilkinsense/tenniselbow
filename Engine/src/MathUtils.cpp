#include "MathUtils.h"
#include <math.h>
#include "MathConstants.h"

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

Vector2 MathUtils::Normalize(Vector2 vector)
{
  float mag = sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f));
  return { vector.x / mag, vector.y / mag };
}

Vector3 MathUtils::Normalize(Vector3 vector)
{
  float mag = sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f) + powf(vector.z, 2.0f));
  return { vector.x / mag, vector.y / mag, vector.z / mag };
}

Vector4 MathUtils::Normalize(Vector4 vector)
{
  float mag = sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f) + powf(vector.z, 2.0f) + powf(vector.w, 2.0f));
  return { vector.x / mag, vector.y / mag, vector.z / mag, vector.w / mag };
}