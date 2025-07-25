/********************************************************************************************************
 * @file    CDCClassCommon.h
 *
 * @brief   This is the header file for CDCClassCommon
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

/* Includes: */
#include "types.h"
#include "bit.h"
#include "StdDescriptors.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


/* Macros: */
/** \name Virtual Control Line Masks */
//@{
/** Mask for the DTR handshake line for use with the \ref CDC_REQ_SetControlLineState class-specific request
 *  from the host, to indicate that the DTR line state should be high.
 */
#define CDC_CONTROL_LINE_OUT_DTR                BIT(0)

/** Mask for the RTS handshake line for use with the \ref CDC_REQ_SetControlLineState class-specific request
 *  from the host, to indicate that the RTS line state should be high.
 */
#define CDC_CONTROL_LINE_OUT_RTS                BIT(1)

/** Mask for the DCD handshake line for use with the \ref CDC_NOTIF_SerialState class-specific notification
 *  from the device to the host, to indicate that the DCD line state is currently high.
 */
#define CDC_CONTROL_LINE_IN_DCD                 BIT(0)

/** Mask for the DSR handshake line for use with the \ref CDC_NOTIF_SerialState class-specific notification
 *  from the device to the host, to indicate that the DSR line state is currently high.
 */
#define CDC_CONTROL_LINE_IN_DSR                 BIT(1)

/** Mask for the BREAK handshake line for use with the \ref CDC_NOTIF_SerialState class-specific notification
 *  from the device to the host, to indicate that the BREAK line state is currently high.
 */
#define CDC_CONTROL_LINE_IN_BREAK               BIT(2)

/** Mask for the RING handshake line for use with the \ref CDC_NOTIF_SerialState class-specific notification
 *  from the device to the host, to indicate that the RING line state is currently high.
 */
#define CDC_CONTROL_LINE_IN_RING                BIT(3)

/** Mask for use with the \ref CDC_NOTIF_SerialState class-specific notification from the device to the host,
 *  to indicate that a framing error has occurred on the virtual serial port.
 */
#define CDC_CONTROL_LINE_IN_FRAMEERROR          BIT(4)

/** Mask for use with the \ref CDC_NOTIF_SerialState class-specific notification from the device to the host,
 *  to indicate that a parity error has occurred on the virtual serial port.
 */
#define CDC_CONTROL_LINE_IN_PARITYERROR         BIT(5)

/** Mask for use with the \ref CDC_NOTIF_SerialState class-specific notification from the device to the host,
 *  to indicate that a data overrun error has occurred on the virtual serial port.
 */
#define CDC_CONTROL_LINE_IN_OVERRUNERROR        BIT(6)
//@}

/** Macro to define a CDC class-specific functional descriptor. CDC functional descriptors have a
 *  uniform structure but variable sized data payloads, thus cannot be represented accurately by
 *  a single \c typedef \c struct. A macro is used instead so that functional descriptors can be created
 *  easily by specifying the size of the payload. This allows \c sizeof() to work correctly.
 *
 *  \param[in] DataSize  Size in bytes of the CDC functional descriptor's data payload.
 */
#define CDC_FUNCTIONAL_DESCRIPTOR(DataSize)     \
     struct                                     \
     {                                          \
          USB_Descriptor_Header_t Header;       \
          u8 SubType;                           \
          u8 Data[DataSize];                    \
     }

/* Enums: */
/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors relating to the CDC
 *  device class.
 */
