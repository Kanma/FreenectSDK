#pragma once

#include <algorithm> // for transform()
#include <math.h> // for M_PI
#include "libfreenect.hpp"
#include "Driver/OniDriverAPI.h"
#include "VideoStream.hpp"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu"
#pragma clang diagnostic ignored "-Wc++11-extensions"


namespace FreenectDriver {
  class ColorStream : public VideoStream {
  public:
    // from NUI library & converted to radians - please check
    static const float DIAGONAL_FOV = 73.9 * (M_PI / 180);
    static const float HORIZONTAL_FOV = 62 * (M_PI / 180);
    static const float VERTICAL_FOV = 48.6 * (M_PI / 180);
  
  private:
    typedef std::map< OniVideoMode, std::pair<freenect_video_format, freenect_resolution> > FreenectVideoModeMap;
    static const OniSensorType sensor_type = ONI_SENSOR_COLOR;
    
    static FreenectVideoModeMap getSupportedVideoModes();
    OniStatus setVideoMode(OniVideoMode requested_mode);
    void populateFrame(void* data, OniFrame* frame) const;
  
  public:
    ColorStream(Freenect::FreenectDevice* pDevice);
    //~ColorStream() { }
    
	  static OniSensorInfo getSensorInfo() {
			FreenectVideoModeMap supported_modes = getSupportedVideoModes();
			OniVideoMode* modes = new OniVideoMode[supported_modes.size()];
			std::transform(supported_modes.begin(), supported_modes.end(), modes, RetrieveKey());
			return { sensor_type, SIZE(modes), modes }; // sensorType, numSupportedVideoModes, pSupportedVideoModes
		}
    
    // from StreamBase
    OniStatus getProperty(int propertyId, void* data, int* pDataSize) {
			switch (propertyId) {
				default:
					return VideoStream::getProperty(propertyId, data, pDataSize);
				
				case ONI_STREAM_PROPERTY_HORIZONTAL_FOV:        // float (radians)
					if (*pDataSize != sizeof(float)) {
						printf("Unexpected size: %d != %lu\n", *pDataSize, sizeof(float));
						return ONI_STATUS_ERROR;
					}
					*(static_cast<float*>(data)) = HORIZONTAL_FOV;
					return ONI_STATUS_OK;
				case ONI_STREAM_PROPERTY_VERTICAL_FOV:          // float (radians)
					if (*pDataSize != sizeof(float)) {
						printf("Unexpected size: %d != %lu\n", *pDataSize, sizeof(float));
						return ONI_STATUS_ERROR;
					}
					*(static_cast<float*>(data)) = VERTICAL_FOV;
					return ONI_STATUS_OK;
			}
		}
		OniStatus setProperty(int propertyId, const void* data, int dataSize) {
			switch (propertyId) {
				default:
					return VideoStream::setProperty(propertyId, data, dataSize);
		
				case ONI_STREAM_PROPERTY_MIRRORING:   					// OniBool
					if (dataSize != sizeof(OniBool)) {
						printf("Unexpected size: %d != %lu\n", dataSize, sizeof(OniBool));
						return ONI_STATUS_ERROR;
					}
					mirroring = *(static_cast<const OniBool*>(data));
					return ONI_STATUS_OK;
			}
		}
  };
}


#pragma clang diagnostic pop
