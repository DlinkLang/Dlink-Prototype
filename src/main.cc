#include <iostream>
#include <memory>

#include "Driver.hh"

int main()
{
	Dlink::Driver driver;
	driver.parse(std::cin);
}
