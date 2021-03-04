#ifndef PICO_EPAPER_EPAPER_H
#define PICO_EPAPER_EPAPER_H

#include <hardware/spi.h>

/// Epaper display configuration
typedef struct epaper {

    /// SPI bus
    ///
    /// SPI bus which has the ENC28J60.
    /// Probably spi0 or spi1 from hardware/spi.h.
    ///
    /// The bus MUST be initialized before calling any epaper_* function.
    /// Use spi_init and gpio_set_function.
    spi_inst_t *spi;

    /// Chip Select pin
    ///
    /// This pin MUST be configured as output before calling any epaper_* function.
    /// Use gpio_init and gpio_set_dir.
    uint8_t cs_pin;

    /// Reset pin
    ///
    /// This pin MUST be configured as output before calling any epaper_* function.
    /// Use gpio_init and gpio_set_dir.
    uint8_t rst_pin;

    /// Data/Command pin
    ///
    /// This pin MUST be configured as output before calling any epaper_* function.
    /// Use gpio_init and gpio_set_dir.
    uint8_t dc_pin;

    /// Busy pin
    ///
    /// This pin MUST be configured as input before calling any epaper_* function.
    /// Use gpio_init and gpio_set_dir.
    uint8_t busy_pin;

    /// Horizontal resolution of the display (px)
    uint16_t width;

    /// Vertical resolution of the display (px)
    uint16_t height;

    /// If set to true, refreshing the display will draw a black border around the content
    ///
    /// Otherwise the border will be white.
    bool black_border;

    /// Pointer to the buffer representing current contents of the display
    uint8_t *buffer;

    /// Pointer to the buffer representing previous contents of the display
    uint8_t *previous_buffer;
} epaper_t;

/// Allocate memory for display buffers and set buffer pointers
void epaper_init(epaper_t *display);

/// Send buffer contents to the display
///
/// \param partial if true, uses partial update lookup table; full update otherwise
void epaper_update(epaper_t *display, bool partial);

#endif //PICO_EPAPER_EPAPER_H
