#ifndef PROFESSORDATABASE_H
#define PROFESSORDATABASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

struct ProfessorInfo {
    QString major;
    QString year;
    QString courseCode;
    QString professorInfo;
    QString bookTitle;
    QString author;
    QString isbn;
    QString summary;
};

class ProfessorDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ProfessorDatabase(QObject *parent = nullptr);

    bool loadFromCsv(const QString &filePath);

    QVector<ProfessorInfo> professorsForCourse(const QString &courseCode) const;

    // all class
    QStringList allCoursesForProfessor(const QString &profName) const;

    // ISBN -match with professor
    ProfessorInfo byIsbn(const QString &isbn) const;

private:
    QVector<ProfessorInfo> m_professors;
};

#endif
// PROFESSORDATABASE_H
