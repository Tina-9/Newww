#include "coursedatabase.h"

#include <QFile>
#include <QTextStream>
#include <QSet>

CourseDatabase::CourseDatabase(QObject *parent)
    : QObject(parent)
{
}

bool CourseDatabase::loadFromCsv(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    if (!in.atEnd())
        in.readLine(); // skip header

    while (!in.atEnd())
    {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        const QStringList p = line.split(',');
        if (p.size() < 4) continue;

        CourseInfo info;
        info.major = p[0].trimmed();
        info.year  = p[1].trimmed();
        info.code  = p[2].trimmed();
        info.name  = p[3].trimmed();

        m_courses[info.code] = info;
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
