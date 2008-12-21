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

