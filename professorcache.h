#include "professordatabase.h"

class ProfessorCache : public ProfessorDatabase
{
public:
    explicit ProfessorCache(QObject *parent = nullptr)
        : ProfessorDatabase(parent) {}
};
