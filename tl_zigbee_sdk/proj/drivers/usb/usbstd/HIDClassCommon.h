/********************************************************************************************************
 * @file    HIDClassCommon.h
 *
 * @brief   This is the header file for HIDClassCommon
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
/** \name Keyboard Standard Report Modifier Masks */
//@{
/** Constant for a keyboard report modifier byte, indicating that the keyboard's left control key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_LEFTCTRL                    BIT(0)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's left shift key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_LEFTSHIFT                   BIT(1)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's left alt key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_LEFTALT                     BIT(2)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's left GUI key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_LEFTGUI                     BIT(3)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's right control key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_RIGHTCTRL                   BIT(4)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's right shift key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_RIGHTSHIFT                  BIT(5)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's right alt key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_RIGHTALT                    BIT(6)

/** Constant for a keyboard report modifier byte, indicating that the keyboard's right GUI key is currently pressed. */
#define HID_KEYBOARD_MODIFIER_RIGHTGUI                    BIT(7)
//@}

/** \name Keyboard Standard Report LED Masks */
//@{
/** Constant for a keyboard output report LED byte, indicating that the host's NUM LOCK mode is currently set. */
#define HID_KEYBOARD_LED_NUMLOCK                          BIT(0)

/** Constant for a keyboard output report LED byte, indicating that the host's CAPS LOCK mode is currently set. */
#define HID_KEYBOARD_LED_CAPSLOCK                         BIT(1)

/** Constant for a keyboard output report LED byte, indicating that the host's SCROLL LOCK mode is currently set. */
#define HID_KEYBOARD_LED_SCROLLLOCK                       BIT(2)

/** Constant for a keyboard output report LED byte, indicating that the host's KATANA mode is currently set. */
#define HID_KEYBOARD_LED_KATANA                           BIT(3)
//@}

