# Waveguide USB Library

This is a hardware independent USB library with easy-to-configure interfaces that supports composite device creation

## Principles

1. Create a device handle. The device handle contains your interfaces and your driver configuration
2. Create interfaces and add them to the device handle
3. Start the USB device - this in turns calls initialization functions for the driver and interfaces
4. At your option choose to stop the USB device