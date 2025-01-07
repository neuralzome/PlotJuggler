#include "error_collectors.h"
#include <QMessageBox>
#include <QDebug>

void FileErrorCollector::RecordError(absl::string_view filename, int line, int column,
                                     absl::string_view message) {
    auto msg = QString("File: [%1] Line: [%2] Column: [%3] Message: %4\n")
                   .arg(QString::fromStdString(std::string(filename)))
                   .arg(line)
                   .arg(column)
                   .arg(QString::fromStdString(std::string(message)));
    _errors.push_back(msg);
}

void FileErrorCollector::RecordWarning(absl::string_view filename, int line, int column,
                                       absl::string_view message) {
    auto msg = QString("Warning in file [%1] Line: %2 Column: %3 Message: %4")
                   .arg(QString::fromStdString(std::string(filename)))
                   .arg(line)
                   .arg(column)
                   .arg(QString::fromStdString(std::string(message)));
    qDebug() << msg;
}


void IoErrorCollector::RecordError(int line, google::protobuf::io::ColumnNumber column,
                                   absl::string_view message) {
    _errors.push_back(
        QString("Line: [%1] Column: [%2] Message: %3\n")
            .arg(line)
            .arg(column)
            .arg(QString::fromStdString(std::string(message))));
}

void IoErrorCollector::RecordWarning(int line, google::protobuf::io::ColumnNumber column,
                                     absl::string_view message) {
    qDebug() << QString("Warning Line: [%1] Column: [%2] Message: %3")
                    .arg(line)
                    .arg(column)
                    .arg(QString::fromStdString(std::string(message)));
}