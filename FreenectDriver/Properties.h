#ifndef _FREENECTDRIVER_PROPERTIES_H_
#define _FREENECTDRIVER_PROPERTIES_H_

enum {
    KINECT_PROPERTY_LED_STATUS = 0x1EC70001,
};


typedef enum {
    KINECT_LED_OFF              = 0, /**< Turn LED off */
    KINECT_LED_GREEN            = 1, /**< Turn LED to Green */
    KINECT_LED_RED              = 2, /**< Turn LED to Red */
    KINECT_LED_YELLOW           = 3, /**< Turn LED to Yellow */
    KINECT_LED_BLINK_GREEN      = 4, /**< Make LED blink Green */
    KINECT_LED_BLINK_RED_YELLOW = 6, /**< Make LED blink Red/Yellow */
} kinect_led_options;


#endif
