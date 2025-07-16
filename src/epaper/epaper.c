#include "epaper.h"
#include "app_button.h"


#define epd_delay(a) sleep_ms(a)

EPD_PAINT EPD_Paint;

static uint8_t _hibernating = 1;
static uint8_t _status_busy = 0;

static const unsigned char lut_partial[] = { 0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0A, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x22, 0x22, 0x22,
        0x22, 0x22, 0x22, 0x0, 0x0, 0x0, };

void epd_res_set() {
    drv_gpio_write(EPD_RST, HIGH);
}

void epd_res_reset() {
    drv_gpio_write(EPD_RST, LOW);
}

void epd_dc_set() {
    drv_gpio_write(EPD_DC, HIGH);
}

void epd_dc_reset() {
    drv_gpio_write(EPD_DC, LOW);
}

void epd_cs_set() {
    drv_gpio_write(EPD_CS, HIGH);
}

void epd_cs_reset() {
    drv_gpio_write(EPD_CS, LOW);
}

uint8_t epd_is_busy() {
    return drv_gpio_read(EPD_BUSY) == LOW ? 0 : 1;
}

void epd_io_init(void) {
    drv_spi_master_init(EPD_SPI_CLOCK, SPI_MODE_0);
    spi_master_gpio_set(EPD_SPI_PIN_GROUP);
}

void epd_write_cmd(uint8_t cmd) {
    epd_dc_reset();
    epd_cs_reset();

    drv_spi_write(&cmd, 1, NULL, 0, EPD_SPI_PIN_CS);

    epd_cs_set();
    epd_dc_set();
}

void epd_write_data(uint8_t data) {
    epd_cs_reset();

    drv_spi_write(NULL, 0, &data, 1, EPD_SPI_PIN_CS);

    epd_cs_set();
}

uint8_t epd_wait_busy() {
    uint32_t timeout = 0;

    while (epd_is_busy()) {
        timeout++;
        /* about 5 seconds */
        if (timeout > 5000) {
            printf("timeout exit 1: %d\r\n", timeout);
            epd_reset();
            _status_busy = 1;
            return 1;
        }
        button_handler();
        epd_delay(1);
    }
//    printf("timeout exit 0: %d\r\n", timeout);
    return 0;
}

void epd_reset(void) {
    epd_res_reset();
    epd_delay(50);
    epd_res_set();
    epd_delay(50);
    _hibernating = 0;
    _status_busy = 0;
}

uint8_t epd_init(void) {
    if (_hibernating)
        epd_reset();

    if (epd_wait_busy())
        return 1;

    epd_write_cmd(0x12); // SWRESET

    epd_delay(10);

    if (epd_wait_busy())
        return 1;

#if defined(EPD_29) || defined(EPD_213) || defined(EPD_154)
    epd_write_cmd(0x01); // Driver output control
#if defined(EPD_29) || defined(EPD_213)
    epd_write_data(0x27);
    epd_write_data(0x01);
    epd_write_data(0x01);
#else
    epd_write_data(0xC7);
    epd_write_data(0x00);
    epd_write_data(0x01);
#endif

    epd_write_cmd(0x11); // data entry mode
    epd_write_data(0x01);

#ifdef EPD_154
	epd_write_cmd(0x44); // set Ram-X address start/end position
	epd_write_data(0x00);
	epd_write_data(0x18);

	epd_write_cmd(0x45);  // set Ram-Y address start/end position
	epd_write_data(0xC7);
	epd_write_data(0x00);
	epd_write_data(0x00);
	epd_write_data(0x00);
#else
	epd_write_cmd(0x44); // set Ram-X address start/end position
	epd_write_data(0x00);
	epd_write_data(0x0F); // 0x0F-->(15+1)*8=128

	epd_write_cmd(0x45);  // set Ram-Y address start/end position
	epd_write_data(0x27); // 0x127-->(295+1)=296
	epd_write_data(0x01);
	epd_write_data(0x00);
	epd_write_data(0x00);
#endif

	epd_write_cmd(0x3C); // BorderWavefrom
	epd_write_data(0x05);

#if defined(EPD_29) || defined(EPD_213)
	epd_write_cmd(0x21); //  Display update control
	epd_write_data(0x00);
	epd_write_data(0x80);
#endif

#elif defined(EPD_42)
    epd_write_cmd(0x21); // Display Update Controll
    epd_write_data(0x40);
    epd_write_data(0x00);
    epd_write_cmd(0x01);  // Set MUX as 300
    epd_write_data(0x2B);
    epd_write_data(0x01);
    epd_write_data(0x00);
    epd_write_cmd(0x3C); //BorderWavefrom
    epd_write_data(0x01);
    epd_write_cmd(0x11);	// data  entry  mode
    epd_write_data(0x03);		// X-mode
    epd_address_set(0, 0, EPD_W - 1, EPD_H - 1);
#endif

    epd_write_cmd(0x18); // Read built-in temperature sensor
    epd_write_data(0x80);

    epd_setpos(0, 0);

    if (epd_power_on())
        return 1;

    return 0;
}

