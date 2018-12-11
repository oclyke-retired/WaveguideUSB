/*

Purpose: This header includes definition of USB protocol terms, masks, etc

Author: Owen Lyke
Copyright 2018 Owen Lyke

https://www.beyondlogic.org/usbnutshell/usb1.shtml
https://github.com/IntergatedCircuits/USBDevice

*/

#ifndef __WGUSB_USB_DEF_H_
#define __WGUSB_USB_DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>


/* USB Specification Versions */
typedef enum{
	usb_spec_bcd_2v00 = 0x0200,		// Currently based on 2.00 spec
	usb_spec_bcd_2v01 = 0x0201		// 2.01 spec required for BOS and LPM
}wgusb_usb_spec_bcd_e;

/* USB Standard Features */
typedef enum{
	usb_std_feat_ep_halt = 0x00,
	usb_std_feat_remote_wakeup = 0x01,
	usb_std_feat_test_mode = 0x02
}wgusb_usb_std_feat_e;

/* USB Maximum Endpoint Packet Sizes */
typedef enum{
	usb_ep_mps_bulk_hs = 512,
	usb_ep_mps_bulk_fs = 64,

	usb_ep_mps_isoc_hs = 1024,
	usb_ep_mps_isoc_fs = 1023,

	usb_ep_mps_intr_hs = 1024,
	usb_ep_mps_intr_fs = 64,
	usb_ep_mps_intr_ls = 8,

	usb_ep_mps_ctrl_hs = 64,
	usb_ep_mps_ctrl_fs = 64,
	usb_ep_mps_ctrl_ls = 8,

	usb_ep_mps_ep0_hs = 64,
	usb_ep_mps_ep0_fs = 64,
	usb_ep_mps_ep0_ls = 8
}wgusb_usb_ep_mps_e;

/* USB Data Direction (Considered from Host Perspective) */
typedef enum{
	usb_dir_out = 0,	// Host to device
	usb_dir_in = 1		// Device to host
}wgusb_usb_dir_e;

/* USB Speeds */
typedef enum{
	usb_speed_full = 0,	// 12 Mbaud
	usb_speed_high = 1,	// 480 Mbaud
	usb_speed_low = 2	// 1.5 Mbaud
}wgusb_usb_speed_e;

/* USB Endpoint Types */
typedef enum{					/*   Maximum packet size for       LS      FS      HS */
	usb_endpt_type_ctrl = 0,	/*!< Control endpoint type    *//*  8      64      64 */
	usb_endpt_type_isoc = 1,	/*!< Isochronous endpoint type*//* NA    1023    1024 */
	usb_endpt_type_bulk = 2,	/*!< Bulk endpoint type       *//* NA      64     512 */
	usb_endpt_type_intr = 3		/*!< Interrupt endpoint type  *//*  8      64    1024 */
}wgusb_usb_endpt_type_e;

/* USB Endpoint Addresses */
typedef union{
	struct{
		uint8_t number 		: 4;	// The endpoint number
		uint8_t 			: 3;	// Filler bits
		wgusb_usb_dir_e dir : 1;	// The direction
	};
	uint8_t b;						// The entire address as one byte
}wgusb_usb_ep_addr_t;

/* USB Endpoint Attributes */
typedef union{
	struct{
		wgusb_usb_endpt_type_e	type 	: 2;
		uint8_t 				synch 	: 2; // (ISO Mode) 00 = No Synchonisation, 01 = Asynchronous, 10 = Adaptive, 11 = Synchronous
		uint8_t 				usage 	: 2; // (ISO MOde) 00 = Data Endpoint, 01 = Feedback Endpoint, 10 = Explicit Feedback Data Endpoint, 11 = Reserved */
		uint8_t 						: 2;
	};
	uint8_t b;						// The attributes as one byte
}wgusb_usb_ep_attr_t;

/* USB Endpoint States */
typedef enum{
	usb_ep_state_closed 	= 0,	// Endpoint closed -> NAK
	usb_ep_state_idle 		= 1,	// Endpoint is idle
	usb_ep_state_stall 		= 2,	// Endpoint is halted -> STALL
	usb_ep_state_setup 		= 3,	// Endpoint is idle
	usb_ep_state_data 		= 4,	// Endpoint is performing data trasfer
	usb_ep_state_status 	= 5,	// Endpoint is performing ZLP transfer
	usb_ep_state_data_in	= 6,	// Not part of the original list
	usb_ep_state_data_out 	= 7,
	usb_ep_state_setup_in	= 8,
	usb_ep_state_setup_out 	= 9
}wgusb_usb_ep_state_e;

