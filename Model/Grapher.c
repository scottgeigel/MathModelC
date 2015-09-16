#include "Grapher.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef enum
{
    CSV_START,
    CSV_ADD_COLS,
    CSV_ADD_ROWS,
    CSV_END
}StateControl;
static struct
{
    FILE *out;
    StateControl state;
    int cols;
    int max_cols;
    int rows;
    char **colnames;
    char **rowdata;
}CSV;
#define MAX_CHARS_PER_CELL 80
void Model_Grapher_Start(FILE* out)
{
    CSV.state = CSV_START;
    CSV.out = out;
    CSV.cols = 0;
    CSV.rows = 0;
    CSV.max_cols = 4;
    CSV.colnames = calloc(CSV.max_cols, sizeof(char*));
    CSV.rowdata = calloc(CSV.max_cols, sizeof(char*));
    int i;
    for (i = 0; i < CSV.max_cols; i++)
    {
        CSV.rowdata[i] = calloc(MAX_CHARS_PER_CELL, sizeof(char));
    }
}

void Model_Grapher_AddColDef(const char* colname)
{
    assert((CSV.state == CSV_START) || (CSV.state == CSV_ADD_COLS));
    CSV.state = CSV_ADD_COLS;
    if (CSV.cols == CSV.max_cols)
    {
        CSV.max_cols *= 2;
        CSV.colnames = realloc(CSV.colnames, CSV.max_cols * sizeof (char *));
        CSV.rowdata  = realloc(CSV.rowdata, CSV.max_cols * sizeof(char[MAX_CHARS_PER_CELL]));
    }
    size_t length = strlen(colname);
    char *tmp = malloc(sizeof(char) * (length+1));
    strncpy(tmp, colname, sizeof(char) * (length+1));
    CSV.colnames[CSV.cols++] = tmp;
}

void Model_Grapher_AddRowToCol(const char* colname, const char* rowdata)
{
    assert((CSV.state == CSV_ADD_ROWS) || (CSV.state == CSV_ADD_COLS));
    CSV.state = CSV_ADD_ROWS;

    size_t length = strlen(rowdata);
    assert(length < MAX_CHARS_PER_CELL);//we aren't supporting this for now

    int i;
    bool found = false;
    //find our colname
    for (i = 0; i < CSV.cols; i++)
    {
        if(strcmp(colname, CSV.colnames[i]) == 0)
        {
            found = true;
            break;
        }
    }
    assert(found);
    strncpy(CSV.rowdata[i], rowdata, sizeof(char) * (length+1));
}

void Model_Grapher_NextRow()
{
    assert(CSV.state == CSV_ADD_ROWS);
    int i;
    if (CSV.rows == 0)
    {
        for (i = 0; i < CSV.cols; i++)
        {
            char delim = ((i+1) == CSV.cols)? '\n' : ',';
            fprintf(CSV.out, "%s%c", CSV.colnames[i], delim);
        }
    }
    CSV.rows++;
    for (i = 0; i < CSV.cols; i++)
    {
        char delim = ((i+1) == CSV.cols)? '\n' : ',';
        fprintf(CSV.out, "%s%c", CSV.rowdata[i], delim);
    }
}

void Model_Grapher_End()
{
    assert(CSV.state == CSV_ADD_ROWS);
    CSV.state = CSV_END;

    int i;
    for (i = 0; i < CSV.cols; i++)
    {
        free(CSV.colnames[i]);
        free(CSV.rowdata[i]);
        CSV.colnames[i] = NULL;
        CSV.rowdata[i] = NULL;
    }
    free(CSV.colnames);
    free(CSV.rowdata);
    CSV.colnames = NULL;
    CSV.rowdata = NULL;
    fflush(CSV.out);
    CSV.out = NULL; //it's your problem to close it... not mine
}
