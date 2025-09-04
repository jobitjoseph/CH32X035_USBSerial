#include "wch_usbcdc_internal.h"

typedef struct {
  uint32_t baudrate;
  uint8_t  stopbits;
  uint8_t  parity;
  uint8_t  databits;
} CDC_LINE_CODING_TYPE;

CDC_LINE_CODING_TYPE CDC_lineCoding = { .baudrate = 115200, .stopbits = 0, .parity = 0, .databits = 8 };

volatile uint8_t CDC_controlLineState = 0;
volatile uint8_t CDC_readByteCount = 0;
volatile uint8_t CDC_readPointer   = 0;
volatile uint8_t CDC_writePointer  = 0;
volatile uint8_t CDC_writeBusyFlag = 0;

#define SET_LINE_CODING           0x20
#define GET_LINE_CODING           0x21
#define SET_CONTROL_LINE_STATE    0x22

void CDC_init(void) { USB_init(); }

uint8_t CDC_available(void) { return CDC_readByteCount; }
uint8_t CDC_ready(void) { return !CDC_writeBusyFlag; }

void CDC_flush(void) {
  if(!CDC_writeBusyFlag && CDC_writePointer > 0) {
    USBFSD->UEP2_TX_LEN = CDC_writePointer;
    USBFSD->UEP2_CTRL_H = (USBFSD->UEP2_CTRL_H & ~USBFS_UEP_T_RES_MASK) | USBFS_UEP_T_RES_ACK;
    CDC_writeBusyFlag = 1;
    CDC_writePointer  = 0;
  }
}

void CDC_write(char c) {
  while(CDC_writeBusyFlag);
  wch_usbcdc_EP2_buffer[64 + CDC_writePointer++] = (unsigned char)c;
  // Flush on each byte to improve interactivity
  CDC_flush();
}

char CDC_read(void) {
  char data;
  while(!CDC_readByteCount);
  data = (char)wch_usbcdc_EP2_buffer[CDC_readPointer++];
  if(--CDC_readByteCount == 0)
    USBFSD->UEP2_CTRL_H = (USBFSD->UEP2_CTRL_H & ~USBFS_UEP_R_RES_MASK) | USBFS_UEP_R_RES_ACK;
  return data;
}

void CDC_EP_init(void) {
  USBFSD->UEP1_DMA    = (uint32_t)wch_usbcdc_EP1_buffer;
  USBFSD->UEP2_DMA    = (uint32_t)wch_usbcdc_EP2_buffer;
  USBFSD->UEP4_1_MOD  = USBFS_UEP1_TX_EN;
  USBFSD->UEP2_3_MOD  = USBFS_UEP2_RX_EN | USBFS_UEP2_TX_EN;
  USBFSD->UEP1_CTRL_H = USBFS_UEP_AUTO_TOG | USBFS_UEP_T_RES_NAK;
  USBFSD->UEP2_CTRL_H = USBFS_UEP_AUTO_TOG | USBFS_UEP_R_RES_ACK | USBFS_UEP_T_RES_NAK;
  USBFSD->UEP1_TX_LEN = 0;
  USBFSD->UEP2_TX_LEN = 0;
  CDC_readByteCount   = 0;
  CDC_writeBusyFlag   = 0;
}

uint8_t CDC_control(void) {
  switch(USB_SetupReq) {
    case GET_LINE_CODING:
      USB_pDescr = (const uint8_t*)&CDC_lineCoding;
      USB_EP0_copyDescr(sizeof(CDC_lineCoding));
      if(USB_SetupLen > sizeof(CDC_lineCoding)) USB_SetupLen = sizeof(CDC_lineCoding);
      return (uint8_t)USB_SetupLen;
    case SET_CONTROL_LINE_STATE:
      CDC_controlLineState = wch_usbcdc_EP0_buffer[2];
      return 0;
    case SET_LINE_CODING:
      return 0;
    default:
      return 0xff;
  }
}

void CDC_EP0_OUT(void) {
  uint8_t i, len;
  if(USB_SetupReq == SET_LINE_CODING) {
    len = USBFSD->RX_LEN;
    for(i=0; i<((sizeof(CDC_lineCoding)<=len)?sizeof(CDC_lineCoding):len); i++)
      ((uint8_t*)&CDC_lineCoding)[i] = wch_usbcdc_EP0_buffer[i];
    USB_SetupLen = 0;
  }
  USBFSD->UEP0_CTRL_H = USBFS_UEP_T_TOG | USBFS_UEP_T_RES_ACK | USBFS_UEP_R_RES_ACK;
}

void CDC_EP2_IN(void) {
  CDC_writeBusyFlag = 0;
  USBFSD->UEP2_CTRL_H = (USBFSD->UEP2_CTRL_H & ~USBFS_UEP_T_RES_MASK) | USBFS_UEP_T_RES_NAK;
}

void CDC_EP2_OUT(void) {
  if((USBFSD->INT_FG & USBFS_U_TOG_OK) && USBFSD->RX_LEN) {
    USBFSD->UEP2_CTRL_H = (USBFSD->UEP2_CTRL_H & ~USBFS_UEP_R_RES_MASK) | USBFS_UEP_R_RES_NAK;
    CDC_readByteCount   = USBFSD->RX_LEN;
    CDC_readPointer     = 0;
  }
}