/* Link Power Management (LPM) States */
typedef enum{
	usb_link_state_active 	= 0,	// Device disconnected
	usb_link_state_sleep 	= 1,	// Device suspended
	usb_link_state_suspend 	= 2,	// Device in L1 sleep mode
	usb_link_state_off 		= 3		// Device connected and active
}wgusb_usb_link_state_e;

/* Power Source Indication */
typedef enum{
	usb_power_source_bus 	= 0,	// Device powered from the bus
	usb_power_source_self 	= 1		// Device powered by itself
}wgusb_usb_power_source_e;

/* USB Request Types */
typedef enum{
	usb_req_type_standard 	= 0,	// Request is specified in the USB standard
	usb_req_type_class		= 1,	// Request is specified in a class standard
	usb_req_type_vendor		= 2		// Request is specified by the vendor (custom)
}wgusb_usb_request_type_e;

/* USB Request Recipient Types */
typedef enum{
	usb_req_recipient_device 	= 0,	// Request is addressed to the device
	usb_req_recipient_interface = 1,	// Request is addressed to the interface
	usb_req_recipient_endpoint 	= 2,	// Request is addressed to an endpoint
	usb_req_recipient_other 	= 3		// Request is custom addressed
}wgusb_usb_req_recipient_e;

/* USB Standard Requests */
typedef enum{
	usb_standard_request_get_status 		= 0x00,		// Get current status of features
	usb_standard_request_clear_feautre 		= 0x01,		// Clear activation of a feature
	usb_standard_request_set_feature 		= 0x03,		// Set activation of a feature
	usb_standard_request_set_address 		= 0x05,		// Set the bus address of the device
	usb_standard_request_get_descriptor 	= 0x06,		// Get a descriptor from the device
	usb_standard_request_set_descriptor 	= 0x07,		// Write a descriptor in the device
	usb_standard_request_get_configuration 	= 0x08,		// Get the current device configuration index
	usb_standard_request_set_configuration 	= 0x09,		// Set the new configuration index
	usb_standard_request_get_interface 		= 0x0A,		// Get current alternate setting value of interface
	usb_standard_request_set_interface 		= 0x0B,		// Set current alternate setting value of interface
	usb_standard_request_synch_frame 		= 0x0C		//
}wgusb_usb_standard_request_e;

/* USB Standard Descriptor Types */
typedef enum{
	usb_descriptor_type_device 						= 0x01,	// Device descriptor
	usb_descriptor_type_configuration 				= 0x02,	// Configuration descriptor
	usb_descriptor_type_string 						= 0x03,	// String descriptor
	usb_descriptor_type_interface 					= 0x04,	// Interface descriptor
	usb_descriptor_type_endpoint 					= 0x05,	// Endpoint descriptor
	usb_descriptor_type_device_qualifier 			= 0x06,	// Device qualifier descriptor
	usb_descriptor_type_other_speed_configuration 	= 0x07,	// Device descriptor for other supported speed
	usb_descriptor_type_iad 						= 0x0B,	// Interface association descriptor
	usb_descriptor_type_bos 						= 0x0F,	// Binary Object Store descriptor
	usb_descriptor_type_device_capability 			= 0x01	// Device capability descriptor (part of BOS)
}wgusb_usb_descriptor_types_e;

/* USB Device Capability Types */
typedef enum{
	usb_dev_cap_usb_2p0_ext = 0x02 			// USB 2.0 extentions (LPM)
}wguab_usb_dev_cap_e;



/* USB Setup Request */
typedef struct{
	union{
		struct{
			wgusb_usb_req_recipient_e 	recipient 	: 5;	// Recipient of request
			wgusb_usb_request_type_e 	type 		: 2;	// Type of request
			wgusb_usb_dir_e 			direction 	: 1;	// Direction of data stage (OUT when no data stage present)
		};
		uint8_t b;											// Whole request type field as a byte
	}request_type;		// Characteristics of this request
	uint8_t request;	// Which request is this (identifier)
	uint16_t value;		// Contents depend on the request. 
	uint16_t index;		// Contents depend on the request
	uint16_t length;	// Length of data transferred in the data stage  (if applicable)
}__packed wgusb_usb_setup_request_t;

