#include "coursedatabase.h"

#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QDebug>
#include <stdexcept>


CourseDatabase::CourseDatabase(QObject *parent)
    : QObject(parent)
{
}

static QString clean(QString s)
{
    s = s.trimmed();
    s.replace("\"", "");
    s.replace(QChar(0x201C), "");
    s.replace(QChar(0x201D), "");
    return s;
}

bool CourseDatabase::loadFromCsv(const QString &filePath)
{
    QFile file(filePath);

    try
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::runtime_error("Failed to open course database file");

        QTextStream in(&file);

        if (!in.atEnd())
            in.readLine(); // skip header

        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList p = line.split(',');
            if (p.size() < 4)
                throw std::runtime_error("Malformed CSV record in course database");

            CourseInfo info;
            info.major = clean(p[0]);
            info.year  = clean(p[1]);
            info.code  = clean(p[2]);
            info.name  = clean(p[3]);

            m_courses[info.code] = info;
        }
    }
    catch (const std::exception &e)
    {
        qDebug() << "Error loading course CSV:" << e.what();
        return false;
    }

    return true;
}



QStringList CourseDatabase::majors() const
{
    QSet<QString> set;
    for (const auto &c : m_courses)
        set.insert(c.major);
    return QStringList(set.begin(), set.end());
}

QStringList CourseDatabase::yearsForMajor(const QString &major) const
{
    QSet<QString> set;
    for (const auto &c : m_courses)
        if (c.major == major)
            set.insert(c.year);
    return QStringList(set.begin(), set.end());
}

QStringList CourseDatabase::coursesForMajorYear(const QString &major, const QString &year) const
{
    QStringList list;
    for (const auto &c : m_courses)
        if (c.major == major && c.year == year)
            list << c.code;
    return list;
}

CourseInfo CourseDatabase::courseByCode(const QString &code) const
{
    return m_courses.value(code);
}
