/*
 *  hdMesh.cpp
 *  LevelEditor
 *
 *  Created by david on 3/07/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdMesh.h"

#ifndef CALLBACK
#define CALLBACK
#endif

#if defined __APPLE_CC__ && __APPLE_CC__ < 5465
typedef GLvoid (*GLUTesselatorFunction) (...);
#else
typedef GLvoid (*GLUTesselatorFunction) ();
#endif


void CALLBACK hdglErrorTess(GLenum errCode, hdMesh * mesh)
{
    //mesh->Error(errCode);
	//printf("Mesh error: %d %s\n", errCode, gluErrorString(errCode));
}


void CALLBACK hdglVertexTess(void * data, hdMesh * polygon)
{
	GLdouble *ptr;
	ptr = (GLdouble *) data;
	printf("Adding mesh vertex: %3.5f, %3.5f, %3.5f\n", ptr[0], ptr[1], ptr[2]);
    polygon->AddPointInternal(ptr[0], ptr[1], ptr[2]);
}


void CALLBACK hdglCombineTess(GLdouble coords[3], GLdouble * vertex_data[4], GLfloat weight[4], GLdouble ** outData, hdMesh * mesh)
{
	printf("AAA: Combining! %3.5f, %3.5f, %3.5f\n", coords[0], coords[1], coords[2]);
    
	GLdouble *vertex;
	
	vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
    vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	
	*outData = vertex;
}


void CALLBACK hdglBeginTess(GLenum type, hdMesh * mesh)
{
    mesh->Begin(type);
}


void CALLBACK hdglEndTess(hdMesh * mesh)
{
    mesh->End();
}


hdMesh::hdMesh(hdGameWorld* gameWorld) : hdPolygon(gameWorld)
{
	m_polygons = new hdTypedefList<hdMeshPolygon *, HD_MESH_POLYGON_COUNT>();
	m_tempVertices = new hdTypedefList<hdVec3, MESH_TEMP_VERTEX_COUNT>;
	m_currentMeshPolygon = NULL;
	m_gluVectors = NULL;
	m_userType = HD_MESH_USER_TYPE;
}


hdMesh::hdMesh() : hdPolygon(NULL)
{
	m_polygons = new hdTypedefList<hdMeshPolygon *, HD_MESH_POLYGON_COUNT>();
	m_tempVertices = new hdTypedefList<hdVec3, MESH_TEMP_VERTEX_COUNT>;
	m_currentMeshPolygon = NULL;
	m_gluVectors = NULL;
	m_userType = HD_MESH_USER_TYPE;
}


hdMesh::~hdMesh()
{
	delete m_polygons;
	delete m_tempVertices;
	free(m_gluVectors);
}


void hdMesh::Init(hdGameWorld *gameWorld)
{
	((hdPolygon *)this)->Init(gameWorld);
}


void hdMesh::Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis)
{
}


void hdMesh::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
}


void hdMesh::MoveTo(const hdVec3& center)
{
}


const bool hdMesh::AddPolygonMesh(hdMeshPolygon *polygonMesh)
{
	if (!m_polygons) return false;
	
	return m_polygons->Add(polygonMesh);
}


void hdMesh::RemovePolygonMeshAtIndex(const uint32 index)
{
	m_polygons->RemoveAtIndex(index);
}



hdPolygon * hdMesh::GenerateTesselation()
{
#ifdef LEVEL_EDITOR
	if (m_gluVectors)
	{
		free(m_gluVectors);
	}
	m_gluVectors = (gluVector_t *)calloc(1, sizeof(gluVector_t) * this->GetVertexCount());
	
	GLUtesselator * tobj = gluNewTess();
	
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)&hdglBeginTess);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)&hdglVertexTess);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)&hdglCombineTess);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)&hdglEndTess);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)&hdglErrorTess);
	
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    gluTessNormal(tobj, 0.0f, 0.0f, 1.0f);
    gluTessBeginPolygon(tobj, this);
	gluTessBeginContour(tobj);
	
	for (unsigned i = 0; i < this->GetVertexCount(); ++i)
	{
		hdVec3 vertex = this->GetVertices()[i];
		printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
		m_gluVectors[i][0] = vertex.x;
		m_gluVectors[i][1] = vertex.y;
		m_gluVectors[i][2] = vertex.z;
		
		gluTessVertex(tobj, (GLdouble *)m_gluVectors[i], (GLvoid *)m_gluVectors[i]);
	}
	
	gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
	
	gluDeleteTess(tobj);
	
	this->ResetAABB();
	
#endif
	return NULL;
}


void hdMesh::AddPointInternal(const float x, const float y, const float z)
{
    m_currentMeshPolygon->AddPoint(x, y, z);
}


void hdMesh::DebugDraw()
{
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (unsigned i = 0; i < this->GetVertexCount(); ++i)
	{
		glVertex3f(this->GetVertices()[i].x,
				   this->GetVertices()[i].y,
				   this->GetVertices()[i].z);
	}
	glEnd();
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		glColor4f(0.0f, 0.0f, hdClamp((float)i/(float)m_polygons->GetItemCount(), 0.0f, 1.0f), 1.0f);
        glBegin(poly->GetPolygonType());
		
        for (unsigned j = 0; j < poly->GetVertexCount(); ++j)
		{
			glVertex3f(poly->GetVertices()[j].x,
					   poly->GetVertices()[j].y,
					   poly->GetVertices()[j].z);
		}
		
		glEnd();
	}
}


const float * hdMesh::Combine(const float x, const float y, const float z)
{
    return NULL;
}


void hdMesh::Begin(GLenum meshType)
{
	printf("Begin mesh\n");
    m_currentMeshPolygon = new hdMeshPolygon();
	m_currentMeshPolygon->SetPolygonType(meshType);
}


void hdMesh::End()
{
	m_currentMeshPolygon->ResetAABB();
    m_polygons->Add(m_currentMeshPolygon);
}
