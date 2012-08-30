# osgAndroid - Openscenegraph for Android #

Manual JNI/Java wrappers for OpenSceneGraph and some utility classes to easily
develop OSG applications in Android.

## Setting up build environment

This section explains the essential steps to configure the build environment
for osgAndroid.

### Eclipse IDE

First of all, you need to download the Eclipse IDE from:

[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)

### Android SDK

The next step is downloading the Android SDK and the ADT Eclipse plugin. The
best way to tackle this is following the instructions from:

[http://developer.android.com/sdk/installing.html](http://developer.android.com/sdk/installing.html)

Once the AVD Manager starts, mark at least the 2.3.3 SDK.

### Android NDK

The Android NDK is the native development kit for Android. It Is absolutely
necessary installing it to compile the native JNI part of osgAndroid. Download
it from:

[http://developer.android.com/sdk/ndk/index.html](http://developer.android.com/sdk/ndk/index.htm)

### Importing osgAndroid into the Eclipse Workspace

Inside Eclipse, select File -> Import and then General -> Existing project into
Workspace. Press Next button and in the next window press the option Select
root directory, and Browse. Select the directory in which you have cloned the
osgAndroid code and press Accept. Finally, select all the projects that are
shown in the dialog and press Finish.  

### Configuring android_ndk variable inside Eclipse

Before compiling you must configure an environmental variable inside Eclipse.
Open Eclipse Preferences, select Run/Debug -> String Substitution and press New
button. Then create a new variable called android_ndk pointing to previously
downloaded Android NDK. 

## Getting started with osgAndroid

The osgAndroid code is distributed with four eclipse projects, the osgAndroid
library and 3 Activities using the library:

 - org.openscenegraph.android: The osgAndroid library itself. It has the jni
   wrappers for the osg classes and a set of Java classes to easily develop
   android applications using the OpenSceneGraph known API in Java.
 - org.openscenegraph.osgsimple: The most simple Activity you can build with
   osgAndroid. 
 - org.openscenegraph.osgcamera: An example of an Activity using multiples
   surfaces. The example shows the android camera in background and the
   osgAndroid GLSurface, which is configured as a transparent background, in
   the foreground.  
 - org.openscenegraph.osgjni: An example of mixed API. It uses the osgAndroid
   Java API to initialize the context and adds a more complex scene using JNI
   in the Activity. It is Useful when the api in osgAndroid is not exposed or
   when the complexity of the solution requires the use of C++ API.

### Building osgAndroid

Most of the code of osgAndroid is Java, but the JNI part needs to be compiled
using the Android NDK. The Eclipse projects are already prepared to build using
the eclipse variables and Android NDK. It is only necessary to configure the
OSG_SDK variable in the Android.mk files:

 - org.openscenegraph.android/jni/Android.mk
 - org.openscenegraph.osgjni/jni/Android.mk

### Current Limitations

 - The GLES version supported in osgAndroid is GLES 1.1, so you must compile
   OpenSceneGraph with support for GLES1.1.
 - The API of OpenSceneGraph is not completely exposed, only the most common
   classes, so probably you will end adding more wrappers by yourself, which I
   will be glad to add to the main repository, or creating mixed api jni
   Activities.

## License

    /* @License 
     -------------------------------------------------------------------------------
     | osgAndroid - Copyright (C) 2012 Rafael Gaitan, Mirage Technologies S.L.     |
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

## Contributors

The osgAndroid project has been initially funded by:

 * IRTIC(http://smagris3.uv.es/irtic/)
 * AI2(http://www.ai2.upv.es)
 * MirageTechnologies S.L.(http://www.mirage-tech.com).  

Contributors (use Name Surname - Company):

 + Rafael Gaitan - Mirage Technologies S.L.
 + Julio Campos - Mirage Technologies S.L.
 + Jorge Izquierdo - AI2
 + Jordi Torreas - AI2