/* USB Device Descriptor */
typedef struct{
	uint8_t  bLength;               /*!< Size of the Descriptor in Bytes (18 bytes) */
	uint8_t  bDescriptorType;       /*!< Device Descriptor (0x01) */
	uint16_t bcdUSB;                /*!< USB Specification Number which device complies to */
	uint8_t  bDeviceClass;          /*!< Class Code (Assigned by USB Org)
                                        If equal to Zero, each interface specifies it’s own class code
                                        If equal to 0xFF, the class code is vendor specified.
                                        Otherwise field is valid Class Code. */
	uint8_t  bDeviceSubClass;       /*!< Subclass Code (Assigned by USB Org) */
	uint8_t  bDeviceProtocol;       /*!< Protocol Code (Assigned by USB Org) */
	uint8_t  bMaxPacketSize;        /*!< Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64 */
	uint16_t idVendor;              /*!< Vendor ID (Assigned by USB Org) */
	uint16_t idProduct;             /*!< Product ID (Assigned by Manufacturer) */
	uint16_t bcdDevice;             /*!< Device Release Number */
	uint8_t  iManufacturer;         /*!< Index of Manufacturer String Descriptor */
	uint8_t  iProduct;              /*!< Index of Product String Descriptor */
	uint8_t  iSerialNumber;         /*!< Index of Serial Number String Descriptor */
	uint8_t  bNumConfigurations;    /*!< Number of Possible Configurations */
}__packed wgusb_usb_device_desc_t;

/* USB Configuration Descriptor */
typedef struct{
	uint8_t  bLength;               /*!< Size of Descriptor in Bytes */
	uint8_t  bDescriptorType;       /*!< Configuration Descriptor (0x02) */
	uint16_t wTotalLength;          /*!< Total length in bytes of data returned */
	uint8_t  bNumInterfaces;        /*!< Number of Interfaces */
	uint8_t  bConfigurationValue;   /*!< Value to use as an argument to select this configuration */
	uint8_t  iConfiguration;        /*!< Index of String Descriptor describing this configuration */
	uint8_t  bmAttributes;          /*!< 0b1[Self Powered][Remote Wakeup]00000 */
	uint8_t  bMaxPower;             /*!< Maximum Power Consumption in 2mA units */
}__packed wgusb_usb_config_desc_t;

/* USB Unicode Language Identifier */
typedef struct{
	uint8_t  bLength;           		/*!< Size of Descriptor in Bytes */
	uint8_t  bDescriptorType;   		/*!< String Descriptor (0x03) */
	uint16_t wLANGID[WGUSB_NUM_LANGID]; /*!< Supported Language Codes (e.g. 0x0409 English - United States) */
}__packed wgusb_usb_lang_id_t;

/* USB String Descriptor */
typedef struct{
	uint8_t	 bLength;				/* Size of Descriptor in Bytes (Variable) */
	uint8_t  bDescriptorType;   	/* !< String Descriptor (0x03) */
	uint8_t* pUniString;			/* Pointer to the unicode string whose number of bytes is bLength-2*/
}__packed wgusb_usb_string_desc_t;


/* USB Interface Descriptor */
typedef struct{
	uint8_t  bLength;               /*!< Size of Descriptor in Bytes (9 Bytes) */
	uint8_t  bDescriptorType;       /*!< Interface Descriptor (0x04) */
	uint8_t  bInterfaceNumber;      /*!< Number of Interface */
	uint8_t  bAlternateSetting;     /*!< Value used to select alternative setting */
	uint8_t  bNumEndpoints;         /*!< Number of Endpoints used for this interface */
	uint8_t  bInterfaceClass;       /*!< Class Code (Assigned by USB Org) */
	uint8_t  bInterfaceSubClass;    /*!< Subclass Code (Assigned by USB Org) */
	uint8_t  bInterfaceProtocol;    /*!< Protocol Code (Assigned by USB Org) */
	uint8_t  iInterface;            /*!< Index of String Descriptor Describing this interface */
}__packed wgusb_usb_intfc_desc_t;

