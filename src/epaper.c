#include "pico/epaper.h"

#include <malloc.h>
#include <memory.h>
#include <stdarg.h>
#include <hardware/gpio.h>

const unsigned char lut_vcom0[] = {
        0x00, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
        0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_ww[] = {
        0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
        0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_bw[] = {
        0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
        0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_bb[] = {
        0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
        0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_wb[] = {
        0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
        0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_partial_vcom0[] = {
        0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00,
};
const unsigned char lut_partial_ww[] = {
        0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
const unsigned char lut_partial_bw[] = {
        0x80	,0x19	,0x01	,0x00	,0x00	,0x01,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
const unsigned char lut_partial_bb[] = {
        0x40	,0x19	,0x01	,0x00	,0x00	,0x01,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
const unsigned char lut_partial_wb[] = {
        0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
        0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

void epaper_write_array(epaper_t *display, bool data, const uint8_t *src, size_t len) {
    if (display->critical_section != NULL) {
        critical_section_enter_blocking(display->critical_section);
    }
    gpio_put(display->dc_pin, data);
    gpio_put(display->cs_pin, 0);
    spi_write_blocking(display->spi, src, len);
    gpio_put(display->cs_pin, 1);
    if (display->critical_section != NULL) {
        critical_section_exit(display->critical_section);
    }
}

void epaper_write(epaper_t *display, bool data, int len, ...) {
    va_list valist;
    va_start(valist, len);
    uint8_t src[len];
    for (int i = 0; i < len; i++) {
        src[i] = va_arg(valist, int);
    }
    va_end(valist);

    epaper_write_array(display, data, src, len);
}

void epaper_command(epaper_t *display, uint8_t command, int len, ...) {
    va_list valist;
    va_start(valist, len);
    uint8_t src[len];
    for (int i = 0; i < len; i++) {
        src[i] = va_arg(valist, int);
    }
    va_end(valist);

    epaper_write_array(display, 0, &command, 1);
    epaper_write_array(display, 1, src, len);
}

void epaper_wait(epaper_t *display) {
    do {
        sleep_us(10);  // after 10 uS busy should be low
    } while (!gpio_get(display->busy_pin));
}

void epaper_update(epaper_t *display, bool partial) {
    // Reset
    gpio_put(display->rst_pin, 0);
    sleep_ms(1);  // 10 uS reset signal should be enough
    gpio_put(display->rst_pin, 1);
    epaper_wait(display);

    // Booster soft start
    epaper_command(display, 0x06, 3, 0x17, 0x17, 0x17);

    // Power setting
    epaper_command(display, 0x01, 4, 0x03, 0x00, 0x2b, 0x2b);

    // Power on
    epaper_command(display, 0x04, 0);
    epaper_wait(display);

    // Panel setting
    epaper_command(display, 0x00, 2, 0xbf, 0x0d);  // second data byte undocumented

    // PLL control
    epaper_command(display, 0x30, 1, 0x3c);

    // Resolution setting
    epaper_command(display, 0x61, 4, 0x01, 0x90, 0x01, 0x2c);

    // VCM_DC setting
    epaper_command(display, 0x82, 1, 0x28);

    // Vcom and data interval setting
    epaper_command(display, 0x50, 1, display->black_border ? 0x77 : 0x97);

    // LUT
    epaper_write(display, 0, 1, 0x20);
    epaper_write_array(display, 1, !partial ? lut_vcom0 : lut_partial_vcom0, 44);
    epaper_write(display, 0, 1, 0x21);
    epaper_write_array(display, 1, !partial ? lut_ww : lut_partial_ww, 42);
    epaper_write(display, 0, 1, 0x22);
    epaper_write_array(display, 1, !partial ? lut_bw : lut_partial_bw, 42);
    epaper_write(display, 0, 1, 0x23);
    epaper_write_array(display, 1, !partial ? lut_wb : lut_partial_wb, 42);
    epaper_write(display, 0, 1, 0x24);
    epaper_write_array(display, 1, !partial ? lut_bb : lut_partial_bb, 42);

    // Transport old data
    epaper_write(display, 0, 1, 0x10);
    epaper_write_array(display, 1, display->previous_buffer, display->height * display->width / 8);

    // Transport new data
    epaper_write(display, 0, 1, 0x13);
    epaper_write_array(display, 1, display->buffer, display->height * display->width / 8);

    // Swap buffers
    memcpy(display->previous_buffer, display->buffer, display->height * display->width / 8);

    // Display refresh
    epaper_command(display, 0x12, 0);
    epaper_wait(display);

    // Border floating
    epaper_command(display, 0x50, 1, 0x17);

    // Power off
    epaper_command(display, 0x02, 0);

    // Enter into deep sleep mode
    epaper_command(display, 0x07, 1, 0xa5);
}