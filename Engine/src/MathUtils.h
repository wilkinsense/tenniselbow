#pragma once

// A structure to hold two floats. Similar to SDL_Point (though it contains ints).
struct Vector2
{
  float x;
  float y;
};

struct Vector3
{
  float x;
  float y;
  float z;
};

struct Vector4
{
  float x;
  float y;
  float z;
  float w;
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

  static Vector2 Normalize(Vector2 vector);
  static Vector3 Normalize(Vector3 vector);
  static Vector4 Normalize(Vector4 vector);
};