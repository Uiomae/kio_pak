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

#include "kPak.h"
#include <kdebug.h>
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>

KPak::KPak(const QString &filename) : KArchive(new QFile(filename)), _filename(filename) {

}

KPak::KPak(QIODevice *dev): KArchive(dev), _filename("") {
}

KPak::~KPak() {
	if (!_filename.isEmpty())
		delete device();
}

bool KPak::openArchive(int mode) {
	kdDebug(7000) << "openArchiving!";
	if ( mode == IO_WriteOnly )
		return true;
	if ( mode != IO_ReadOnly && mode != IO_ReadWrite )
	{
		kdWarning(7000) << "Unsupported mode " << mode << endl;
		return false;
	}
	
	QIODevice *dev = device();

	if (!dev)
		return false;

	_pakFile = new libPak(dev);
	if (!_pakFile->isValidPak()) {
		kdWarning(7000) << "Invalid file magic" << endl;
		return false;
	}

	QValueList<libPak::PakFile> files(_pakFile->getAllFiles());
	
	for ( QValueList<libPak::PakFile>::Iterator it = files.begin(); it != files.end(); ++it ) {
		QString temp((*it).filename.mid(1));
		temp.replace("\\", "/");
		int pos = temp.findRev("/");
		QString file;
		file = temp.mid(pos + 1);
		// TODO: Add position and size
		KArchiveEntry *e = new KArchiveFile(this, file, 0444, 0, /*uid*/0, /*gid*/0, /*symlink*/0, (int)(*it).offset, (*it).size);
		if (pos == -1) {
			rootDir()->addEntry(e);
		} else {
			QString path = QDir::cleanDirPath( (*it).filename.mid(1).replace("\\", "/").left(pos) );
			KArchiveDirectory *d = findOrCreate(path);
			d->addEntry(e);
		}
	}
	return true;
}

bool KPak::closeArchive() {
	return true;
}

