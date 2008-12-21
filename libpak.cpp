#include "libpak.h"
#include <string>

#include <iostream>

libPak::libPak(QIODevice *dev):_dev(dev) {
	memset(_magic, 0, 8);
}

libPak::~libPak() {
	_dev = NULL;
}

bool libPak::isValidPak() {
	if (!_magic[0]) {
		// Store position
		QIODevice::Offset lastOffset = _dev->at();
		_dev->reset();
		_dev->readBlock(_magic, 8);
		// Restore original position
		_dev->at(lastOffset);

		std::cout << _magic << std::endl;

	}
	std::cout << _magic << std::endl;

	return (qstrncmp(_magic, "SceeWhPk", 8) == 0);
}