enum CDC_Descriptor_ClassSubclassProtocol_t {
    CDC_CSCP_CDCClass               = 0x02, /**< Descriptor Class value indicating that the device or interface
                                             *   belongs to the CDC class.
                                             */
    CDC_CSCP_NoSpecificSubclass     = 0x00, /**< Descriptor Subclass value indicating that the device or interface
                                             *   belongs to no specific subclass of the CDC class.
                                             */
    CDC_CSCP_ACMSubclass            = 0x02, /**< Descriptor Subclass value indicating that the device or interface
                                             *   belongs to the Abstract Control Model CDC subclass.
                                             */
    CDC_CSCP_ATCommandProtocol      = 0x01, /**< Descriptor Protocol value indicating that the device or interface
                                             *   belongs to the AT Command protocol of the CDC class.
                                             */
    CDC_CSCP_NoSpecificProtocol     = 0x00, /**< Descriptor Protocol value indicating that the device or interface
                                             *   belongs to no specific protocol of the CDC class.
                                             */
    CDC_CSCP_VendorSpecificProtocol = 0xFF, /**< Descriptor Protocol value indicating that the device or interface
                                             *   belongs to a vendor-specific protocol of the CDC class.
                                             */
    CDC_CSCP_CDCDataClass           = 0x0A, /**< Descriptor Class value indicating that the device or interface
                                             *   belongs to the CDC Data class.
                                             */
    CDC_CSCP_NoDataSubclass         = 0x00, /**< Descriptor Subclass value indicating that the device or interface
                                             *   belongs to no specific subclass of the CDC data class.
                                             */
    CDC_CSCP_NoDataProtocol         = 0x00, /**< Descriptor Protocol value indicating that the device or interface
                                             *   belongs to no specific protocol of the CDC data class.
                                             */
};

/** Enum for the CDC class specific control requests that can be issued by the USB bus host. */
enum CDC_ClassRequests_t {
    CDC_REQ_SendEncapsulatedCommand = 0x00, /**< CDC class-specific request to send an encapsulated command to the device. */
    CDC_REQ_GetEncapsulatedResponse = 0x01, /**< CDC class-specific request to retrieve an encapsulated command response from the device. */
    CDC_REQ_SetLineEncoding         = 0x20, /**< CDC class-specific request to set the current virtual serial port configuration settings. */
    CDC_REQ_GetLineEncoding         = 0x21, /**< CDC class-specific request to get the current virtual serial port configuration settings. */
    CDC_REQ_SetControlLineState     = 0x22, /**< CDC class-specific request to set the current virtual serial port handshake line states. */
    CDC_REQ_SendBreak               = 0x23, /**< CDC class-specific request to send a break to the receiver via the carrier channel. */
};

/** Enum for the CDC class specific notification requests that can be issued by a CDC device to a host. */
enum CDC_ClassNotifications_t {
    CDC_NOTIF_SerialState = 0x20, /**< Notification type constant for a change in the virtual serial port
                                   *   handshake line states, for use with a \ref USB_Request_Header_t
                                   *   notification structure when sent to the host via the CDC notification
                                   *   endpoint.
                                   */
};

/** Enum for the CDC class specific interface descriptor subtypes. */
enum CDC_DescriptorSubtypes_t {
    CDC_DSUBTYPE_CSInterface_Header           = 0x00, /**< CDC class-specific Header functional descriptor. */
    CDC_DSUBTYPE_CSInterface_CallManagement   = 0x01, /**< CDC class-specific Call Management functional descriptor. */
    CDC_DSUBTYPE_CSInterface_ACM              = 0x02, /**< CDC class-specific Abstract Control Model functional descriptor. */
    CDC_DSUBTYPE_CSInterface_DirectLine       = 0x03, /**< CDC class-specific Direct Line functional descriptor. */
    CDC_DSUBTYPE_CSInterface_TelephoneRinger  = 0x04, /**< CDC class-specific Telephone Ringer functional descriptor. */
    CDC_DSUBTYPE_CSInterface_TelephoneCall    = 0x05, /**< CDC class-specific Telephone Call functional descriptor. */
    CDC_DSUBTYPE_CSInterface_Union            = 0x06, /**< CDC class-specific Union functional descriptor. */
    CDC_DSUBTYPE_CSInterface_CountrySelection = 0x07, /**< CDC class-specific Country Selection functional descriptor. */
    CDC_DSUBTYPE_CSInterface_TelephoneOpModes = 0x08, /**< CDC class-specific Telephone Operation Modes functional descriptor. */
    CDC_DSUBTYPE_CSInterface_USBTerminal      = 0x09, /**< CDC class-specific USB Terminal functional descriptor. */
    CDC_DSUBTYPE_CSInterface_NetworkChannel   = 0x0A, /**< CDC class-specific Network Channel functional descriptor. */
    CDC_DSUBTYPE_CSInterface_ProtocolUnit     = 0x0B, /**< CDC class-specific Protocol Unit functional descriptor. */
    CDC_DSUBTYPE_CSInterface_ExtensionUnit    = 0x0C, /**< CDC class-specific Extension Unit functional descriptor. */
    CDC_DSUBTYPE_CSInterface_MultiChannel     = 0x0D, /**< CDC class-specific Multi-Channel Management functional descriptor. */
    CDC_DSUBTYPE_CSInterface_CAPI             = 0x0E, /**< CDC class-specific Common ISDN API functional descriptor. */
    CDC_DSUBTYPE_CSInterface_Ethernet         = 0x0F, /**< CDC class-specific Ethernet functional descriptor. */
    CDC_DSUBTYPE_CSInterface_ATM              = 0x10, /**< CDC class-specific Asynchronous Transfer Mode functional descriptor. */
};