uint8_t epd_init_partial(void) {
    if (epd_init())
        return 1;

#if defined(EPD_29) || defined(EPD_213)
    epd_write_cmd(0x32);
    epd_cs_reset();
    for (int j = 0; j < sizeof(lut_partial); j++)
    {
        //_epd_write_data(lut_partial[j]);
    }
    //_epd_write_data_over();
    epd_cs_set();
#elif defined(EPD_42)
    epd_write_cmd(0x3C); //BorderWavefrom
    epd_write_data(0x80);
    epd_write_cmd(0x21); // Display Update Controll
    epd_write_data(0x00);    // RED normal
    epd_write_data(0x00);    // single chip application
#endif

    return 0;
}

uint8_t epd_hibernating() {

    return _hibernating;
}

void epd_enter_deepsleepmode(uint8_t mode) {
    //epd_power_off();
    epd_write_cmd(0x10);
    epd_write_data(mode);
    _hibernating = 1;
}

uint8_t epd_power_on(void) {
#if defined EPD_42
    epd_write_cmd(0x22); // Display Update Control
    epd_write_data(0xe0);
#else
	epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xf8);
#endif
	epd_write_cmd(0x20); // Activate Display Update Sequence

	return epd_wait_busy();
}

uint8_t epd_power_off(void) {
    epd_write_cmd(0x22); // Display Update Control
    epd_write_data(0x83);
    epd_write_cmd(0x20); // Activate Display Update Sequence

    return epd_wait_busy();
}

void epd_init_internalTempSensor(void) {
    epd_write_cmd(0x18);
    epd_write_data(0x80);

    epd_write_cmd(0x1A);
    epd_write_data(0x7F);
//  epd_write_data(0xF0);
}

void epd_update(void) {

#ifdef EPD_154
    epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xF4);
#elif defined EPD_42
	epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xF7);
#else
	epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xF7);
#endif
	epd_write_cmd(0x20); // Activate Display Update Sequence

	epd_wait_busy();
}

void epd_update_partial(void) {
#ifdef EPD_154
    epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xFC);
#elif defined EPD_42
	epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xFF);
#else
	epd_write_cmd(0x22); // Display Update Control
	epd_write_data(0xCC);
#endif
	epd_write_cmd(0x20); // Activate Display Update Sequence

	epd_wait_busy();
}

void epd_address_set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
    epd_write_cmd(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    epd_write_data((x_start >> 3) & 0xFF);
    epd_write_data((x_end >> 3) & 0xFF);

    epd_write_cmd(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    epd_write_data(y_start & 0xFF);
    epd_write_data((y_start >> 8) & 0xFF);
    epd_write_data(y_end & 0xFF);
    epd_write_data((y_end >> 8) & 0xFF);
}

void epd_setpos(uint16_t x, uint16_t y) {
    uint8_t _x;
    uint16_t _y;

    _x = x / 8;

#ifdef EPD_154
	_y = 199 - y;
	#elif defined(EPD_29) || defined(EPD_213)
  _y = 295 - y;
	#elif defined(EPD_42)
    _y = y;
#endif

    epd_write_cmd(0x4E); // set RAM x address count to 0;
    epd_write_data(_x);
    epd_write_cmd(0x4F); // set RAM y address count to 0x127;
    epd_write_data(_y & 0xff);
    epd_write_data(_y >> 8 & 0x01);
}

void epd_writedata(uint8_t *Image1, uint32_t length) {
    drv_spi_write(NULL, 0, Image1, length, EPD_SPI_PIN_CS);
}

void epd_writedata2(uint8_t data, uint32_t length) {
    for (uint32_t j = 0; j < length; j++) {
        epd_write_data(data);
    }
}

void epd_display(uint8_t *Image1, uint8_t *Image2) {
    uint32_t Width, Height, i, j;
    uint32_t k = 0;
    Width = EPD_H;
    Height = EPD_W_BUFF_SIZE;

    epd_setpos(0, 0);

    epd_write_cmd(0x24);
    epd_writedata(Image1, Width * Height);

    epd_setpos(0, 0);

    epd_write_cmd(0x26);
    k = 0;
    epd_cs_reset();
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            epd_write_data(~Image2[k]);
            k++;
        }
    }
    epd_cs_set();

