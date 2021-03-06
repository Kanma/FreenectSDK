#pragma once

#include "libfreenect.hpp"
#include "Driver/OniDriverAPI.h"
#include "PS1080.h"
#include "OniVersion.h"


#define SIZE(array) sizeof array / sizeof 0[array]

struct RetrieveKey {
  template <typename T>
  typename T::first_type operator()(T pair) const {
    return pair.first;
  }
};

struct FreenectStreamFrameCookie {
  int refCount;
  
  FreenectStreamFrameCookie() :
    refCount(1) { }
};

static OniVideoMode makeOniVideoMode(OniPixelFormat pixel_format, int resolution_x, int resolution_y, int frames_per_second) {
  OniVideoMode mode;
  mode.pixelFormat = pixel_format;
  mode.resolutionX = resolution_x;
  mode.resolutionY = resolution_y;
  mode.fps = frames_per_second;
  return mode;
}

static bool operator==(const OniVideoMode& left, const OniVideoMode& right) {
  return (left.pixelFormat == right.pixelFormat && left.resolutionX == right.resolutionX
          && left.resolutionY == right.resolutionY && left.fps == right.fps);
}
static bool operator<(const OniVideoMode& left, const OniVideoMode& right) { return (left.resolutionX*left.resolutionY < right.resolutionX*right.resolutionY); }

namespace FreenectDriver {
  class VideoStream : public oni::driver::StreamBase {
  private:
    unsigned int frame_id; // number each frame
    
    virtual OniStatus setVideoMode(OniVideoMode requested_mode) = 0;
    virtual void populateFrame(void* data, OniFrame* frame) const = 0;

  protected:
    static const OniSensorType sensor_type;
    Freenect::FreenectDevice* device;
    int32_t dataSize;
    bool running; // acquireFrame() does something iff true
    OniVideoMode video_mode;
    bool mirroring;
    unsigned int framerate;
  
  public:
    VideoStream(Freenect::FreenectDevice* device) :
      device(device),
      frame_id(1),
      dataSize(0),
      mirroring(false),
      framerate(30) { }
    //~VideoStream() { stop();  }


#if (ONI_VERSION_MAJOR == 2) && (ONI_VERSION_MINOR == 1)
    void addRefToFrame(OniDriverFrame* pFrame)
    {
        *((size_t*) pFrame->pDriverCookie) = *((size_t*) pFrame->pDriverCookie) + 1;
    }

    void releaseFrame(OniDriverFrame* pFrame)
    {
        *((size_t*) pFrame->pDriverCookie) = *((size_t*) pFrame->pDriverCookie) - 1;
        if (0 == *((size_t*) pFrame->pDriverCookie))
        {
            free(pFrame->frame.data);
            free(pFrame->pDriverCookie);
            free(pFrame);
        }
    }
#endif


    void buildFrame(void* data, uint32_t timestamp) {
      if (!running)
        return;     

#if (ONI_VERSION_MAJOR == 2) && (ONI_VERSION_MINOR == 1)
      OniDriverFrame* pFrame = (OniDriverFrame*) malloc(sizeof(OniDriverFrame));
      pFrame->frame.data = malloc(dataSize);
      pFrame->pDriverCookie = malloc(sizeof(size_t));
      *((size_t*) pFrame->pDriverCookie) = 1;

      pFrame->frame.dataSize = dataSize;
      pFrame->frame.frameIndex = frame_id++;
      pFrame->frame.timestamp = frame_id * 1000000 / framerate;
      pFrame->frame.videoMode = video_mode;
      pFrame->frame.width = video_mode.resolutionX;
      pFrame->frame.height = video_mode.resolutionY;

      populateFrame(data, &pFrame->frame);
      raiseNewFrame(pFrame);

#elif (ONI_VERSION_MAJOR == 2) && (ONI_VERSION_MINOR == 2)
      OniFrame* frame = getServices().acquireFrame();
      frame->frameIndex = frame_id++;
      frame->timestamp = frame_id * 1000000 / framerate;
      frame->videoMode = video_mode;
      frame->width = video_mode.resolutionX;
      frame->height = video_mode.resolutionY;

      populateFrame(data, frame);
      raiseNewFrame(frame);
      getServices().releaseFrame(frame);
#else
    #error "Unsupported OpenNI version"
#endif

    }
  
    // from StreamBase
    
    OniStatus start() {
      running = true;
      return ONI_STATUS_OK;
    }
    void stop() { running = false; }

