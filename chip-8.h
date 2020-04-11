/* chip-8.h */


#define S_WIDTH 640
#define S_HEIGHT 320
#define mem_size 0x1000

typedef struct
{
    uint8_t video[64 * 32];
    uint8_t memory[mem_size];
    uint8_t V[0x10];
    uint16_t i_register;
    uint16_t pc; /* Program counter */
    uint16_t sp; /* Stack pointer */
   /* TODO: Add SDL delay timer & sound timer */
} chp8_t;

chp8_t * init_chip();
