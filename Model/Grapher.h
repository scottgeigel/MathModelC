#ifndef __GRAPHER_H__
#define __GRAPHER_H__
#include <stdio.h>
void Model_Grapher_Start(FILE* out);
void Model_Grapher_AddColDef(const char* colname);
void Model_Grapher_AddRowToCol(const char* colname, const char* rowdata);
void Model_Grapher_NextRow();
void Model_Grapher_End();
#endif
