#pragma once

#include "physics_types.h"

#include "ofxFiducialBackedGui.h"
#include "ofxPoint2f.h"

#include "ofxFiducialPhysicsRoutines.h"

class ofxFiducialPhysics : public ofRectangle
{
public:
	ofxFiducialPhysics();
	~ofxFiducialPhysics();

	void setup();
	void destroy();

	void update();

	ofxFiducialTracker *tracker;
	
#ifdef NOTIFY_FIDUCIAL_STATUS
	ofEvent<ofxFiducialBacked::types::events::fiducialEvtArgs> fiducialFoundEvt;
	ofEvent<ofxFiducialBacked::types::events::fiducialEvtArgs> fiducialLostEvt;
	ofEvent<ofxFiducialBacked::types::events::fiducialEvtArgs> fiducialUpdatedEvt;
	ofxFiducialBacked::types::fiducial_category_t ignoreFiducialIds;
#endif
	
#ifdef NOTIFY_FIDUCIAL_INTERSECTION
	ofEvent<ofxFiducialBacked::types::events::fiducialRayIntersectionEvtArgs> fiducialRayIntersectionFoundEvt;
	ofEvent<ofxFiducialBacked::types::events::fiducialRayIntersectionEvtArgs> fiducialRayIntersectionLostEvt;
	ofEvent<ofxFiducialBacked::types::events::fiducialRayIntersectionEvtArgs> fiducialRayIntersectionUpdatedEvt;

	ofEvent<ofxFiducialBacked::types::events::fiducialRayIntersectionEvtArgs> fiducialRayTerminationEvt;
	ofxFiducialBacked::types::fiducial_category_t		ignoreHitFiducialIds;
	ofxFiducialBacked::types::fiducial_ranges_flags_t	specialFiducialIdRanges;
#endif

	bool bForceFiducialCorners;

protected:
#ifdef NOTIFY_FIDUCIAL_INTERSECTION
	ofxFiducialPhysicsRoutines* engine;

	ofxFiducialBacked::types::physics::fiducial_physics_types_t fiducialPhysicsTypes;

	vector<ofPoint> window_corners;
	float x_scale, y_scale;	

	ofxFiducialBacked::types::physics::fiducial_edges_t	fiducialEdges;
	ofxFiducial	*dummyFiducial;
	
	virtual bool validFiducial			(ofxFiducial* fiducial);
	virtual bool validInputFiducial		(ofxFiducial* fiducial);
	virtual bool validOutputFiducial	(ofxFiducial* fiducial);
#endif
	
#ifdef NOTIFY_FIDUCIAL_STATUS
	ofxFiducialBacked::types::physics::fiducial_states_t fiducialStates;
#endif
};
