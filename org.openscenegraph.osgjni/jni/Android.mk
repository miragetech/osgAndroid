LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

# OSG SDK for Android
OSG_SDK := /Users/rgaitan/Projects/android/osg-3.0.1

LOCAL_MODULE            := libjniosgconfiguration

LOCAL_C_INCLUDES        := $(OSG_SDK)/include
LOCAL_CFLAGS    		:= -Werror -fno-short-enums
LOCAL_CPPFLAGS  		:= -DOSG_LIBRARY_STATIC 
LOCAL_LDFLAGS   		:= -L$(OSG_SDK)/obj/local/armeabi -fno-short-enums
LOCAL_SRC_FILES 		:= JNIOSGConfiguration.cpp

LOCAL_LDLIBS    		:= -llog -lGLESv1_CM -ldl

TARGET_LDLIBS := \
$(OSG_SDK)/obj/local/armeabi/libosgdb_osg.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_ive.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgvolume.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgtext.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgterrain.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgsim.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgshadow.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgparticle.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgmanipulator.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osgfx.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osganimation.a \
$(OSG_SDK)/obj/local/armeabi/libosgdb_serializers_osg.a \
$(OSG_SDK)/obj/local/armeabi/libosgViewer.a \
$(OSG_SDK)/obj/local/armeabi/libosgVolume.a \
$(OSG_SDK)/obj/local/armeabi/libosgTerrain.a \
$(OSG_SDK)/obj/local/armeabi/libosgText.a \
$(OSG_SDK)/obj/local/armeabi/libosgShadow.a \
$(OSG_SDK)/obj/local/armeabi/libosgSim.a \
$(OSG_SDK)/obj/local/armeabi/libosgParticle.a \
$(OSG_SDK)/obj/local/armeabi/libosgManipulator.a \
$(OSG_SDK)/obj/local/armeabi/libosgGA.a \
$(OSG_SDK)/obj/local/armeabi/libosgFX.a \
$(OSG_SDK)/obj/local/armeabi/libosgDB.a \
$(OSG_SDK)/obj/local/armeabi/libosgAnimation.a \
$(OSG_SDK)/obj/local/armeabi/libosgUtil.a \
$(OSG_SDK)/obj/local/armeabi/libosg.a \
$(OSG_SDK)/obj/local/armeabi/libOpenThreads.a \
$(OSG_SDK)/obj/local/armeabi/libgnustl_static.a


include $(BUILD_SHARED_LIBRARY)

