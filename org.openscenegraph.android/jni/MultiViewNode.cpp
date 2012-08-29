/* @License
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait‡n, Mirage Technologies S.L.     |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */

#include <osg/Camera>
#include <osg/Vec3>
#include <osg/observer_ptr>
#include <osg/Notify>
#include <osg/io_utils>

#include <osgViewer/Viewer>

#include <iostream>
#include <sstream>

class MultiViewNode: public osg::Node {
public:
	typedef osg::Texture2D TextureType;

	MultiViewNode(unsigned int numCameras = 2,
				  unsigned int cameraCols = 2,
				  unsigned int cameraRows = 1,
				  unsigned int screenWidth = 720,
				  unsigned int screenHeight = 480);

	virtual void init();
	virtual void traverse(osg::NodeVisitor &nv);

	void setSceneData(osg::Node *scene) { _scene = scene; _dirty = true; }
	const osg::Node *getSceneData() { return _scene.get(); }

	typedef std::vector<osg::ref_ptr<osg::Camera> > CameraList;

	const CameraList &getCameraList() { return _cameraList; }

	virtual osg::BoundingSphere computeBound() const;

	void setNumCameraRows(unsigned int numCameras) { _cameraRows = numCameras; _dirty = true; }
	unsigned int getNumCameraRows() { return _cameraRows; }

	void setNumCameraCols(unsigned int numCameras) { _cameraCols = numCameras; _dirty = true; }
	unsigned int getNumCameraCols() { return _cameraCols; }

	void setScreenWidth(unsigned int width) { _screenWidth = width; _dirty = true; }
	unsigned int getScreenWidth() { return _screenWidth; }

	void setScreenHeight(unsigned int height) { _screenHeight = height; _dirty = true; }
	unsigned int getScreenHeight() { return _screenHeight; }

	unsigned int getViewTextureWidth() { return _texWidth; }
	unsigned int getViewTextureHeight() { return _texHeight; }

	void setUseMainCamera(bool value) { _useMainCamera = value; _dirty = true; }
	bool getUseMainCamera() { return _useMainCamera; }

	void setFusionDistance(double fusionDistance) { _fixedFusionDistance = fusionDistance; }
	double getFusionDistance() { return _fixedFusionDistance; }

	void setScreenDistance(double screenDistance) { _screenDistance = screenDistance; }
	double getScreenDistance() { return _screenDistance; }

protected:
	virtual ~MultiViewNode();

	virtual osg::Node *createGraph();

	unsigned int _numCameras;

	unsigned int _cameraCols;

	unsigned int _cameraRows;

	unsigned int _screenWidth;

	unsigned int _screenHeight;

	unsigned int _texWidth;

	unsigned int _texHeight;

	bool _dirty;

	CameraList _cameraList;

	osg::ref_ptr<osg::Node> _scene;

	osg::ref_ptr<osg::Node> _renderScene;

	bool _useMainCamera;

	double _fixedFusionDistance;

	double _screenDistance;
};

class UpdateMultiViewCamerasEventCallback: public osgGA::GUIEventHandler {
public:
    UpdateMultiViewCamerasEventCallback();
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa,
                        osg::Object* node, osg::NodeVisitor* nv);
protected:
    float _fusionDistanceInc;
    bool _changeProj;

    osg::observer_ptr<osgViewer::View> _viewer;
    osg::observer_ptr<MultiViewNode> _multiViewNode;
};

UpdateMultiViewCamerasEventCallback::UpdateMultiViewCamerasEventCallback() {
    _fusionDistanceInc = 1.0f;
    _changeProj = true;
}

