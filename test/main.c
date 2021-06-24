#include "sput-1.4.0/sput.h"

#include <die.h>

int main() {
	sput_start_testing();

	sput_enter_suite("str");
	sput_leave_suite();

	sput_finish_testing();
	return sput_get_return_value();
}
