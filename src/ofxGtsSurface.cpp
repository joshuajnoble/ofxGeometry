#include "ofxGtsSurface.h"

void build_list (gpointer data, GSList ** list)
{
	*list = g_slist_prepend (*list, data);
}

static void build_vertex_vector(gpointer data, vector<GtsVertex*>* vertices) {
	vertices->push_back((GtsVertex*)data);
}

static void build_edge_vector(gpointer data, vector<GtsEdge*>* edges) {
	edges->push_back((GtsEdge*)data);
}

static void build_triangle_vector(gpointer data, vector<GtsTriangle*>* triangles) {
	triangles->push_back((GtsTriangle*)data);
}

ofxGtsSurface::ofxGtsSurface() {
	s = gts_surface_new(
			 gts_surface_class()
			,gts_face_class()
			,gts_edge_class()
			,gts_vertex_class()
	);
}

ofxGtsSurface::~ofxGtsSurface() {
	{
		vector<GtsVertex*>::iterator it = vertices.begin();
		while(it != vertices.end()) {
			delete *it;
			++it;
		}
	}
	{
		vector<GtsEdge*>::iterator it = edges.begin();
		while(it != edges.end()) {
			delete *it;
			++it;
		}
	}
	{
		vector<GtsFace*>::iterator it = faces.begin();
		while(it != faces.end()) {
			delete *it;
			++it;
		}
	}
}

// level:  http://mathworld.wolfram.com/GeodesicDome.html
void ofxGtsSurface::createSphere(guint level) {
	gts_surface_generate_sphere(s, level);
}

GtsVertex* ofxGtsSurface::createVertex(float x, float y, float z) {
	GtsVertex* v = gts_vertex_new(s->vertex_class, x, y, z);
	vertices.push_back(v);
	return v;
}

GtsEdge* ofxGtsSurface::createEdge(GtsVertex* v1, GtsVertex* v2) {
	GtsEdge* edge = gts_edge_new(s->edge_class, v1, v2);
	edges.push_back(edge);
	return edge;
}

GtsFace* ofxGtsSurface::createFace(GtsEdge* e1, GtsEdge* e2, GtsEdge* e3) {
	GtsFace* face = gts_face_new(s->face_class, e1, e2, e3);
	faces.push_back(face);
	return face;
}

vector<GtsVertex*> ofxGtsSurface::getFaceVertices(GtsFace* face) {
	// THIS IS NOT CORRECT!!!!
	cout << "GETFACEVERTICES IS NOT YET READY!!! " << endl;
	GtsVertex* v1 = face->triangle.e1->segment.v1;
	GtsVertex* v2 = face->triangle.e2->segment.v2;
	GtsVertex* v3 = face->triangle.e3->segment.v1;
	vector<GtsVertex*> result;
	result.push_back(v1);
	result.push_back(v2);
	result.push_back(v3);
	return result;
}

vector<GtsVertex*>& ofxGtsSurface::getVertices() {
	//vector<GtsVertex*> vertices;
	gts_surface_foreach_vertex(s, (GtsFunc) build_vertex_vector, &surfaceVertices);
	return surfaceVertices;
}

vector<GtsEdge*>& ofxGtsSurface::getEdges() {
	//vector<GtsEdge*> edges;
	gts_surface_foreach_edge(s, (GtsFunc) build_edge_vector, &surfaceEdges);
	return surfaceEdges;
}

vector<GtsTriangle*>& ofxGtsSurface::getTriangles() {
	//vector<GtsTriangle*> triangles;
	gts_surface_foreach_face(s, (GtsFunc) build_triangle_vector, &surfaceTriangles);
	return surfaceTriangles;
}

void ofxGtsSurface::addVertices(vector<ofVec3f> verts) {
	
	
}


bool ofxGtsSurface::intersects(ofxGtsSurface* surfaceToCheck) {
	
	return false;
}

void ofxGtsSurface::merge(ofxGtsSurface* surfaceToMerge) {
	
	gts_surface_merge (s, surfaceToMerge->getSurface());
	
	// now delete the other surface?
}

void ofxGtsSurface::cleanupTriangles()
{
	
	
	GSList * triangles = NULL;
	GSList * i;
	
	g_return_if_fail (s != NULL);
	
	/* build list of triangles */
	gts_surface_foreach_face (s, (GtsFunc) build_list, &triangles);
	
	/* remove duplicate triangles */
	i = triangles;
	while (i) {
		GtsTriangle * t = (GtsTriangle*) i->data ;
		if (gts_triangle_is_duplicate (t))
		/* destroy t, its edges (if not used by any other triangle)
		 and its corners (if not used by any other edge) */
			gts_object_destroy (GTS_OBJECT (t));
		i = i->next;
	}
	
	/* free list of triangles */
	g_slist_free (triangles);
}

void ofxGtsSurface::cleanupVertices()
{
	
	GSList * edges = NULL;
	GSList * i;
	
	g_return_if_fail (s != NULL);
	
	/* build list of edges */
	gts_surface_foreach_edge (s, (GtsFunc) build_list, &edges);
	
	/* remove degenerate and duplicate edges.
     Note: we could use gts_edges_merge() to remove the duplicates and then
     remove the degenerate edges but it is more efficient to do everything 
     at once (and it's more pedagogical too ...) */
	
	/* We want to control manually the destruction of edges */
	gts_allow_floating_edges = TRUE;
	
	i = edges;
	while (i) {
		GtsEdge * e = (GtsEdge*) i->data;
		GtsEdge * duplicate;
		if (GTS_SEGMENT (e)->v1 == GTS_SEGMENT (e)->v2) /* edge is degenerate */
		/* destroy e */
			gts_object_destroy (GTS_OBJECT (e));
		else if ((duplicate = gts_edge_is_duplicate (e))) {
			/* replace e with its duplicate */
			gts_edge_replace (e, duplicate);
			/* destroy e */
			gts_object_destroy (GTS_OBJECT (e));
		}
		i = i->next;
	}
	
	/* don't forget to reset to default */
	gts_allow_floating_edges = FALSE;
	
	/* free list of edges */
	g_slist_free (edges);
	
	
}