bool UpdateMultiViewCamerasEventCallback::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa,
                                                 osg::Object* node, osg::NodeVisitor* nv) {

    switch( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::FRAME: {
            if(!_viewer.valid()) {
                _viewer = dynamic_cast<osgViewer::View*>(&aa);
                if(!_viewer.valid()) {
                    osg::notify(osg::FATAL) << "Hey that's not a viewer" << std::endl;
                    return false;
                }
            }
            if(!_multiViewNode.valid()) {
                _multiViewNode = dynamic_cast<MultiViewNode*>(node);
                if(!_multiViewNode.valid()) {
                    osg::notify(osg::FATAL) << "Hey that's not a multiview Node" << std::endl;
                    return false;
                }
            }
            double iod = 0.056;
            double sd = _multiViewNode->getScreenDistance();
            /*const osg::DisplaySettings *displaySettings = _viewer->getDisplaySettings();
            if(displaySettings) {
                iod = displaySettings->getEyeSeparation();
                sd = displaySettings->getScreenDistance();
            }*/

            double fusionDistance = 1.0;
            //if(_viewer->getCameraManipulator())
            //    fusionDistance=_viewer->getCameraManipulator()->getFusionDistanceValue();
            if(_multiViewNode->getFusionDistance()>0.0)
            {
                fusionDistance = _multiViewNode->getFusionDistance();
            }
            else
            {
                fusionDistance = _viewer->getFusionDistanceValue();
            }
            fusionDistance*=_fusionDistanceInc;

            //osg::notify(osg::NOTICE) << "fusionDistance:" << fusionDistance << std::endl;
            //osg::notify(osg::NOTICE) << "sd:" << sd << std::endl;
            //osg::notify(osg::NOTICE) << "iod:" << iod << std::endl;
            osg::Matrixd view = _viewer->getCamera()->getViewMatrix();
            osg::Matrixd proj = _viewer->getCamera()->getProjectionMatrix();
            const MultiViewNode::CameraList &cameraList = _multiViewNode->getCameraList();
            for(unsigned int i=0;i<cameraList.size();i++) {
                osg::Camera *cam = cameraList[i];
                osg::Vec4 clearColor = _viewer->getCamera()->getClearColor();
                clearColor.a() = 0.0f;
                cam->setClearColor(clearColor);
                double cam_id = double(i)-double(cameraList.size()-1.0)/2.0;
                double es = cam_id*iod*(fusionDistance/sd);
                osg::Matrixd finalViewMatrix = view *
                                               osg::Matrixd(1.0,0.0,0.0,0.0,
                                                            0.0,1.0,0.0,0.0,
                                                            0.0,0.0,1.0,0.0,
                                                            es,0.0,0.0,1.0);
                cam->setViewMatrix(finalViewMatrix);

                if(_changeProj) {
                    osg::Matrixd finalProjMatrix = proj*osg::Matrixd(1.0,0.0,0.0,0.0,
                                                                     0.0,1.0,0.0,0.0,
                                                                     -(cam_id*iod)/sd,0.0,1.0,0.0,
                                                                     0.0,0.0,0.0,1.0);
                    cam->setProjectionMatrix(finalProjMatrix);
                }
                else
                    cam->setProjectionMatrix(proj);
            }
            return false;
        }
    case osgGA::GUIEventAdapter::KEYDOWN: {
            if (ea.getKey()=='+')
            {
                _fusionDistanceInc+=0.1;
                osg::notify(osg::NOTICE) << "fusion distance inc=" << _fusionDistanceInc << std::endl;
                return true;
            }
            else if(ea.getKey()=='-')
            {
                _fusionDistanceInc-=0.1;
                osg::notify(osg::NOTICE) << "fusion distance inc=" << _fusionDistanceInc << std::endl;
                return true;
            }
            else if(ea.getKey()=='P') {
                _changeProj = !_changeProj;
                osg::notify(osg::NOTICE) << "Proj changing:" << _changeProj << std::endl;
                return true;
            }
            return false;
        }
    }
    return false;
}

MultiViewNode::MultiViewNode(
		unsigned int numCameras,
		unsigned int cameraCols,
		unsigned int cameraRows,
		unsigned int screenWidth,
		unsigned int screenHeight):
