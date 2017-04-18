#ifndef FILEIO_H
#define FILEIO_H

#include <QApplication>
#include <QFileDialog>

class FileIO: public QWidget {
public:
    void Open();
    void Save();
};

#endif // FILEIO_H
