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

	QStringList files(_pakFile->getAllFiles());
	
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		QString temp((*it).mid(1));
		temp.replace("\\", "/");
		int pos = temp.findRev("/");
		QString file;
		file = temp.mid(pos + 1);
		// TODO: Add position and size
		KArchiveEntry *e = new KArchiveFile(this, file, 0444, 0, /*uid*/0, /*gid*/0, /*symlink*/0, 0, 0);
		if (pos == -1) {
			rootDir()->addEntry(e);
		} else {
			QString path = QDir::cleanDirPath( (*it).mid(1).replace("\\", "/").left(pos) );
			KArchiveDirectory *d = findOrCreate(path);
			d->addEntry(e);
		}
	}
	return true;
}

bool KPak::closeArchive() {
	return true;
}