/** \name Keyboard Standard Report Key Scan-codes */
//@{
#define HID_KEYBOARD_SC_ERROR_ROLLOVER                    0x01
#define HID_KEYBOARD_SC_POST_FAIL                         0x02
#define HID_KEYBOARD_SC_ERROR_UNDEFINED                   0x03
#define HID_KEYBOARD_SC_A                                 0x04
#define HID_KEYBOARD_SC_B                                 0x05
#define HID_KEYBOARD_SC_C                                 0x06
#define HID_KEYBOARD_SC_D                                 0x07
#define HID_KEYBOARD_SC_E                                 0x08
#define HID_KEYBOARD_SC_F                                 0x09
#define HID_KEYBOARD_SC_G                                 0x0A
#define HID_KEYBOARD_SC_H                                 0x0B
#define HID_KEYBOARD_SC_I                                 0x0C
#define HID_KEYBOARD_SC_J                                 0x0D
#define HID_KEYBOARD_SC_K                                 0x0E
#define HID_KEYBOARD_SC_L                                 0x0F
#define HID_KEYBOARD_SC_M                                 0x10
#define HID_KEYBOARD_SC_N                                 0x11
#define HID_KEYBOARD_SC_O                                 0x12
#define HID_KEYBOARD_SC_P                                 0x13
#define HID_KEYBOARD_SC_Q                                 0x14
#define HID_KEYBOARD_SC_R                                 0x15
#define HID_KEYBOARD_SC_S                                 0x16
#define HID_KEYBOARD_SC_T                                 0x17
#define HID_KEYBOARD_SC_U                                 0x18
#define HID_KEYBOARD_SC_V                                 0x19
#define HID_KEYBOARD_SC_W                                 0x1A
#define HID_KEYBOARD_SC_X                                 0x1B
#define HID_KEYBOARD_SC_Y                                 0x1C
#define HID_KEYBOARD_SC_Z                                 0x1D
#define HID_KEYBOARD_SC_1_AND_EXCLAMATION                 0x1E
#define HID_KEYBOARD_SC_2_AND_AT                          0x1F
#define HID_KEYBOARD_SC_3_AND_HASHMARK                    0x20
#define HID_KEYBOARD_SC_4_AND_DOLLAR                      0x21
#define HID_KEYBOARD_SC_5_AND_PERCENTAGE                  0x22
#define HID_KEYBOARD_SC_6_AND_CARET                       0x23
#define HID_KEYBOARD_SC_7_AND_AND_AMPERSAND               0x24
#define HID_KEYBOARD_SC_8_AND_ASTERISK                    0x25
#define HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS         0x26
#define HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS         0x27
#define HID_KEYBOARD_SC_ENTER                             0x28
#define HID_KEYBOARD_SC_ESCAPE                            0x29
#define HID_KEYBOARD_SC_BACKSPACE                         0x2A
#define HID_KEYBOARD_SC_TAB                               0x2B
#define HID_KEYBOARD_SC_SPACE                             0x2C
#define HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE              0x2D
#define HID_KEYBOARD_SC_EQUAL_AND_PLUS                    0x2E
#define HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE 0x2F
#define HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE 0x30
#define HID_KEYBOARD_SC_BACKSLASH_AND_PIPE                0x31
#define HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE         0x32
#define HID_KEYBOARD_SC_SEMICOLON_AND_COLON               0x33
#define HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE              0x34
#define HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE            0x35
#define HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN          0x36
#define HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN         0x37
#define HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK           0x38
#define HID_KEYBOARD_SC_CAPS_LOCK                         0x39
#define HID_KEYBOARD_SC_F1                                0x3A
#define HID_KEYBOARD_SC_F2                                0x3B
#define HID_KEYBOARD_SC_F3                                0x3C
#define HID_KEYBOARD_SC_F4                                0x3D
#define HID_KEYBOARD_SC_F5                                0x3E
#define HID_KEYBOARD_SC_F6                                0x3F
#define HID_KEYBOARD_SC_F7                                0x40
#define HID_KEYBOARD_SC_F8                                0x41
#define HID_KEYBOARD_SC_F9                                0x42
#define HID_KEYBOARD_SC_F10                               0x43
#define HID_KEYBOARD_SC_F11                               0x44
#define HID_KEYBOARD_SC_F12                               0x45
#define HID_KEYBOARD_SC_PRINT_SCREEN                      0x46
#define HID_KEYBOARD_SC_SCROLL_LOCK                       0x47
#define HID_KEYBOARD_SC_PAUSE                             0x48
#define HID_KEYBOARD_SC_INSERT                            0x49
#define HID_KEYBOARD_SC_HOME                              0x4A
#define HID_KEYBOARD_SC_PAGE_UP                           0x4B
#define HID_KEYBOARD_SC_DELETE                            0x4C
#define HID_KEYBOARD_SC_END                               0x4D
#define HID_KEYBOARD_SC_PAGE_DOWN                         0x4E
#define HID_KEYBOARD_SC_RIGHT_ARROW                       0x4F
#define HID_KEYBOARD_SC_LEFT_ARROW                        0x50
#define HID_KEYBOARD_SC_DOWN_ARROW                        0x51
#define HID_KEYBOARD_SC_UP_ARROW                          0x52
#define HID_KEYBOARD_SC_NUM_LOCK                          0x53
#define HID_KEYBOARD_SC_KEYPAD_SLASH                      0x54
#define HID_KEYBOARD_SC_KEYPAD_ASTERISK                   0x55
#define HID_KEYBOARD_SC_KEYPAD_MINUS                      0x56
#define HID_KEYBOARD_SC_KEYPAD_PLUS                       0x57
#define HID_KEYBOARD_SC_KEYPAD_ENTER                      0x58
#define HID_KEYBOARD_SC_KEYPAD_1_AND_END                  0x59
#define HID_KEYBOARD_SC_KEYPAD_2_AND_DOWN_ARROW           0x5A
#define HID_KEYBOARD_SC_KEYPAD_3_AND_PAGE_DOWN            0x5B
#define HID_KEYBOARD_SC_KEYPAD_4_AND_LEFT_ARROW           0x5C
#define HID_KEYBOARD_SC_KEYPAD_5                          0x5D
#define HID_KEYBOARD_SC_KEYPAD_6_AND_RIGHT_ARROW          0x5E
#define HID_KEYBOARD_SC_KEYPAD_7_AND_HOME                 0x5F
#define HID_KEYBOARD_SC_KEYPAD_8_AND_UP_ARROW             0x60
#define HID_KEYBOARD_SC_KEYPAD_9_AND_PAGE_UP              0x61
#define HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT               0x62
#define HID_KEYBOARD_SC_KEYPAD_DOT_AND_DELETE             0x63
#define HID_KEYBOARD_SC_NON_US_BACKSLASH_AND_PIPE         0x64
#define HID_KEYBOARD_SC_POWER                             0x66
#define HID_KEYBOARD_SC_EQUAL_SIGN                        0x67
#define HID_KEYBOARD_SC_F13                               0x68
#define HID_KEYBOARD_SC_F14                               0x69
#define HID_KEYBOARD_SC_F15                               0x6A
#define HID_KEYBOARD_SC_F16                               0x6B
#define HID_KEYBOARD_SC_F17                               0x6C
#define HID_KEYBOARD_SC_F18                               0x6D
#define HID_KEYBOARD_SC_F19                               0x6E
#define HID_KEYBOARD_SC_F20                               0x6F
#define HID_KEYBOARD_SC_F21                               0x70
#define HID_KEYBOARD_SC_F22                               0x71
#define HID_KEYBOARD_SC_F23                               0x72
#define HID_KEYBOARD_SC_F24                               0x73
#define HID_KEYBOARD_SC_EXECUTE                           0x74
#define HID_KEYBOARD_SC_HELP                              0x75
#define HID_KEYBOARD_SC_MANU                              0x76
#define HID_KEYBOARD_SC_SELECT                            0x77
#define HID_KEYBOARD_SC_STOP                              0x78
#define HID_KEYBOARD_SC_AGAIN                             0x79
#define HID_KEYBOARD_SC_UNDO                              0x7A
#define HID_KEYBOARD_SC_CUT                               0x7B
#define HID_KEYBOARD_SC_COPY                              0x7C
#define HID_KEYBOARD_SC_PASTE                             0x7D
#define HID_KEYBOARD_SC_FIND                              0x7E
#define HID_KEYBOARD_SC_MUTE                              0x7F
#define HID_KEYBOARD_SC_VOLUME_UP                         0x80
#define HID_KEYBOARD_SC_VOLUME_DOWN                       0x81
#define HID_KEYBOARD_SC_LOCKING_CAPS_LOCK                 0x82
#define HID_KEYBOARD_SC_LOCKING_NUM_LOCK                  0x83
#define HID_KEYBOARD_SC_LOCKING_SCROLL_LOCK               0x84
#define HID_KEYBOARD_SC_KEYPAD_COMMA                      0x85
#define HID_KEYBOARD_SC_KEYPAD_EQUAL_SIGN                 0x86
#define HID_KEYBOARD_SC_INTERNATIONAL1                    0x87
#define HID_KEYBOARD_SC_INTERNATIONAL2                    0x88
#define HID_KEYBOARD_SC_INTERNATIONAL3                    0x89
#define HID_KEYBOARD_SC_INTERNATIONAL4                    0x8A
#define HID_KEYBOARD_SC_INTERNATIONAL5                    0x8B
#define HID_KEYBOARD_SC_INTERNATIONAL6                    0x8C
#define HID_KEYBOARD_SC_INTERNATIONAL7                    0x8D
#define HID_KEYBOARD_SC_INTERNATIONAL8                    0x8E
#define HID_KEYBOARD_SC_INTERNATIONAL9                    0x8F
#define HID_KEYBOARD_SC_LANG1                             0x90
#define HID_KEYBOARD_SC_LANG2                             0x91
#define HID_KEYBOARD_SC_LANG3                             0x92
#define HID_KEYBOARD_SC_LANG4                             0x93
#define HID_KEYBOARD_SC_LANG5                             0x94
#define HID_KEYBOARD_SC_LANG6                             0x95
#define HID_KEYBOARD_SC_LANG7                             0x96
#define HID_KEYBOARD_SC_LANG8                             0x97
#define HID_KEYBOARD_SC_LANG9                             0x98
#define HID_KEYBOARD_SC_ALTERNATE_ERASE                   0x99
#define HID_KEYBOARD_SC_SISREQ                            0x9A
#define HID_KEYBOARD_SC_CANCEL                            0x9B
#define HID_KEYBOARD_SC_CLEAR                             0x9C
#define HID_KEYBOARD_SC_PRIOR                             0x9D
#define HID_KEYBOARD_SC_RETURN                            0x9E
#define HID_KEYBOARD_SC_SEPARATOR                         0x9F
#define HID_KEYBOARD_SC_OUT                               0xA0
#define HID_KEYBOARD_SC_OPER                              0xA1
#define HID_KEYBOARD_SC_CLEAR_AND_AGAIN                   0xA2
#define HID_KEYBOARD_SC_CRSEL_ANDPROPS                    0xA3
#define HID_KEYBOARD_SC_EXSEL                             0xA4
#define HID_KEYBOARD_SC_KEYPAD_00                         0xB0
#define HID_KEYBOARD_SC_KEYPAD_000                        0xB1
#define HID_KEYBOARD_SC_THOUSANDS_SEPARATOR               0xB2
#define HID_KEYBOARD_SC_DECIMAL_SEPARATOR                 0xB3
#define HID_KEYBOARD_SC_CURRENCY_UNIT                     0xB4
#define HID_KEYBOARD_SC_CURRENCY_SUB_UNIT                 0xB5
#define HID_KEYBOARD_SC_KEYPAD_OPENING_PARENTHESIS        0xB6
#define HID_KEYBOARD_SC_KEYPAD_CLOSING_PARENTHESIS        0xB7
#define HID_KEYBOARD_SC_KEYPAD_OPENING_BRACE              0xB8
#define HID_KEYBOARD_SC_KEYPAD_CLOSING_BRACE              0xB9
#define HID_KEYBOARD_SC_KEYPAD_TAB                        0xBA
#define HID_KEYBOARD_SC_KEYPAD_BACKSPACE                  0xBB
#define HID_KEYBOARD_SC_KEYPAD_A                          0xBC
#define HID_KEYBOARD_SC_KEYPAD_B                          0xBD
#define HID_KEYBOARD_SC_KEYPAD_C                          0xBE
#define HID_KEYBOARD_SC_KEYPAD_D                          0xBF
#define HID_KEYBOARD_SC_KEYPAD_E                          0xC0
#define HID_KEYBOARD_SC_KEYPAD_F                          0xC1
#define HID_KEYBOARD_SC_KEYPAD_XOR                        0xC2
#define HID_KEYBOARD_SC_KEYPAD_CARET                      0xC3
#define HID_KEYBOARD_SC_KEYPAD_PERCENTAGE                 0xC4
#define HID_KEYBOARD_SC_KEYPAD_LESS_THAN_SIGN             0xC5
#define HID_KEYBOARD_SC_KEYPAD_GREATER_THAN_SIGN          0xC6
#define HID_KEYBOARD_SC_KEYPAD_AMP                        0xC7
#define HID_KEYBOARD_SC_KEYPAD_AMP_AMP                    0xC8
#define HID_KEYBOARD_SC_KEYPAD_PIPE                       0xC9
#define HID_KEYBOARD_SC_KEYPAD_PIPE_PIPE                  0xCA
#define HID_KEYBOARD_SC_KEYPAD_COLON                      0xCB
#define HID_KEYBOARD_SC_KEYPAD_HASHMARK                   0xCC
#define HID_KEYBOARD_SC_KEYPAD_SPACE                      0xCD
#define HID_KEYBOARD_SC_KEYPAD_AT                         0xCE
#define HID_KEYBOARD_SC_KEYPAD_EXCLAMATION_SIGN           0xCF
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_STORE               0xD0
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_RECALL              0xD1
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_CLEAR               0xD2
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_ADD                 0xD3
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_SUBTRACT            0xD4
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_MULTIPLY            0xD5
#define HID_KEYBOARD_SC_KEYPAD_MEMORY_DIVIDE              0xD6
#define HID_KEYBOARD_SC_KEYPAD_PLUS_AND_MINUS             0xD7
#define HID_KEYBOARD_SC_KEYPAD_CLEAR                      0xD8
#define HID_KEYBOARD_SC_KEYPAD_CLEAR_ENTRY                0xD9
#define HID_KEYBOARD_SC_KEYPAD_BINARY                     0xDA
#define HID_KEYBOARD_SC_KEYPAD_OCTAL                      0xDB
#define HID_KEYBOARD_SC_KEYPAD_DECIMAL                    0xDC
#define HID_KEYBOARD_SC_KEYPAD_HEXADECIMAL                0xDD
#define HID_KEYBOARD_SC_LEFT_CONTROL                      0xE0
#define HID_KEYBOARD_SC_LEFT_SHIFT                        0xE1
#define HID_KEYBOARD_SC_LEFT_ALT                          0xE2
#define HID_KEYBOARD_SC_LEFT_GUI                          0xE3
#define HID_KEYBOARD_SC_RIGHT_CONTROL                     0xE4
#define HID_KEYBOARD_SC_RIGHT_SHIFT                       0xE5
#define HID_KEYBOARD_SC_RIGHT_ALT                         0xE6
#define HID_KEYBOARD_SC_RIGHT_GUI                         0xE7
//@}

