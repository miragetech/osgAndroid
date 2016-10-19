#ANDROID APPLICATION MAKEFILE
APP_BUILD_SCRIPT := $(call my-dir)/Android.mk
NDK_MODULE_PATH := $(call my-dir)
#APP_PROJECT_PATH := $(call my-dir)

APP_OPTIM := release

APP_PLATFORM 	:= android-8
APP_STL         := gnustl_shared
APP_CFLAGS      := -fexceptions
APP_CPPFLAGS 	:= -fexceptions -frtti
APP_ABI 	:= armeabi armeabi-v7a
#APP_MODULES     := jniosg-gles1 jniosg-gles2
NDK_TOOLCHAIN_VERSION := 4.9
