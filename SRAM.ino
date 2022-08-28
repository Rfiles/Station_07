#include <SRAM_23LC.h>

SRAM_23LC SRAM(&SPI, 5, SRAM_23LCV1024);

uint8_t sram_module_num;

const uint32_t  Ram_Size = 0x1FFFF;

bool sram_boot ( uint8_t mybootnum ) {
  sram_module_num = mybootnum;
  MaxReg = Ram_Size / SensorData_size;

  SRAM.begin();
  if (TestRam()) return true;
  return false;

}

void sram_print_info() {
    printf("RAM: Total %04u regs of %04u bytes each.\n", MaxReg, SensorData_size);
}

void EraseRamChip() {
  printf("SRAM erasing.... ");
  uint8_t temp[0xFF]={0};

  for (uint32_t pages; pages < Ram_Size; Ram_Size + 0xFF) {
    printf("%u ", pages);    //debug
    SRAM.writeBlock( pages, 0xFF, temp );
  }
  printf("done\n");
}

bool TestRam() {
  SRAM.writeByte( 50000, 0x55);
  if (SRAM.readByte( 50000 ) == 0x55) return true;
  return false;
}


void sram_write (uint32_t addr, uint32_t dsize, void* data) {
  SRAM.writeBlock( addr, dsize, data );
}

void sram_read (uint32_t addr, uint32_t dsize, void* dest) {
  SRAM.readBlock( addr, dsize, dest );
}



//EOF