/** \name Common HID Device Report Descriptors */
//@{
/** \hideinitializer
 *  A list of HID report item array elements that describe a typical HID USB Joystick. The resulting report
 *  descriptor is structured according to the following layout:
 *
 *  \code
 *  struct
 *  {
 *      intB_t X; // Signed X axis value
 *      intB_t Y; // Signed Y axis value
 *      s8 Z; // Signed Z axis value
 *      // Additional axis elements here
 *      uintA_t Buttons; // Pressed buttons bitmask
 *  } Joystick_Report;
 *  \endcode
 *
 *  Where \c uintA_t is a type large enough to hold one bit per button, and \c intB_t is a type large enough to hold the
 *  ranges of the signed \c MinAxisVal and \c MaxAxisVal values.
 *
 *  \param[in] NumAxis         Number of axis in the joystick (8-bit)
 *  \param[in] MinAxisVal      Minimum logical axis value (16-bit).
 *  \param[in] MaxAxisVal      Maximum logical axis value (16-bit).
 *  \param[in] MinPhysicalVal  Minimum physical axis value, for movement resolution calculations (16-bit).
 *  \param[in] MaxPhysicalVal  Maximum physical axis value, for movement resolution calculations (16-bit).
 *  \param[in] Buttons         Total number of buttons in the device (8-bit).
 */
