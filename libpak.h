#ifndef __LIBPAK_H_
#define __LIBPAK_H_
#pragma once

#include <qvaluelist.h>
#include <qstringlist.h>
#include <qiodevice.h>
#include <qdatetime.h>

class libPak {
	public:
		struct PakFile {
			QIODevice::Offset offset;
			unsigned int size;
			unsigned int CRC;
			QString filename;
		};

		enum PakType {
			PakUnknown, PakPC = 10, PakPK
		};
	private:
		QIODevice *_dev;

		// Pak file stuff
		PakType _type;
		QDateTime _dateTime;
		unsigned int _headSize;
		unsigned int _nFiles;
		QValueList<PakFile> _files;
		QStringList _filenamesOnly;
		// End of Pak file stuff

		char _magic[8];
	public:
		libPak(QIODevice *dev);
		~libPak();

		inline bool isValidPak() { return _type != PakUnknown; }
		QStringList getFilesAt(const QString &path);
};

#endif // __LIBPAK_H_