#if defined EPD_42
    epd_write_cmd(0x21); // Display Update Controll
    epd_write_data(0x00);    // RED normal
    epd_write_data(0x00);    // single chip application
#endif

    epd_update();
}

void epd_displayBW(uint8_t *Image) {
    uint32_t Width, Height;

    Width = EPD_H;
    Height = EPD_W_BUFF_SIZE;

    epd_setpos(0, 0);
    epd_write_cmd(0x26);
    epd_writedata(Image, Width * Height);

    epd_setpos(0, 0);
    epd_write_cmd(0x24);
    epd_writedata(Image, Width * Height);

    epd_update();
}

void epd_displayBW_partial(uint8_t *Image) {
    uint32_t Width, Height;

    Width = EPD_H;
    Height = EPD_W_BUFF_SIZE;

    epd_setpos(0, 0);
    epd_write_cmd(0x24);
    epd_writedata(Image, Width * Height);

    epd_update_partial();

    epd_setpos(0, 0);
    epd_write_cmd(0x26);
    epd_writedata(Image, Width * Height);
}

void epd_displayRED(uint8_t *Image) {
    uint32_t Width, Height;

    Width = EPD_H;
    Height = EPD_W_BUFF_SIZE;

    epd_setpos(0, 0);

    epd_write_cmd(0x26);
    epd_writedata(Image, Width * Height);

#if defined EPD_42
    epd_write_cmd(0x21); // Display Update Controll
    epd_write_data(0x00);    // RED normal
    epd_write_data(0x00);    // single chip application
#endif
    epd_update();
}

void epd_paint_newimage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color) {
    EPD_Paint.Image = 0x00;
    EPD_Paint.Image = image;

    EPD_Paint.WidthMemory = Width;
    EPD_Paint.HeightMemory = Height;
    EPD_Paint.Color = Color;
    EPD_Paint.WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
    EPD_Paint.HeightByte = Height;
    EPD_Paint.Rotate = Rotate;
    if (Rotate == EPD_ROTATE_0 || Rotate == EPD_ROTATE_180) {

        EPD_Paint.Width = Height;
        EPD_Paint.Height = Width;
    } else {
        EPD_Paint.Width = Width;
        EPD_Paint.Height = Height;
    }
}

void epd_paint_setpixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color) {
    uint16_t X, Y;
    uint32_t Addr;
    uint8_t Rdata;
    switch (EPD_Paint.Rotate) {
        case 0:

            X = EPD_Paint.WidthMemory - Ypoint - 1;
            Y = Xpoint;
            break;
        case 90:
            X = EPD_Paint.WidthMemory - Xpoint - 1;
            Y = EPD_Paint.HeightMemory - Ypoint - 1;
            break;
        case 180:
            X = Ypoint;
            Y = EPD_Paint.HeightMemory - Xpoint - 1;
            break;
        case 270:
            X = Xpoint;
            Y = Ypoint;
            break;
        default:
            return;
    }
    Addr = X / 8 + Y * EPD_Paint.WidthByte;
    Rdata = EPD_Paint.Image[Addr];
    if (Color == EPD_COLOR_BLACK) {
        EPD_Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
    } else
        EPD_Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
}

void epd_paint_clear(uint16_t color) {
    uint16_t X, Y;
    uint32_t Addr;

    for (Y = 0; Y < EPD_Paint.HeightByte; Y++) {
        for (X = 0; X < EPD_Paint.WidthByte; X++) { // 8 pixel =  1 byte
            Addr = X + Y * EPD_Paint.WidthByte;
            EPD_Paint.Image[Addr] = color;
        }
    }
}

void epd_paint_selectimage(uint8_t *image) {
    EPD_Paint.Image = image;
}

void epd_paint_drawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color) {
    epd_paint_setpixel(Xpoint - 1, Ypoint - 1, Color);
}

