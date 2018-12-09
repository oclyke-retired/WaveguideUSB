/*

Purpose: This file is an example of what the user should provide as "wgusb_conf.h." Generally it takes care of system-wide
	configurations like the maximum number of interfaces per device.

Aythor: Owen Lyke

*/

#ifndef __WGUSB_CONF_H_
#define __WGUSB_CONF_H_


#define WGUSB_LPM_SUPPORT 	0		// Change to 1 if your driver supports link power management
#define WGUSB_MAX_ENDPTS_IN		5	// The number of IN endpoints that the driver supports - not including ep0
#define WGUSB_MAX_ENDPTS_OUT	5	// The number of OUT endpoints that the driver supports - not including ep0
#define WGUSB_EP0_MPS 64			// The maximum packet size for the control (ep0) endpoint of the driver

#define WGUSB_MAX_CONF 1		// This is the number of configurations that each device type contains
#define WGUSB_MAX_INTFC 2		// This is the number of interfaces that each configuration type contains. 

#define WGUSB_DEF_LANGID 0x0409
#define WGUSB_NUM_LANGID 1			// This is the number of language IDs that are stored in the 0th string descriptor


// Add any necessary definitions for your driver below



#endif /* __WGUSB_CONF_H_ */
