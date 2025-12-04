#include "professordatabase.h"
#include <QFile>
#include <QTextStream>

ProfessorDatabase::ProfessorDatabase(QObject *parent)
    : QObject(parent)
{
}

bool ProfessorDatabase::loadFromCsv(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    m_professors.clear();

    bool firstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (firstLine) { firstLine = false; continue; }

        QStringList parts = line.split(',');

        if (parts.size() < 8) continue;

        ProfessorInfo p;
        p.major         = parts[0];
        p.year          = parts[1];
        p.courseCode    = parts[2];
        p.professorInfo = parts[3];
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
