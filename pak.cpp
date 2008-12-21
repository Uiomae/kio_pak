#include "pak.h"
#include "kPak.h"

#include <stdlib.h>

// KDE includes
#include <kde_file.h>
#include <klocale.h>
#include <kdebug.h>
#include <kinstance.h>
#include <kremoteencoding.h>

// QT includes
#include <qfile.h>

extern "C" { int KDE_EXPORT kdemain(int argc, char **argv); }

int kdemain( int argc, char **argv )
{
	KInstance instance( "kio_pak" );

	kdDebug(PAK_DEBUG_ID) << "Starting " << getpid() << endl;

	if (argc != 4)
	{
		fprintf(stderr, "Usage: kio_pak protocol domain-socket1 domain-socket2\n");
		exit(-1);
	}

	PakProtocol slave(argv[2], argv[3]);
	slave.dispatchLoop();

	kdDebug(PAK_DEBUG_ID) << "Done" << endl;
	return 0;
}

PakProtocol::PakProtocol(const QCString &pool, const QCString &app): SlaveBase("pak", pool, app) {
	kdDebug(PAK_DEBUG_ID) << "PakProtocol::PakProtocol" << endl;
}

void PakProtocol::listDir(const KURL &url) {
	QString path;
	KIO::Error errorNum;
	if ( !checkNewFile( url, path, errorNum ) )
	{
		if ( errorNum == KIO::ERR_CANNOT_OPEN_FOR_READING )
		{
			// If we cannot open, it might be a problem with the archive header (e.g. unsupported format)
			// Therefore give a more specific error message
			error( KIO::ERR_SLAVE_DEFINED,
					i18n( "Could not open the file, probably due to an unsupported file format.\n%1")
					.arg( url.prettyURL() ) );
			return;
		}
		else if ( errorNum != KIO::ERR_IS_DIRECTORY )
		{
			// We have any other error
			error( errorNum, url.prettyURL() );
			return;
		}
		// It's a real dir -> redirect
		KURL redir;
		redir.setPath( url.path() );
		kdDebug( PAK_DEBUG_ID ) << "Ok, redirection to " << redir.url() << endl;
		redirection( redir );
		finished();
		// And let go of the tar file - for people who want to unmount a cdrom after that
		delete _pakFile;
		_pakFile = 0L;
		return;
	}

	if ( path.isEmpty() )
	{
		KURL redir( url.protocol() + QString::fromLatin1( ":/") );
		kdDebug( PAK_DEBUG_ID ) << "url.path()==" << url.path() << endl;
		redir.setPath( url.path() + QString::fromLatin1("/") );
		kdDebug( PAK_DEBUG_ID ) << "ArchiveProtocol::listDir: redirection " << redir.url() << endl;
		redirection( redir );
		finished();
		return;
	}

	path = QString::fromLocal8Bit(remoteEncoding()->encode(path));

	kdDebug( PAK_DEBUG_ID ) << "checkNewFile done" << endl;
	const KArchiveDirectory* root = _pakFile->directory();
	const KArchiveDirectory* dir;
	if (!path.isEmpty() && path != "/")
	{
		kdDebug(PAK_DEBUG_ID) << QString("Looking for entry %1").arg(path) << endl;
		const KArchiveEntry* e = root->entry( path );
		if ( !e )
		{
			error( KIO::ERR_DOES_NOT_EXIST, url.prettyURL() );
			return;
		}
		if ( ! e->isDirectory() )
		{
			error( KIO::ERR_IS_FILE, url.prettyURL() );
			return;
		}
		dir = (KArchiveDirectory*)e;
	} else {
		dir = root;
	}

	QStringList l = dir->entries();
	totalSize( l.count() );

	KIO::UDSEntry entry;
	QStringList::Iterator it = l.begin();
	for( ; it != l.end(); ++it )
	{
		kdDebug(PAK_DEBUG_ID) << (*it) << endl;
		const KArchiveEntry* archiveEntry = dir->entry( (*it) );

		createUDSEntry( archiveEntry, entry );

		listEntry( entry, false );
	}

	listEntry( entry, true ); // ready

	finished();

	kdDebug( PAK_DEBUG_ID ) << "ArchiveProtocol::listDir done" << endl;

	/*kdDebug(PAK_DEBUG_ID) << "Entering listDir()" << endl;
	totalSize(1);		// ONE file (Magic!!)
	KIO::UDSEntry entry;
	createUDSEntry(NULL, entry);
	kdDebug(PAK_DEBUG_ID) << "Giving file away" << endl;
	listEntry(entry, false);
	listEntry(entry, true);		// End
	finished();
	kdDebug(PAK_DEBUG_ID) << "-<> Exiting listDir()" << endl;*/
}

