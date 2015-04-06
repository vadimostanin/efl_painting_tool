/*
 * GeometryPointDynamic.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: vostanin
 */

#include "GeometryPointDynamic.h"
#include <cstdio>
#include <iostream>
using namespace std;

GeometryPointDynamic::GeometryPointDynamic( cpSpace * space ) : m_Space( space ), m_Radius( 8 ), m_Mass( 1 )
{
	initPoint();
}

GeometryPointDynamic::GeometryPointDynamic( cpSpace * space, GeometryPoint * geometryPoint ) : m_Space( space ), m_Radius( 8 ), m_Mass( 1 )
{
	setX( geometryPoint->getX() );
	setY( geometryPoint->getY() );
	setId( geometryPoint->getId() );

	initPoint();
}

GeometryPointDynamic::~GeometryPointDynamic()
{
	cpSpaceRemoveShape( m_Space, m_BallShape );
	cpShapeFree( m_BallShape );
	cpSpaceRemoveBody( m_Space, m_BallBody );
	cpBodyFree( m_BallBody );
}

const IGeometryObject & GeometryPointDynamic::getGeometryObject() const
{
	return *this;
}

GeometryPointDynamic * lpThis = 0;

void GeometryPointDynamic::update()
{
	cpVect pos = cpBodyGetPosition( m_BallBody );
//	cpVect vel = cpBodyGetVelocity( m_BallBody );

	setX( pos.x );
	setY( pos.y );

//	cout << "GeometryPointDynamic::update x=" << lpThis->getX() << "; y=" << lpThis->getY() << endl << flush;
}

void GeometryPointDynamic::onBodyPositionChangeFunc(cpBody *body, cpFloat dt)
{
	cpBodyUpdatePosition( body, dt );

	cpVect pos = cpBodyGetPosition( body );

	lpThis->setX( pos.x );
	lpThis->setY( pos.y );

//	cout << "GeometryPointDynamic::onBodyPositionChangeFunc x=" << lpThis->getX() << "; y=" << lpThis->getY() << endl << flush;
}

void GeometryPointDynamic::initPoint()
{
	lpThis = this;
	// The moment of inertia is like mass for rotation
	// Use the cpMomentFor*() functions to help you approximate it.
	cpFloat moment = cpMomentForCircle( m_Mass, 0, m_Radius, cpvzero );

	// The cpSpaceAdd*() functions return the thing that you are adding.
	// It's convenient to create and add an object in one line.
	m_BallBody = cpSpaceAddBody( m_Space, cpBodyNew( m_Mass, moment ) );
	cpBodySetPosition( m_BallBody, cpv( getX(), getY() ) );

	// Now we create the collision shape for the ball.
	// You can create multiple collision shapes that point to the same body.
	// They will all be attached to the body and move around to follow it.
	m_BallShape = cpSpaceAddShape( m_Space, cpCircleShapeNew( m_BallBody, m_Radius, cpvzero ) );
	cpShapeSetFriction( m_BallShape, 0.0 );

	cpShapeSetElasticity( m_BallShape, 0.3 );


//	cpBodySetPositionUpdateFunc( m_BallBody, onBodyPositionChangeFunc );
}

cpFloat GeometryPointDynamic::getRadius() const
{
	return m_Radius;
}

cpFloat GeometryPointDynamic::getMass() const
{
	return m_Mass;
}

cpBody * GeometryPointDynamic::getBody() const
{
	return m_BallBody;
}

cpShape * GeometryPointDynamic::getShape() const
{
	return m_BallShape;
}

cpSpace * GeometryPointDynamic::getSpace() const
{
	return m_Space;
}
















