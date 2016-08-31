LOCAL_PATH := $(call my-dir)
 
OSG_SRC_FILES := \
JNIosgViewer.cpp \
JNIosg.cpp \
JNIosgDB.cpp \
JNIosgUtil.cpp \
JNIosgGA.cpp \
JNIUtils.cpp \
MultiViewNode.cpp \
GLES2ShaderGenVisitor.cpp \
screenview.cpp

OSG_LDLIBS := \
-losgdb_osg \
-losgdb_ive \
-losgdb_rgb \
-losgdb_bmp \
-losgdb_tga \
-losgdb_stl \
-losgdb_obj \
-losgdb_dxf \
-losgdb_curl \
-losgdb_gif \
-losgdb_jpeg \
-losgdb_openflight \
-losgdb_serializers_osgvolume \
-losgdb_serializers_osgtext \
-losgdb_serializers_osgterrain \
-losgdb_serializers_osgsim \
-losgdb_serializers_osgshadow \
-losgdb_serializers_osgparticle \
-losgdb_serializers_osgmanipulator \
-losgdb_serializers_osgfx \
-losgdb_serializers_osganimation \
-losgdb_serializers_osgui \
-losgdb_serializers_osgviewer \
-losgdb_serializers_osgga \
-losgdb_serializers_osgutil \
-losgdb_serializers_osg \
-losgdb_deprecated_osgwidget \
-losgdb_deprecated_osgviewer \
-losgdb_deprecated_osgvolume \
-losgdb_deprecated_osgtext \
-losgdb_deprecated_osgterrain \
-losgdb_deprecated_osgsim \
-losgdb_deprecated_osgshadow \
-losgdb_deprecated_osgparticle \
-losgdb_deprecated_osgfx \
-losgdb_deprecated_osganimation \
-losgdb_deprecated_osg \
-losgPresentation \
-losgWidget \
-losgUI \
-losgViewer \
-losgVolume \
-losgTerrain \
-losgText \
-losgShadow \
-losgSim \
-losgParticle \
-losgManipulator \
-losgGA \
-losgFX \
-losgDB \
-losgAnimation \
-losgUtil \
-losg \
-lOpenThreads \
-ltiff \
-ljpeg \
-lgif \
-lpng \
-lcurl \
-lzlib

### Static preparation
OSG_HEAD:=<path-to-osg-gles1-sdk> 
#OSG_HEAD:=/media/christian/DATA/android-osg-sdk/gles1
#/Users/rgaitan/Projects/OSG/osg-trunk-android-static-gles1-sdk 
#/media/christian/DATA/android-osg-sdk/gles1
OSG_SDK:=$(OSG_HEAD)/$(TARGET_ARCH_ABI)
OSG_3RDPARTY_DIR:=obj/local/$(TARGET_ARCH_ABI)
OSG_3RDPARTY_LIBS_DIR:=
OSG_3RDPARTY_COMPONENTS:=jpeg gif png tiff zlib curl

define add_osg_3rdparty_component
  include $(CLEAR_VARS)
  LOCAL_MODULE:=$1
  ifeq ($(strip $1),gif)
    LOCAL_SRC_FILES:=$(OSG_HEAD)/$1lib/$(OSG_3RDPARTY_DIR)/lib$1.a
    OSG_3RDPARTY_LIBS_DIR +=$(OSG_HEAD)/$1lib/$(OSG_3RDPARTY_DIR) 
  else
    ifeq ($(strip $1),png)
      LOCAL_SRC_FILES:=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR)/lib$1.a
      OSG_3RDPARTY_LIBS_DIR +=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR) 
    else
      ifeq ($(strip $1),tiff)
        LOCAL_SRC_FILES:=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR)/lib$1.a
        OSG_3RDPARTY_LIBS_DIR +=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR) 
      else
        ifeq ($(strip $1),jpeg)
          LOCAL_SRC_FILES:=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR)/lib$1.a
          OSG_3RDPARTY_LIBS_DIR +=$(OSG_HEAD)/lib$1/$(OSG_3RDPARTY_DIR) 
        else
          LOCAL_SRC_FILES:=$(OSG_HEAD)/$1/$(OSG_3RDPARTY_DIR)/lib$1.a
          OSG_3RDPARTY_LIBS_DIR +=$(OSG_HEAD)/$1/$(OSG_3RDPARTY_DIR) 
        endif
      endif
    endif
  endif
  include $(PREBUILT_STATIC_LIBRARY)
