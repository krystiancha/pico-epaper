/// Example program for the pico-epaper library

#include <math.h>
#include <memory.h>
#include <pico/stdlib.h>
#include <pico/epaper.h>

#define WIDTH 400
#define HEIGHT 300
#define BUFFER_SIZE (HEIGHT * WIDTH / 8)

uint8_t display_buffer[BUFFER_SIZE];
uint8_t prev_display_buffer[BUFFER_SIZE];

struct epaper display = {
        .spi = spi0,
        .cs_pin = 6,
        .rst_pin = 8,
        .dc_pin = 7,
        .busy_pin = 9,
        .width = WIDTH,
        .height = HEIGHT,
        .black_border = false,
        .buffer = display_buffer,
        .previous_buffer = prev_display_buffer,
};

int main() {
    spi_init(spi0, 4000000);
    gpio_set_function(2, GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);

    gpio_init(display.cs_pin);
    gpio_init(display.rst_pin);
    gpio_init(display.dc_pin);
    gpio_init(display.busy_pin);

    gpio_set_dir(display.cs_pin, GPIO_OUT);
    gpio_set_dir(display.rst_pin, GPIO_OUT);
    gpio_set_dir(display.dc_pin, GPIO_OUT);
    gpio_set_dir(display.busy_pin, GPIO_IN);

    memset(display_buffer, 0xff, BUFFER_SIZE);
    memset(prev_display_buffer, 0xff, BUFFER_SIZE);
    
    // Display contents of the buffer (clear display to white)
    epaper_update(&display, false);
    
    // Draw diagonal lines
    for (int x = 0; x < display.width; ++x) {
        int y = (int) ((float) display.height * (float) x / (float) display.width);

        display.buffer[y * display.width / 8 + x / 8] &= ~(0x80 >> (x & 0x07));
        display.buffer[(display.height - y) * display.width / 8 + x / 8] &= ~(0x80 >> (x & 0x07));
    }

    // Display contents of the buffer
    epaper_update(&display, false);

    // Draw cricles
    for (int x0 = 10; x0 < display.width; x0 += 20) {
        for (int i = 0; i < 100; ++i) {
            int x = x0 + 10 * sinf(2.0f * M_PI * i / 100);
            int y = 10 + 10 * cosf(2.0f * M_PI * i / 100);
            display.buffer[y * display.width / 8 + x / 8] &= ~(0x80 >> (x & 0x07));
        }

        // Perform a pratial refresh
        epaper_update(&display, true);
    }

    // Draw sine wave
    for (int x = 0; x < display.width; ++x) {
        int y = (display.height / 2) + 100 * sin(4.0 * M_PI * x / display.width);

        display.buffer[y * display.width / 8 + x / 8] &= ~(0x80 >> (x & 0x07));
    }

    // Display contents of the buffer
    epaper_update(&display, false);

    while (true) { 
        tight_loop_contents(); 
    }
}