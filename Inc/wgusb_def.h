/*

This file specifies the types created for and used in the Waveguide USB library

*/


#ifndef __WGUSB_DEF_H_
#define __WGUSB_DEF_H_

#include "wgusb_usb_def.h"	// USB Standard Types and Values


#define WGUSB_MAX_ENDPTS_TOTAL (WGUSB_MAX_ENDPTS_IN + WGUSB_MAX_ENDPTS_OUT)


////////////////////////////////////
// 			Status Enum
////////////////////////////////////

/* Status Return Type */
typedef enum{				// A standard status return 
	WGUSB_Nominal = 0x00,

	WGUSB_Error,
	WGUSB_Busy
}wgusb_status_t;

/* USB Device State */
typedef enum{
	usb_dev_state_default = 1,
	usb_dev_state_addressed = 2,
	usb_dev_state_configured = 3,
	usb_dev_state_suspended = 4
}wgusb_dev_state_e;



////////////////////////////////////
// 		String Linked List
////////////////////////////////////

/* WGUSB String Linked List */
typedef struct _wgusb_strng_handle_t{
	uint8_t* pdata;
	uint16_t strSize;
	struct _wgusb_strng_handle_t* pnext;
}wgusb_strng_handle_t;

wgusb_status_t wgusb_append_string_handle(wgusb_strng_handle_t* pbase, uint8_t* pdata, uint16_t len, uint8_t* index);




////////////////////////////////////
// 		Endpoint Handle
////////////////////////////////////

struct _wgusb_intfc_handle_t;	// Forward declaration of interface handle structure

typedef struct{
	wgusb_usb_ep_addr_t address;
	wgusb_usb_ep_attr_t attributes;
	wgusb_usb_ep_mps_e  mps;
	uint8_t 			interval;
}wgusb_ep_desc_t;

typedef struct{
	// External
	wgusb_ep_desc_t	descSettings;

	// Internal
	struct _wgusb_intfc_handle_t* pIntfc;	// The interface with which this endpoint is associated (for non ep0)
	struct{
		uint8_t* pdata;
		uint16_t length;
		uint16_t progress;
	}transfer;
	wgusb_usb_endpt_type_e type;
	wgusb_usb_ep_state_e state;
	uint8_t ifNum;					// Index of the interface to which this endpoint is associated. (N/A for ep0)
	// void*	user;					// User associatable data 
}wgusb_ep_handle_t;






////////////////////////////////////
// 		Interface Handle
////////////////////////////////////

typedef struct{
	const char* name;						// Null terminated c-string with name of the interface
	uint8_t numAlternates;					// How many alternates does this interface have?
	uint8_t intfcClass;
	uint8_t intfcSubClass;
	uint8_t intfcProtocol;
}wgusb_intfc_desc_t;

typedef wgusb_status_t 	(*Intfc_Gen_Cbk_t)			(struct _wgusb_intfc_handle_t *pintfc);
typedef	wgusb_status_t  (*Intfc_EP_Cbk_t)           (struct _wgusb_intfc_handle_t *pintfc , wgusb_ep_handle_t* pep);
typedef wgusb_status_t 	(*Intfc_Get_Cbk_t)			(struct _wgusb_intfc_handle_t * pintfc, uint8_t* pdata, uint16_t* len);

typedef struct{
	Intfc_Gen_Cbk_t 		Init;			// Called when an interface needs to be set up, for example when the configuration that it is part of is selected
	Intfc_Gen_Cbk_t 		DeInit;			// Called when the configuration is cleared or the device is shut down
	Intfc_Gen_Cbk_t			Setup;			// Called when the control endpoint gets a setup request with interface recipient
	Intfc_Gen_Cbk_t			EP0TxSent;		// Called when the control endpoint completes sending data
	Intfc_Gen_Cbk_t			EP0RxReady;		// Called when the control endpoint completes receiving data
	Intfc_EP_Cbk_t			DataIn;			// Called when IN data is completed for an endpoint associated with the interface
	Intfc_EP_Cbk_t			DataOUt;		// Called when OUT data is completed for an endpoint associated with the interface
	Intfc_EP_Cbk_t			IsoIN;			// Called when an ISO IN is completed for an associated endpoint
	Intfc_EP_Cbk_t			IsoOUT;			// Called when an ISO OUT is completed(?) for an associated endpoint
	Intfc_Gen_Cbk_t			SOF;			// Called when a SOF signal is received
	Intfc_Get_Cbk_t			Descriptor;		// Called to return the interface descriptor
	Intfc_Get_Cbk_t			String;			// Called to return the string descriptor of the interface
}wgusb_intfc_reactions_t;

