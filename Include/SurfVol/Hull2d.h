/////////////////////////////////////////////////////////////////////
// File    : Hull2d.h
// Desc    : 
// Created : Monday, October 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Math/Vec2.h>

///////////////////////////////////////////////////////////////////// 
// Hull2d

class Hull2d
{
// construction
public:
	Hull2d(std::vector<Vec2f> &);

// interface
public:
	const std::vector<Vec2f> &GetPoints() const { return m_points; }

	float DistSqr(const Vec2f &pt) const;
	int IntersectSegment(const Vec2f &A, const Vec2f &B) const;


	// Temporary segment access. Clean this up later.
	size_t GetNumSegments() const { return m_points.size(); }
	bool GetSegment(size_t Segment, const Vec2f * &A, const Vec2f * &B) const;

	bool PtInside(const Vec2f &pt) const;

// fields
private:
	std::vector<Vec2f> m_points;

// copy -- do not define
private:
	Hull2d(const Hull2d &);
	Hull2d& operator= (const Hull2d &);
};
