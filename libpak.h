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
 * @file libpak.h
 * @author Uiomae
 * @date 2008/12/21
 *
 * @brief libPak and libPak::PakFile file header
 */

#ifndef __LIBPAK_H_
#define __LIBPAK_H_
#pragma once

#include <qvaluelist.h>
#include <qstringlist.h>
#include <qiodevice.h>
#include <qdatetime.h>

/**
 * @class libPak
 *
 * @brief Open and parse a .pak file, getting it contained files
 */
class libPak {
	public:
		/**
		 * @struct PakFile
		 * @brief Represents each file on the .pak archive
		 */
		struct PakFile {
			/// The offset of the file inside the .pak archive
			QIODevice::Offset offset;
			/// The size of the file
			unsigned int size;
			/// The CRC of the file (if .pak is a PC .pak type file)
			unsigned int CRC;
			/// The name of the file
			QString filename;
		};

		/**
		 * @enum PakType
		 * @brief Stores possible values for the type of .pak file
		 */
		enum PakType {
			/// Unknown .pak file type
			PakUnknown,
			/// .pak archive files have CRC
			PakPC = 10,
			/// .pak archive files doesn't have CRC
			PakPK
		};
	private:
		/// The stored QIODevice with the .pak file
		QIODevice *_dev;

		// Pak file stuff
		/// Type of .pak file
		PakType _type;
		/// Date and time of the .pak file (stored on the header)
		QDateTime _dateTime;
		/// Size of the header (which holds files info)
		unsigned int _headSize;
		/// Number of files inside this .pak archive
		unsigned int _nFiles;
		/// Actual files inside the .pak archive
		QValueList<PakFile> _files;
		/// List of the filenames inside the .pak archive
		QStringList _filenamesOnly;
		// End of Pak file stuff

		/// Stores the current magic stamp of the file
		char _magic[8];
	public:
		/**
		 * @brief Opens and parses the .pak file
		 * @param dev The QIODevice which holds the .pak file
		 */
		libPak(QIODevice *dev);
		~libPak();


		/**
		 * @brief Checks if this is a valid .pak file
		 * @returns True if this is a valid .pak file, false otherwise
		 */
		inline bool isValidPak() { return _type != PakUnknown; }

		/**
		 * @brief Obtains a list of all files
		 * @returns A list of all files
		 */
		inline QValueList<PakFile> getAllFiles() { return _files; }

		/**
		 * @brief Obtains a list of all files inside a given path
		 * @param path The path to search files into
		 * @returns A list of files matching the path
		 */
		QStringList getFilesAt(const QString &path);

		/**
		 * @brief Obtains a list of all folders inside a given path
		 * @param path The path to search folders into
		 * @returns A list of folders matching the path
		 */
		QStringList getFoldersAt(const QString &path);
};

#endif // __LIBPAK_H_
