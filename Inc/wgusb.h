/*

This is the main header file for Waveguide USB

Author: Owen Lyke

*/

#ifndef __WGUSB_H_
#define __WGUSB_H_

#include <stdlib.h>
#include "wgusb_conf.h"
#include "wgusb_def.h"


// User-facing API:
wgusb_status_t WGUSB_Init(wgusb_device_handle_t *pdev);		// Setup but don't start yet
wgusb_status_t WGUSB_DeInit(wgusb_device_handle_t *pdev);	// Un-setup
wgusb_status_t WGUSB_Start  (wgusb_device_handle_t *pdev);	// Start the device
wgusb_status_t WGUSB_Stop   (wgusb_device_handle_t *pdev);	// Stop the device

wgusb_status_t WGUSB_AddConfiguration(wgusb_device_handle_t* pdev, wgusb_cnfg_handle_t* pcnfg);	// Add a configuration to a device
wgusb_status_t WGUSB_AddInterface(wgusb_cnfg_handle_t* pcnfg, wgusb_intfc_handle_t* pintfc);	// Add an interface to a configuration






// Callbacks that can handle events from the peripheral driver (Consider using these in your driver code, like interrupt handlers)
wgusb_status_t WGUSB_LL_SetupStage(wgusb_device_handle_t* pdev, uint8_t* setupbuff);
wgusb_status_t WGUSB_LL_DataOutStage(wgusb_device_handle_t* pdev, uint8_t epnum, uint8_t* xferbuff);
wgusb_status_t WGUSB_LL_DataInStage(wgusb_device_handle_t* pdev, uint8_t epnum, uint8_t* xferbuff);
wgusb_status_t WGUSB_LL_SOF(wgusb_device_handle_t* pdev);
wgusb_status_t WGUSB_LL_SetSpeed(wgusb_device_handle_t* pdev, wgusb_usb_speed_e speed);
wgusb_status_t WGUSB_LL_Reset(wgusb_device_handle_t* pdev);
wgusb_status_t WGUSB_LL_Suspend(wgusb_device_handle_t* pdev);
wgusb_status_t WGUSB_LL_Resume(wgusb_device_handle_t* pdev);
wgusb_status_t WGUSB_LL_IsoOUTIncomplete(wgusb_device_handle_t* pdev, uint8_t epnum);
wgusb_status_t WGUSB_LL_IsoINIncomplete(wgusb_device_handle_t* pdev, uint8_t epnum);
wgusb_status_t WGUSB_LL_DevConnected(wgusb_device_handle_t* pdev);
wgusb_status_t WGUSB_LL_DevDisconnected(wgusb_device_handle_t* pdev);


// Driver specific functions (You implement these for your particular driver)
wgusb_status_t 	WGUSB_DRV_Init(wgusb_device_handle_t* pdev);
wgusb_status_t 	WGUSB_DRV_DeInit(wgusb_device_handle_t* pdev);
wgusb_status_t 	WGUSB_DRV_Start(wgusb_device_handle_t* pdev);
wgusb_status_t 	WGUSB_DRV_Stop(wgusb_device_handle_t* pdev);
wgusb_status_t 	WGUSB_DRV_OpenEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr, wgusb_usb_endpt_type_e ep_type, wgusb_usb_ep_mps_e ep_mps);
wgusb_status_t 	WGUSB_DRV_CloseEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
wgusb_status_t 	WGUSB_DRV_FlushEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
wgusb_status_t 	WGUSB_DRV_StallEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
wgusb_status_t 	WGUSB_DRV_ClearStallEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
uint8_t 	   	WGUSB_DRV_IsStallEP(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
wgusb_status_t 	WGUSB_DRV_SetUSBAddress(wgusb_device_handle_t* pdev, uint8_t dev_addr);
wgusb_status_t 	WGUSB_DRV_Transmit(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr, uint8_t *pbuf, uint16_t size);
wgusb_status_t 	WGUSB_DRV_PrepareReceive(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr, uint8_t *pbuf, uint16_t size);
uint32_t 		WGUSB_DRV_GetRxDataSize(wgusb_device_handle_t* pdev, wgusb_usb_ep_addr_t ep_addr);
void 			WGUSB_DRV_Delay(uint32_t Delay);


#endif /* __WGUSB_H_ */
