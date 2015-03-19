/*
 * DrawingContent.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: vostanin
 */

#include "Window.h"
#include "DrawingContent.h"
#include "GeometryObjectsManager.h"
#include <Elementary.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <GL/gl.h>
using namespace std;

#define MAIN_EDJ "./main.edj"

#define SEARCHING_POINT_RADIUS 20


DrawingContent::DrawingContent( Evas_Object *mainWindowObject, Evas_Object *mainLayout ) : m_MainLayout( mainLayout ), m_MainWindowObject( mainWindowObject )
{
	createDrawingLayout();
	createDrawingCanvas();
}

DrawingContent::~DrawingContent()
{
}

void DrawingContent::createDrawingLayout()
{
	m_DrawingLayout = elm_layout_add( m_MainLayout );
	elm_layout_file_set( m_DrawingLayout, MAIN_EDJ, "group.drawing_layout");
	evas_object_size_hint_weight_set( m_DrawingLayout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set( m_MainLayout, "part.main.drawing", m_DrawingLayout );
}

void DrawingContent::on_init_gles( Evas_Object * glview )
{
   int w = 0, h = 0;

   Evas_GL_API * __evas_gl_glapi = elm_glview_gl_api_get( glview );

   __evas_gl_glapi->glClearColor( 0.0, 0.0, 0.0, 1.0 );

//   __evas_gl_glapi->glShadeModel(GL_SMOOTH);

   __evas_gl_glapi->glEnable( GL_CULL_FACE );
   __evas_gl_glapi->glCullFace( GL_BACK );
//
   __evas_gl_glapi->glEnable( GL_DEPTH_TEST );
   __evas_gl_glapi->glDepthFunc( GL_LESS );
//
   elm_glview_size_get( glview, &w, &h );

   __evas_gl_glapi->glViewport( 0, 0, w, h );

   __evas_gl_glapi->glEnable( GL_DEPTH_TEST );
}

// resize callback gets called every time object is resized
void DrawingContent::on_resize_gl( Evas_Object * glview )
{
	DrawingContent * lpThis = ( DrawingContent * )evas_object_data_get( glview, "DrawingContent");

	Evas_GL_API * __evas_gl_glapi = lpThis->m_glApi;

	int w, h;

	elm_glview_size_get( glview, &w, &h );

	// GL Viewport stuff. you can avoid doing this if viewport is all the
	// same as last frame if you want
	__evas_gl_glapi->glViewport( 0, 0, w, h );
}

// draw callback is where all the main GL rendering happens
void DrawingContent::on_draw_gl( Evas_Object * glview )
{
	DrawingContent * lpThis = ( DrawingContent * )evas_object_data_get( glview, "DrawingContent");

	Evas_GL_API * __evas_gl_glapi = lpThis->m_glApi;

	int w, h;

	elm_glview_size_get( glview, &w, &h);

	__evas_gl_glapi->glViewport(0, 0, w, h);
	__evas_gl_glapi->glClearColor( 1.0, 0.8, 0.3, 1 );
	__evas_gl_glapi->glClear(GL_COLOR_BUFFER_BIT);

	// Draw a Triangle
	__evas_gl_glapi->glEnable(GL_BLEND);

	Evas * canvas = evas_object_evas_get( glview );

	vector<IGraphicObject *>::iterator begin = lpThis->m_GraphicObjects.begin();
	vector<IGraphicObject *>::iterator end = lpThis->m_GraphicObjects.end();
	vector<IGraphicObject *>::iterator iter = begin;
	for( ; iter != end ; iter++ )
	{
		(*iter)->draw( canvas );
	}

	// Optional - Flush the GL pipeline
	__evas_gl_glapi->glFinish();
}

void DrawingContent::createDrawingCanvas()
{
	Evas_Object * glview = elm_glview_version_add( m_DrawingLayout, EVAS_GL_GLES_2_X );

	elm_object_part_content_set( m_DrawingLayout, "drawing_canvas", glview );

	evas_object_size_hint_align_set( glview, EVAS_HINT_FILL, EVAS_HINT_FILL );
	evas_object_size_hint_weight_set( glview, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND );

	elm_glview_mode_set( glview, (Elm_GLView_Mode)( ELM_GLVIEW_ALPHA/* | ELM_GLVIEW_DEPTH *//*cause problem when rendering, leave empty peaces*/ ) );

	elm_glview_resize_policy_set( glview, ELM_GLVIEW_RESIZE_POLICY_RECREATE );
	elm_glview_render_policy_set( glview, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND );

	elm_glview_init_func_set( glview, on_init_gles );
	elm_glview_resize_func_set( glview, on_resize_gl );

	elm_glview_render_func_set( glview, on_draw_gl );

	elm_object_focus_set( glview, EINA_TRUE);

	evas_object_show( glview );

	m_DrawingCanvas = glview;
	m_glApi = elm_glview_gl_api_get( m_DrawingCanvas );
	evas_object_data_set( glview, "DrawingContent", this );
}

Evas_Object * DrawingContent::getDrawingCanvas()
{
	return m_DrawingCanvas;
}

void DrawingContent::update()
{
	drawObjects();
}

void DrawingContent::setGraphicObjects( vector<IGraphicObject *> & graphicObjects )
{
	clearObjects();

	m_GraphicObjects = graphicObjects;

	size_t count = m_GraphicObjects.size();

	update();
}

void DrawingContent::drawObjects()
{
	Evas * canvas = evas_object_evas_get( m_DrawingLayout );

	vector<IGraphicObject *>::iterator begin = m_GraphicObjects.begin();
	vector<IGraphicObject *>::iterator end = m_GraphicObjects.end();
	vector<IGraphicObject *>::iterator iter = begin;
	for( ; iter != end ; iter++ )
	{
		(*iter)->draw( canvas );
	}
}

void DrawingContent::clearObjects()
{
	vector<IGraphicObject *>::iterator begin = m_GraphicObjects.begin();
	vector<IGraphicObject *>::iterator end = m_GraphicObjects.end();
	vector<IGraphicObject *>::iterator iter = begin;
	for( ; iter != end ; iter++ )
	{
		delete (*iter);
	}
	m_GraphicPoints.clear();
}











