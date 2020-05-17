// Variable Definitions

// State
int button_press = 3;
int state = 0;

// Switch states
int left_limit_state = 0;
int right_limit_state = 0;
int for_switch_state = 0;
int rev_switch_state = 0;

// Voltage and Current
float curr_read = 0;
float volt_read = 0;
int count = 0;
const int total_count = 100;
const float curr_thres = 5;
const float  volt_thres = 10;
