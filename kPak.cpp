#include "kPak.h"
#include "qfile.h"

KPak::KPak(const QString &filename) : KArchive(new QFile(filename)), _filename(filename) {
}

KPak::KPak(QIODevice *dev): KArchive(dev), _filename("") {
}

KPak::~KPak() {
	if (!_filename.isEmpty())
		delete _dev;
}

