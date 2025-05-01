#include <stdio.h>
#include <stdint.h>
#include <unistd.h>     // for sleep
#include <termios.h>    // for terminal settings
#include <fcntl.h>      // for non-blocking input
#include <string.h>
#include <stdlib.h> 

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
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            break;
    }
}

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

// Dino game loop with countdown
void run_dino_game() {
    struct termios oldt, newt;
    int ch;
    int pos = 0;
    int countdown = 15;  // 15-second countdown
    
    // Set non-blocking input
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    fcntl(0, F_SETFL, O_NONBLOCK);
    
    while (countdown > 0) {
        system("clear");  // clear screen
        
        // Display introduction and countdown timer
        if (countdown == 15) {
            printf("=== Xenox Chip-8 Emulator ===\n");
            printf("Xenox is a Chip-8 virtual machine emulator. It supports standard Chip-8 opcodes and allows you to run Chip-8 programs.\n");
            printf("Press 'c' to exit the Dino game at any time.\n");
            printf("\nPress any key to begin the Dino game...\n");
            getchar();
            sleep (5); // Wait for user input before continuing
            system("clear");
        }

        // Display the countdown timer
        printf("Time Left: %d seconds\n\n", countdown);
        
        // Display Dino
        for (int i = 0; i < pos; i++) printf(" ");
        printf("  __\n");
        for (int i = 0; i < pos; i++) printf(" ");
        printf(" /o_)\n");
        for (int i = 0; i < pos; i++) printf(" ");
        printf("/   \\\n");
        
        printf("\n----------------------------------------------\n");
        
        ch = getchar();
        if (ch == 'c' || ch == 'C') break;
        
        pos = (pos + 1) % (WIDTH - 10);

        usleep(15000);  // slow down animation

        // Countdown
        countdown--;
        sleep(1);
    }

    // Restore terminal settings
    tcsetattr(0, TCSANOW, &oldt);

    printf("\nGame Over or Exited - Emulation Ended\n");
}

int main(int argc, char** argv) {
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

    // Original emulation loop
    for (int cycles = 0; cycles < 10; cycles++) {
        uint16_t opcode = fetch_opcode();
        execute_opcode(opcode);
        update_timers();
        draw_screen();
        handle_input();
    }

    printf("PC now at: 0x%X\n", PC);
    printf("=== Emulation Finished ===\n");

    // Run Dino game after emulation
    run_dino_game();

    return 0;
}
