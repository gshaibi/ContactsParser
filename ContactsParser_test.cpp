
#include "ContactsParser.hpp"

int main()
{
    std::ifstream file("ex_v7");
    if (file.is_open())
    {
		std::ofstream dest("contacts.txt");
		if (dest.is_open())
		{
        	ContactsParser p(file, dest);
		}
    }
    return 0;
}
