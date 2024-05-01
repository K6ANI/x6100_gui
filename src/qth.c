/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Xiegu X6100 LVGL GUI
 *
 *  Copyright (c) 2022-2023 Belousov Oleg aka R1CBU
 */

#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "qth.h"
#include "params.h"

static double qth_lon = 0.0;
static double qth_lat = 0.0;

void qth_set(const char *qth) {
    params_str_set(&params.qth, qth);
    qth_update(qth);
}

void qth_update(const char *qth) {
    grid_pos(qth, &qth_lat, &qth_lon);
    
    qth_lat = qth_lat * M_PI / 180.0;
    qth_lon = qth_lon * M_PI / 180.0;
}

bool grid_check(const char *grid) {

    /* Jerry Shaw K6ANI
    Corrected the code to check for all 4 fields. Changed the range
    of each alphabetic field to correspond with the current Grid Square
    specification ranges. According to IARU rules of 2019, all alphabetic 
    fields are upper case (see "Maidenhead Locator System" description
    in Wikipedia.org, and the IARU VHF Handbook).
    */

    uint8_t len = strlen(grid);

    if (!((len == 8) || (len == 6) || (len == 4) || (len == 2)))
        return false;

    if (len == 8)
    {
        if (grid[7] < '0' || grid[7] > '9') return false;
        if (grid[6] < '0' || grid[6] > '9') return false;
    }

    if (len >= 6)
    {
        if (toupper(grid[5]) < 'A' || toupper(grid[5]) > 'X') return false;
        if (toupper(grid[4]) < 'A' || toupper(grid[4]) > 'X') return false;
    }

    if (len >= 4)
    {
        if (grid[3] < '0' || grid[3] > '9') return false;
        if (grid[2] < '0' || grid[2] > '9') return false;
    }

    if (toupper(grid[1]) < 'A' || toupper(grid[1]) > 'R') return false;
    if (toupper(grid[0]) < 'A' || toupper(grid[0]) > 'R') return false;
    
    return true;
}

const char *pos_grid(double lat, double lon) {
    static char buf[9];

    int t1;

    if (180.001 < fabs(lon) ||
        90.001 < fabs(lat)) {
        return "    n/a ";
    }

    if (179.99999 < lon) {
        lon = 179.99999;
    }

    /* Jerry Shaw K6ANI
    Added overrange checks to t1. Changed sructure
    to be consistent with new checks. Changed syntax to
    be consistent with char/int/double types.
    */

    lon += 180.0;

    t1 = (int)(lon / 20.0);

    if (t1 < 0) {
        t1 = 0;
    } else if (t1 > ('R' - 'A')) {
            t1 = ('R' - 'A');
    }

    buf[0] = (char)t1 + 'A';

    lon -= (double)t1 * 20.0;

    t1 = (int)(lon / 2.0);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > 9) {
        t1 = 9;
    }
    
    buf[2] = (char)t1 + '0';

    lon -= (double)t1 * 2.0;

    lon *= 60.0;

    /* Jerry Shaw K6ANI
    buf[4] is now in the range 'A' to 'X'.
    */

    t1 = (int)(lon / 5.0);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > ('X' - 'A')) {
        t1 = ('X' - 'A');
    }

    buf[4] = (char)t1 + 'A';

    lon -= (double)t1 * 5.0;

    lon *= 60.0;

    t1 = (int)(lon / 30.0);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > 9) {
        t1 = 9;
    }

    buf[6] = (char)t1 + '0';

    if (89.99999 < lat) {
        lat = 89.99999;
    }

    lat += 90.0;

    t1 = (int)(lat / 10.0);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > ('R' - 'A')) {
        t1 = ('R' - 'A');
    }

    buf[1] = (char)t1 + 'A';

    lat -= (double)t1 * 10.0;

    t1 = (int)lat;

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > 9) {
        t1 = 9;
    }

    buf[3] =(char)t1 + '0';

    lat -= (double)t1;

    lat *= 60.0;

    /* Jerry Shaw K6ANI
    buf[5] is now in the range 'A' to 'X'.
    */

    t1 = (int)(lat / 2.5);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > ('X' - 'A')) {
        t1 = ('X' - 'A');
    }

    buf[5] = (char)t1 + 'A';

    lat -= (double)t1 * 2.5;

    lat *= 60.0;

    t1 = (int)(lat / 15.0);

    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > 9) {
        t1 = 9;
    }

    buf[7] = (char)t1 + '0';

    buf[8] = '\0';
    
    return buf;
}

void grid_pos(const char *grid, double *lat, double *lon) {
    uint8_t n = strlen(grid);

    *lon = -180.0;
    *lat = -90.0;
    
    /* Jerry Shaw K6ANI
    Added check for valid grid. Return lat/lon 0
    if not valid.
    */

    if (!grid_check(grid)) {
        *lat = 0.0;
        *lon = 0.0
        return;
    }

    *lon += (toupper(grid[0]) - 'A') * 20.0;
    *lat += (toupper(grid[1]) - 'A') * 10.0;
    
    if (n >= 4) {
        *lon += (grid[2] - '0') * 2.0;
        *lat += (grid[3] - '0') * 1.0;
    }
    
    if (n >= 6) {
        *lon += (toupper(grid[4]) - 'A') * 5.0 / 60.0;
        *lat += (toupper(grid[5]) - 'A') * 2.5 / 60.0;
    }

    if (n >= 8) {
        *lon += (grid[6] - '0') * 5.0 / 600.0;
        *lat += (grid[7] - '0') * 2.5 / 600.0;
    }
    
    switch (n) {
        case 2:
            *lon += 20.0 / 2.0;
            *lat += 10.0 / 2.0;
            break;

        case 4:
            *lon += 2.0 / 2.0;
            *lat += 1.0 / 2.0;
            break;
            
        case 6:
            *lon += 5.0 / 60.0 / 2.0;
            *lat += 2.5 / 60.0 / 2.0;
            break;
            
        case 8:
            *lon += 5.0 / 600.0 / 2.0;
            *lat += 2.5 / 600.0 / 2.0;
    }
}

int32_t grid_dist(const char *grid) {
    double lat = 0;
    double lon = 0;

    /* Jerry Shaw K6ANI
    Added check for valid grid. Return distance 0
    if not valid.
    */

    if (!grid_check(grid)) {
        return 0;
    }

    grid_pos(grid, &lat, &lon);
    
    lat = lat * M_PI / 180.0;
    lon = lon * M_PI / 180.0;
    
    double dlat = lat - qth_lat;
    double dlon = lon - qth_lon;
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos(lat) * cos(qth_lat) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    
    return c * 6371.0;
}