#define HID_DESCRIPTOR_JOYSTICK(NumAxis, MinAxisVal, MaxAxisVal, MinPhysicalVal, MaxPhysicalVal, Buttons) \
    HID_RI_USAGE_PAGE(8, 0x01),                 \
    HID_RI_USAGE(8, 0x04),                      \
    HID_RI_COLLECTION(8, 0x01),                 \
    HID_RI_USAGE(8, 0x01),                      \
    HID_RI_COLLECTION(8, 0x00),                 \
    HID_RI_USAGE_MINIMUM(8, 0x30),              \
    HID_RI_USAGE_MAXIMUM(8, (0x30 + (NumAxis - 1))), \
    HID_RI_LOGICAL_MINIMUM(16, MinAxisVal),     \
    HID_RI_LOGICAL_MAXIMUM(16, MaxAxisVal),     \
    HID_RI_PHYSICAL_MINIMUM(16, MinPhysicalVal),\
    HID_RI_PHYSICAL_MAXIMUM(16, MaxPhysicalVal),\
    HID_RI_REPORT_COUNT(8, NumAxis),            \
    HID_RI_REPORT_SIZE(8, ((((MinAxisVal >= -0xFF) && (MaxAxisVal <= 0xFF)) ? 8 : 16))), \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
    HID_RI_END_COLLECTION(0),                   \
    HID_RI_USAGE_PAGE(8, 0x09),                 \
    HID_RI_USAGE_MINIMUM(8, 0x01),              \
    HID_RI_USAGE_MAXIMUM(8, Buttons),           \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),            \
    HID_RI_REPORT_SIZE(8, 0x01),                \
    HID_RI_REPORT_COUNT(8, Buttons),            \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
    HID_RI_REPORT_SIZE(8, (8 - (Buttons % 8))), \
    HID_RI_REPORT_COUNT(8, 0x01),               \
    HID_RI_INPUT(8, HID_IOF_CONSTANT),          \
    HID_RI_END_COLLECTION(0)