void epd_paint_drawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color) {
    uint16_t Xpoint, Ypoint;
    int32_t dx, dy;
    int32_t XAddway, YAddway;
    int32_t Esp;
    char Dotted_Len;
    Xpoint = Xstart;
    Ypoint = Ystart;
    dx = (int) Xend - (int) Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    dy = (int) Yend - (int) Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    XAddway = Xstart < Xend ? 1 : -1;
    YAddway = Ystart < Yend ? 1 : -1;

    Esp = dx + dy;
    Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        epd_paint_drawPoint(Xpoint, Ypoint, Color);
        if (2 * Esp >= dy) {
            if (Xpoint == Xend) {
                break;
            }
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend) {
                break;
            }
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

void epd_paint_drawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, uint8_t mode) {
    uint16_t i;
    if (mode) {
        for (i = Ystart; i < Yend; i++) {
            epd_paint_drawLine(Xstart, i, Xend, i, Color);
        }
    } else {
        epd_paint_drawLine(Xstart, Ystart, Xend, Ystart, Color);
        epd_paint_drawLine(Xstart, Ystart, Xstart, Yend, Color);
        epd_paint_drawLine(Xend, Yend, Xend, Ystart, Color);
        epd_paint_drawLine(Xend, Yend, Xstart, Yend, Color);
    }
}

