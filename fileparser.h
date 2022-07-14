#pragma once

#include <QMap>
#include <QMessageBox>
#include <QObject>

class FileParser : public QObject
{
    Q_OBJECT
public:
    explicit FileParser(QObject *parent = nullptr);

public slots:
    void startParsing(const QString &filePath);

signals:
    void progressChanged(const double &value);
    void barDataChanged(const QVariantMap& barData);
    void finished();

private:
    void printMessage(const QString &msg, const QMessageBox::Icon &msgBoxIcon);
    QVariantMap getTop15(const QMap<QString, int>& barData) const;

private:
    QMap<QString, int> m_barData;
};