/** \hideinitializer
 *  A list of HID report item array elements that describe a typical HID USB keyboard. The resulting report descriptor
 *  is compatible with \ref USB_KeyboardReport_Data_t when \c MaxKeys is equal to 6. For other values, the report will
 *  be structured according to the following layout:
 *
 *  \code
 *  struct
 *  {
 *      u8 Modifier; // Keyboard modifier byte indicating pressed modifier keys (\c HID_KEYBOARD_MODIFIER_* masks)
 *      u8 Reserved; // Reserved for OEM use, always set to 0.
 *      u8 KeyCode[MaxKeys]; // Length determined by the number of keys that can be reported
 *  } Keyboard_Report;
 *  \endcode
 *
 *  \param[in] MaxKeys  Number of simultaneous keys that can be reported at the one time (8-bit).
 */
#define HID_DESCRIPTOR_KEYBOARD(MaxKeys) \
    HID_RI_USAGE_PAGE(8, 0x01),                 \
    HID_RI_USAGE(8, 0x06),                      \
    HID_RI_COLLECTION(8, 0x01),                 \
    HID_RI_USAGE_PAGE(8, 0x07),                 \
    HID_RI_USAGE_MINIMUM(8, 0xE0),              \
    HID_RI_USAGE_MAXIMUM(8, 0xE7),              \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),            \
    HID_RI_REPORT_COUNT(8, 0x08),               \
    HID_RI_REPORT_SIZE(8, 0x01),                \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
    HID_RI_INPUT(8, HID_IOF_CONSTANT | HID_IOF_VARIABLE), \
    HID_RI_REPORT_COUNT(8, 0x05),               \
    HID_RI_USAGE_PAGE(8, 0x08),                 \
    HID_RI_USAGE_MINIMUM(8, 0x01),              \
    HID_RI_USAGE_MAXIMUM(8, 0x05),              \
    HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE), \
    HID_RI_REPORT_COUNT(8, 0x01),               \
    HID_RI_REPORT_SIZE(8, 0x03),                \
    HID_RI_OUTPUT(8, HID_IOF_CONSTANT),         \
    HID_RI_REPORT_COUNT(8, 0x06),               \
    HID_RI_REPORT_SIZE(8, 0x08),                \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    0x26, 0xa4, 0x00,                           \
    HID_RI_USAGE_PAGE(8, 0x07),                 \
    HID_RI_USAGE_MINIMUM(8, 0x00),              \
    0x2a, 0xa4, 0x00,                           \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE), \
    HID_RI_END_COLLECTION(0)

    ////////////////////////////////////////////////////////
    //HID_RI_LOGICAL_MAXIMUM(8, 0x00a4): 0x2a,0xa4,0x00
    //HID_RI_USAGE_MAXIMUM(8, 0x00a4): 0x2a,0xa4,0x00
    ///////////////////////////////////////////////////////