/** Enum for the possible line encoding formats of a virtual serial port. */
enum CDC_LineEncodingFormats_t {
    CDC_LINEENCODING_OneStopBit          = 0, /**< Each frame contains one stop bit. */
    CDC_LINEENCODING_OneAndAHalfStopBits = 1, /**< Each frame contains one and a half stop bits. */
    CDC_LINEENCODING_TwoStopBits         = 2, /**< Each frame contains two stop bits. */
};

/** Enum for the possible line encoding parity settings of a virtual serial port. */
enum CDC_LineEncodingParity_t {
    CDC_PARITY_None  = 0, /**< No parity bit mode on each frame. */
    CDC_PARITY_Odd   = 1, /**< Odd parity bit mode on each frame. */
    CDC_PARITY_Even  = 2, /**< Even parity bit mode on each frame. */
    CDC_PARITY_Mark  = 3, /**< Mark parity bit mode on each frame. */
    CDC_PARITY_Space = 4, /**< Space parity bit mode on each frame. */
};

/* Type Defines: */
/** \brief CDC class-specific Functional Header Descriptor (LUFA naming conventions).
 *
 *  Type define for a CDC class-specific functional header descriptor. This indicates to the host that the device
 *  contains one or more CDC functional data descriptors, which give the CDC interface's capabilities and configuration.
 *  See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_StdDescriptor_FunctionalHeader_t for the version of this type with standard element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    USB_Descriptor_Header_t Header;     /**< Regular descriptor header containing the descriptor's type and length. */
    u8 Subtype;                         /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                         *   must be \ref CDC_DSUBTYPE_CSInterface_Header.
                                         */
    u16 CDCSpecification;               /**< Version number of the CDC specification implemented by the device,
                                         *   encoded in BCD format.
                                         *
                                         *   \see \ref VERSION_BCD() utility macro.
                                         */
} USB_CDC_Descriptor_FunctionalHeader_t;

/** \brief CDC class-specific Functional Header Descriptor (USB-IF naming conventions).
 *
 *  Type define for a CDC class-specific functional header descriptor. This indicates to the host that the device
 *  contains one or more CDC functional data descriptors, which give the CDC interface's capabilities and configuration.
 *  See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_Descriptor_FunctionalHeader_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    u8 bFunctionLength;         /**< Size of the descriptor, in bytes. */
    u8 bDescriptorType;         /**< Type of the descriptor, either a value in \ref USB_DescriptorTypes_t or a value
                                 *   given by the specific class.
                                 */
    u8 bDescriptorSubType;      /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                 *   must be \ref CDC_DSUBTYPE_CSInterface_Header.
                                 */
    u16 bcdCDC;                 /**< Version number of the CDC specification implemented by the device, encoded in BCD format.
                                 *
                                 *   \see \ref VERSION_BCD() utility macro.
                                 */
} USB_CDC_StdDescriptor_FunctionalHeader_t;

