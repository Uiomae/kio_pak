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

#ifndef __KPAK_H_
#define __KPAK_H_

#include <karchive.h>
#include "libpak.h"

class KPak : public KArchive {
	public:
		KPak(const QString &filename);
		KPak(QIODevice *dev);
		~KPak();

		QString filename() { return _filename; }
		/*
		 * Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool prepareWriting( const QString& name, const QString& user, const QString& group, uint size ) { Q_UNUSED(name); Q_UNUSED(user); Q_UNUSED(group); Q_UNUSED(size); return false; }

		/*
		 * Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool doneWriting( uint size ) { Q_UNUSED(size); return false; }

		/*
		 * Writing not supported by this class, will always fail.
		 * @return always false
		 */
		virtual bool writeDir( const QString& name, const QString& user, const QString& group )  { Q_UNUSED(name); Q_UNUSED(user); Q_UNUSED(group); return false; }
	protected:
		/**
		 * Opens the archive for reading.
		 * Parses the directory listing of the archive
		 * and creates the KArchiveDirectory/KArchiveFile entries.
		 *
		 */
		virtual bool openArchive( int mode );
		virtual bool closeArchive();

	private:
		QString _filename;
		libPak *_pakFile;
};

#endif //__KPAK_H_