/** \hideinitializer
 *  A list of HID report item array elements that describe a typical HID USB mouse. The resulting report descriptor
 *  is compatible with \ref USB_MouseReport_Data_t if the \c MinAxisVal and \c MaxAxisVal values fit within a \c s8 range
 *  and the number of Buttons is less than 8. For other values, the report is structured according to the following layout:
 *
 *  \code
 *  struct
 *  {
 *      uintA_t Buttons; // Pressed buttons bitmask
 *      intB_t X; // X axis value
 *      intB_t Y; // Y axis value
 *  } Mouse_Report;
 *  \endcode
 *
 *  Where \c intA_t is a type large enough to hold one bit per button, and \c intB_t is a type large enough to hold the
 *  ranges of the signed \c MinAxisVal and \c MaxAxisVal values.
 *
 *  \param[in] MinAxisVal      Minimum X/Y logical axis value (16-bit).
 *  \param[in] MaxAxisVal      Maximum X/Y logical axis value (16-bit).
 *  \param[in] MinPhysicalVal  Minimum X/Y physical axis value, for movement resolution calculations (16-bit).
 *  \param[in] MaxPhysicalVal  Maximum X/Y physical axis value, for movement resolution calculations (16-bit).
 *  \param[in] Buttons         Total number of buttons in the device (8-bit).
 *  \param[in] AbsoluteCoords  Boolean \c true to use absolute X/Y coordinates (e.g. touchscreen).
 */
#define HID_DESCRIPTOR_MOUSE(MinAxisVal, MaxAxisVal, MinPhysicalVal, MaxPhysicalVal, Buttons, AbsoluteCoords) \
    HID_RI_USAGE_PAGE(8, 0x01),                 \
    HID_RI_USAGE(8, 0x02),                      \
    HID_RI_COLLECTION(8, 0x01),                 \
    HID_RI_USAGE(8, 0x01),                      \
    HID_RI_COLLECTION(8, 0x00),                 \
    HID_RI_USAGE_PAGE(8, 0x09),                 \
    HID_RI_USAGE_MINIMUM(8, 0x01),              \
    HID_RI_USAGE_MAXIMUM(8, Buttons),           \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),            \
    HID_RI_REPORT_COUNT(8, Buttons),            \
    HID_RI_REPORT_SIZE(8, 0x01),                \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
    HID_RI_REPORT_COUNT(8, 0x01),               \
    HID_RI_REPORT_SIZE(8, (8 - (Buttons % 8))), \
    HID_RI_INPUT(8, HID_IOF_CONSTANT),          \
    HID_RI_USAGE_PAGE(8, 0x01),                 \
    HID_RI_USAGE(8, 0x30),                      \
    HID_RI_USAGE(8, 0x31),                      \
    HID_RI_LOGICAL_MINIMUM(16, MinAxisVal),     \
    HID_RI_LOGICAL_MAXIMUM(16, MaxAxisVal),     \
    HID_RI_PHYSICAL_MINIMUM(16, MinPhysicalVal),\
    HID_RI_PHYSICAL_MAXIMUM(16, MaxPhysicalVal),\
    HID_RI_REPORT_COUNT(8, 0x02),               \
    HID_RI_REPORT_SIZE(8, ((((MinAxisVal >= -0xFF) && (MaxAxisVal <= 0xFF)) ? 8 : 16))), \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | (AbsoluteCoords ? HID_IOF_ABSOLUTE : HID_IOF_RELATIVE)), \
    HID_RI_END_COLLECTION(0),                   \
    HID_RI_END_COLLECTION(0)

