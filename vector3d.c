/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#include "vector3d.h"

#include <math.h>
#include <stdio.h>

/*----------------------------------------------------------------------------*/

void vector3d_set( vector3d a, const double x, const double y, const double z )
{
  a[0] = x;
  a[1] = y;
  a[2] = z;
}
/*----------------------------------------------------------------------------*/
double vector3d_get_x( const vector3d a ) { return a[0]; }
double vector3d_get_y( const vector3d a ) { return a[1]; }
double vector3d_get_z( const vector3d a ) { return a[2]; }
/*----------------------------------------------------------------------------*/
void vector3d_copy( vector3d a, const vector3d b )
{
  a[0] = b[0];
  a[1] = b[1];
  a[2] = b[2];
}
/*----------------------------------------------------------------------------*/
void matrix33_set( matrix33 a,
  const double a1, const double a2, const double a3,
  const double b1, const double b2, const double b3,
  const double c1, const double c2, const double c3
 )
{
  a[0][0] = a1;  a[0][1] = a2;  a[0][2] = a3;
  a[1][0] = b1;  a[1][1] = b2;  a[1][2] = b3;
  a[2][0] = c1;  a[2][1] = c2;  a[2][2] = c3;
}
/*-----------------------------------------------------------------------------*/
void matrix33_get_rotation_matrix(
  matrix33 m,  /* OUT: rotation matrix of IN: theta about vector IN: v */
  const vector3d v,
  const double theta )
{
  double s = sin(theta);                /* s */
  double c = cos(theta);                /* c */
  double u = 1.0 - c;                   /* u */

  matrix33_set( m,
    v[0]*v[0]*u + c,      v[1]*v[0]*u - v[2]*s, v[2]*v[0]*u + v[1]*s,
    v[0]*v[1]*s + v[2]*s, v[1]*v[1]*u + c,      v[2]*v[1]*u - v[0]*s,
    v[0]*v[2]*u - v[1]*s, v[1]*v[2]*u + v[0]*s, v[2]*v[2]*u + c       );

}
/*----------------------------------------------------------------------------*/
void matrix33_vector_mult(
  vector3d out,
  matrix33 m,
  const vector3d a )
{
  out[0] = m[0][0] * a[0] + m[0][1] * a[1] + m[0][2] * a[2];
  out[1] = m[1][0] * a[0] + m[1][1] * a[1] + m[1][2] * a[2];
  out[2] = m[2][0] * a[0] + m[2][1] * a[1] + m[2][2] * a[2];
}
/*----------------------------------------------------------------------------*/
double vector3d_dot_product( const vector3d a, const vector3d b )
{
  return
    a[0] * b[0] +
    a[1] * b[1] +
    a[2] * b[2];
}
/*----------------------------------------------------------------------------*/
void vector3d_cross_product(
  vector3d out,
  const vector3d a,
  const vector3d b )
{
  out[0] = a[1]*b[2] - a[2]*b[1];
  out[1] = a[2]*b[0] - a[0]*b[2];
  out[2] = a[0]*b[1] - a[1]*b[0];
}
/*----------------------------------------------------------------------------*/
double vector3d_get_mag( const vector3d a )
{
  return sqrt(vector3d_dot_product( a, a ));
}

/*----------------------------------------------------------------------------*/
void vector3d_add( vector3d a, const vector3d b )
{
  a[0] += b[0];
  a[1] += b[1];
  a[2] += b[2];
}
/*----------------------------------------------------------------------------*/
void vector3d_sub( vector3d a, const vector3d b )
{
  a[0] -= b[0];
  a[1] -= b[1];
  a[2] -= b[2];
}
/*----------------------------------------------------------------------------*/
void vector3d_scale( vector3d a, const double n )
{
  a[0] *= n;
  a[1] *= n;
  a[2] *= n;
}
/*----------------------------------------------------------------------------*/
void vector3d_scale_3(
   vector3d a,
   const double x,
   const double y,
   const double z )
{
  a[0] *= x;
  a[1] *= y;
  a[2] *= z;
}
/*----------------------------------------------------------------------------*/
void vector3d_normalise( vector3d a )
{
  double mag = vector3d_get_mag( a );

  if ( mag > 0 )
    vector3d_scale( a, 1.0 / mag );
}
/*----------------------------------------------------------------------------*/
void vector3d_rotate_x( vector3d a, const double theta )
{
  matrix33 m;
  vector3d temp;
  double sin_theta = sin(theta);
  double cos_theta = cos(theta);

  vector3d_copy( temp, a );

  matrix33_set( m,
    1, 0, 0,
    0, cos_theta, -sin_theta,
    0, sin_theta, cos_theta );

  matrix33_vector_mult( a, m, temp );

}
/*----------------------------------------------------------------------------*/
void vector3d_rotate_y( vector3d a, const double theta )
{
  matrix33 m;
  vector3d temp;
  double sin_theta = sin(theta);
  double cos_theta = cos(theta);

  vector3d_copy( temp, a );

  matrix33_set( m,
    cos_theta, 0, sin_theta,
    0, 1, 0,
    -sin_theta, 0, cos_theta );

  matrix33_vector_mult( a, m, temp );
}
/*----------------------------------------------------------------------------*/
void vector3d_rotate_z( vector3d a, const double theta )
{
  matrix33 m;
  vector3d temp;
  double sin_theta = sin(theta);
  double cos_theta = cos(theta);

  vector3d_copy( temp, a );

  matrix33_set( m,
    cos_theta, -sin_theta, 0,
    sin_theta, cos_theta, 0,
    0 , 0, 1 );

  matrix33_vector_mult( a, m, temp );
}
/*----------------------------------------------------------------------------*/
double vector3d_get_x_rot_angle( const vector3d a )
{
  return atan2( a[2], a[1] );
}
/*----------------------------------------------------------------------------*/
double vector3d_get_y_rot_angle( const vector3d a)
{
  return atan2( a[0], a[2] );
}
/*----------------------------------------------------------------------------*/
double vector3d_get_z_rot_angle( const vector3d a )
{
  return atan2( a[1], a[0] );
}

/*----------------------------------------------------------------------------*/
void vector3d_to_string( char* str, const vector3d a )
{
  sprintf( str, "v=(%f,%f,%f) |v|=%f",
    a[0], a[1], a[2], vector3d_get_mag( a ) );
}
/*----------------------------------------------------------------------------*/