struct _wgusb_cnfg_handle_t;				// Forward declaration of coniguration handle structure

typedef struct _wgusb_intfc_handle_t{
	// External
	wgusb_intfc_desc_t*			descSettings;
	wgusb_intfc_reactions_t* 	preactions;		// Pointer to a structure containing the reaction function pointers
	void*						pData;

	// Internal
	struct _wgusb_cnfg_handle_t*	pCnfg;		// Parent configuration for this interface
	uint8_t number;								// Number of interface (assigned when linked to configuration)
	uint8_t currentAlternate;					// Current alternate setting (assigned by USB host from available options)
	uint8_t numEndpoints;						// How many endpoints are associated with the interface (set when linking in endpoints)
	uint8_t strIndex;							// Index used to get the string descriptor for the interface. Set when linked into a configuration

	wgusb_ep_handle_t* pEPs[WGUSB_MAX_ENDPTS_TOTAL]; 	// Array of pointers to the endpoint handles used for this interface
}wgusb_intfc_handle_t;







////////////////////////////////////
// 		Configuration Handle
////////////////////////////////////

typedef struct{
	const char* 	name;       /*!< String description of the configuration - null terminated */
    union {
        struct {
            uint8_t 				: 5;
            uint8_t remoteWakeup 	: 1; 	/*!< Remote wakeup support */
            uint8_t selfPowered 	: 1;  	/*!< Self powered vs USB bus powered */
            uint8_t 				: 1;
        };
        uint8_t b;
    }attributes;	
	uint16_t 		halfMaxMa; 	/*!< Maximum current demand (2 .. 500 mA) in 2 mA units*/
}wgusb_cnfg_desc_t;				/* A structure that the user can configure all on their own to describe the capabilities of the interface */

typedef wgusb_status_t 	(*Cnfg_Get_Cbk_t)		(struct _wgusb_cnfg_handle_t* pcnfg, uint8_t* pdata, uint16_t* len);

typedef struct{
	// Cnfg_Get_Cbk_t			Descriptor;		// Called to return the configuration descriptor
	// Cnfg_Get_Cbk_t			String;			// Called to return the string descriptor of the interface
	Cnfg_Get_Cbk_t		HSDescriptor;
	Cnfg_Get_Cbk_t		FSDescriptor;
	Cnfg_Get_Cbk_t		OSDescriptor;
	Cnfg_Get_Cbk_t		DeviceQualifierDescriptor;
}wgusb_cnfg_reactions_t;

struct _wgusb_device_handle_t;		// Forward declaration of device handle structure

typedef struct{
	// External
	wgusb_cnfg_desc_t* descSettings;	
	wgusb_cnfg_reactions_t* preactions;

	// Internal
	uint8_t 		numIntfc;	/* The number of interfaces used in the configuration - set by linking in interfaces */
	uint8_t 		confValue;	/* The system-assigned interface number used to select this configuration (set by the system when linked to a device) */
    uint8_t 		strIndex;	// Index used to access the string descriptor of the configuration. Set by the system when the config is linked into the device
    uint8_t 		lpmEnabled;	/* LPM capable? */

    wgusb_intfc_handle_t* pIntfcs[WGUSB_MAX_INTFC];	// Array of pointers to the interface handles
	// void* 			user;		/* Any necessary user data */
}wgusb_cnfg_handle_t;

// typedef	wgusb_status_t	(*Intfc_GetHSConfigDescriptor_t)(uint8_t* pdata, uint16_t *length);
// typedef	wgusb_status_t	(*Intfc_GetFSConfigDescriptor_t)(uint8_t* pdata, uint16_t *length);
// typedef	wgusb_status_t	(*Intfc_GetOtherSpeedConfigDescriptor_t)(uint8_t* pdata, uint16_t *length);
// typedef	wgusb_status_t	(*Intfc_GetDeviceQualifierDescriptor_t)(uint8_t* pdata, uint16_t *length);











////////////////////////////////////
// 		Device Handle
////////////////////////////////////
typedef struct{
    struct {
        const char *name;       /*!< Vendor name */
        uint16_t ID;            /*!< Vendor IDentifier (VID) */
    }Vendor;                    /*!< Vendor properties */

    struct {
        const char *name;       /*!< Product name */
        uint16_t ID;            /*!< Product IDentifier (PID) */
        union {
            struct {
                uint8_t Minor;  /*!< Minor version */
                uint8_t Major;  /*!< Major version */
            };
            uint16_t bcd;       /*!< BCD-coded version number */
        }Version;               /*!< Version number */
        const char* sn;			// Serial number null terminated string
    }Product;                   /*!< Product properties */

	struct {					// A USB class as set for the whole device. Set all to zero to use interface classes
		uint8_t devClass;		
		uint8_t devSubClass;
		uint8_t devProtocol;
	}devClass;

}wgusb_device_desc_t;



