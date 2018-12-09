#include "wgusb.h"




/*
	This function should be called once all the desired configurations have been set up and linked to the device
*/
wgusb_status_t WGUSB_Init(wgusb_device_handle_t *pdev)
{
	wgusb_status_t retval = WGUSB_Nominal;

	/* Check whether the USB Host handle is valid */
	if(pdev == NULL){ return WGUSB_Error; }

	/* Set Device initial State */
	pdev->state  = usb_dev_state_default;

	/* Initialize low level driver */
	retval = WGUSB_DRV_Init(pdev);
	if(retval != WGUSB_Nominal){ return retval; }

	return retval;
}

wgusb_status_t WGUSB_DeInit(wgusb_device_handle_t *pdev)
{
	wgusb_status_t retval = WGUSB_Nominal;

	/* Set Default State */
	pdev->state  = usb_dev_state_default;

//	  /* Free Class Resources */
//	  pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);

	/* Stop the low level driver  */
	retval = WGUSB_DRV_Stop(pdev);
	if(retval != WGUSB_Nominal){ return retval; }

	/* Initialize low level driver */
	retval = WGUSB_DRV_DeInit(pdev);
	if(retval != WGUSB_Nominal){ return retval; }

	return retval;
}

wgusb_status_t WGUSB_Start  (wgusb_device_handle_t *pdev)
{

	  /* Start the low level driver  */
	  USBD_LL_Start(pdev);

	  return USBD_OK;
}

wgusb_status_t WGUSB_Stop   (wgusb_device_handle_t *pdev)
{

}


wgusb_status_t WGUSB_AddConfiguration(wgusb_device_handle_t* pdev, wgusb_cnfg_handle_t* pcnfg)
{

}

wgusb_status_t WGUSB_AddInterface(wgusb_cnfg_handle_t* pcnfg, wgusb_intfc_handle_t* pintfc)
{

}

























wgusb_status_t WGUSB_LL_SetupStage(wgusb_device_handle_t* pdev, uint8_t* setupbuff)
{

}

wgusb_status_t WGUSB_LL_DataOutStage(wgusb_device_handle_t* pdev, uint8_t epnum, uint8_t* xferbuff)
{

}

wgusb_status_t WGUSB_LL_DataInStage(wgusb_device_handle_t* pdev, uint8_t epnum, uint8_t* xferbuff)
{

}

wgusb_status_t WGUSB_LL_SOF(wgusb_device_handle_t* pdev)
{

}

wgusb_status_t WGUSB_LL_SetSpeed(wgusb_device_handle_t* pdev, wgusb_usb_speed_e speed)
{

}

wgusb_status_t WGUSB_LL_Reset(wgusb_device_handle_t* pdev)
{

}

wgusb_status_t WGUSB_LL_Suspend(wgusb_device_handle_t* pdev)
{

}

wgusb_status_t WGUSB_LL_Resume(wgusb_device_handle_t* pdev)
{

}

wgusb_status_t WGUSB_LL_IsoOUTIncomplete(wgusb_device_handle_t* pdev, uint8_t epnum)
{

}

wgusb_status_t WGUSB_LL_IsoINIncomplete(wgusb_device_handle_t* pdev, uint8_t epnum)
{

}

wgusb_status_t WGUSB_LL_DevConnected(wgusb_device_handle_t* pdev)
{

}

wgusb_status_t WGUSB_LL_DevDisconnected(wgusb_device_handle_t* pdev)
{

}