/* USB Endpoint Descriptor */
typedef struct{
	uint8_t  bLength;           /*!< Size of Descriptor in Bytes (7 Bytes) */
	uint8_t  bDescriptorType;   /*!< Interface Descriptor (0x05) */
	uint8_t  bEndpointAddress;  /*!< Endpoint Address 0b[0=Out / 1=In]000[Endpoint Number] */
	uint8_t  bmAttributes;      /*!< Bits 0..1 Transfer Type
                                        00 = Control
                                        01 = Isochronous
                                        10 = Bulk
                                        11 = Interrupt
                                    Bits 2..7 are reserved. If Isochronous endpoint,
                                    Bits 3..2 = Synchronisation Type (Iso Mode)
                                        00 = No Synchonisation
                                        01 = Asynchronous
                                        10 = Adaptive
                                        11 = Synchronous
                                    Bits 5..4 = Usage Type (Iso Mode)
                                        00 = Data Endpoint
                                        01 = Feedback Endpoint
                                        10 = Explicit Feedback Data Endpoint
                                        11 = Reserved */
	uint16_t wMaxPacketSize;    /*!< Maximum Packet Size this endpoint is capable of sending or receiving */
	uint8_t  bInterval;         /*!< Interval for polling endpoint data transfers. Value in frame counts.
                                     Ignored for Bulk & Control Endpoints. Isochronous must equal 1 and
                                     field may range from 1 to 255 for interrupt endpoints. */
}__packed wgusb_usb_endpt_desc_t;

/* USB Device Qualifier Descriptor */
typedef struct{
    uint8_t  bLength;               /*!< Size of Descriptor in Bytes */
    uint8_t  bDescriptorType;       /*!< Device Qualifier Descriptor (0x06) */
    uint16_t bcdUSB;                /*!< USB Specification Number which device complies to */
    uint8_t  bDeviceClass;          /*!< Class Code (Assigned by USB Org)
                                        If equal to Zero, each interface specifies it’s own class code
                                        If equal to 0xFF, the class code is vendor specified.
                                        Otherwise field is valid Class Code. */
    uint8_t  bDeviceSubClass;       /*!< Subclass Code (Assigned by USB Org) */
    uint8_t  bDeviceProtocol;       /*!< Protocol Code (Assigned by USB Org) */
    uint8_t  bMaxPacketSize;        /*!< Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64 */
    uint8_t  bNumConfigurations;    /*!< Number of Possible Configurations */
    uint8_t  bReserved;             /*!< Keep 0 */
}__packed wgusb_usb_dev_qual_desc_t;

/* USB Binary Object Store (BOS) Header Descriptor */
typedef struct{
    uint8_t  bLength;               /*!< Size of Descriptor in Bytes */
    uint8_t  bDescriptorType;       /*!< BOS Descriptor (0x0F) */
    uint16_t wTotalLength;          /*!< Total length in bytes of data returned */
    uint8_t  bNumDeviceCaps;        /*!< Number of device capabilities to follow */
}__packed wgusb_usb_bos_desc_t;

/* USB Device Capability Descriptor */
typedef struct{
	uint8_t  bLength;               /*!< Size of Descriptor in Bytes */
	uint8_t  bDescriptorType;       /*!< Device Capability Descriptor (0x10) */
	uint8_t  bDevCapabilityType;    /*!< Capability type: USB 2.0 EXTENSION (0x02) */
	uint32_t bmAttributes;          /*!< Bit 0 Reserved (set to 0)
                                         Bit 1 Link Power Management support */
}__packed wgusb_usb_dev_cap_desc_t;

/* USB Interface Association Descriptor */
typedef struct{
	uint8_t  bLength;               /*!< Size of Descriptor in Bytes */
	uint8_t  bDescriptorType;       /*!< Interface Association Descriptor (0x0B) */
	uint8_t  bFirstInterface;       /*!< First associated interface */
	uint8_t  bInterfaceCount;       /*!< Number of contiguous associated interfaces */
	uint8_t  bFunctionClass;        /*!< Class Code (Assigned by USB Org) */
	uint8_t  bFunctionSubClass;     /*!< Subclass Code (Assigned by USB Org) */
	uint8_t  bFunctionProtocol;     /*!< Protocol Code (Assigned by USB Org) */
	uint8_t  iFunction;             /*!< Index of String Descriptor Describing this function */
}__packed wgusb_usb_iad_des_t;

#ifdef __cplusplus
}
#endif

#endif /* __WGUSB_USB_DEF_H_ */
