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

