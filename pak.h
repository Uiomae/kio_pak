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

#ifndef __PAK_H_
#define __PAK_H_
#pragma once

#include <karchive.h>
#include <kio/global.h>
#include <kio/slavebase.h>

static const int PAK_DEBUG_ID = 7000;	// kio

/**
 * Ths class implements a sample kio-slave for KDE3
 */
class PakProtocol : public KIO::SlaveBase {
	public:
		PakProtocol(const QCString &pool, const QCString &app);

		virtual void listDir(const KURL &url);
		virtual void stat(const KURL &url);
		virtual void get(const KURL &url);
	
	private:
		void createUDSEntry(const KArchiveEntry *archiveEntry, KIO::UDSEntry &entry);

		bool checkNewFile(const KURL &url, QString &path, KIO::Error &errorNum);

		KArchive *_pakFile;
		QString _pakFileName;
		time_t _pakFileTime;
};

#endif // __PAK_H_