void epd_paint_drawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, uint8_t mode) {
    uint16_t Esp, sCountY;
    uint16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;
    Esp = 3 - (Radius << 1);
    if (mode) {
        while (XCurrent <= YCurrent) { // Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY++) {
                epd_paint_drawPoint(X_Center + XCurrent, Y_Center + sCountY,
                        Color); // 1
                epd_paint_drawPoint(X_Center - XCurrent, Y_Center + sCountY,
                        Color); // 2
                epd_paint_drawPoint(X_Center - sCountY, Y_Center + XCurrent,
                        Color); // 3
                epd_paint_drawPoint(X_Center - sCountY, Y_Center - XCurrent,
                        Color); // 4
                epd_paint_drawPoint(X_Center - XCurrent, Y_Center - sCountY,
                        Color); // 5
                epd_paint_drawPoint(X_Center + XCurrent, Y_Center - sCountY,
                        Color); // 6
                epd_paint_drawPoint(X_Center + sCountY, Y_Center - XCurrent,
                        Color); // 7
                epd_paint_drawPoint(X_Center + sCountY, Y_Center + XCurrent,
                        Color);
            }
            if ((int) Esp < 0)
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    } else { // Draw a hollow circle
        while (XCurrent <= YCurrent) {
            epd_paint_drawPoint(X_Center + XCurrent, Y_Center + YCurrent,
                    Color); // 1
            epd_paint_drawPoint(X_Center - XCurrent, Y_Center + YCurrent,
                    Color); // 2
            epd_paint_drawPoint(X_Center - YCurrent, Y_Center + XCurrent,
                    Color); // 3
            epd_paint_drawPoint(X_Center - YCurrent, Y_Center - XCurrent,
                    Color); // 4
            epd_paint_drawPoint(X_Center - XCurrent, Y_Center - YCurrent,
                    Color); // 5
            epd_paint_drawPoint(X_Center + XCurrent, Y_Center - YCurrent,
                    Color); // 6
            epd_paint_drawPoint(X_Center + YCurrent, Y_Center - XCurrent,
                    Color); // 7
            epd_paint_drawPoint(X_Center + YCurrent, Y_Center + XCurrent,
                    Color); // 0
            if ((int) Esp < 0)
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

void epd_paint_showChar(uint16_t x, uint16_t y, uint16_t chr, sFont *font, uint16_t color) {
    uint16_t i, j;
    unsigned int char_offset = (chr - ' ') * font->height * (font->width / 8 + (font->width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];

    if (EPD_Paint.Rotate == EPD_ROTATE_90 || EPD_Paint.Rotate == EPD_ROTATE_270) {
        if (x + font->width > EPD_W)
            return;
        if (y + font->height > EPD_H)
            return;
    } else {
        if (x + font->width > EPD_H)
            return;
        if (y + font->height > EPD_W)
            return;
    }

    x += 1, y += 1;

    for (j = 0; j < font->height; j++) {
        button_handler();
        for (i = 0; i < font->width; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                epd_paint_drawPoint(x+i, y+j, color);
            } else {
                epd_paint_drawPoint(x+i, y+j, !color);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->width % 8 != 0) {
            ptr++;
        }
    }
}

void epd_paint_showString(uint16_t x, uint16_t y, uint8_t *text, sFont *font, uint16_t color, bool reducing) {

    uint8_t *p_text = text;
    uint16_t counter = 0;
    uint16_t refcolumn = x;

    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        epd_paint_showChar(refcolumn, y, *p_text, font, color);
        /* Decrement the column position by 16 */
        if (reducing) {
            if (font->width > 11)
                if (*(p_text+1) == '.' || *(p_text+1) == ',' || *(p_text+1) == '0' || *(p_text+1) == ' ') {
                    refcolumn += font->width-1;
                } else {
                    refcolumn += font->width-2;
                }
            else if (font->width > 9) {
                if (counter && *(p_text-1) == ' ')
                    refcolumn += font->width-2;
                else
                    refcolumn += font->width-1;
            } else
                refcolumn += font->width;
        } else {
            refcolumn += font->width;
        }
        /* Point on the next character */
        p_text++;
        counter++;
    }


//    uint16_t size1 = font->height;
//    while (*chr != '\0') {
//        epd_paint_showChar(x, y, *chr, font, color);
//        chr++;
//        if (size1 == 8) {
//            x += 6;
//            if (EPD_Paint.Rotate == EPD_ROTATE_90 || EPD_Paint.Rotate == EPD_ROTATE_270) {
//                if (x > EPD_W - 6)
//                    break;
//            } else {
//                if (x > EPD_H - 6)
//                    break;
//            }
//        } else {
//            x += size1 / 2;
//            if (EPD_Paint.Rotate == EPD_ROTATE_90 || EPD_Paint.Rotate == EPD_ROTATE_270) {
//                if (x > EPD_W - size1 / 2)
//                    break;
//            } else {
//                if (x > EPD_H - size1 / 2)
//                    break;
//            }
//        }
//    }
}

// m^n
static uint32_t _Pow(uint16_t m, uint16_t n) {
    uint32_t result = 1;
    while (n--) {
        result *= m;
    }
    return result;
}

void epd_paint_showNum(uint16_t x, uint16_t y, uint32_t num, uint16_t len, sFont *font, uint16_t color) {
    uint16_t size1 = font->height;
    uint8_t t, temp, m = 0;
    if (size1 == 8)
        m = 2;
    for (t = 0; t < len; t++) {
        temp = (num / _Pow(10, len - t - 1)) % 10;
        if (temp == 0) {
            epd_paint_showChar(x + (size1 / 2 + m) * t, y, '0', font, color);
        } else {
            epd_paint_showChar(x + (size1 / 2 + m) * t, y, temp + '0', font, color);
        }
    }
}


void epd_paint_showPicture(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, const uint8_t BMP[], uint16_t Color) {
    int i, j;
    x += 1, y += 1;
    const unsigned char* prt = BMP;
      for (j = 0; j < sizey; j++) {
          button_handler();
          for (i = 0; i < sizex; i++) {
            if (* prt & (0x80 >> (i % 8))){
                epd_paint_drawPoint(x+i, y+j, Color);
            } else {
                epd_paint_drawPoint(x+i, y+j, !Color);
            }
            if (i % 8 == 7) {
              prt++;
            }
          }
          if (sizex % 8 != 0) {
            prt++;
          }
      }
}

uint8_t epd_get_status_busy() {
    return _status_busy;
}

//void epd_paint_showPicture(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, const uint8_t BMP[], uint16_t Color) {
//    uint16_t j = 0;
//    uint16_t i, n = 0, temp = 0, m = 0;
//    uint16_t x0 = 0, y0 = 0;
//    x += 1, y += 1, x0 = x, y0 = y;
//    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
//    for (n = 0; n < sizey; n++) {
//        button_handler();
//        for (i = 0; i < sizex; i++) {
//            temp = BMP[j];
//            j++;
//            for (m = 0; m < 8; m++) {
//                if (temp & 0x01)
//                    epd_paint_drawPoint(x, y, !Color);
//                else
//                    epd_paint_drawPoint(x, y, Color);
//                temp >>= 1;
//                y++;
//            }
//            x++;
//            if ((x - x0) == sizex) {
//                x = x0;
//                y0 = y0 + 8;
//            }
//            y = y0;
//        }
//    }
//}
