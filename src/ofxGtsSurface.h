#pragma once
#include <gts.h>
#include <vector>
#include "ofMain.h"

using namespace std;

class ofxGtsSurface {
	
public: 
	
	ofxGtsSurface();
	ofxGtsSurface(GtsSurface* passIn);
	~ofxGtsSurface();
	GtsVertex* createVertex(float x, float y, float z);
	GtsEdge* createEdge(GtsVertex* v1, GtsVertex* v2);
	GtsFace* createFace(GtsEdge* e1, GtsEdge* e2, GtsEdge* e3);

	vector<GtsVertex*> getFaceVertices(GtsFace* face);
	void createSphere(guint level);
	vector<GtsVertex*>& getVertices();
	vector<GtsEdge*>& getEdges();
	vector<GtsTriangle*>& getTriangles();
	
	void addVertices(vector<ofVec3f> verts);
	bool intersects(ofxGtsSurface* surfaceToCheck);
	void merge(ofxGtsSurface* surfaceToMerge);
	
	void drawBounding();
	
	void cleanupTriangles();
	void cleanupVertices();
	
	float getCenterOfMass();
	float getVolume();
	float getArea();
	
	pair<ofVec3f, ofVec3f> getBoundingBox();
	bool checkIsPointInside(ofVec3f point);
	bool areOverlapping( ofxGtsSurface* other);
	
	void bbFromPair(GtsBBox* box, pair<ofVec3f, ofVec3f>* p);
	
	GtsSurface* getSurface() { return s; }
	
	
private:
	
	GtsSurface* s;
	ofNode drawNode;
	
	vector<GtsTriangle*> triangles;
	vector<GtsVertex*> vertices;
	vector<GtsEdge*> edges;
	vector<GtsFace*> faces;
};