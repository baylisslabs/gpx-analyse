#ifndef _ios_degrade_h_
#define _ios_degrade_h_

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "vector3d.h"

#define LOS_PI             (3.1415926535897932384626433832795)
#define LOS_DEG_TO_RAD     ( 1.0 / 180.0 * LOS_PI )
#define LOS_FEET_TO_METRES ( 12.0 * 25.4 / 1000.0 )

#define LOS_WGS84_EQ_RADIUS  6378137.0
#define LOS_WGS84_POL_RADIUS 6356752.3

#define LOS_WGS84_EQ_RADIUS_SQRD  \
   ( LOS_WGS84_EQ_RADIUS * LOS_WGS84_EQ_RADIUS )
#define LOS_WGS84_POL_RADIUS_SQRD \
   ( LOS_WGS84_POL_RADIUS * LOS_WGS84_POL_RADIUS)

#define LOS_WGS84_FLATTENING  \
   (( LOS_WGS84_EQ_RADIUS - LOS_WGS84_POL_RADIUS ) \
      / LOS_WGS84_EQ_RADIUS)

#define LOS_DEFAULT_K_FACTOR    ( 4.0 / 3.0)
#define LOS_K_FACTOR_CORRECTION ( 3.5 )

/*----------------------------------------------------------------------------*/
typedef struct {
	double 			first_point_latitude;
	double			first_point_longitude;
	double			first_point_altitude;
	double			second_point_latitude;
	double			second_point_longitude;
	double			second_point_altitude;
} Occultation_request_metric;



/*----------------------------------------------------------------------------*/
float _los_wgs84_earth_radius( const float lat );

double _los_wgs84_geog_lat_to_geoc_lat( const double geog_lat );

double _los_wgs84_geoc_lat_to_geog_lat( const double geoc_lat );

float _los_mid_point_earth_bulge(
  const float dist,
  const float k_factor,
  const float ref_lat );

float _los_mid_point_earth_bulge_delta(
  const float dist,
  const float k_factor,
  const float ref_lat );

void _los_geocentric_to_lat_lon_alt(
  const vector3d point,
  float* lat,       /* OUT: point latitude ( radians ) */
  float* lon,       /* OUT: point longitude ( radians )*/
  float* alt );      /* OUT: point altitude  ( metres ) */

void _los_lat_lon_alt_to_geocentric(
  vector3d point,
  const float lat,       /* IN: point latitude ( radians ) */
  const float lon,       /* IN: point longitude ( radians )*/
  const float alt );      /* IN: point altitude  ( metres ) */

void _los_mid_point_get(
  vector3d mid_point, /* OUT: mid point in space */
  vector3d normal,    /* OUT: unit normal vector to the line between the two points */
  vector3d boresight, /* OUT: vector difference in space between the two points */
  const vector3d first_point,
  const vector3d second_point );

void _los_get_first_fresnel_sample_point(
  vector3d fresnel_point,   /* OUT: point in first fresnel zone */
  const vector3d origin,    /* IN:  centre of fresnel zone */
  const vector3d boresight, /* IN:  vector of direct line between tx&rx */
  const vector3d normal,    /* IN:  unit normal vector */
  const float       radius,    /* IN:  radius of fresnel zone */
  const float       theta );    /* IN:  angle to rotate normal in the fresnel zone */

 /* returns distance over ground(m) betweenn two points */
double _los_horizontal_dist_get(const Occultation_request_metric* ocb );

/* returns: horizontal distance over ground(m) to geometric horizon */
float _los_get_geometric_horizon( float alt, float ref_lat, float k_factor );



#endif /* header */
