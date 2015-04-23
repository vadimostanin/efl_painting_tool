/*
 * AngleBetweenTwo.cpp
 *
 *  Created on: 19 апр. 2015
 *      Author: vadim
 */

#include "IsAngleBetweenTwoPredicate.h"
#include <cmath>
#include <algorithm>
#include <cassert>
using namespace std;

IsAngleBetweenTwoPredicate::IsAngleBetweenTwoPredicate( int angleFrom, int angleTo, bool searchInShortPath, int testAngle ) :
	m_AngleFrom( angleFrom ), m_AngleTo( angleTo ), m_SearchInShortPath( searchInShortPath ), m_TestAngle( testAngle ),
	m_MinAngle( 0 ), m_MaxAngle( 0 ), m_Result( false )
{
	bool leftResult = calcLeftSide();
	bool rightResult = calcRightSide();

	m_Result = leftResult || rightResult;
}

IsAngleBetweenTwoPredicate::~IsAngleBetweenTwoPredicate()
{
}

int IsAngleBetweenTwoPredicate::getMinAngle() const
{
	return m_MinAngle;
}

int IsAngleBetweenTwoPredicate::getMaxAngle() const
{
	return m_MaxAngle;
}

bool IsAngleBetweenTwoPredicate::calcLeftSide()
{
	int minAngle = min( m_AngleFrom, m_AngleTo );
	int maxAngle = max( m_AngleFrom, m_AngleTo );

	bool between = m_TestAngle >= minAngle && m_TestAngle <= maxAngle ;

	bool result = false;

	// left side calculating
	int angleFrom = m_AngleFrom;
	int angleTo = m_AngleTo;
	{
		if( abs( angleFrom - angleTo ) > 180 )//long
		{
			if( true == between )
			{
				if( true == m_SearchInShortPath )
				{
					result = false;
				}
				else
				{
					result = true;
				}

				if( angleFrom < m_TestAngle )
				{
					m_MinAngle = m_AngleFrom;
					m_MaxAngle = m_TestAngle;
				}
				else
				{
					m_MinAngle = m_TestAngle;
					m_MaxAngle = m_AngleFrom;
				}
			}
		}
		else//short
		{
			if( true == between )
			{
				if( true == m_SearchInShortPath )
				{
					result = true;
				}
				else
				{
					result = false;
				}

				if( angleFrom < m_TestAngle )
				{
					m_MinAngle = m_AngleFrom;
					m_MaxAngle = m_TestAngle;
				}
				else
				{
					m_MinAngle = m_TestAngle;
					m_MaxAngle = m_AngleFrom;
				}
			}
		}
	}

	return result;
}

bool IsAngleBetweenTwoPredicate::calcRightSide()
{
	int tempMinAngle = min( m_AngleFrom, m_AngleTo );
	int tempMaxAngle = max( m_AngleFrom, m_AngleTo ) - 360;

	int minAngle = min( tempMinAngle, tempMaxAngle );
	int maxAngle = max( tempMinAngle, tempMaxAngle );

	int tempTestAngle = m_TestAngle;
	if( tempTestAngle > max( m_AngleFrom, m_AngleTo ) )
//	if( tempTestAngle >= 180 )
	{
		tempTestAngle -= 360;
	}

	bool between = tempTestAngle > minAngle && tempTestAngle < maxAngle ;

	bool result = false;

	{
		if( abs( minAngle - maxAngle ) > 180 )//long
		{
			if( true == between )
			{
				if( true == m_SearchInShortPath )
				{
					result = false;
				}
				else
				{
					result = true;
				}

				if( m_AngleFrom < m_AngleTo )
				{
					int tempMin = min( tempTestAngle, m_AngleFrom );
					int tempMax = max( tempTestAngle, m_AngleFrom );

					m_MinAngle = tempMin;
					m_MaxAngle = tempMax;
				}
				else
				{
					int tempMin = min( tempTestAngle, tempMaxAngle );
					int tempMax = max( tempTestAngle, tempMaxAngle );

					m_MinAngle = tempMin;
					m_MaxAngle = tempMax;
				}
			}
		}
		else//short
		{
			if( true == between )
			{
				if( true == m_SearchInShortPath )
				{
					result = true;
				}
				else
				{
					result = false;
				}

				if( m_AngleFrom < m_AngleTo )
				{
					int tempMin = min( tempTestAngle, m_AngleFrom );
					int tempMax = max( tempTestAngle, m_AngleFrom );

					m_MinAngle = tempMin;
					m_MaxAngle = tempMax;
				}
				else
				{
					int tempMin = min( tempTestAngle, ( m_AngleFrom - 360 ) );
					int tempMax = max( tempTestAngle, ( m_AngleFrom - 360 ) );

					m_MinAngle = tempMin;
					m_MaxAngle = tempMax;
				}
			}
		}
	}

	return result;
}

int IsAngleBetweenTwoPredicate::getIsBigArea() const
{
	return false;
}

bool IsAngleBetweenTwoPredicate::operator ()()
{
	return m_Result;
}











