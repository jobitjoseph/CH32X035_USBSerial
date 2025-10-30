#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "wch_usbcdc_usb.h"
#include "wch_usbcdc_config.h"
#include "wch_usbfs_compat.h"

// Prefer local header (project-proven) for USBFS macros; fallback to core header
#if __has_include("ch32x035.h")
#include "ch32x035.h"
#else
#include <ch32x035.h>
#endif

// Endpoint sizes
#define WCH_USBCDC_EP0_SIZE 8
#define WCH_USBCDC_EP1_SIZE 8
#define WCH_USBCDC_EP2_SIZE 64

// Exposed endpoint buffers
extern unsigned char wch_usbcdc_EP0_buffer[];
extern unsigned char wch_usbcdc_EP1_buffer[];
extern unsigned char wch_usbcdc_EP2_buffer[];

// Descriptors
extern const USB_DEV_DESCR wch_usbcdc_DevDescr;
typedef struct __attribute__((packed)) {
  USB_CFG_DESCR config;
  USB_IAD_DESCR association;
  USB_ITF_DESCR interface0;
  unsigned char functional[19];
  USB_ENDP_DESCR ep1IN;
  USB_ITF_DESCR interface1;
  USB_ENDP_DESCR ep2OUT;
  USB_ENDP_DESCR ep2IN;
} WCH_USBCDC_CFG_DESCR_CDC;
extern const WCH_USBCDC_CFG_DESCR_CDC wch_usbcdc_CfgDescr;

// String descriptors (from descr.c)
extern const USB_STR_DESCR wch_usbcdc_LangDescr;

// Variable-sized string descriptors - actual sizes defined by config macros
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[WCH_USBCDC_MANUF_LEN];
} WCH_USBCDC_MANUF_DESCR;

typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[WCH_USBCDC_PROD_LEN];
} WCH_USBCDC_PROD_DESCR;

typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[WCH_USBCDC_SERIAL_LEN];
} WCH_USBCDC_SER_DESCR;

typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[WCH_USBCDC_INTERF_LEN];
} WCH_USBCDC_INTERF_DESCR;

extern WCH_USBCDC_MANUF_DESCR wch_usbcdc_ManufDescr;
extern WCH_USBCDC_PROD_DESCR wch_usbcdc_ProdDescr;
extern WCH_USBCDC_SER_DESCR wch_usbcdc_SerDescr;
extern WCH_USBCDC_INTERF_DESCR wch_usbcdc_InterfDescr;

// USB handler state
extern volatile uint8_t  USB_SetupReq, USB_SetupTyp, USB_Config, USB_Addr, USB_ENUM_OK;
extern volatile uint16_t USB_SetupLen;
extern const uint8_t*    USB_pDescr;

// CDC hooks
void CDC_EP_init(void);
uint8_t CDC_control(void);
void CDC_EP0_OUT(void);
void CDC_EP2_IN(void);
void CDC_EP2_OUT(void);

// Core helpers
void USB_init(void);
void USB_EP0_copyDescr(uint8_t len);

// Setup buffer accessor
typedef struct __attribute__((packed)) {
    uint8_t  bRequestType;
    uint8_t  bRequest;
    uint8_t  wValueL;
    uint8_t  wValueH;
    uint8_t  wIndexL;
    uint8_t  wIndexH;
    uint8_t  wLengthL;
    uint8_t  wLengthH;
} USB_SETUP_REQ, *PUSB_SETUP_REQ;

#define USB_SetupBuf ((PUSB_SETUP_REQ)wch_usbcdc_EP0_buffer)

// Functions to generate all dynamic string descriptors
void generate_unique_serial_descriptor(void);
void generate_manufacturer_descriptor(void);
void generate_product_descriptor(void);
void generate_interface_descriptor(void);
void generate_all_string_descriptors(void);

// Helper functions
void uint32_to_hex_string(uint32_t value, char* output);
void string_to_utf16le_descriptor(const char* source, uint16_t* dest, int max_len);

#ifdef __cplusplus
}
#endif

