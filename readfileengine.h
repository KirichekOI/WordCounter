#pragma once

#include <QObject>
#include <QPointer>
#include <QVariant>
#include <QThread>

QT_FORWARD_DECLARE_CLASS(FileParser)

class ReadFileEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
public:
    explicit ReadFileEngine(QObject *parent = nullptr);
    ~ReadFileEngine();

    QString filePath() const;
    void setFilePath(const QString &filePath);

    Q_INVOKABLE void startParsing();

signals:
    void start(const QString &filePath);
    void finished();
    void filePathChanged(const QString &filePath);
    void progressChanged(const double &value);
    void barDataChanged(const QVariantMap& barData);

private:
    QString m_filePath;
    QPointer<FileParser> m_fileParser;
    QPointer<QThread> m_workThread;
};

