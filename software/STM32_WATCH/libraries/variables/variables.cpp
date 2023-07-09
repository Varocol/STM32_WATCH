#include "variables.h"

volatile Key_Status key1_status = Released;
volatile Key_Status key2_status = Released;
volatile Key_Status key3_status = Released;
volatile Key_Status key4_status = Released;

volatile uint32_t key1_status_time;
volatile uint32_t key2_status_time;
volatile uint32_t key3_status_time;
volatile uint32_t key4_status_time;