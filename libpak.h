/*
	kio_pak: A KDE3 kioslave to explore and modify SingStar .pak files
	Copyright (C) 2008  Uiomae <uiomae@gmail.com>

	This file is part of kio_pak.

	kio_pak is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	kio_pak is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with kio_pak.  If not, see <http://www.gnu.org/licenses/>.
 */

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
		inline QValueList<PakFile> getAllFiles() { return _files; }
		QStringList getFilesAt(const QString &path);
		QStringList getFoldersAt(const QString &path);
};

#endif // __LIBPAK_H_