    // only add to property handlers if the property is generic to all children
    // otherwise, implement in child and call these in default case
    OniBool isPropertySupported(int propertyId) { return (getProperty(propertyId, NULL, NULL) != ONI_STATUS_NOT_SUPPORTED); }
    virtual OniStatus getProperty(int propertyId, void* data, int* pDataSize) {
      switch (propertyId) {
        default:
        case ONI_STREAM_PROPERTY_CROPPING:            // OniCropping*
        case ONI_STREAM_PROPERTY_HORIZONTAL_FOV:      // float: radians
        case ONI_STREAM_PROPERTY_VERTICAL_FOV:        // float: radians
        case ONI_STREAM_PROPERTY_MAX_VALUE:           // int
        case ONI_STREAM_PROPERTY_MIN_VALUE:           // int
        case ONI_STREAM_PROPERTY_STRIDE:              // int
        case ONI_STREAM_PROPERTY_NUMBER_OF_FRAMES:    // int
        // camera
        case ONI_STREAM_PROPERTY_AUTO_WHITE_BALANCE:  // OniBool
        case ONI_STREAM_PROPERTY_AUTO_EXPOSURE:       // OniBool
        // xn
        case XN_STREAM_PROPERTY_INPUT_FORMAT:         // unsigned long long
        case XN_STREAM_PROPERTY_CROPPING_MODE:        // XnCroppingMode
          return ONI_STATUS_NOT_SUPPORTED;
          
        case ONI_STREAM_PROPERTY_VIDEO_MODE:          // OniVideoMode*
          if (*pDataSize != sizeof(OniVideoMode)) {
            printf("Unexpected size: %d != %lu\n", *pDataSize, sizeof(OniVideoMode));
            return ONI_STATUS_ERROR;
          }       
          *(static_cast<OniVideoMode*>(data)) = video_mode;
          return ONI_STATUS_OK;
        case ONI_STREAM_PROPERTY_MIRRORING:           // OniBool
          if (*pDataSize != sizeof(OniBool))
          {
            printf("Unexpected size: %d != %lu\n", *pDataSize, sizeof(OniBool));
            return ONI_STATUS_ERROR;
          }
          *(static_cast<OniBool*>(data)) = mirroring;						
          return ONI_STATUS_OK;
      }
    }
    virtual OniStatus setProperty(int propertyId, const void* data, int dataSize) {
      switch (propertyId) {
        default:
        case ONI_STREAM_PROPERTY_CROPPING:            // OniCropping*
        case ONI_STREAM_PROPERTY_HORIZONTAL_FOV:      // float: radians
        case ONI_STREAM_PROPERTY_VERTICAL_FOV:        // float: radians
        case ONI_STREAM_PROPERTY_MAX_VALUE:           // int
        case ONI_STREAM_PROPERTY_MIN_VALUE:           // int
        case ONI_STREAM_PROPERTY_STRIDE:              // int
        case ONI_STREAM_PROPERTY_MIRRORING:           // OniBool
        case ONI_STREAM_PROPERTY_NUMBER_OF_FRAMES:    // int
        // camera
        case ONI_STREAM_PROPERTY_AUTO_WHITE_BALANCE:  // OniBool
        case ONI_STREAM_PROPERTY_AUTO_EXPOSURE:       // OniBool
        // xn
        case XN_STREAM_PROPERTY_INPUT_FORMAT:         // unsigned long long
        case XN_STREAM_PROPERTY_CROPPING_MODE:        // XnCroppingMode
          return ONI_STATUS_NOT_SUPPORTED;
          
        case ONI_STREAM_PROPERTY_VIDEO_MODE:          // OniVideoMode*
          if (dataSize != sizeof(OniVideoMode)) {
            printf("Unexpected size: %d != %lu\n", dataSize, sizeof(OniVideoMode));
            return ONI_STATUS_ERROR;
          }
          if (ONI_STATUS_OK != setVideoMode(*(static_cast<const OniVideoMode*>(data))))
            return ONI_STATUS_NOT_SUPPORTED;
          raisePropertyChanged(propertyId, data, dataSize);
          return ONI_STATUS_OK;
      }
    }
    
  
    /* todo : from StreamBase
    virtual OniStatus convertDepthToColorCoordinates(StreamBase* colorStream, int depthX, int depthY, OniDepthPixel depthZ, int* pColorX, int* pColorY) { return ONI_STATUS_NOT_SUPPORTED; }  
    */
  };
}


/* image video modes reference

FREENECT_VIDEO_RGB             = 0, //< Decompressed RGB mode (demosaicing done by libfreenect)
FREENECT_VIDEO_BAYER           = 1, //< Bayer compressed mode (raw information from camera)
FREENECT_VIDEO_IR_8BIT         = 2, //< 8-bit IR mode
FREENECT_VIDEO_IR_10BIT        = 3, //< 10-bit IR mode
FREENECT_VIDEO_IR_10BIT_PACKED = 4, //< 10-bit packed IR mode
FREENECT_VIDEO_YUV_RGB         = 5, //< YUV RGB mode
FREENECT_VIDEO_YUV_RAW         = 6, //< YUV Raw mode
FREENECT_VIDEO_DUMMY           = 2147483647, //< Dummy value to force enum to be 32 bits wide

ONI_PIXEL_FORMAT_RGB888 = 200,
ONI_PIXEL_FORMAT_YUV422 = 201,
ONI_PIXEL_FORMAT_GRAY8 = 202,
ONI_PIXEL_FORMAT_GRAY16 = 203,
ONI_PIXEL_FORMAT_JPEG = 204,
*/
