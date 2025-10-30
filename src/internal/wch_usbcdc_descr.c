#include "wch_usbcdc_usb.h"
#include "wch_usbcdc_config.h"
#include "wch_usbcdc_internal.h"

#define EP0_SIZE 8
#define EP1_SIZE 8
#define EP2_SIZE 64

__attribute__((aligned(4))) unsigned char wch_usbcdc_EP0_buffer[(EP0_SIZE+2<64?EP0_SIZE+2:64)];
__attribute__((aligned(4))) unsigned char wch_usbcdc_EP1_buffer[(EP1_SIZE+2<64?EP1_SIZE+2:64)];
__attribute__((aligned(4))) unsigned char wch_usbcdc_EP2_buffer[(EP2_SIZE+2<64?EP2_SIZE+2:64) + 64];

const USB_DEV_DESCR wch_usbcdc_DevDescr = {
  .bLength            = sizeof(USB_DEV_DESCR),
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,
  .bcdUSB             = 0x0200,
  .bDeviceClass       = 0,
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = EP0_SIZE,
  .idVendor           = WCH_USBCDC_VENDOR_ID,
  .idProduct          = WCH_USBCDC_PRODUCT_ID,
  .bcdDevice          = WCH_USBCDC_DEVICE_VERSION,
  .iManufacturer      = 1,
  .iProduct           = 2,
  .iSerialNumber      = 3,
  .bNumConfigurations = 1
};

const WCH_USBCDC_CFG_DESCR_CDC wch_usbcdc_CfgDescr = {
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,
    .wTotalLength       = sizeof(WCH_USBCDC_CFG_DESCR_CDC),
    .bNumInterfaces     = 2,
    .bConfigurationValue= 1,
    .iConfiguration     = 0,
    .bmAttributes       = 0x80,
    .MaxPower           = WCH_USBCDC_MAX_POWER_mA / 2
  },
  .association = {
    .bLength            = sizeof(USB_IAD_DESCR),
    .bDescriptorType    = 0x0B,
    .bFirstInterface    = 0,
    .bInterfaceCount    = 2,
    .bFunctionClass     = USB_DEV_CLASS_COMM,
    .bFunctionSubClass  = 2,
    .bFunctionProtocol  = 1,
    .iFunction          = 4
  },
  .interface0 = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 0,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 1,
    .bInterfaceClass    = USB_DEV_CLASS_COMM,
    .bInterfaceSubClass = 2,
    .bInterfaceProtocol = 1,
    .iInterface         = 4
  },
  .functional = { 0x05,0x24,0x00,0x10,0x01, 0x05,0x24,0x01,0x00,0x00, 0x04,0x24,0x02,0x02, 0x05,0x24,0x06,0x00,0x01 },
  .ep1IN = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_IN,
    .bmAttributes       = USB_ENDP_TYPE_INTER,
    .wMaxPacketSize     = EP1_SIZE,
    .bInterval          = 1
  },
  .interface1 = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 1,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 2,
    .bInterfaceClass    = USB_DEV_CLASS_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface         = 4
  },
  .ep2OUT = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_OUT,
    .bmAttributes       = USB_ENDP_TYPE_BULK,
    .wMaxPacketSize     = EP2_SIZE,
    .bInterval          = 0
  },
  .ep2IN = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_IN,
    .bmAttributes       = USB_ENDP_TYPE_BULK,
    .wMaxPacketSize     = EP2_SIZE,
    .bInterval          = 0
  }
};

// Language descriptor
const USB_STR_DESCR wch_usbcdc_LangDescr = {
  .bLength         = 4,
  .bDescriptorType = USB_DESCR_TYP_STRING,
  .bString         = { WCH_USBCDC_LANGUAGE }
};

// Dynamic string descriptors - automatically sized based on config
WCH_USBCDC_MANUF_DESCR wch_usbcdc_ManufDescr = {
  .bLength = (uint8_t)(2 + 2 * WCH_USBCDC_MANUF_LEN),
  .bDescriptorType = USB_DESCR_TYP_STRING,
  .bString = {0} // Will be filled at runtime
};

WCH_USBCDC_PROD_DESCR wch_usbcdc_ProdDescr = {
  .bLength = (uint8_t)(2 + 2 * WCH_USBCDC_PROD_LEN),
  .bDescriptorType = USB_DESCR_TYP_STRING,
  .bString = {0} // Will be filled at runtime
};

WCH_USBCDC_SER_DESCR wch_usbcdc_SerDescr = {
  .bLength = (uint8_t)(2 + 2 * WCH_USBCDC_SERIAL_LEN),
  .bDescriptorType = USB_DESCR_TYP_STRING,
  .bString = {0} // Will be filled at runtime
};

WCH_USBCDC_INTERF_DESCR wch_usbcdc_InterfDescr = {
  .bLength = (uint8_t)(2 + 2 * WCH_USBCDC_INTERF_LEN),
  .bDescriptorType = USB_DESCR_TYP_STRING,
  .bString = {0} // Will be filled at runtime
};

// Helper functions
void uint32_to_hex_string(uint32_t value, char* output) {
    const char hex_chars[] = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        output[7-i] = hex_chars[(value >> (i * 4)) & 0xF];
    }
}

void string_to_utf16le_descriptor(const char* source, uint16_t* dest, int max_len) {
    int i = 0;
    while (source[i] != '\0' && i < max_len) {
        dest[i] = (uint16_t)source[i];
        i++;
    }
}

// Dynamic string generation functions
void generate_manufacturer_descriptor(void) {
    string_to_utf16le_descriptor(WCH_USBCDC_MANUF_STR, wch_usbcdc_ManufDescr.bString, WCH_USBCDC_MANUF_LEN);
}

void generate_product_descriptor(void) {
    string_to_utf16le_descriptor(WCH_USBCDC_PROD_STR, wch_usbcdc_ProdDescr.bString, WCH_USBCDC_PROD_LEN);
}

void generate_interface_descriptor(void) {
    string_to_utf16le_descriptor(WCH_USBCDC_INTERF_STR, wch_usbcdc_InterfDescr.bString, WCH_USBCDC_INTERF_LEN);
}

void generate_unique_serial_descriptor(void) {
    // Read the three 32-bit UID registers
    uint32_t uid1 = *(volatile uint32_t*)CH32X035_ESIG_UNIID1;
    uint32_t uid2 = *(volatile uint32_t*)CH32X035_ESIG_UNIID2; 
    uint32_t uid3 = *(volatile uint32_t*)CH32X035_ESIG_UNIID3;
    
    // Create full 24-character hex string from 96-bit UID
    char hex_string[24];
    uint32_to_hex_string(uid3, &hex_string[0]);
    uint32_to_hex_string(uid2, &hex_string[8]);
    uint32_to_hex_string(uid1, &hex_string[16]);
    
    // Store prefix from config in UTF-16LE descriptor
    for (int i = 0; i < WCH_USBCDC_SERIAL_PREFIX_LEN; i++) {
        wch_usbcdc_SerDescr.bString[i] = (uint16_t)WCH_USBCDC_SERIAL_PREFIX[i];
    }
    
    // Store full 24-character hex string in UTF-16LE descriptor  
    for (int i = 0; i < WCH_USBCDC_UID_HEX_CHARS; i++) {
        wch_usbcdc_SerDescr.bString[WCH_USBCDC_SERIAL_PREFIX_LEN + i] = (uint16_t)hex_string[i];
    }
}

// Master function to generate all string descriptors
void generate_all_string_descriptors(void) {
    generate_manufacturer_descriptor();
    generate_product_descriptor();
    generate_interface_descriptor();
    generate_unique_serial_descriptor();
}