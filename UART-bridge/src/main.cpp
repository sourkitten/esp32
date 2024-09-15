#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h> // TODO - maybe unnecessary
#include <driver/uart.h>
#include <string.h>
#include <esp_system.h>

#define UART_TX 17
#define UART_RX 16
uart_port_t uart = UART_NUM_0;

// initializes the external UART interface
void uart_setup()
{
    int uart_tx = UART_TX;
    int uart_rx = UART_RX;
    
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB
    };
    
    ESP_ERROR_CHECK(uart_param_config(uart, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, uart_tx, uart_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(uart, 1024, 1024, 0, NULL, 0));
}

void uart_read_task(void *pvParameters)
{
    while (1) {
        char c;
        // TODO - mutex and buffering while UART is busy. OR interrupt based write ?
        if (uart_read_bytes(uart, &c, 1, 1000) == 1) {
            printf("%c", c);
        }
    }
}

void uart_write_task(void *pvParameters)
{
    while (1) {
        char c;
        scanf("%c", &c);
        // TODO - mutex and buffering while UART is busy.
        // OR interrupt based write ? 
        // Write gets frozen until scanf receives input.
        // However, it still messes up the output.
        // Anyways, will make sure the threads / mutexes will be prioritized
        if (uart_write_bytes(uart, &c, 1) == 1) {
            //printf("%c", c);
        }
    }
}

extern "C" void app_main(void)
{
    printf("T UART bridge - setup\n");
    uart_setup();
    printf("T UART bridge - read\n");
    xTaskCreate(uart_read_task, "UART_READ", 2048, NULL, 5, NULL);
    //printf("T UART bridge - write\n");
    //xTaskCreate(uart_write_task, "UART_WRITE", 2048, NULL, 5, NULL);
}
