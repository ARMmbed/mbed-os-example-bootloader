#include "mbed.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

SDBlockDevice sd(D11, D12, D13, D10);
FATFileSystem fs("sd");
FlashIAP flash;

void apply_update(FILE *file, uint32_t address);

int main()
{
    sd.init();
    fs.mount(&sd);

    FILE *file = fopen(MBED_CONF_APP_UPDATE_FILE, "rb");
    if (file != NULL) {
        printf("Firmware update found\r\n");

        apply_update(file, POST_APPLICATION_ADDR);

        fclose(file);
        remove(MBED_CONF_APP_UPDATE_FILE);
    } else {
        printf("No update found to apply\r\n");
    }

    fs.unmount();
    sd.deinit();

    printf("Starting application\r\n");

    mbed_start_application(POST_APPLICATION_ADDR);
}

void apply_update(FILE *file, uint32_t address)
{
    flash.init();

    const uint32_t page_size = flash.get_page_size();
    char *page_buffer = new char[page_size];
    uint32_t addr = address;
    uint32_t next_sector = addr + flash.get_sector_size(addr);
    bool sector_erased = false;
    while (true) {

        // Read data for this page
        memset(page_buffer, 0, sizeof(page_buffer));
        int size_read = fread(page_buffer, 1, page_size, file);
        if (size_read <= 0) {
            break;
        }

        // Erase this page if it hasn't been erased
        if (!sector_erased) {
            flash.erase(addr, flash.get_sector_size(addr));
            sector_erased = true;
        }

        // Program page
        flash.program(page_buffer, addr, page_size);

        addr += page_size;
        if (addr >= next_sector) {
            next_sector = addr + flash.get_sector_size(addr);
            sector_erased = false;

        }
    }
    delete[] page_buffer;

    flash.deinit();
}