/** \hideinitializer
 *  A list of HID report item array elements that describe a typical Vendor Defined byte array HID report descriptor,
 *  used for transporting arbitrary data between the USB host and device via HID reports. The resulting report should be
 *  a \c u8 byte array of the specified length in both Device to Host (IN) and Host to Device (OUT) directions.
 *
 *  \param[in] VendorPageNum    Vendor Defined HID Usage Page index, ranging from 0x00 to 0xFF.
 *  \param[in] CollectionUsage  Vendor Usage for the encompassing report IN and OUT collection, ranging from 0x00 to 0xFF.
 *  \param[in] DataINUsage      Vendor Usage for the IN report data, ranging from 0x00 to 0xFF.
 *  \param[in] DataOUTUsage     Vendor Usage for the OUT report data, ranging from 0x00 to 0xFF.
 *  \param[in] NumBytes         Length of the data IN and OUT reports.
 */
#define HID_DESCRIPTOR_VENDOR(VendorPageNum, CollectionUsage, DataINUsage, DataOUTUsage, NumBytes) \
    HID_RI_USAGE_PAGE(16, (0xFF00 | VendorPageNum)), \
    HID_RI_USAGE(8, CollectionUsage),           \
    HID_RI_COLLECTION(8, 0x01),                 \
    HID_RI_USAGE(8, DataINUsage),               \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    HID_RI_LOGICAL_MAXIMUM(8, 0xFF),            \
    HID_RI_REPORT_SIZE(8, 0x08),                \
    HID_RI_REPORT_COUNT(8, NumBytes),           \
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE), \
    HID_RI_USAGE(8, DataOUTUsage),              \
    HID_RI_LOGICAL_MINIMUM(8, 0x00),            \
    HID_RI_LOGICAL_MAXIMUM(8, 0xFF),            \
    HID_RI_REPORT_SIZE(8, 0x08),                \
    HID_RI_REPORT_COUNT(8, NumBytes),           \
    HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE), \
    HID_RI_END_COLLECTION(0)
//@}

/* Type Defines: */
/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors relating to the HID
 *  device class.
 */
enum HID_Descriptor_ClassSubclassProtocol_t {
    HID_CSCP_HIDClass             = 0x03, /**< Descriptor Class value indicating that the device or interface
                                           *   belongs to the HID class.
                                           */
    HID_CSCP_NonBootSubclass      = 0x00, /**< Descriptor Subclass value indicating that the device or interface
                                           *   does not implement a HID boot protocol.
                                           */
    HID_CSCP_BootSubclass         = 0x01, /**< Descriptor Subclass value indicating that the device or interface
                                           *   implements a HID boot protocol.
                                           */
    HID_CSCP_NonBootProtocol      = 0x00, /**< Descriptor Protocol value indicating that the device or interface
                                           *   does not belong to a HID boot protocol.
                                           */
    HID_CSCP_KeyboardBootProtocol = 0x01, /**< Descriptor Protocol value indicating that the device or interface
                                           *   belongs to the Keyboard HID boot protocol.
                                           */
    HID_CSCP_MouseBootProtocol    = 0x02, /**< Descriptor Protocol value indicating that the device or interface
                                           *   belongs to the Mouse HID boot protocol.
                                           */
};

/** Enum for the HID class specific control requests that can be issued by the USB bus host. */
enum HID_ClassRequests_t {
    HID_REQ_GetReport       = 0x01, /**< HID class-specific Request to get the current HID report from the device. */
    HID_REQ_GetIdle         = 0x02, /**< HID class-specific Request to get the current device idle count. */
    HID_REQ_GetProtocol     = 0x03, /**< HID class-specific Request to get the current HID report protocol mode. */
    HID_REQ_SetReport       = 0x09, /**< HID class-specific Request to set the current HID report to the device. */
    HID_REQ_SetIdle         = 0x0A, /**< HID class-specific Request to set the device's idle count. */
    HID_REQ_SetProtocol     = 0x0B, /**< HID class-specific Request to set the current HID report protocol mode. */
};

