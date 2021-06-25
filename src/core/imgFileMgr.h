
#ifndef IMGFILEMGR_H
#define IMGFILEMGR_H

#include <stdexcept>
#include <QSet>
#include <QString>
#include <QStringList>

class QFileInfo;

class ImgFileMgr
{
public:

	enum Flags {
		RemovableMedia = 0x00000001,	//!< Search on removable media if present (default is not to).
		Writable       = 0x00000002,	//!< Only return writable paths. For directories this means
						//!< that it is possible to create files within the directory.
		Directory      = 0x00000004,	//!< Exclude non-directories.
		File           = 0x00000008,	//!< Exclude non-files.
		New            = 0x00000010	//!< Exclude existing paths.
	};

	static void init();

    static void makeSureDirExistsAndIsWritable(const QString& dirFullPath);

    static QString findFile(const QString& path, Flags flags=(Flags)0);

    static bool fileFlagsCheck(const QFileInfo& thePath, const Flags& flags);

    static bool exists(const QString& path);

    static bool isAbsolute(const QString& path);

    static bool isReadable(const QString& path);

    static bool isWritable(const QString& path);

    static bool isDirectory(const QString& path);

private:
#ifdef Q_OS_WIN
    static QString getWin32SpecialDirPath(int csidlId);
#endif

    static QString userDir_;
    static QStringList fileLocations_;
};

#endif
