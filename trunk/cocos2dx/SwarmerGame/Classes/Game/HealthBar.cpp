#include "HealthBar.h"

using namespace cocos2d;

HealthBar::HealthBar(int maxVal, int value)
: m_size(32, 3)
, m_maxVal(maxVal)
, m_value(value)
{
}

HealthBar::~HealthBar(void)
{
}

void HealthBar::draw(void)
{
    CCPoint verts[4];

    verts[0].x = 0.0; verts[0].y = 0.0;
    verts[1].x = 0.0; verts[1].y = m_size.height;
    verts[2].x = m_size.width * float(m_value)/float(m_maxVal); verts[2].y = m_size.height;
    verts[3].x = m_size.width * float(m_value)/float(m_maxVal); verts[3].y = 0.0;

    glColor4f(0.0f, 0.5f, 0.0f, 0.1f);
    ccFillPoly( verts, 4, true );
}

void HealthBar::ccFillPoly( const CCPoint *vertices, int numOfVertices, bool closePolygon )
{
	ccVertex2F* newPoint = new ccVertex2F[numOfVertices];
	
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY, GL_COLOR_ARRAY	
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	
	// iPhone and 32-bit machines
	if( sizeof(CCPoint) == sizeof(ccVertex2F) ) {
		
		// convert to pixels ?
		if( CC_CONTENT_SCALE_FACTOR() != 1 ) {
			memcpy( newPoint, vertices, numOfVertices * sizeof(ccVertex2F) );
			for( int i = 0; i < numOfVertices; i++)
            {
				newPoint[i].x = vertices[i].x * CC_CONTENT_SCALE_FACTOR();
                newPoint[i].y = vertices[i].y * CC_CONTENT_SCALE_FACTOR();
            }
			
			glVertexPointer(2, GL_FLOAT, 0, newPoint);
			
		} else
			glVertexPointer(2, GL_FLOAT, 0, vertices);
		
		
	}
	
	if( closePolygon )
		glDrawArrays(GL_TRIANGLE_FAN, 0, numOfVertices);
	else
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numOfVertices);
	
	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

   delete [] newPoint;
}