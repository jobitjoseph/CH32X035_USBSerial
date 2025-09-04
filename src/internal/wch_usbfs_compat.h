#pragma once

// Compatibility defines for USBFS/AFIO macros if missing from the core header

#ifndef USBFS_UC_DMA_EN
#define USBFS_UC_DMA_EN                         ((uint8_t)0x01)
#define USBFS_UC_CLR_ALL                        ((uint8_t)0x02)
#define USBFS_UC_RESET_SIE                      ((uint8_t)0x04)
#define USBFS_UC_INT_BUSY                       ((uint8_t)0x08)
#define USBFS_UC_SYS_CTRL_MASK                  ((uint8_t)0x30)
#define USBFS_UC_DEV_PU_EN                      ((uint8_t)0x20)
#define USBFS_UC_LOW_SPEED                      ((uint8_t)0x40)
#define USBFS_UC_HOST_MODE                      ((uint8_t)0x80)
#endif

#ifndef USBFS_UIE_TRANSFER
#define USBFS_UIE_DETECT                        ((uint8_t)0x01)
#define USBFS_UIE_BUS_RST                       ((uint8_t)0x01)
#define USBFS_UIE_TRANSFER                      ((uint8_t)0x02)
#define USBFS_UIE_SUSPEND                       ((uint8_t)0x04)
#define USBFS_UIE_HST_SOF                       ((uint8_t)0x08)
#define USBFS_UIE_FIFO_OV                       ((uint8_t)0x10)
#define USBFS_UIE_DEV_NAK                       ((uint8_t)0x40)
#define USBFS_UIE_DEV_SOF                       ((uint8_t)0x80)
#endif

#ifndef USBFS_UDA_GP_BIT
#define USBFS_USB_ADDR_MASK                     ((uint8_t)0x7f)
#define USBFS_UDA_GP_BIT                        ((uint8_t)0x80)
#endif

#ifndef USBFS_UIF_TRANSFER
#define USBFS_UIF_DETECT                        ((uint8_t)0x01)
#define USBFS_UIF_BUS_RST                       ((uint8_t)0x01)
#define USBFS_UIF_TRANSFER                      ((uint8_t)0x02)
#define USBFS_UIF_SUSPEND                       ((uint8_t)0x04)
#define USBFS_U_SIE_FREE                        ((uint8_t)0x20)
#define USBFS_U_TOG_OK                          ((uint8_t)0x40)
#endif

#ifndef USBFS_UIS_TOKEN_SETUP
#define USBFS_UIS_H_RES_MASK                    ((uint8_t)0x0f)
#define USBFS_UIS_ENDP_MASK                     ((uint8_t)0x0f)
#define USBFS_UIS_TOKEN_MASK                    ((uint8_t)0x30)
#define USBFS_UIS_TOKEN_OUT                     ((uint8_t)0x00)
#define USBFS_UIS_TOKEN_SOF                     ((uint8_t)0x10)
#define USBFS_UIS_TOKEN_IN                      ((uint8_t)0x20)
#define USBFS_UIS_TOKEN_SETUP                   ((uint8_t)0x30)
#endif

#ifndef USBFS_UD_PD_DIS
#define USBFS_UD_PORT_EN                        ((uint8_t)0x01)
#define USBFS_UD_GP_BIT                         ((uint8_t)0x02)
#define USBFS_UD_LOW_SPEED                      ((uint8_t)0x04)
#define USBFS_UD_DM_PIN                         ((uint8_t)0x10)
#define USBFS_UD_DP_PIN                         ((uint8_t)0x20)
#define USBFS_UD_PD_DIS                         ((uint8_t)0x80)
#endif

#ifndef USBFS_UEP_T_RES_MASK
#define USBFS_UEP_T_RES_MASK                    ((uint16_t)0x0003)
#define USBFS_UEP_T_RES_ACK                     ((uint16_t)0x0000)
#define USBFS_UEP_T_RES_TOUT                    ((uint16_t)0x0001)
#define USBFS_UEP_T_RES_NAK                     ((uint16_t)0x0002)
#define USBFS_UEP_T_RES_STALL                   ((uint16_t)0x0003)
#define USBFS_UEP_R_RES_MASK                    ((uint16_t)0x000c)
#define USBFS_UEP_R_RES_ACK                     ((uint16_t)0x0000)
#define USBFS_UEP_R_RES_TOUT                    ((uint16_t)0x0004)
#define USBFS_UEP_R_RES_NAK                     ((uint16_t)0x0008)
#define USBFS_UEP_R_RES_STALL                   ((uint16_t)0x000c)
#define USBFS_UEP_AUTO_TOG                      ((uint16_t)0x0010)
#define USBFS_UEP_T_TOG                         ((uint16_t)0x0040)
#define USBFS_UEP_R_TOG                         ((uint16_t)0x0080)
#endif

#ifndef USBFS_UEP1_TX_EN
#define USBFS_UEP1_TX_EN                        ((uint8_t)0x40)
#define USBFS_UEP1_RX_EN                        ((uint8_t)0x80)
#define USBFS_UEP2_TX_EN                        ((uint8_t)0x04)
#define USBFS_UEP2_RX_EN                        ((uint8_t)0x08)
#endif

// AFIO compatibility for USB pull-up config
#ifndef AFIO_CTLR_UDM_PUE
#define AFIO_CTLR_UDM_PUE                        ((uint32_t)0x00000003)
#define AFIO_CTLR_UDM_PUE_1K5                    ((uint32_t)0x00000003)
#define AFIO_CTLR_UDM_PUE_10K                    ((uint32_t)0x00000002)
#endif

#ifndef AFIO_CTLR_UDP_PUE
#define AFIO_CTLR_UDP_PUE                        ((uint32_t)0x0000000c)
#define AFIO_CTLR_UDP_PUE_1K5                    ((uint32_t)0x0000000c)
#define AFIO_CTLR_UDP_PUE_10K                    ((uint32_t)0x00000008)
#endif

#ifndef AFIO_CTLR_USB_PHY_V33
#define AFIO_CTLR_USB_PHY_V33                    ((uint32_t)0x00000040)
#endif

#ifndef AFIO_CTLR_USB_IOEN
#define AFIO_CTLR_USB_IOEN                       ((uint32_t)0x00000080)
#endif

