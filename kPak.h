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
 * @file kPak.h
 * @author Uiomae
 * @date 2008/12/21
 *
 * @brief KPak file header
 */

#ifndef __KPAK_H_
#define __KPAK_H_

#include <karchive.h>
#include "libpak.h"

/**
 * @class KPak
 *
 * @brief Represents a .pak file
 */
class KPak : public KArchive {
	public:
		/**
		  @brief Constructs a KPak object with a given filename
		  @param filename The EXISTING .pak file
		 */
		KPak(const QString &filename);
		/**
		  @brief Constructs a KPak object with a given QIODevice
		  @param dev A VALID QIODevice
		 */
		KPak(QIODevice *dev);
		~KPak();

		/// Returns the associated filename
		QString filename() { return _filename; }

		/**
		 * @brief Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool prepareWriting( const QString& name, const QString& user, const QString& group, uint size ) { Q_UNUSED(name); Q_UNUSED(user); Q_UNUSED(group); Q_UNUSED(size); return false; }

		/**
		 * @brief Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool doneWriting( uint size ) { Q_UNUSED(size); return false; }

		/**
		 * @brief Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool writeDir( const QString& name, const QString& user, const QString& group )  { Q_UNUSED(name); Q_UNUSED(user); Q_UNUSED(group); return false; }
	protected:
		/**
		 * @brief Opens the archive for reading, parses the directory listing of the archive and creates the KArchiveDirectory/KArchiveFile entries.
		 * @param mode Mode used to open the archive
		 */
		virtual bool openArchive( int mode );
		/**
		 * @brief Closes the .pak archive
		 */
		virtual bool closeArchive();

	private:
		/// The stored .pak filename
		QString _filename;
		/// Current .pak file
		libPak *_pakFile;
};

#endif //__KPAK_H_

