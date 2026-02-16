#include "extractor.h"

#include <QDebug>
#include <QFile>
#include <QDir>

#include <7z.h>
#include <7zFile.h>
#include <7zCrc.h>
#include <7zAlloc.h>

#define kInputBufSize ((size_t)1 << 18)

static const ISzAlloc g_Alloc = { SzAlloc, SzFree };

Extractor::Extractor(QObject *parent) :
    QObject(parent) {

    moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, this, &Extractor::doWork);
}

Extractor::~Extractor() {
    m_thread.quit();
    m_thread.wait();
}

void Extractor::extract(const QString archiveName, const QString outputPath) {
    m_archiveName = archiveName;
    m_outputPath = outputPath;

    m_thread.start();
}

void Extractor::doWork() {
    CFileInStream archiveStream;
    CLookToRead2 lookStream;

    if(InFile_Open(&archiveStream.file, qPrintable(m_archiveName))) {
        qCritical() << "Unable to open input file.";
        return;
    }

    FileInStream_CreateVTable(&archiveStream);
    LookToRead2_CreateVTable(&lookStream, False);
    lookStream.buf = nullptr;

    SRes res = SZ_OK;

    ISzAlloc allocImp = g_Alloc;
    ISzAlloc allocTempImp = g_Alloc;

    lookStream.buf = (Byte *)ISzAlloc_Alloc(&allocImp, kInputBufSize);
    if(!lookStream.buf) {
        res = SZ_ERROR_MEM;
    } else {
        lookStream.bufSize = kInputBufSize;
        lookStream.realStream = &archiveStream.vt;
    }

    CrcGenerateTable();

    CSzArEx db;
    SzArEx_Init(&db);

    if(res == SZ_OK) {
        res = SzArEx_Open(&db, &lookStream.vt, &allocImp, &allocTempImp);
    }

    uint32_t blockIndex = 0xFFFFFFFF;
    Byte *outBuffer = nullptr;
    size_t outBufferSize = 0;

    UInt16 *temp = nullptr;
    size_t tempSize = 0;
    for(int i = 0; i < db.NumFiles; i++) {
        bool isDir = SzArEx_IsDir(&db, i);

        size_t len = SzArEx_GetFileNameUtf16(&db, i, nullptr);
        if(len > tempSize) {
            SzFree(nullptr, temp);
            tempSize = len;
            temp = (UInt16 *)SzAlloc(nullptr, tempSize * sizeof(temp[0]));
            if (!temp) {
                res = SZ_ERROR_MEM;
                break;
            }
        }
        SzArEx_GetFileNameUtf16(&db, i, temp);

        QString sub(QString::fromUtf16(temp));
        sub.remove("sdk/");
        sub.remove("install-root/");
        sub.remove("release/");
        QString path(m_outputPath + "/" + sub);

        if(isDir) {
            QDir dir;
            dir.mkpath(path);
            continue;
        }

        size_t offset = 0;
        size_t outSizeProcessed = 0;
        res = SzArEx_Extract(&db, &lookStream.vt, i, &blockIndex, &outBuffer, &outBufferSize,
                             &offset, &outSizeProcessed, &allocImp, &allocTempImp);
        if(res != SZ_OK) {
            qCritical() << "Unable to extract" << QString::fromUtf16(temp);
            break;
        }

        QFile file(path);
        if(file.open(QIODevice::WriteOnly)) {
            file.write(reinterpret_cast<const char *>(outBuffer + offset), outSizeProcessed);

            file.close();
        }

        emit extractProgress(i, db.NumFiles);
    }

    SzFree(nullptr, temp);
    SzArEx_Free(&db, &allocImp);
    ISzAlloc_Free(&allocImp, lookStream.buf);

    File_Close(&archiveStream.file);

    m_thread.exit();

    emit extractFinished(res);
}
