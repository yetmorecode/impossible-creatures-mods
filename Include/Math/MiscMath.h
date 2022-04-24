// MiscMath .h
// 
// Miscellanious math routines.
//

#pragma once

// Forward Declarations
class Vec3f;


// Functions
float Volume( const Vec3f& a, const Vec3f& b, const Vec3f& c );

void FindBoundingBox( const Vec3f *verts, unsigned nVerts, Vec3f& min, Vec3f& max );

float CRSpline(float x, int nknots, float *knot);
float KBSpline(float x, int nknots, float *knot, float *tension, float *continuity, float *bias);
