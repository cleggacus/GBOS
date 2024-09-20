#include <gb/gb.h>
#include "core/date_time.h"
#include "core/input.h"
#include "state/state.h"

void main(void) {
    while(1) {
        update_input();
        update_date_time();
        update_state();
        wait_vbl_done();
    }
}
