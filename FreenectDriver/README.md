OpenNI2-FreenectDriver
======================

FreenectDriver is a bridge to libfreenect implemented as an OpenNI2 driver. It allows use of Kinect hardware as supported by libfreenect. FreenectDriver is available under the Apache 2 license; copyright information appears in src/DeviceDriver.cpp and LICENSE.

Dependencies
------------
* g++ (Linux) or clang++ (OSX)
* python
* libfreenect

Build
-----
    ./waf configure build

Install
-------
1. Download and unpack [OpenNI](http://www.openni.org/openni-sdk/) 2.2 or higher.
2. Copy build/libFreenectDriver.so (Linux) or build/libFreenectDriver.dylib (OSX) to OpenNI2/Drivers/ in your OpenNI2 redist directory.

__________________________________________________

Structure
---------
This driver is modeled on TestDevice.cpp and Drivers/Kinect/. In the FreenectDriver namespace, it ties together the C++ interfaces of OpenNI2 and libfreenect using multiple inheritance.

Driver inherits publically from oni::driver::DriverBase and privately from Freenect::Freenect. A custom libfreenect.hpp allows protected access to the Freenect context, so that FreenectDriver can call the Freenect's C API. As a DriverBase, FreenectDriver manages devices and sets up device state callbacks.

Device inherits publically from oni::driver::DeviceBase and Freenect::FreenectDevice. Because of this, it can be built by Freenect::Freenect::createDevice() and it can define Device's depth and video callbacks. Those callbacks trigger acquireFrame() in FreenectStream.

VideoStream is a virtual base class inheriting from oni::driver::StreamBase. It does generic frame setup in buildFrame() and then calls pure virtual populateFrame() to let derived classes finish the frame. It also provides the base skeleton for setting and getting properties, which cascades down the inheritance tree.

DepthStream and ColorStream are nearly identical in definition and implementation, both inheriting from VideoStream. They differ mostly in the formats they use to process data and the video modes they support. These two classes offer a system to store and report supported video modes. To implement a new mode, simply add it to getSupportedVideoModes() and modify populateFrame() if necessary.

__________________________________________________

Todo
----
* switch to my own advanced libfreenect C++ header
* Gentoo ebuilds
* PROPER LOGGING!
* support more FREENECT_RESOLUTION_\*, FREENECT_VIDEO_\*, and FREENECT_DEPTH_\*
* provide more OniVideoMode and OniStreamProperty
* implement remaining derived functions
