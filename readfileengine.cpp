#include "readfileengine.h"
#include "fileparser.h"
#include <QCoreApplication>

ReadFileEngine::ReadFileEngine(QObject *parent) :
    QObject(parent),
    m_fileParser{new FileParser()},
    m_workThread{new QThread(this)}
{
    m_fileParser->moveToThread(m_workThread);
    connect(m_fileParser, &FileParser::progressChanged, this, &ReadFileEngine::progressChanged);
    connect(m_fileParser, &FileParser::barDataChanged, this, &ReadFileEngine::barDataChanged);
    connect(m_fileParser, &FileParser::finished, this, &ReadFileEngine::finished);
    connect(m_fileParser, &FileParser::finished, this, [](){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Word counter");
        msgBox.setIcon(QMessageBox::Icon::Information);
        msgBox.setText("Word count is finished");
        msgBox.setModal(true);
        msgBox.exec();

    }, Qt::QueuedConnection);
    connect(this, &ReadFileEngine::start, m_fileParser, &FileParser::startParsing);
    m_workThread->start();
}

ReadFileEngine::~ReadFileEngine()
{
    m_workThread->quit();
    m_workThread->wait();
}

QString ReadFileEngine::filePath() const
{
    return m_filePath;
}

void ReadFileEngine::setFilePath(const QString &filePath)
{
    if (filePath != m_filePath) {
        m_filePath = filePath;
        m_filePath.replace("file://", "");
        emit filePathChanged(m_filePath);
    }
}

void ReadFileEngine::startParsing()
{
    emit start(m_filePath);
}
