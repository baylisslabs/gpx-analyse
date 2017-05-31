#ifndef _vector_h_
#define _vector_h_

typedef double vector3d[3];

typedef double matrix33[3][3];
/*----------------------------------------------------------------------------*/

void vector3d_set( vector3d a, const double x, const double y, const double z );

/*----------------------------------------------------------------------------*/
void vector3d_copy( vector3d a, const vector3d b );

/*----------------------------------------------------------------------------*/
void matrix33_set( matrix33 a,
  const double a1, const double a2, const double a3,
  const double b1, const double b2, const double b3,
  const double c1, const double c2, const double c3
 );
/*----------------------------------------------------------------------------*/
void matrix33_get_rotation_matrix(
  matrix33 m,  /* OUT: rotation matrix of IN: theta about vector IN: v */
  const vector3d v,
  const double theta );
/*----------------------------------------------------------------------------*/
void matrix33_vector_mult(
  vector3d out,
  matrix33 m,
  const vector3d a );
/*----------------------------------------------------------------------------*/
double vector3d_dot_product( const vector3d a, const vector3d b );
/*----------------------------------------------------------------------------*/
void vector3d_cross_product(
  vector3d out,
  const vector3d a,
  const vector3d b );
/*----------------------------------------------------------------------------*/
double vector3d_get_mag( const vector3d a );

/*----------------------------------------------------------------------------*/
void vector3d_add( vector3d a, const vector3d b );

/*----------------------------------------------------------------------------*/
void vector3d_sub( vector3d a, const vector3d b );

/*----------------------------------------------------------------------------*/
void vector3d_scale( vector3d a, const double n );

/*----------------------------------------------------------------------------*/
void vector3d_normalise( vector3d a );

/*----------------------------------------------------------------------------*/
void vector3d_rotate_x( vector3d a, const double theta );

/*----------------------------------------------------------------------------*/
void vector3d_rotate_y( vector3d a, const double theta );

/*----------------------------------------------------------------------------*/
void vector3d_rotate_z( vector3d a, const double theta );

/*----------------------------------------------------------------------------*/
double vector3d_get_x_rot_angle( const vector3d a );

/*----------------------------------------------------------------------------*/
double vector3d_get_y_rot_angle( const vector3d a);

/*----------------------------------------------------------------------------*/
double vector3d_get_z_rot_angle( const vector3d a );
/*----------------------------------------------------------------------------*/
void vector3d_to_string( char* str, const vector3d a );

/*----------------------------------------------------------------------------*/

#endif
