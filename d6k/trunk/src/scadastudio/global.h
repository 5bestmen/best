#ifndef GLOBAL_H
#define GLOBAL_H

//#include <QString>

extern IMainModuleInterface *s_pGlobleCore;

bool CheckTagNameIsValid(const QString &strTagName, const char *szModuleName);
bool CheckDescIsValid(const QString &strDesc, const char *szModuleName, int nCounts = MAX_NAME_LENGTH_SCADASTUDIO);

#endif
