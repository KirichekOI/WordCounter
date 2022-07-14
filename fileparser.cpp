#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "fileparser.h"

FileParser::FileParser(QObject *parent) : QObject(parent)
{

}

void FileParser::startParsing(const QString &filePath)
{
    QFile file(filePath);
    if  (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printMessage("Can't open file.", QMessageBox::Icon::Critical);
        emit finished();
        return;
    }

    const auto reSeparator { QRegularExpression{ QStringLiteral("[0-9.,:?!\\s\")(—]") } };
    constexpr auto bufLen { 1024*512 };
    auto skipPol { Qt::SkipEmptyParts };

    QString prevBuf{};
    QTextStream textStream{ &file };
    emit progressChanged(0.0);
    m_barData.clear();
    while (!textStream.atEnd()) {
        QString str { prevBuf + textStream.read(bufLen) };
        const auto tokens { str.split(reSeparator, skipPol) };
        for(const auto& token : tokens) {
            ++m_barData[token.toLower()];
        }

        const auto idxLastSep { str.lastIndexOf(reSeparator) };
        if(idxLastSep != -1) {
            const auto partialStr { str.mid(idxLastSep + 1) };
            if(!partialStr.isEmpty() && !textStream.atEnd()) {
                --m_barData[partialStr];
                if(!m_barData[partialStr]) {
                    m_barData.remove(partialStr);
                }
            }
            prevBuf = partialStr;
        }
        emit progressChanged(file.pos() / (double)file.size());
        emit barDataChanged(getTop15(m_barData));
    }
    emit finished();
    printMessage("Word count is finished", QMessageBox::Icon::Information);
}

void FileParser::printMessage(const QString &msg, const QMessageBox::Icon &msgBoxIcon)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Word counter");
    msgBox.setIcon(msgBoxIcon);
    msgBox.setText(msg);
    msgBox.setButtonText(QMessageBox::Ok, "OK");
    msgBox.setModal(true);
    msgBox.exec();
}

QVariantMap FileParser::getTop15(const QMap<QString, int> &barData) const
{
    QMultiMap<int, QString> barDataSorted;
    std::for_each(barData.keyValueBegin(), barData.keyValueEnd(),
    [&barDataSorted](const auto& token) {
        barDataSorted.insert(token.second, token.first);
    });
    QVariantMap returnedBarData;
    QMutableMapIterator<int, QString> it{ barDataSorted };
    it.toBack();
    auto bound { 15 };
    while (it.hasPrevious() && bound--) {
        it.previous();
        returnedBarData[it.value()] = it.key();
    }
    return returnedBarData;
}