osg::Node(),
_numCameras(numCameras),
_cameraCols(cameraCols),
_cameraRows(cameraRows),
_screenWidth(screenWidth),
_screenHeight(screenHeight),
_dirty(true),
_useMainCamera(true),
_fixedFusionDistance(0.0),
_screenDistance(0.5)
{
    _texWidth = (_screenWidth/_numCameras);
    _texHeight = (_screenHeight);

}

osg::Node *MultiViewNode::createGraph()
{
    osg::Group *scene = new osg::Group();
    _cameraList.clear();
    _texWidth = (_screenWidth/_numCameras);
    _texHeight = (_screenHeight);
    unsigned int out_tex_width = _texWidth*_cameraCols;
    unsigned int out_tex_height = _texHeight*_cameraRows;

    for(unsigned int i=0;i<_numCameras;i++)
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera();
        _cameraList.push_back(camera.get());
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setProjectionMatrixAsPerspective(30,_texWidth/_texHeight,0.01,1000);
        camera->setRenderOrder(osg::Camera::NESTED_RENDER);
        camera->addChild(_scene.get());
        unsigned int col = i%_cameraCols;
        unsigned int row = (i-col)/_cameraCols;
        camera->setViewport(col*_texWidth,(out_tex_height-_texHeight)-row*_texHeight,_texWidth,_texHeight);
        scene->addChild(camera.get());
    }
    
    return scene;
}

void MultiViewNode::init()
{
    if(!_scene.valid()) return;

    _renderScene = createGraph();

    if(_useMainCamera)
        this->setEventCallback(new UpdateMultiViewCamerasEventCallback());

    _dirty = false;

    setNumChildrenRequiringUpdateTraversal(_numCameras);
    setNumChildrenRequiringEventTraversal(_numCameras);
}

void MultiViewNode::traverse(osg::NodeVisitor &nv) {
    if(_dirty) {
        init();
    }
    if(_renderScene.valid())
        _renderScene->accept(nv);

}

MultiViewNode::~MultiViewNode() {
}

osg::BoundingSphere MultiViewNode::computeBound() const {
    osg::BoundingSphere bs;
    if(_scene.valid()) {
        bs.expandBy(_scene->computeBound());
    }
    return bs;
}

/// Java JNI Part

#include <jni.h>
extern "C" {

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeCreateMultiViewNode(JNIEnv *env, jclass,
		jint numCameras = 2,
		jint cameraCols = 2,
		jint cameraRows = 1,
		jint screenWidth = 720,
		jint screenHeight = 480)
{
	MultiViewNode *m = new MultiViewNode(
						numCameras,
						cameraCols,
						cameraRows,
						screenWidth,
						screenHeight);
	m->ref();
	return reinterpret_cast<jlong>(m);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeDispose(JNIEnv *, jclass,
		jlong cptr)
{
	MultiViewNode *m = reinterpret_cast<MultiViewNode *>(cptr);
    if(m != 0)
        m->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeSetSceneData(JNIEnv *env, jclass,
		jlong cptr, jlong scene_cptr)
{
	MultiViewNode *m = reinterpret_cast<MultiViewNode *>(cptr);
	osg::Node *scene = reinterpret_cast<osg::Node *>(scene_cptr);
	if(m != 0 && scene != 0)
	{
		m->setSceneData(scene);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeSetFusionDistance(JNIEnv *env, jclass,
		jlong cptr, jdouble fusionDistance)
{
	MultiViewNode *m = reinterpret_cast<MultiViewNode *>(cptr);
	if(m != 0)
	{
		m->setFusionDistance(fusionDistance);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeSetScreenDistance(JNIEnv *env, jclass,
		jlong cptr, jdouble screenDistance)
{
	MultiViewNode *m = reinterpret_cast<MultiViewNode *>(cptr);
	if(m != 0)
	{
		m->setScreenDistance(screenDistance);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_MultiViewNode_nativeSetScreenSize(JNIEnv *env, jclass,
		jlong cptr, jint width, jint height)
{
	MultiViewNode *m = reinterpret_cast<MultiViewNode *>(cptr);
	if(m != 0)
	{
		m->setScreenWidth(width);
		m->setScreenHeight(height);
	}
}

}

