#include <avr/io.h>
#include <util/delay.h>

#define LED1_PIN PB0
#define LED2_PIN PB1
#define LED3_PIN PB2
#define LED4_PIN PB3
#define LED5_PIN PB4

#define NUM_COMBINATIONS 8

void init_leds() {
    // Set LED pins as output
    DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN) | (1 << LED5_PIN);
}

void blink_leds(uint8_t patterns[]) {
    if (patterns[0]) {
        PORTB |= (1 << LED1_PIN);
    } else {
        PORTB &= ~(1 << LED1_PIN);
    }

    if (patterns[1]) {
        PORTB |= (1 << LED2_PIN);
    } else {
        PORTB &= ~(1 << LED2_PIN);
    }

    if (patterns[2]) {
        PORTB |= (1 << LED3_PIN);
    } else {
        PORTB &= ~(1 << LED3_PIN);
    }

    if (patterns[3]) {
        PORTB |= (1 << LED4_PIN);
    } else {
        PORTB &= ~(1 << LED4_PIN);
    }

    if (patterns[4]) {
        PORTB |= (1 << LED5_PIN);
    } else {
        PORTB &= ~(1 << LED5_PIN);
    }

    _delay_ms(500);  // Delay for 500 milliseconds
}

int arrays_are_equal(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    init_leds();  // Initialize LED pins

    int f_ref[NUM_COMBINATIONS];
    int f1[NUM_COMBINATIONS];
    int f2[NUM_COMBINATIONS];
    int f3[NUM_COMBINATIONS];
    int f4[NUM_COMBINATIONS];

    int index = 0;

    for (int x = 0; x <= 1; x++) {
        for (int y = 0; y <= 1; y++) {
            for (int z = 0; z <= 1; z++) {
                f_ref[index] = x || (y && z);
                f1[index] = x || z || (x && y);
                f2[index] = (x || y) && (x || z);
                f3[index] = x || (x && y) || (y && z);
                f4[index] = x || (x && z) || (x && y);
                index++;
            }
        }
    }

    index = 0;

    while (1) {
        uint8_t patterns[5] = {f_ref[index], f1[index], f2[index], f3[index], f4[index]};
        blink_leds(patterns);

        index++;
        if (index >= NUM_COMBINATIONS) {
            index = 0;
        }
    }

    return 0;
