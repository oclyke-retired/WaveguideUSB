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
	wgusb_status_t retval = WGUSB_Nominal;

	/* Start the low level driver  */
	retval = WGUSB_DRV_Start(pdev);

	return retval;
}

wgusb_status_t WGUSB_Stop   (wgusb_device_handle_t *pdev)
{
	wgusb_status_t retval = WGUSB_Nominal;

//	/* Free Class Resources */
//	pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);

	/* Stop the low level driver  */
	retval = WGUSB_DRV_Stop(pdev);

	return retval;
}


wgusb_status_t WGUSB_AddConfiguration(wgusb_device_handle_t* pdev, wgusb_cnfg_handle_t* pcnfg)
{

}

wgusb_status_t WGUSB_AddInterface(wgusb_cnfg_handle_t* pcnfg, wgusb_intfc_handle_t* pintfc)
{

}

























wgusb_status_t WGUSB_LL_SetupStage(wgusb_device_handle_t* pdev, uint8_t* setupbuff)
{
	wgusb_status_t retval = WGUSB_Nominal;

	retval = WGUSB_ParseSetupRequest(&pdev->setupRequest, setupbuff);
	if(retval != WGUSB_Nominal){ return retval; }

	pdev->pEP0->state = usb_ep_state_setup;

	pdev->pEP0->transfer.length = pdev->setupRequest.length; // I'm not sure about this line... It is setting the length of the transfer in ep0 but not resetting the progress or pointer to the data. maybe this should be done later when needed?

	switch (pdev->setupRequest.request_type.recipient)
	{
	case usb_req_recipient_device:
		retval = WGUSB_StdDevReq (pdev, &pdev->setupRequest);
		break;

	case usb_req_recipient_interface:
		retval = WGUSB_StdItfReq(pdev, &pdev->setupRequest);
		break;

	case usb_req_recipient_endpoint:
		retval = WGUSB_StdEPReq(pdev, &pdev->setupRequest);
		break;

	case usb_req_recipient_other:
		// Other recipients not yet supported, continue on to default case

	default:
		{
			wgusb_usb_ep_addr_t ep;
			ep.b = 0x00;
			if(pdev->setupRequest.request_type.b & 0x80U)
			{
				ep.b = 0x80;
			}

			retval = WGUSB_DRV_StallEP(pdev, ep);	// Stall endpoint zero (IN or OUT)
		}
		break;
	}

	return retval;
}

wgusb_status_t WGUSB_LL_DataOutStage(wgusb_device_handle_t* pdev, uint8_t epnum, uint8_t* xferbuff)
{
	wgusb_status_t retval = WGUSB_Nominal;

	wgusb_ep_handle_t    *pep;

	if(epnum == 0U)
	{
		pep = pdev->pEP0;

		if ( pep->state == usb_ep_state_data_out)
		{
		  if(pep->transfer.remaining > pep->descSettings.mps)
		  {
			pep->transfer.remaining -=  pep->descSettings.mps;

			retval = WGUSB_CtlContinueRx(pdev, xferbuff, (uint16_t)MIN(pep->transfer.remaining, pep->descSettings.mps));
		  }
		  else
		  {
			if((pdev->pClass->EP0_RxReady != NULL)&&
			   (pdev->dev_state == USBD_STATE_CONFIGURED))
			{
			  pdev->pClass->EP0_RxReady(pdev);
			}
			USBD_CtlSendStatus(pdev);
		  }
		}
		else
		{
		  if (pdev->ep0_state == USBD_EP0_STATUS_OUT)
		  {
			USBD_LL_StallEP(pdev, 0U);
		  }
		}
	}
	else if((pdev->pClass->DataOut != NULL) &&
		  (pdev->dev_state == USBD_STATE_CONFIGURED))
	{
	pdev->pClass->DataOut(pdev, epnum);
	}
	else
	{
	/* should never be in this condition */
	return USBD_FAIL;
	}

	return USBD_OK;
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
