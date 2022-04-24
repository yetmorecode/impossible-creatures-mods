/////////////////////////////////////////////////////////////////////
// File    : MathTypes.h
// Desc    : 
// Created : Tuesday, June 11, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#ifndef PI
#define PI (3.1415926535897932384626433832795f)
#endif

#ifndef TWOPI
#define TWOPI (2.0f * PI)
#endif


#define	DEG2RAD(a)	((a) * (PI / 180.0f))
#define RAD2DEG(a)	((a) * 180.0f / PI)
#define RAD(a)		DEG2RAD(a)
#define DEG(a)		RAD2DEG(a)

#define DEG_PER_RADIAN (360.0f/(2.0f*PI))
#define RADIAN_PER_DEG (2.0f*PI/360.0f)