endef

$(foreach module,$(OSG_3RDPARTY_COMPONENTS),$(eval $(call add_osg_3rdparty_component,$(module))))
LOCAL_INCLUDE:=${LOCAL_C_INCLUDES}

### GLES1 build
include $(CLEAR_VARS)
OSG_HEAD:=<path-to-osg-gles1-sdk>
#OSG_HEAD:=/media/christian/DATA/android-osg-sdk/gles1
#/Users/rgaitan/Projects/OSG/osg-trunk-android-static-gles1-sdk
#/media/christian/DATA/android-osg-sdk/gles1
OSG_SDK:=${OSG_HEAD}/$(TARGET_ARCH_ABI)

define add_lib_dir
    LOCAL_LDFLAGS += -L$1
endef

ifneq (,$(wildcard $(OSG_SDK)/include/osg/Config))

    APP_MODULES       := jniosg-gles1
    OSG_SDK_LIB_PATH:=$(OSG_SDK)/lib
    OSG_SDK_PLUGIN_PATH:=$(OSG_SDK_LIB_PATH)/osgPlugins-3.3.8
    LOCAL_CFLAGS      := -Werror -fno-short-enums -fPIC
    LOCAL_CPPFLAGS    := -DOSG_LIBRARY_STATIC 
    LOCAL_SRC_FILES   := $(OSG_SRC_FILES)
    LOCAL_MODULE      := libjniosg-gles1
    LOCAL_LDLIBS      := -llog -lGLESv1_CM -ldl -lm
    LOCAL_WHOLE_STATIC_LIBRARIES+=$(OSG_3RDPARTY_COMPONENTS)
    LOCAL_C_INCLUDES  := ${LOCAL_INCLUDE} $(OSG_SDK)/include
    TARGET_LDLIBS     := $(OSG_LDLIBS)
    LOCAL_LDFLAGS := -L$(OSG_SDK_LIB_PATH) -L$(OSG_SDK_PLUGIN_PATH) 
    $(foreach path,$(OSG_3RDPARTY_LIBS_DIR),$(eval $(call add_lib_dir,$(path))))
    include $(BUILD_SHARED_LIBRARY)
else
    $(warning Unable to find osg/Config file in the headers, not building libjniosg-gles1 module)
endif

### GLES2 build
include $(CLEAR_VARS)
OSG_HEAD:=<path-to-osg-gles2-sdk>
#OSG_HEAD:=/media/christian/DATA/android-osg-sdk/gles2
#/Users/rgaitan/Projects/OSG/osg-trunk-android-static-gles2-sdk
#/media/christian/DATA/android-osg-sdk/gles2
OSG_SDK2:=${OSG_HEAD}/$(TARGET_ARCH_ABI)

ifneq ( ,$(wildcard $(OSG_SDK)/include/osg/Config))
    APP_MODULES       := jniosg-gles2
    OSG_SDK2_LIB_PATH:=$(OSG_SDK2)/lib
    OSG_SDK2_PLUGIN_PATH:=$(OSG_SDK2_LIB_PATH)/osgPlugins-3.3.8
    LOCAL_CFLAGS      := -Werror -fno-short-enums -fPIC
    LOCAL_CPPFLAGS    := -DOSG_LIBRARY_STATIC 
    LOCAL_SRC_FILES   := $(OSG_SRC_FILES)
    LOCAL_MODULE      := libjniosg-gles2
    LOCAL_LDLIBS      := -llog -lGLESv2 -ldl -lm 
    LOCAL_WHOLE_STATIC_LIBRARIES+=$(OSG_3RDPARTY_COMPONENTS)
    LOCAL_C_INCLUDES  := $(OSG_SDK)/include
    LOCAL_C_INCLUDES:=${LOCAL_INCLUDE} $(OSG_SDK2)/include
    TARGET_LDLIBS := $(OSG_LDLIBS)
    LOCAL_LDFLAGS  := -L$(OSG_SDK2_LIB_PATH) -L$(OSG_SDK2_PLUGIN_PATH)
    $(foreach path,$(OSG_3RDPARTY_LIBS_DIR),$(eval $(call add_lib_dir,$(path))))
    include $(BUILD_SHARED_LIBRARY)
else
    $(warning Unable to find osg/Config file in the headers, not building libjniosg-gles2 module)
endif

