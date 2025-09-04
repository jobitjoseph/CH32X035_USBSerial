// Minimal configuration for CH32X035 USB CDC library
#pragma once

// CH32X035 Electronic Signature (ESIG) register addresses
#define CH32X035_ESIG_UNIID1    0x1FFFF7E8  // UID bits [31:0]
#define CH32X035_ESIG_UNIID2    0x1FFFF7EC  // UID bits [63:32]
#define CH32X035_ESIG_UNIID3    0x1FFFF7F0  // UID bits [95:64]

// USB String Configuration - ONLY CHANGE THESE STRINGS
#define WCH_USBCDC_MANUF_STR            "Jobit Joseph"      // Change this
#define WCH_USBCDC_PROD_STR             "CH32X035-CDC"      // Change this  
#define WCH_USBCDC_INTERF_STR           "CDC-Serial"        // Change this
#define WCH_USBCDC_SERIAL_PREFIX        "JJ"                // Change this

// Helper macro to calculate string length at compile time
#define STR_LEN(s) (sizeof(s) - 1)

// Automatic length calculations - DO NOT MODIFY
#define WCH_USBCDC_MANUF_LEN            STR_LEN(WCH_USBCDC_MANUF_STR)
#define WCH_USBCDC_PROD_LEN             STR_LEN(WCH_USBCDC_PROD_STR)  
#define WCH_USBCDC_INTERF_LEN           STR_LEN(WCH_USBCDC_INTERF_STR)
#define WCH_USBCDC_SERIAL_PREFIX_LEN    STR_LEN(WCH_USBCDC_SERIAL_PREFIX)
#define WCH_USBCDC_UID_HEX_CHARS        24  // 96-bit UID = 24 hex characters
#define WCH_USBCDC_SERIAL_LEN           (WCH_USBCDC_SERIAL_PREFIX_LEN + WCH_USBCDC_UID_HEX_CHARS)

// User-provided VID/PID and other settings
#define WCH_USBCDC_VENDOR_ID        0x16C0
#define WCH_USBCDC_PRODUCT_ID       0x27DD
#define WCH_USBCDC_DEVICE_VERSION   0x0100
#define WCH_USBCDC_LANGUAGE         0x0409
#define WCH_USBCDC_MAX_POWER_mA     500
/*
VOTI (0x16C0) - Van Ooijen Technische Informatica
0x16C0:0x0483  // CDC ACM alternative
0x16C0:0x27D8  // CDC ACM 
0x16C0:0x0482  // CDC ACM

OpenMoko (0x1D50) - Free for open-source projects
0x1D50:0x607D  // CDC ACM
0x1D50:0x6028  // CDC ACM
0x1D50:0x602B  // CDC ACM composite
*/
// Default CDC FIFO size; can be overridden at runtime
#ifndef WCH_USBCDC_DEFAULT_FIFO_SIZE
#define WCH_USBCDC_DEFAULT_FIFO_SIZE 128
#endif