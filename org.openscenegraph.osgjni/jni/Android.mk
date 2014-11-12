LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

# OSG SDK for Android
OSG_SDK := /home/rgaitan/Projects/OSG/osg-trunk-static-armeabi-android-sdk
OSG_SDK_LIB_PATH := $(OSG_SDK)/lib
OSG_SDK_PLUGIN_PATH := $(OSG_SDK_LIB_PATH)/osgPlugins-3.3.3
#OSG_SDK_LIB_PATH := $(OSG_SDK)/obj/local/armeabi-v7a

LOCAL_MODULE            := libjniosgconfiguration

LOCAL_C_INCLUDES        := $(OSG_SDK)/include
LOCAL_CFLAGS    		:= -Werror -fno-short-enums
LOCAL_CPPFLAGS  		:= -DOSG_LIBRARY_STATIC 
LOCAL_LDFLAGS   		:= -L$(OSG_SDK)/obj/local/armeabi -fno-short-enums
LOCAL_SRC_FILES 		:= JNIOSGConfiguration.cpp

LOCAL_LDLIBS    		:= -llog -lGLESv1_CM -ldl

TARGET_LDLIBS := \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_osg.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_ive.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_rgb.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_openflight.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgpresentation.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgvolume.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgtext.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgterrain.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgsim.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgshadow.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgparticle.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgmanipulator.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgfx.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osganimation.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgui.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgviewer.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgga.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osgutil.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_serializers_osg.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgwidget.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgviewer.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgvolume.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgtext.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgterrain.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgsim.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgshadow.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgparticle.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osgfx.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osganimation.a \
$(OSG_SDK_PLUGIN_PATH)/libosgdb_deprecated_osg.a \
$(OSG_SDK_LIB_PATH)/libosgPresentation.a \
$(OSG_SDK_LIB_PATH)/libosgWidget.a \
$(OSG_SDK_LIB_PATH)/libosgUI.a \
$(OSG_SDK_LIB_PATH)/libosgViewer.a \
$(OSG_SDK_LIB_PATH)/libosgVolume.a \
$(OSG_SDK_LIB_PATH)/libosgTerrain.a \
$(OSG_SDK_LIB_PATH)/libosgText.a \
$(OSG_SDK_LIB_PATH)/libosgShadow.a \
$(OSG_SDK_LIB_PATH)/libosgSim.a \
$(OSG_SDK_LIB_PATH)/libosgParticle.a \
$(OSG_SDK_LIB_PATH)/libosgManipulator.a \
$(OSG_SDK_LIB_PATH)/libosgGA.a \
$(OSG_SDK_LIB_PATH)/libosgFX.a \
$(OSG_SDK_LIB_PATH)/libosgDB.a \
$(OSG_SDK_LIB_PATH)/libosgAnimation.a \
$(OSG_SDK_LIB_PATH)/libosgUtil.a \
$(OSG_SDK_LIB_PATH)/libosg.a \
$(OSG_SDK_LIB_PATH)/libOpenThreads.a \
$(OSG_SDK_LIB_PATH)/libgnustl_static.a 


include $(BUILD_SHARED_LIBRARY)

