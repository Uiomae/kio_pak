#include "libpak.h"
#include <qstringlist.h> 
#include <qregexp.h>
#include <qmap.h>

libPak::libPak(QIODevice *dev):_dev(dev), _type(PakUnknown) {
	// Store IODevice original position
	QIODevice::Offset lastOffset = _dev->at();

	/* Get type */
	_dev->reset();
	_dev->readBlock(_magic, 8);

	if (qstrncmp(_magic, "SceeWhPk", 8) == 0) {
		_type = PakPK;
	} else if (qstrncmp(_magic, "SceeWhPC", 8) == 0)
		// This type has CRC information along with files
		_type = PakPC;

	if (_type != PakUnknown) {
		/* Get date and time */
		char dateTime[20];
		_dev->readBlock(dateTime, 20);
		// TODO: Parse date and time
		//_dateTime.fromString(dateTime, 

		/* Get files header size and number of files */
		_dev->readBlock((char*)&_headSize, 4);
		_dev->readBlock((char*)&_nFiles, 4);

		/* Get extensions */
		_dev->at(0x0114);
		char exts[0x84];
		_dev->readBlock(exts, 0x84);
		QStringList extsList;
		for (int j = 0; j < 0x84; j += 4) {
			if (exts[j])
				extsList.append(QCString(exts+j));
		}

		/* Get files */
		char filesBuf[_headSize];
		_dev->readBlock(filesBuf, _headSize);
		register int offset = 0;

		// TODO: Find a better way to reserve space on the list!
		for (unsigned int j = 0; j < _nFiles; j++) { _files.push_back(PakFile()); }
		QString last("");
		for (unsigned int j = 0; j < _nFiles; j++) {
			_files[j].offset = (*(unsigned int *)(filesBuf+offset) & 0x00FFFFFF) * 0x800;
			offset += 3;
			unsigned int save = *(unsigned char *)(filesBuf+offset);
			unsigned int strLen = filesBuf[offset + 1] - 1;
			// Unknown use!
			unsigned int dir = *(unsigned short *)(filesBuf+offset + 2);
			offset += 4;
			_files[j].size = *(unsigned int *)(filesBuf+offset);
			offset += 4;
			if (_type == PakPC) {
				_files[j].CRC = *(unsigned int *)(filesBuf+offset);
				offset += 4;
			}
			last = _files[j].filename = last.left(save).append(QCString(filesBuf+offset, strLen + 1));
			offset += strLen;
			_files[j].filename.append("." + extsList[*(unsigned char*)(filesBuf + offset++) - 1]).prepend("\\");
			_filenamesOnly.push_back(_files[j].filename);
		}
	}

	// Restore original position
	_dev->at(lastOffset);
}

libPak::~libPak() {
	// We're not owners of the object, so nullify it only
	_dev = NULL;
}

QStringList libPak::getFilesAt(const QString &path) {
	QString newPath(path);
	newPath.replace("\\", "\\\\");
	return _filenamesOnly.grep(QRegExp("^" + newPath + "\\\\[^\\\\]+$")).gres(QRegExp("^" + newPath + "\\\\"), "");
}

QStringList libPak::getFoldersAt(const QString &path) {
	QString newPath(path);
	newPath.replace("\\", "\\\\");
	QStringList tempList(_filenamesOnly.grep(QRegExp("^" + newPath + "\\\\[^\\\\]+\\\\[^\\\\]+$")).gres(QRegExp("^" + newPath + "\\\\([^\\\\]+)\\\\.*"), "\\1"));
	// Remove duplicates
	QMap<QString, int> tempMap;
	QStringList::Iterator it = tempList.end();
	while (it != tempList.begin())
	{
		tempMap[*it] = 0;
		it--;
	}
	return tempMap.keys();
}

