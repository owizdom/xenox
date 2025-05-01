#include <stdio.h>
#include <stdint.h>

#define WIDTH 64
#define HEIGHT 32

// Chip-8 specs: 4KB Memory, 16 8-bit registers
uint8_t screen[WIDTH * HEIGHT];     // 2048 pixels
uint8_t memory[4096];               // 4KB RAM
uint8_t V[16];                      // General purpose registers (V0-VF)
uint16_t I;                         // Index register
uint16_t PC = 0x200;                // Program Counter starts at 0x200
uint8_t delay_timer;                // Delay timer
uint8_t sound_timer;                // Sound timer

// Fontset (80 bytes)
uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Load fontset into memory
void load_fontset() {
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}

// Fetch opcode from memory
uint16_t fetch_opcode() {
    uint16_t opcode = memory[PC] << 8 | memory[PC + 1];
    PC += 2;
    return opcode;
}

// Execute opcode
void execute_opcode(uint16_t opcode) {
    switch (opcode) {
        case 0x00E0:
            for (int i = 0; i < WIDTH * HEIGHT; i++) {
                screen[i] = 0; // Clear screen
            }
            printf("Screen Cleared!\n");
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            break;
    }
}

// 🟢 Planned Improvements — placeholders

// Initialize display (SDL/OpenGL stub)
void initialize_display() {
    printf("[TODO] Initialize graphics (SDL/OpenGL)\n");
}

// Draw the current screen
void draw_screen() {
    printf("[TODO] Render display output\n");
}

// Handle keypad input
void handle_input() {
    printf("[TODO] Handle hex keypad input\n");
}

// Load external ROM into memory
void load_rom(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open ROM: %s\n", filename);
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    if (size > (4096 - 512)) {
        printf("ROM too large!\n");
        fclose(file);
        return;
    }
    fread(&memory[0x200], size, 1, file);
    fclose(file);
    printf("Loaded ROM (%ld bytes)\n", size);
}

// Update timers
void update_timers() {
    if (delay_timer > 0) delay_timer--;
    if (sound_timer > 0) {
        if (--sound_timer == 0) {
            printf("BEEP!\n");
        }
    }
}

int main(int argc, char** argv) {
    printf("=== Xenox Chip-8 Emulator ===\n");

    // Load fontset
    load_fontset();

    // If ROM path provided, load it
    if (argc > 1) {
        load_rom(argv[1]);
    } else {
        printf("No ROM provided. Running default test opcode (00E0).\n");
        memory[0x200] = 0x00;
        memory[0x201] = 0xE0;
    }

    // Initialize display (placeholder)
    initialize_display();

    // Main emulation loop (simplified)
    for (int cycles = 0; cycles < 10; cycles++) { // test loop
        uint16_t opcode = fetch_opcode();
        execute_opcode(opcode);
        update_timers();
        draw_screen();
        handle_input();
    }

    printf("PC now at: 0x%X\n", PC);
    printf("=== Emulation Finished ===\n");

    return 0;
}