void PakProtocol::stat(const KURL &url) {
	QString path;
	KIO::UDSEntry entry;
	KIO::Error errorNum;
	if ( !checkNewFile( url, path, errorNum ) )
	{
		// We may be looking at a real directory - this happens
		// when pressing up after being in the root of an archive
		if ( errorNum == KIO::ERR_CANNOT_OPEN_FOR_READING )
		{
			// If we cannot open, it might be a problem with the archive header (e.g. unsupported format)
			// Therefore give a more specific error message
			error( KIO::ERR_SLAVE_DEFINED,
					i18n( "Could not open the file, probably due to an unsupported file format.\n%1")
					.arg( url.prettyURL() ) );
			return;
		}
		else if ( errorNum != KIO::ERR_IS_DIRECTORY )
		{
			// We have any other error
			error( errorNum, url.prettyURL() );
			return;
		}
		// Real directory. Return just enough information for KRun to work
		KIO::UDSAtom atom;
		atom.m_uds = KIO::UDS_NAME;
		atom.m_str = url.fileName();
		entry.append( atom );
		kdDebug( PAK_DEBUG_ID ) << "ArchiveProtocol::stat returning name=" << url.fileName() << endl;

		KDE_struct_stat buff;
		if ( KDE_stat( QFile::encodeName( url.path() ), &buff ) == -1 )
		{
			// Should not happen, as the file was already stated by checkNewFile
			error( KIO::ERR_COULD_NOT_STAT, url.prettyURL() );
			return;
		}

		atom.m_uds = KIO::UDS_FILE_TYPE;
		atom.m_long = buff.st_mode & S_IFMT;
		entry.append( atom );

		statEntry( entry );

		finished();

		// And let go of the tar file - for people who want to unmount a cdrom after that
		delete _pakFile;
		_pakFile = 0L;
		return;
	}

	const KArchiveDirectory* root = _pakFile->directory();
	const KArchiveEntry* archiveEntry;
	if ( path.isEmpty() )
	{
		path = QString::fromLatin1( "/" );
		archiveEntry = root;
	} else {
		path = QString::fromLocal8Bit(remoteEncoding()->encode(path));
		archiveEntry = root->entry( path );
	}
	if ( !archiveEntry )
	{
		error( KIO::ERR_DOES_NOT_EXIST, url.prettyURL() );
		return;
	}

	createUDSEntry( archiveEntry, entry );
	statEntry( entry );

	finished();

	/*kdDebug(PAK_DEBUG_ID) << "Entering stat()" << endl;
	KIO::UDSEntry entry;
	createUDSEntry(NULL, entry);
	kdDebug(PAK_DEBUG_ID) << "Giving file away" << endl;
	statEntry(entry);
	finished();
	kdDebug(PAK_DEBUG_ID) << "-<> Exiting stat()" << endl;*/
}

void PakProtocol::get(const KURL &url) {
	kdDebug(PAK_DEBUG_ID) << "Entering get()" << endl;
	mimetype("text/plain");
	QCString str("Hello Pak World!!");
	data(str);
	finished();
	kdDebug(PAK_DEBUG_ID) << "Exiting get()" << endl;
}

void PakProtocol::createUDSEntry(const KArchiveEntry *archiveEntry, KIO::UDSEntry &entry) {
	KIO::UDSAtom atom;
	entry.clear();

	atom.m_uds = KIO::UDS_NAME;
	atom.m_str = remoteEncoding()->decode(archiveEntry->name().local8Bit());
	entry.append(atom);

	atom.m_uds = KIO::UDS_SIZE;
	atom.m_long = archiveEntry->isFile() ? ((KArchiveFile *)archiveEntry)->size() : 0L ;
	entry.append(atom);

	atom.m_uds = KIO::UDS_FILE_TYPE;
	atom.m_long = 444;
	entry.append(atom);
}

