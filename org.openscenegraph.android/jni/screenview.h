/*
 * screenview.h
 *
 *  Created on: Aug 13, 2015
 *      Author: christian
 */

#ifndef SCREENVIEW_H_
#define SCREENVIEW_H_

#include <osg/GL>
#include <osg/GLDefines>
#include <osg/GLExtensions>
#include <osg/GL2Extensions>
#include <osg/GLObjects>
#include <osg/BufferObject>
#include <osg/FrameBufferObject>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>

#include <osg/Switch>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <jni.h>
#include <android/log.h>

class WindowCaptureCallback : public osg::Camera::DrawCallback
{
    public:

        enum Mode
        {
            READ_PIXELS,
            SINGLE_PBO,
            DOUBLE_PBO,
            TRIPLE_PBO
        };

        enum FramePosition
        {
            START_FRAME,
            END_FRAME
        };

        struct ContextData : public osg::Referenced
        {

            inline ContextData(osg::GraphicsContext* gc, Mode mode, GLenum readBuffer, const std::string& name):
                _gc(gc),
                _mode(mode),
                _readBuffer(readBuffer),
                _fileName(name),
                _pixelFormat(GL_BGRA),
                _type(GL_UNSIGNED_BYTE),
                _width(0),
                _height(0),
                _currentImageIndex(0),
                _currentPboIndex(0),
                _reportTimingFrequency(100),
                _numTimeValuesRecorded(0),
                _timeForReadPixels(0.0),
                _timeForFullCopy(0.0),
                _timeForMemCpy(0.0),
                _image_available(false),
                _currentImageToTransfer(NULL)
            {
                _previousFrameTick = osg::Timer::instance()->tick();

                if (gc->getTraits())
                {
                    if (gc->getTraits()->alpha)
                    {
                        osg::notify(osg::NOTICE)<<"Select GL_RGBA read back format"<<std::endl;
                        //_pixelFormat = GL_BGRA;
                        _pixelFormat = GL_RGBA;
                    }
                    else
                    {
                        osg::notify(osg::NOTICE)<<"Select GL_RGB read back format"<<std::endl;
                        //_pixelFormat = GL_BGR;
                        _pixelFormat = GL_RGB;
                    }
                }

                getSize(gc, _width, _height);

                std::cout<<"Window size "<<_width<<", "<<_height<<std::endl;

                // single buffered image
                _imageBuffer.push_back(new osg::Image);

                // double buffer PBO.
                switch(_mode)
                {
                    case(READ_PIXELS):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with out PixelBufferObject."<<std::endl;
                        break;
                    case(SINGLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a single PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        break;
                    case(DOUBLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a double buffer PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        break;
                    case(TRIPLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a triple buffer PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        break;
                    default:
                        break;
                }
            }

            inline void getSize(osg::GraphicsContext* gc, int& width, int& height)
            {
                if (gc->getTraits())
                {
                    width = gc->getTraits()->width;
                    height = gc->getTraits()->height;
                }
            }

            void updateTimings(osg::Timer_t tick_start,
                               osg::Timer_t tick_afterReadPixels,
                               osg::Timer_t tick_afterMemCpy,
                               unsigned int dataSize);

            inline void read()
            {
            	osg::GLExtensions* ext = osg::GLExtensions::Get(_gc->getState()->getContextID(),true);

                if (ext->isPBOSupported && !_pboBuffer.empty())
                {
                    if (_pboBuffer.size()==1)
                    {
                        singlePBO(ext);
                    }
                    else
                    {
                        multiPBO(ext);
                    }
                }
                else
                {
                    readPixels();
                }
            }

            void readPixels();

            inline osg::Image* getImage(void)
            {
            	if( (!_imageBuffer.empty()) && (_currentImageToTransfer != NULL) && (_image_available==true) )
            	{
            		return _currentImageToTransfer;
            	}
            	return NULL;
            }

            void singlePBO(osg::GLExtensions* ext);

            void multiPBO(osg::GLExtensions* ext);

            typedef std::vector< osg::ref_ptr<osg::Image> >             ImageBuffer;
            typedef std::vector< GLuint > PBOBuffer;

            osg::GraphicsContext*   _gc;
            Mode                    _mode;
            GLenum                  _readBuffer;
            std::string             _fileName;

            GLenum                  _pixelFormat;
            GLenum                  _type;
            int                     _width;
            int                     _height;

            unsigned int            _currentImageIndex;
            ImageBuffer             _imageBuffer;

            unsigned int            _currentPboIndex;
            PBOBuffer               _pboBuffer;

            unsigned int            _reportTimingFrequency;
            unsigned int            _numTimeValuesRecorded;
            double                  _timeForReadPixels;
            double                  _timeForFullCopy;
            double                  _timeForMemCpy;
            osg::Timer_t            _previousFrameTick;
            bool					_image_available;
            osg::Image*				_currentImageToTransfer;
        };

        inline WindowCaptureCallback(Mode mode, FramePosition position, GLenum readBuffer):
            _mode(mode),
            _position(position),
            _readBuffer(readBuffer)
        {
        }

        inline FramePosition getFramePosition() const { return _position; }

        inline ContextData* createContextData(osg::GraphicsContext* gc) const
        {
            std::stringstream filename;
            filename << "test_"<<_contextDataMap.size()<<".jpg";
            return new ContextData(gc, _mode, _readBuffer, filename.str());
        }

        inline ContextData* getContextData(osg::GraphicsContext* gc) const
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
            osg::ref_ptr<ContextData>& data = _contextDataMap[gc];
            if (!data) data = createContextData(gc);

            return data.get();
        }

        inline virtual void operator () (osg::RenderInfo& renderInfo) const
        {
            //glReadBuffer(_readBuffer);
            osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
            osg::ref_ptr<ContextData> cd = getContextData(gc);
            cd->read();
        }

        inline virtual void operator() (const osg::Camera& camera) const
        {
        	//glReadBuffer(_readBuffer);
        	const osg::GraphicsContext* gc = camera.getGraphicsContext();
            osg::ref_ptr<ContextData> cd = getContextData(const_cast<osg::GraphicsContext*>(gc));
            cd->read();
        }

        inline osg::Image* getImage(osg::GraphicsContext* graphicscontext)
        {
        	osg::ref_ptr<ContextData> cd = getContextData(graphicscontext);
        	return cd->getImage();
        }

        typedef std::map<osg::GraphicsContext*, osg::ref_ptr<ContextData> > ContextDataMap;

        Mode                        _mode;
        FramePosition               _position;
        GLenum                      _readBuffer;
        mutable OpenThreads::Mutex  _mutex;
        mutable ContextDataMap      _contextDataMap;


};

void addCallbackToViewer(osgViewer::ViewerBase& viewer, WindowCaptureCallback* callback);


#endif /* SCREENVIEW_H_ */
