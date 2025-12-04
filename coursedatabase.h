#ifndef COURSEDATABASE_H
#define COURSEDATABASE_H

#include <QObject>
#include <QMap>

struct CourseInfo
{
    QString major;
    QString year;
    QString code;
    QString name;
};

class CourseDatabase : public QObject
{
    Q_OBJECT
public:
    explicit CourseDatabase(QObject *parent = nullptr);

    bool loadFromCsv(const QString &filePath);

    QStringList majors() const;
    QStringList yearsForMajor(const QString &major) const;
    QStringList coursesForMajorYear(const QString &major,const QString &year) const;

    CourseInfo courseByCode(const QString &code) const;

private:
    // key: courseCode
    QMap<QString, CourseInfo> m_courses;
};

#endif
// COURSEDATABASE_H
