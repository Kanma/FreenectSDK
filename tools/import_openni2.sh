#! /bin/bash


echo "Removing previous OpenNI2 installation..."
rm -rf OpenNI2/bin
rm -rf OpenNI2/include
rm -rf OpenNI2/examples


echo "Copying and modifying the library..."
mkdir -p OpenNI2/bin
cp $1/Redist/libOpenNI2.dylib OpenNI2/bin/.
cp $1/Redist/OpenNI.ini OpenNI2/bin/.
mkdir -p OpenNI2/bin/OpenNI2/Drivers
#cp -R $1/Redist/OpenNI2 OpenNI2/bin/.

install_name_tool -id @rpath/libOpenNI2.dylib OpenNI2/bin/libOpenNI2.dylib
install_name_tool -add_rpath ./ OpenNI2/bin/libOpenNI2.dylib


echo "Copying and modifying the headers..."
mkdir -p OpenNI2/include
mkdir -p OpenNI2/include/MacOSX
mkdir -p OpenNI2/include/Linux-x86
mkdir -p OpenNI2/include/Driver
cp $1/Include/Oni*.h OpenNI2/include/.
cp $1/Include/OpenNI.h OpenNI2/include/.
cp $1/Include/MacOSX/*.h OpenNI2/include/MacOSX/.
cp $1/Include/Linux-x86/*.h OpenNI2/include/Linux-x86/.
cp $1/Include/Driver/*.h OpenNI2/include/Driver/.


echo "Copying the source code of the examples..."
mkdir -p OpenNI2/examples/ClosestPointViewer
cp $1/Samples/ClosestPointViewer/Viewer.h OpenNI2/examples/ClosestPointViewer/.
cp $1/Samples/ClosestPointViewer/Viewer.cpp OpenNI2/examples/ClosestPointViewer/.
cp $1/Samples/ClosestPointViewer/main.cpp OpenNI2/examples/ClosestPointViewer/.

mkdir -p OpenNI2/examples/EventBasedRead
cp $1/Samples/EventBasedRead/main.cpp OpenNI2/examples/EventBasedRead/.

mkdir -p OpenNI2/examples/MWClosestPoint
cp $1/Samples/MWClosestPoint/MWClosestPoint.h OpenNI2/examples/MWClosestPoint/.
cp $1/Samples/MWClosestPoint/MWClosestPoint.cpp OpenNI2/examples/MWClosestPoint/.

mkdir -p OpenNI2/examples/SimpleRead
cp $1/Samples/SimpleRead/main.cpp OpenNI2/examples/SimpleRead/.

mkdir -p OpenNI2/examples/SimpleViewer
cp $1/Samples/SimpleViewer/Viewer.h OpenNI2/examples/SimpleViewer/.
cp $1/Samples/SimpleViewer/Viewer.cpp OpenNI2/examples/SimpleViewer/.
cp $1/Samples/SimpleViewer/main.cpp OpenNI2/examples/SimpleViewer/.

mkdir -p OpenNI2/examples/Common
if [ -f $1/Samples/Common/OniSampleUtilities.h ]
then
    cp $1/Samples/Common/OniSampleUtilities.h OpenNI2/examples/Common/.
else
    cp $1/Samples/SimpleRead/OniSampleUtilities.h OpenNI2/examples/Common/.
fi
