#include "professordatabase.h"
#include <QFile>       // These headers enable file I/O.
#include <QTextStream> // This no file output in this file. only file input(same as the coursedatabase.cpp)

ProfessorDatabase::ProfessorDatabase(QObject *parent)
    : QObject(parent)
{
}

bool ProfessorDatabase::loadFromCsv(const QString &filePath)
{
    // This is where the program opens the CSV file for reading.
    // 13-15 This is the file input setup. The CSV file is opened for reading using QFile
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);  //18-19
    m_professors.clear(); // QTextStream is used to read the file line-by-line.

    bool firstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();  // 23-25 Reads each line of the CSV.
        if (line.isEmpty()) continue;
        if (firstLine) { firstLine = false; continue; }

        QStringList parts = line.split(',');  // 27-29:Parses each CSV row into fields.

        if (parts.size() < 8) continue;

        ProfessorInfo p;                  // Then entire block
                                         // This uses the CSV data, No output is written here,only reading.
        p.major         = parts[0];
        p.year          = parts[1];
        p.courseCode    = parts[2];
        p.professorInfo = parts[3].trimmed();
        p.professorInfo.remove("\"");
        p.bookTitle     = parts[4];
        p.author        = parts[5];
        p.isbn          = parts[6];
        p.summary       = parts[7];

        m_professors.push_back(p);
    }

    return true;
}

QVector<ProfessorInfo> ProfessorDatabase::professorsForCourse(const QString &courseCode) const
{
    QVector<ProfessorInfo> list;
    for (const auto &p : m_professors)
    {
        if (p.courseCode == courseCode)
            list.push_back(p);
    }
    return list;
}

QStringList ProfessorDatabase::allCoursesForProfessor(const QString &profName) const
{
    QStringList list;
    for (const auto &p : m_professors)
    {
        if (p.professorInfo == profName)
            list << p.courseCode;
    }
    list.removeDuplicates();
    return list;
}

ProfessorInfo ProfessorDatabase::byIsbn(const QString &isbn) const
{
    for (const auto &p : m_professors)
    {
        if (p.isbn == isbn)
            return p;
    }
    return ProfessorInfo{};
}


