
#include "los.h"

#include <time.h>
#include <expat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct trkpt
{
  double lat;
  double lon;
  time_t timestamp;
} trkpt_t;

static trkpt_t current_trkpt;
static char current_elem[256] = { 0 };
static char current_cdata[256] = { 0 };

static trkpt_t last_trkpt = { 0 };
static double total_gcd = 0;
static double total_t = 0;

extern char* strptime(const char* str, const char* fmt, struct tm* tmptr);

void next_trk_pt(const trkpt_t* trkpt)
{
  if(last_trkpt.timestamp!=0)
  {
    Occultation_request_metric orm;
    orm.first_point_latitude = last_trkpt.lat * LOS_DEG_TO_RAD;
    orm.first_point_longitude = last_trkpt.lon * LOS_DEG_TO_RAD;
    orm.first_point_altitude = 0;
    orm.second_point_latitude = trkpt->lat * LOS_DEG_TO_RAD;
    orm.second_point_longitude = trkpt->lon * LOS_DEG_TO_RAD;
    orm.second_point_altitude = 0;

    double delta_t = trkpt->timestamp - last_trkpt.timestamp;
    double gcd = los_horizontal_dist_get(&orm);
    double sog = gcd / delta_t;
    double sog_knots = sog*3600.0/1852.0;
    total_gcd += gcd/1852.0;
    total_t += delta_t;

    printf("trkpt time=%ld, lat=%.15lf, lon=%.15lf, dt=%f, total_t=%f, gcd=%f, total_gcd=%f, sog=%f\n",
      trkpt->timestamp,trkpt->lat,trkpt->lon,delta_t,total_t,gcd,total_gcd,sog_knots);
  }
  else
  {
    printf("trkpt time=%ld, lat=%.15lf, lon=%.15lf\n",
      trkpt->timestamp,trkpt->lat,trkpt->lon);
  }
  last_trkpt = *trkpt;
}

void start(void *userData, const XML_Char *name, const XML_Char **attrs)
{
  snprintf(current_elem,256,"%s",name);
  memset(current_cdata,0,256);

  if(strcmp(name,"trkpt")==0)
  {
    while(*attrs)
    {
      if(strcmp(*attrs,"lat")==0)
      {
        current_trkpt.lat = strtod(attrs[1],0);
      }
      else if(strcmp(*attrs,"lon")==0)
      {
        current_trkpt.lon = strtod(attrs[1],0);
      }
      attrs++; attrs++;
    }
  }
  else if(strcmp(name,"time")==0)
  {

  }
}

void end(void *userData, const XML_Char *name)
{
  if(strcmp(name,"trkpt")==0)
  {
    next_trk_pt(&current_trkpt);
  }
  else if(strcmp(current_elem,"time")==0)
  {
    /* todo UTC */
    struct tm tp;
    strptime(&current_cdata[0],"%FT%H:%M:%S%Z",&tp);
    current_trkpt.timestamp = mktime(&tp);
  }
  memset(current_cdata,0,256);
  memset(current_elem,0,256);
}

void cdata(void *userData, const XML_Char *s, int len)
{
  if(len && len+strlen(current_cdata)<sizeof(current_cdata))
  {
    strncat(current_cdata,s,len);
  }
}

int main(int argc, char** argv)
{
  XML_Parser xp = XML_ParserCreate(0);
  XML_SetElementHandler(xp,start,end);
  XML_SetCharacterDataHandler(xp,cdata);

  char buf[256];
  int fd = open(argv[1], O_RDONLY);
  int len = 0;
  while((len = read(fd,buf,256)) > 0)
  {
    if(!XML_Parse(xp,buf,len,0))
    {
      printf("Error parsing xml");
      break;
    }
  }
  XML_Parse(xp,buf,0,1);

  return 0;
}
