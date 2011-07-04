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
	printf("Mesh error: %d %s\n", errCode, gluErrorString(errCode));
}


void CALLBACK hdglVertexTess(void * data, hdMesh * polygon)
{
	GLdouble *ptr;
	ptr = (GLdouble *) data;
	printf("Adding mesh vertex: %3.5f, %3.5f, %3.5f\n", ptr[0], ptr[1], ptr[2]);//vertex->x, vertex->y, vertex->z);
    polygon->AddPointInternal(ptr[0], ptr[1], ptr[2]);
}


void CALLBACK hdglCombineTess(GLdouble coords[3], GLdouble * vertex_data[4], GLfloat weight[4], GLdouble ** outData, hdMesh * mesh)
{
	printf("AAA: Combining! %3.5f, %3.5f, %3.5f\n", coords[0], coords[1], coords[2]);
    //const float * vertex = static_cast<const float *>(coords);
	//hdVec3 *vertex = (hdVec3 *)malloc(sizeof(hdVec3));
	//vertex->Set(coords[0], coords[1], coords[2]);
    //*outData = (float *)mesh->Combine(vertex->x, vertex->y, vertex->z);
	
	
	GLdouble *vertex;
	
	vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
	//
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	
	//mesh->Combine(coords[0], coords[1], coords[2]);
	
	//for (int i = 3; i < 6; i++)
	//{
	//	vertex[i] = weight[0] * vertex_data[0][i] +
	//		weight[1] * vertex_data[1][i] +
	//		weight[2] * vertex_data[2][i] +
	//		weight[3] * vertex_data[3][i];
	//}
	*outData = vertex; //(float *)mesh->Combine(vertex[0], vertex[1], vertex[2]);
	
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
	
    //gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	//gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GLU_TRUE);
	
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    gluTessNormal(tobj, 0.0f, 0.0f, 1.0f);
    gluTessBeginPolygon(tobj, this);
	gluTessBeginContour(tobj);
	
	
	for (unsigned i = 0; i < this->GetVertexCount(); ++i)
	{
		hdVec3 vertex = this->GetVertices()[i];
		printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
		//m_gluVectors[i] = (GLdouble *)calloc(1, sizeof(gluVector_t));
		m_gluVectors[i][0] = vertex.x;
		m_gluVectors[i][1] = vertex.y;
		m_gluVectors[i][2] = vertex.z;
		
		gluTessVertex(tobj, (GLdouble *)m_gluVectors[i], (GLvoid *)m_gluVectors[i]);
	}
	
	gluTessEndContour(tobj);
	/*
	for(size_t c = 0; c < ContourCount(); ++c)
	{
		switch(outsetType)
		{
			case 1 : contourList[c]->buildFrontOutset(outsetSize); break;
			case 2 : contourList[c]->buildBackOutset(outsetSize); break;
		}
		const FTContour* contour = contourList[c];
		
		
		gluTessBeginContour(tobj);
		for(size_t p = 0; p < contour->PointCount(); ++p)
		{
			const FTGL_DOUBLE* d;
			switch(outsetType)
			{
				case 1: d = contour->FrontPoint(p); break;
				case 2: d = contour->BackPoint(p); break;
				case 0: default: d = contour->Point(p); break;
			}
			// XXX: gluTessVertex doesn't modify the data but does not
			// specify "const" in its prototype, so we cannot cast to
			// a const type.
			gluTessVertex(tobj, (GLdouble *)d, (GLvoid *)d);
		}
		
		gluTessEndContour(tobj);
	}
	 */
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
		//glBegin(GL_POLYGON);
		
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
	//printf("BBB: Combining! %3.5f, %3.5f, %3.5f\n", x, y, z);
    //tempPointList.push_back(FTPoint(x, y, z));
	//m_tempVertices->Add(hdVec3(x,y,z));
	//m_tempVertex[0] = x; //.Set(x, y, z);
	//m_tempVertex[1] = y;
	//m_tempVertex[2] = z;
   // return m_tempVertex;
	//m_currentMeshPolygon->AddPoint(x, y, z);
}


void hdMesh::Begin(GLenum meshType)
{
	printf("Begin mesh\n");
    m_currentMeshPolygon = new hdMeshPolygon();
	//m_currentMeshPolygon->Init(this->GetWorld());
	m_currentMeshPolygon->SetPolygonType(meshType);
}


void hdMesh::End()
{
	m_currentMeshPolygon->ResetAABB();
    m_polygons->Add(m_currentMeshPolygon);
}
