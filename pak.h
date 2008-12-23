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

/**
 * @file pak.h
 * @author Uiomae
 * @date 2008/12/21
 *
 * @brief PakProtocol file header
 */

#ifndef __PAK_H_
#define __PAK_H_
#pragma once

#include <karchive.h>
#include <kio/global.h>
#include <kio/slavebase.h>

/// Debug id to send the debug messages
static const int PAK_DEBUG_ID = 7000;	// kio

/**
 * @class PakProtocol
 * 
 * @brief The actual kioslave, derived from KIO::SlaveBase
 */
class PakProtocol : public KIO::SlaveBase {
	public:
		PakProtocol(const QCString &pool, const QCString &app);

		/**
		  @brief Used when listing directory contents

		  @param url The full URL of the directory (including path to the .pak file)
		*/
		virtual void listDir(const KURL &url);
		/**
		  @brief Used when getting information of a file inside the .pak

		  @param url The full URL of the file (including path to the .pak file)
		*/
		virtual void stat(const KURL &url);
		/**
		  @brief Used when getting file contents

		  @param url The full URL of the file (including path to the .pak file)
		*/
		virtual void get(const KURL &url);
	
	private:
		/**
		  @brief Helper function used to create a new UDS entry from an archive entry

		  @param archiveEntry The retrieved KArchiveEntry, which holds wanted file
		  @param entry The new UDSEntry
		*/
		void createUDSEntry(const KArchiveEntry *archiveEntry, KIO::UDSEntry &entry);

		/**
		  @brief Helper function used to check if we're trying to access the same .pak file, or a directory, or anything else.
		  Also opens the .pak file if unopened.

		  @param url The full URL of the directory/file currently accessed
		  @param path The returned path
		  @param errorNum If it were any errors, this holds them
		*/
		bool checkNewFile(const KURL &url, QString &path, KIO::Error &errorNum);

		/// Stores the current opened .pak file
		KArchive *_pakFile;
		/// Stores the name of the current .pak file
		QString _pakFileName;
		/// Stores the .pak date and time (retrieved from header)
		time_t _pakFileTime;
};

#endif // __PAK_H_

