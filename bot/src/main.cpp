#include "Banbot.hpp"


int main() {
	Banbot bot;

	try {
		bot.parse_conf_file();
		bot.initial_connection();
		bot.routine();
	}
	catch (exception& x) {
		cout << RED << x.what() << RESET << endl;
	}
}
