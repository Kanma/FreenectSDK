#! /bin/bash


echo "Removing previous NiTE2 installation..."
rm -rf NiTE2/bin
rm -rf NiTE2/include
rm -rf NiTE2/examples


echo "Copying and modifying the library..."
mkdir -p NiTE2/bin
cp $1/Redist/libNiTE2.dylib NiTE2/bin/.
cp $1/Redist/NiTE.ini NiTE2/bin/.
cp -R $1/Redist/NiTE2 NiTE2/bin/.

install_name_tool -id @rpath/libNiTE2.dylib NiTE2/bin/libNiTE2.dylib
install_name_tool -change libOpenNI2.dylib @rpath/libOpenNI2.dylib NiTE2/bin/libNiTE2.dylib
install_name_tool -add_rpath ./ NiTE2/bin/libNiTE2.dylib


echo "Copying and modifying the headers..."
mkdir -p NiTE2/include
cp $1/Include/*.h NiTE2/include/.


echo "Copying the source code of the examples..."
mkdir -p NiTE2/examples/HandViewer
cp $1/Samples/HandViewer/*.h NiTE2/examples/HandViewer/.
cp $1/Samples/HandViewer/*.cpp NiTE2/examples/HandViewer/.

mkdir -p NiTE2/examples/SimpleHandTracker
cp $1/Samples/SimpleHandTracker/*.cpp NiTE2/examples/SimpleHandTracker/.

mkdir -p NiTE2/examples/SimpleUserTracker
cp $1/Samples/SimpleUserTracker/*.cpp NiTE2/examples/SimpleUserTracker/.

mkdir -p NiTE2/examples/UserViewer
cp $1/Samples/UserViewer/*.h NiTE2/examples/UserViewer/.
cp $1/Samples/UserViewer/*.cpp NiTE2/examples/UserViewer/.

mkdir -p NiTE2/examples/Common
if [ -f $1/Samples/Common/NiteSampleUtilities.h ]
then
    cp $1/Samples/Common/NiteSampleUtilities.h NiTE2/examples/Common/.
else
    cp $1/Samples/SimpleUserTracker/NiteSampleUtilities.h NiTE2/examples/Common/.
fi
