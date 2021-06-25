#include "imgFileMgr.h"


#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#ifndef _SHOBJ_H
    #include <shlobj.h>
    #include <QLibrary>
#endif
#endif

#define CHECK_FILE "default_cfg.ini"

QString ImgFileMgr::userDir_;

QStringList ImgFileMgr::fileLocations_;
void ImgFileMgr::init()
{

#ifdef Q_OS_WIN
    QString winApiPath = getWin32SpecialDirPath(CSIDL_APPDATA);
    if (!winApiPath.isEmpty())
    {
        userDir_ = winApiPath + "\\ImgRes";
    }
#elif defined(Q_OS_MAC)
    userDir_ = QDir::homePath() + "/Library/Application Support/ImgRes";
#else
    userDir_ = QDir::homePath() + "/.ImgRes";
#endif

    if (!QFile(userDir_).exists())
    {
        qWarning() << "User config directory does not exist: " << QDir::toNativeSeparators(userDir_);
    }
    try
    {
        makeSureDirExistsAndIsWritable(userDir_);
    }
    catch (std::runtime_error &e)
    {
        qFatal("Error: cannot create user config directory: %s", e.what());
    }

    fileLocations_.append(userDir_);

    QFileInfo installLocation(QCoreApplication::applicationDirPath());
    QFileInfo checkFile(installLocation.filePath() + QDir::separator() + QString(CHECK_FILE));
    if (checkFile.exists())
    {
        fileLocations_.append(installLocation.filePath());
    }
}

void ImgFileMgr::makeSureDirExistsAndIsWritable(const QString &dirFullPath)
{
    QFileInfo uDir(dirFullPath);
    if (!uDir.exists())
    {
        // 不存在则创建
        qDebug() << "Creating directory " << QDir::toNativeSeparators(uDir.filePath());
        if (!QDir("/").mkpath(uDir.filePath()))
        {
            throw std::runtime_error(QString("Could not create directory: " +uDir.filePath()).toStdString());
        }
        QFileInfo uDir2(dirFullPath);
        if (!uDir2.isWritable())
        {
            throw std::runtime_error(QString("Directory is not writable: " +uDir2.filePath()).toStdString());
        }
    }
    else if (!uDir.isWritable())
    {
        throw std::runtime_error(QString("Directory is not writable: " +uDir.filePath()).toStdString());
    }
}

QString ImgFileMgr::findFile(const QString &path, ImgFileMgr::Flags flags)
{
    if (path.isEmpty())
    {
        qWarning() << "Empty file path";
        return "";
    }

    if (path.startsWith(":/"))
        return path;

    if (path[0] == '.')
    {
        if (fileFlagsCheck(path, flags))
            return path;
        else
        {
            qWarning() << QString("file does not match flags: %1").arg(path);
            return "";
        }
    }

    if (isAbsolute(path))
    {
        if (fileFlagsCheck(path, flags))
            return path;
        else
        {
            qWarning() << QString("file does not match flags: %1").arg(path);
            return "";
        }
    }

    for (const auto& i : fileLocations_)
    {
        const QFileInfo finfo(i + "/" + path);
        if (fileFlagsCheck(finfo, flags))
            return i + "/" + path;
    }

    return "";
}

bool ImgFileMgr::fileFlagsCheck(const QFileInfo& thePath, const Flags& flags)
{
    const bool exists = thePath.exists();

    if (flags & New)
    {
        if (exists)
            return false;
        QFileInfo pInfo(thePath.dir().absolutePath());
        if (!pInfo.exists() || !pInfo.isWritable())
        {
            return false;
        }
    }
    else if (exists)
    {
        if (flags==0)
            return true;

        if ((flags & Writable) && !thePath.isWritable())
            return false;

        if ((flags & Directory) && !thePath.isDir())
            return false;

        if ((flags & File) && !thePath.isFile())
            return false;
    }
    else
    {
        return false ;
    }

    return true;
}

bool ImgFileMgr::exists(const QString &path)
{
    return QFileInfo(path).exists();
}

bool ImgFileMgr::isAbsolute(const QString &path)
{
    return QFileInfo(path).isAbsolute();
}

bool ImgFileMgr::isReadable(const QString& path)
{
    return QFileInfo(path).isReadable();
}

bool ImgFileMgr::isWritable(const QString& path)
{
    return QFileInfo(path).isWritable();
}

bool ImgFileMgr::isDirectory(const QString& path)
{
    return QFileInfo(path).isDir();
}



#ifdef Q_OS_WIN
QString ImgFileMgr::getWin32SpecialDirPath(int csidlId)
{
    QLibrary library(QLatin1String("shell32"));
    typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, LPTSTR, int, BOOL);
    GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathW");
    if (SHGetSpecialFolderPath)
    {
        TCHAR tpath[MAX_PATH];
        SHGetSpecialFolderPath(0, tpath, csidlId, FALSE);
        return QString::fromUtf16((ushort*)tpath);
    }

    return QString();
}
#endif
