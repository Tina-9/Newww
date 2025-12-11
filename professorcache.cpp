#include "professorcache.h"
#include <QDebug>

QStringList ProfessorCache::allCoursesForProfessor(const QString &p) const
{
    qDebug() << "[CACHE] Searching professor courses...";
    return ProfessorDatabase::allCoursesForProfessor(p);
}