typedef struct _wgusb_device_handle_t{
	// External 
	wgusb_device_desc_t* descSettings;

	// Internal
	uint8_t bcdUSB; 			// USB specification compliance - set by the system
	uint8_t ep0mps;				// Max packet size for ep0 - set by the system using special ep0 link function
	uint8_t manufacturerIndex;	// Index of string descriptor for vendor. Set by the system when starting the device
	uint8_t productIndex; 		// Index of string descriptor for the product. Set by the system when starting the device
	uint8_t serialIndex;		// Index of string descriptor of the serial number. Set by the system when starting the device
	uint8_t numConfg;			// The number of configurations. Incremented when linking in configurations

	wgusb_strng_handle_t* 	pbaseStringHandle;		// The head of the linked list of string handles for this device
	wgusb_ep_handle_t*		pEP0;					// Pointer to the endpoint 0 structure
	wgusb_cnfg_handle_t*	pCnfgs[WGUSB_MAX_CONF];	// Array of pointers to the configuration handles that may be in use


	wgusb_usb_std_feat_e 	features;				// Lists active features (tet mode, remote wakeup)
	wgusb_usb_link_state_e	linkState;				// Link Power Management state 
	wgusb_dev_state_e		state;					// Device state
	uint8_t 				addr;					// Device address, set by the system through the host's requests
	wgusb_usb_speed_e		speed;					// Device speed
	uint8_t 				config;					// Which config index is currently selected

	wgusb_usb_setup_request_t setupRequest;			// Storage of the latest setup request received on EP0

	void*					pDriver;




	// Ideas from HAL library:

  // uint8_t                 id;
  // uint32_t                dev_config;
  // uint32_t                dev_default_config;
  // uint32_t                dev_config_status;
  // USBD_SpeedTypeDef       dev_speed;
  // USBD_EndpointTypeDef    ep_in[15];
  // USBD_EndpointTypeDef    ep_out[15];
  // uint32_t                ep0_state;
  // uint32_t                ep0_data_len;
  // uint8_t                 dev_state;
  // uint8_t                 dev_old_state;
  // uint8_t                 dev_address;
  // uint8_t                 dev_connection_status;
  // uint8_t                 dev_test_mode;
  // uint32_t                dev_remote_wakeup;

  // USBD_SetupReqTypedef    request;
  // USBD_DescriptorsTypeDef *pDesc;




  // Ideas from the GitHub guy:

      // const USBD_DescriptionType *Desc;       /*!< Reference of the device description */
    // USB_SetupRequestType Setup;             /*!< Setup request is stored */

// #ifdef USBD_PD_DEV_FIELDS
//     USBD_PD_DEV_FIELDS;                     /*!< Peripheral Driver specific device context */
// #endif
    // USB_LinkStateType LinkState;            /*!< USB link power state (handled by PD) */

    // USB_SpeedType Speed;                    /*!< USB current speed */
    // union {
    //     struct {
    //         uint16_t SelfPowered : 1;       /*!< Self powered vs USB bus powered */
    //         uint16_t RemoteWakeup : 1;      /*!< Remote wakeup enabled */
    //         uint16_t : 14;
    //     };
    //     uint16_t w;
    // }Features;                              /*!< Device feature configuration */
    // uint8_t ConfigSelector;                 /*!< Device active configuration index */

    // uint8_t IfCount;                                /*!< Number of device interfaces */
    // USBD_IfHandleType* IF[USBD_MAX_IF_COUNT];       /*!< Device interface references */

    // struct {
    //     USBD_EpHandleType IN [USBD_MAX_EP_COUNT];   /*!< IN endpoint status */
    //     USBD_EpHandleType OUT[USBD_MAX_EP_COUNT];   /*!< OUT endpoint status */
    // }EP;                                            /*!< Endpoint management */

    // uint8_t CtrlData[USBD_EP0_BUFFER_SIZE] __align(USBD_DATA_ALIGNMENT); /*!< Control EP buffer for common use */
}wgusb_device_handle_t;

// typedef	wgusb_status_t	(*Intfc_GetUsrStrDescriptor_t)(struct _wgusb_device_handle_t *pdev ,uint8_t index, uint8_t* pdata, uint16_t *length);



















#endif /* __WGUSB_DEF_H_ */
