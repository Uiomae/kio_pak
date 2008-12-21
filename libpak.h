#ifndef __LIBPAK_H_
#define __LIBPAK_H_
#pragma once

#include <qiodevice.h>

class libPak {
	private:
		QIODevice *_dev;

		char _magic[8];
	public:
		libPak(QIODevice *dev);
		~libPak();

		bool isValidPak();
};

#endif // __LIBPAK_H_
