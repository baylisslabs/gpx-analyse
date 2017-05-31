
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "vector3d.h"
#include "los.h"

/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
float los_wgs84_earth_radius( const float lat )
{
  double sin_theta = sin(lat);
  double sin_squared_theta = sin_theta*sin_theta;
  double cos_squared_theta = 1.0 - sin_squared_theta;

  return 1.0 / sqrt( cos_squared_theta / LOS_WGS84_EQ_RADIUS_SQRD +
                     sin_squared_theta / LOS_WGS84_POL_RADIUS_SQRD );
}
/*----------------------------------------------------------------------------*/
double los_wgs84_geog_lat_to_geoc_lat( const double geog_lat )
{
  static const double one_minus_flattening_const_sqrd =
     (1.0 - LOS_WGS84_FLATTENING) * (1.0 - LOS_WGS84_FLATTENING);

  return atan( one_minus_flattening_const_sqrd * tan( geog_lat ) );

}
/*----------------------------------------------------------------------------*/
double los_wgs84_geoc_lat_to_geog_lat( const double geoc_lat )
{
  static const double one_over_one_minus_flattening_const_sqrd =
    1.0 / ( ( 1.0 - LOS_WGS84_FLATTENING ) * ( 1.0 - LOS_WGS84_FLATTENING ) );

  return atan( one_over_one_minus_flattening_const_sqrd * tan( geoc_lat ) );

}
/*----------------------------------------------------------------------------*/
float los_mid_point_earth_bulge(
  const float dist,
  const float k_factor,
  const float ref_lat )
{

  double earth_r = los_wgs84_earth_radius( ref_lat ) * k_factor;

  double unit_half_d = sin( 0.5 * dist / earth_r );

  return ( 1.0 - sqrt( 1.0 - unit_half_d * unit_half_d ) ) * earth_r;
}
/*----------------------------------------------------------------------------*/
float los_mid_point_earth_bulge_delta(
  const float dist,
  const float k_factor,
  const float ref_lat )
{
  return  los_mid_point_earth_bulge( dist, 1.0, ref_lat )
     - los_mid_point_earth_bulge( dist, k_factor, ref_lat );
}
/*----------------------------------------------------------------------------*/

void los_geocentric_to_lat_lon_alt(
  const vector3d point,
  float* lat,       /* OUT: point latitude ( radians ) */
  float* lon,       /* OUT: point longitude ( radians )*/
  float* alt )      /* OUT: point altitude  ( metres ) */
{
  vector3d south_pole_u;
  vector3d point_u;
  double      cos_theta;
  double      theta;

  vector3d_set(  south_pole_u, 0 , 0 , 1 );

  vector3d_copy( point_u, point );
  vector3d_normalise( point_u );

  /* the longitude is the rotation of the vector about the north_pole */
  *lon = vector3d_get_z_rot_angle( point_u );

   /* the latitude comes from the angle between the vector and the south pole */
  cos_theta = vector3d_dot_product( point_u, south_pole_u );
  if ( cos_theta > 1.0 ) cos_theta = 1.0;
  if ( cos_theta < -1.0 ) cos_theta = -1.0;

  theta = acos( cos_theta ) - 0.5*LOS_PI ;
  *lat = los_wgs84_geoc_lat_to_geog_lat(  theta );

  /* the altitude is the length of the vector minus the earth radius */
  *alt = vector3d_get_mag( point ) - los_wgs84_earth_radius( theta );
}
/*----------------------------------------------------------------------------*/
void los_lat_lon_alt_to_geocentric(
  vector3d point,
  const float lat,       /* IN: point latitude ( radians ) */
  const float lon,       /* IN: point longitude ( radians )*/
  const float alt )      /* IN: point altitude  ( metres ) */

{
  double geoc_lat = los_wgs84_geog_lat_to_geoc_lat( lat );

  vector3d_set( point, 1 , 0 , 0 );
  vector3d_rotate_y( point, geoc_lat );
  vector3d_rotate_z( point, lon );

  vector3d_scale(
     point,
     los_wgs84_earth_radius( geoc_lat ) + alt );
}
/*----------------------------------------------------------------------------*/
void los_mid_point_get(
  vector3d mid_point, /* OUT: mid point in space */
  vector3d normal,    /* OUT: unit normal vector to the line between the two points */
  vector3d boresight, /* OUT: vector difference in space between the two points */
  const vector3d first_point,
  const vector3d second_point )
{
  vector3d_copy( boresight, second_point );
  vector3d_sub( boresight, first_point );

  vector3d_cross_product( normal, boresight, first_point );
  vector3d_normalise( normal );

  vector3d_copy( mid_point, boresight );
  vector3d_scale( mid_point, 0.5 );
  vector3d_add( mid_point, first_point );
}
/*----------------------------------------------------------------------------*/
void los_get_first_fresnel_sample_point(
  vector3d fresnel_point,   /* OUT: point in first fresnel zone */
  const vector3d origin,    /* IN:  centre of fresnel zone */
  const vector3d boresight, /* IN:  vector of direct line between tx&rx */
  const vector3d normal,    /* IN:  unit normal vector */
  const float       radius,    /* IN:  radius of fresnel zone */
  const float       theta )    /* IN:  angle to rotate normal in the fresnel zone */
{
  matrix33 rotation_matrix;
  vector3d boresight_u;

  vector3d_copy( boresight_u, boresight );
  vector3d_normalise( boresight_u );

  matrix33_get_rotation_matrix(
    rotation_matrix,
    boresight_u,
    theta );

  matrix33_vector_mult(
    fresnel_point,
    rotation_matrix,
    normal );

  vector3d_scale( fresnel_point, radius );
  vector3d_add( fresnel_point, origin );
}
/*----------------------------------------------------------------------------*/
double los_horizontal_dist_get(
  const Occultation_request_metric* ocb ) /* returns distance over ground(m)
                                                  between two points */
{
  double   cos_theta;
  vector3d first_point_u;  /* unit vector for 1st point geocentric coords */
  vector3d second_point_u; /* unit vector for 2nd point geocentric coords */

  vector3d_set( first_point_u, 1.0 , 0 , 0 );
  vector3d_rotate_y( first_point_u,
     los_wgs84_geog_lat_to_geoc_lat( ocb->first_point_latitude ) );
  vector3d_rotate_z( first_point_u, ocb->first_point_longitude );

  vector3d_set( second_point_u, 1.0 , 0 , 0 );
  vector3d_rotate_y( second_point_u,
     los_wgs84_geog_lat_to_geoc_lat( ocb->second_point_latitude ) );
  vector3d_rotate_z( second_point_u, ocb->second_point_longitude );

  cos_theta = vector3d_dot_product( first_point_u, second_point_u );

  if ( cos_theta > 1.0 ) cos_theta = 1.0;
  if ( cos_theta < -1.0 ) cos_theta = -1.0;

  return acos( cos_theta ) *
    los_wgs84_earth_radius(
      ( ocb->first_point_latitude  + ocb->second_point_latitude) * 0.5 );
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
float los_get_geometric_horizon( float alt, float ref_lat, float k_factor )
/* returns: horizontal distance over ground(m) to geometric horizon */
{
  float cos_theta;
  float earth_radius = los_wgs84_earth_radius( ref_lat ) * k_factor;

  if ( alt >= 0 )
  {
    cos_theta = 1.0 / ( alt / earth_radius + 1.0 );
    return earth_radius * acos( cos_theta );
  }
  else
    return 0.0;
}
/*----------------------------------------------------------------------------*/