/** Enum for the HID class specific descriptor types. */
enum HID_DescriptorTypes_t {
    HID_DTYPE_HID           = 0x21, /**< Descriptor header type value, to indicate a HID class HID descriptor. */
    HID_DTYPE_Report        = 0x22, /**< Descriptor header type value, to indicate a HID class HID report descriptor. */
    HID_DTYPE_Physical      = 0x23,
};

/** Enum for the different types of HID reports. */
enum HID_ReportItemTypes_t {
    HID_REPORT_ITEM_In      = 0, /**< Indicates that the item is an IN report type. */
    HID_REPORT_ITEM_Out     = 1, /**< Indicates that the item is an OUT report type. */
    HID_REPORT_ITEM_Feature = 2, /**< Indicates that the item is a FEATURE report type. */
    HID_REPORT_CUSTOM       = 3,
};

/** \brief HID class-specific HID Descriptor (LUFA naming conventions).
 *
 *  Type define for the HID class-specific HID descriptor, to describe the HID device's specifications. Refer to the HID
 *  specification for details on the structure elements.
 *
 *  \see \ref USB_HID_StdDescriptor_HID_t for the version of this type with standard element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    USB_Descriptor_Header_t Header;     /**< Regular descriptor header containing the descriptor's type and length. */

    u16 HIDSpec;                        /**< BCD encoded version that the HID descriptor and device complies to. */
    u8 CountryCode;                     /**< Country code of the localized device, or zero if universal. */

    u8 TotalReportDescriptors;          /**< Total number of HID report descriptors for the interface. */

    u8 HIDReportType;                   /**< Type of HID report, set to \ref HID_DTYPE_Report. */
    u8 HIDReportLength[2];              /**< Length of the associated HID report descriptor, in bytes. */
} USB_HID_Descriptor_HID_t;

/** \brief HID class-specific HID Descriptor (USB-IF naming conventions).
 *
 *  Type define for the HID class-specific HID descriptor, to describe the HID device's specifications. Refer to the HID
 *  specification for details on the structure elements.
 *
 *  \see \ref USB_HID_Descriptor_HID_t for the version of this type with non-standard LUFA specific
 *       element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct _attribute_packed_ {
    u8 bLength;                 /**< Size of the descriptor, in bytes. */
    u8 bDescriptorType;         /**< Type of the descriptor, either a value in \ref USB_DescriptorTypes_t or a value
                                 *   given by the specific class.
                                 */

    u16 bcdHID;                 /**< BCD encoded version that the HID descriptor and device complies to. */
    u8 bCountryCode;            /**< Country code of the localized device, or zero if universal. */

    u8 bNumDescriptors;         /**< Total number of HID report descriptors for the interface. */

    u8 bDescriptorType2;        /**< Type of HID report, set to \ref HID_DTYPE_Report. */
    u8 wDescriptorLength[2];    /**< Length of the associated HID report descriptor, in bytes. */
} USB_HID_StdDescriptor_HID_t;

/** \brief Standard HID Boot Protocol Mouse Report.
 *
 *  Type define for a standard Boot Protocol Mouse report
 */
typedef struct _attribute_packed_ {
    u8 Button;  /**< Button mask for currently pressed buttons in the mouse. */
    s8 X;       /**< Current delta X movement of the mouse. */
    s8 Y;       /**< Current delta Y movement on the mouse. */
} USB_MouseReport_Data_t;

/** \brief Standard HID Boot Protocol Keyboard Report.
 *
 *  Type define for a standard Boot Protocol Keyboard report
 */
typedef struct _attribute_packed_ {
    u8 Modifier;        /**< Keyboard modifier byte, indicating pressed modifier keys (a combination of
                         *   \c HID_KEYBOARD_MODIFIER_* masks).
                         */
    u8 Reserved;        /**< Reserved for OEM use, always set to 0. */
    u8 KeyCode[6];      /**< Key codes of the currently pressed keys. */
} USB_KeyboardReport_Data_t;

/** Type define for the data type used to store HID report descriptor elements. */
typedef u8 USB_Descriptor_HIDReport_Datatype_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
