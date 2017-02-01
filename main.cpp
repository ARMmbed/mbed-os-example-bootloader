#include "mbed.h"

DigitalOut led1(LED1);

int main() {
    int i = 0;
    while (true) {
        printf("Application count %i\r\n", i);
        led1 = !led1;
        i++;
        wait(0.5);
    }
}

int main_bl()
{
    int i;
    for (i = 0; i < 3; i++) {
        printf("Bootloader count %i\r\n", i);
        led1 = !led1;
        wait(0.5);
    }
    printf("Transferring control to application\r\n");
    wait(0.5);
    mbed_application_start(MAIN_ADDR);
    return 0;
}
