/* TEST_GEOM


This is a testing case based on the NAIF documentation
    



A. Lucas, 2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SpiceUsr.h"


int main () 
{
system ( "clear" );   

#define  FILESZ  256
#define  WORDSZ   41
#define  ROOM     10
#define  LENOUT   32
    
SpiceBoolean   found;
SpiceBoolean eof;
    
SpiceChar    line[LENOUT];
SpiceChar iframe[WORDSZ];
SpiceChar instnm[WORDSZ];
SpiceChar satnm [WORDSZ];
SpiceChar fixref[WORDSZ];
SpiceChar scnm  [WORDSZ];
SpiceChar setupf[FILESZ];
SpiceChar shape [WORDSZ];
SpiceChar time  [WORDSZ];
    
SpiceDouble    alt;
SpiceDouble    bundry[ROOM][3];
SpiceDouble    emissn;
SpiceDouble    et;
SpiceDouble    f;
SpiceDouble    insite[3];
SpiceDouble    srfvec[3];
SpiceDouble    pclat;
SpiceDouble    pclon;
SpiceDouble    pdlat;
SpiceDouble    pdlon;
SpiceDouble    phase;
SpiceDouble    point [3];
SpiceDouble    r;
SpiceDouble    radii [3];
SpiceDouble    re;
SpiceDouble    rp;
SpiceDouble    solar;
SpiceDouble    trgepc;
    
SpiceInt       i;
SpiceInt       instid;
SpiceInt       n;

 ConstSpiceChar  * versn;   
    
 versn = tkvrsn_c ( "TOOLKIT" ); 
 printf("Boresight Ray detection test  (Toolkit version: %s)\n",versn);       

    
/*  Parsing settings file    */
        eof = SPICEFALSE;

         rdtext_c ( "settings", FILESZ, setupf, &eof );
         furnsh_c ( setupf );    
       
         rdtext_c ( "settings", WORDSZ, satnm, &eof );
         rdtext_c ( "settings", WORDSZ, fixref, &eof );
         rdtext_c ( "settings", WORDSZ, scnm, &eof );
         rdtext_c ( "settings", WORDSZ, instnm, &eof );
         rdtext_c ( "settings", WORDSZ, time, &eof );

  
str2et_c ( time, &et );

bodvrd_c ( satnm, "RADII", 3, &i, radii );
   
/* Get the instrument boresight and frame name. */
   bodn2c_c ( instnm, &instid, &found );
   if ( !found )
   {
        setmsg_c ( "Instrument name # could not be "
                           "translated to an ID code.");
        errch_c  ( "#",  instnm                          );
        sigerr_c ( "NAMENOTFOUND"         );
   }
   getfov_c ( instid, ROOM,    WORDSZ,  WORDSZ,
                     shape,  iframe,  insite,  &n, bundry );
                     
                     
 /* Compute the boresight ray intersection with the surface of the satellite. */
 sincpt_c ( "Ellipsoid", satnm, et, fixref, "CN+S", scnm,
           iframe, insite, point, &trgepc, srfvec, &found  );
           
/* If an intercept is found, compute planetocentric and planetodetic  latitude and longitude of the point. */
if ( found ) {           
 reclat_c ( point, &r, &pclon, &pclat );
/* Let re, rp, and f be the satellite's longer equatorial radius, polar radius, and flattening factor. */
re = radii[0];
rp = radii[2];
f = (re-rp) / re;
recgeo_c ( point, re, f, &pdlon, &pdlat, &alt );
    
/* Compute illumination angles at the surface point. */
   ilumin_c ( "Ellipsoid", satnm, et, fixref, "CN+S", scnm, point,
                    &trgepc, srfvec,  &phase,  &solar,  &emissn );
 /* Display results.  Convert angles to degrees for output. */
printf ( "\n=============================================================\n"
         "Intercept planetocentric longitude       (deg):   %11.6f\n"
         "Intercept planetocentric latitude        (deg):   %11.6f\n"
         "Intercept planetodetic longitude         (deg):   %11.6f\n"
         "Intercept planetodetic latitude          (deg):   %11.6f\n"
         "Range from spacecraft to intercept point  (km):   %11.6f\n"
         "Intercept phase angle                    (deg):   %11.6f\n"
         "Intercept solar incidence angle          (deg):   %11.6f\n"
         "Intercept emission angle                 (deg):   %11.6f\n",
           dpr_c() * pclon,
           dpr_c() * pclat,
           dpr_c() * pdlon,
           dpr_c() * pdlat,
           vnorm_c( srfvec ),
           dpr_c() * phase,
           dpr_c() * solar,
           dpr_c() * emissn
       );
    
 
    
    double ang_sumckc;
        
    ang_sumckc=  39.843719-dpr_c() * pclon + 4.195878-dpr_c() * pclat + 39.843719- dpr_c() * pdlon + 5.048011-dpr_c() * pdlat + 28.139479-dpr_c() * phase + 18.247220-dpr_c() * solar + 17.858309-dpr_c() * emissn;
//    printf("\n %11.11f\n",ang_sumckc);
    
   if ( fabs(ang_sumckc) > 0.00001 ) {  
      printf ( "\n\n\n");
      printf ( " !!! Computing check !!!  \n"
         "Intercept planetocentric longitude       (deg):   %11.6f\n"
         "Intercept planetocentric latitude        (deg):   %11.6f\n"
         "Intercept planetodetic longitude         (deg):   %11.6f\n"
         "Intercept planetodetic latitude          (deg):   %11.6f\n"
         "Range from spacecraft to intercept point  (km):   %11.6f\n"
         "Intercept phase angle                    (deg):   %11.6f\n"
         "Intercept solar incidence angle          (deg):   %11.6f\n"
         "Intercept emission angle                 (deg):   %11.6f\n",
           39.843719-dpr_c() * pclon,
           4.195878-dpr_c() * pclat,
           39.843719- dpr_c() * pdlon,
           5.048011-dpr_c() * pdlat,
           2089.169724-vnorm_c( srfvec ),
           28.139479-dpr_c() * phase,
           18.247220-dpr_c() * solar,
            17.858309-dpr_c() * emissn
    );  
       
     printf ( "\n !!!! Computing errors too large, check you files (%11.11f)\n\n",ang_sumckc );
    }
    else {
          printf( "\n ==> Check: passed! \n\n" );  
    }
}
else {
printf ( "No intercept point found at %s\n", time );
        }
return(0); 
}