bool PakProtocol::checkNewFile(const KURL &url, QString &path, KIO::Error &errorNum) {
	QString fullPath = url.path();

	kdDebug(PAK_DEBUG_ID) << "Entering checkNewFile() - " << fullPath << endl;

	// Are we already looking at that file ?
	if ( _pakFile && _pakFileName == fullPath.left(_pakFileName.length()) )
	{
		// Has it changed ?
		KDE_struct_stat statbuf;
		if ( KDE_stat( QFile::encodeName( _pakFileName ), &statbuf ) == 0 )
		{
			if ( _pakFileTime == statbuf.st_mtime )
			{
				// It hasn't changed, so just return
				path = fullPath.mid( _pakFileName.length() );
				kdDebug(PAK_DEBUG_ID) << "checkNewFile() returning " << path << endl;
				return true;
			}
		}
	}
	kdDebug(PAK_DEBUG_ID) << "Need to open a new file" << endl;

	// Close previous file
	if ( _pakFile )
	{
		_pakFile->close();
		delete _pakFile;
		_pakFile = NULL;
	}

	// Find where the tar file is in the full path
	int pos = 0;
	QString archiveFile;
	path = QString::null;

	int len = fullPath.length();
	if ( len != 0 && fullPath[ len - 1 ] != '/' )
		fullPath += '/';

	kdDebug(PAK_DEBUG_ID) << "the full path is " << fullPath << endl;
	KDE_struct_stat statbuf;
	statbuf.st_mode = 0; // be sure to clear the directory bit
	while ( (pos=fullPath.find( '/', pos+1 )) != -1 )
	{
		QString tryPath = fullPath.left( pos );
		kdDebug(PAK_DEBUG_ID) << fullPath << "  trying " << tryPath << endl;
		if ( KDE_stat( QFile::encodeName(tryPath), &statbuf ) == -1 )
		{
			// We are not in the file system anymore, either we have already enough data or we will never get any useful data anymore
			break;
		}
		if ( !S_ISDIR(statbuf.st_mode) )
		{
			archiveFile = tryPath;
			_pakFileTime = statbuf.st_mtime;
			path = fullPath.mid( pos + 1 );
			kdDebug(PAK_DEBUG_ID) << "fullPath=" << fullPath << " path=" << path << endl;
			len = path.length();
			if ( len > 1 )
			{
				if ( path[ len - 1 ] == '/' )
					path.truncate( len - 1 );
			}
			else
				path = QString::fromLatin1("/");
			kdDebug(PAK_DEBUG_ID) << "Found. archiveFile=" << archiveFile << " path=" << path << endl;
			break;
		}
	}
	if ( archiveFile.isEmpty() )
	{
		kdDebug(PAK_DEBUG_ID) << "checkNewFile(): not found" << endl;
		if ( S_ISDIR(statbuf.st_mode) ) // Was the last stat about a directory?
		{
			// Too bad, it is a directory, not an archive.
			kdDebug(PAK_DEBUG_ID) << "Path is a directory, not an archive." << endl;
			errorNum = KIO::ERR_IS_DIRECTORY;
		}
		else
			errorNum = KIO::ERR_DOES_NOT_EXIST;
		return false;
	}

	if (url.protocol() == "pak") {
		kdDebug(PAK_DEBUG_ID) << "Creating a KPak object on " << archiveFile << endl;
		_pakFile = new KPak(archiveFile);
	} else {
		kdWarning(PAK_DEBUG_ID) << "Protocol " << url.protocol() << " not supported by this IOSlave" << endl;
		errorNum = KIO::ERR_UNSUPPORTED_PROTOCOL;
		return false;
	}

	if ( !_pakFile->open(IO_ReadOnly) )
	{
		kdDebug(PAK_DEBUG_ID) << "Opening " << archiveFile << "failed." << endl;
		delete _pakFile;
		_pakFile = NULL;
		errorNum = KIO::ERR_CANNOT_OPEN_FOR_READING;
		return false;
	}

	_pakFileName = archiveFile;
	return true;
}