/** \brief CDC class-specific Functional ACM Descriptor (LUFA naming conventions).
 *
 *  Type define for a CDC class-specific functional ACM descriptor. This indicates to the host that the CDC interface
 *  supports the CDC ACM subclass of the CDC specification. See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_StdDescriptor_FunctionalACM_t for the version of this type with standard element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    USB_Descriptor_Header_t Header;     /**< Regular descriptor header containing the descriptor's type and length. */
    u8 Subtype;                         /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                         *   must be \ref CDC_DSUBTYPE_CSInterface_ACM.
                                         */
    u8 Capabilities;                    /**< Capabilities of the ACM interface, given as a bit mask. For most devices,
                                         *   this should be set to a fixed value of 0x06 - for other capabilities, refer
                                         *   to the CDC ACM specification.
                                         */
} USB_CDC_Descriptor_FunctionalACM_t;

/** \brief CDC class-specific Functional ACM Descriptor (USB-IF naming conventions).
 *
 *  Type define for a CDC class-specific functional ACM descriptor. This indicates to the host that the CDC interface
 *  supports the CDC ACM subclass of the CDC specification. See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_Descriptor_FunctionalACM_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    u8 bFunctionLength;         /**< Size of the descriptor, in bytes. */
    u8 bDescriptorType;         /**< Type of the descriptor, either a value in \ref USB_DescriptorTypes_t or a value
                                 *   given by the specific class.
                                 */
    u8 bDescriptorSubType;      /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                 *   must be \ref CDC_DSUBTYPE_CSInterface_ACM.
                                 */
    u8 bmCapabilities;          /**< Capabilities of the ACM interface, given as a bit mask. For most devices,
                                 *   this should be set to a fixed value of 0x06 - for other capabilities, refer
                                 *   to the CDC ACM specification.
                                 */
} USB_CDC_StdDescriptor_FunctionalACM_t;

/** \brief CDC class-specific Functional Union Descriptor (LUFA naming conventions).
 *
 *  Type define for a CDC class-specific functional Union descriptor. This indicates to the host that specific
 *  CDC control and data interfaces are related. See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_StdDescriptor_FunctionalUnion_t for the version of this type with standard element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    USB_Descriptor_Header_t Header;     /**< Regular descriptor header containing the descriptor's type and length. */
    u8 Subtype;                         /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                         *   must be \ref CDC_DSUBTYPE_CSInterface_Union.
                                         */
    u8 MasterInterfaceNumber;           /**< Interface number of the CDC Control interface. */
    u8 SlaveInterfaceNumber;            /**< Interface number of the CDC Data interface. */
} USB_CDC_Descriptor_FunctionalUnion_t;

/** \brief CDC class-specific Functional Union Descriptor (USB-IF naming conventions).
 *
 *  Type define for a CDC class-specific functional Union descriptor. This indicates to the host that specific
 *  CDC control and data interfaces are related. See the CDC class specification for more details.
 *
 *  \see \ref USB_CDC_Descriptor_FunctionalUnion_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    u8 bFunctionLength;         /**< Size of the descriptor, in bytes. */
    u8 bDescriptorType;         /**< Type of the descriptor, either a value in \ref USB_DescriptorTypes_t or a value
                                 *   given by the specific class.
                                 */
    u8 bDescriptorSubType;      /**< Sub type value used to distinguish between CDC class-specific descriptors,
                                 *   must be \ref CDC_DSUBTYPE_CSInterface_Union.
                                 */
    u8 bMasterInterface;        /**< Interface number of the CDC Control interface. */
    u8 bSlaveInterface0;        /**< Interface number of the CDC Data interface. */
} USB_CDC_StdDescriptor_FunctionalUnion_t;

/** \brief CDC Virtual Serial Port Line Encoding Settings Structure.
 *
 *  Type define for a CDC Line Encoding structure, used to hold the various encoding parameters for a virtual
 *  serial port.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct {
    u32 BaudRateBPS;    /**< Baud rate of the virtual serial port, in bits per second. */
    u8 CharFormat;      /**< Character format of the virtual serial port, a value from the
                         *   \ref CDC_LineEncodingFormats_t enum.
                         */
    u8 ParityType;      /**< Parity setting of the virtual serial port, a value from the
                         *   \ref CDC_LineEncodingParity_t enum.
                         */
    u8 DataBits;        /**< Bits of data per character of the virtual serial port. */
} CDC_LineEncoding_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
