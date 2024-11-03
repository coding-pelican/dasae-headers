#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../dh/debug/debug_assert.h"
#include "../dh/primitive.h"


#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

enum {
    ALLOC_SIZE           = 64,
    COLUMN_USERNAME_SIZE = 32,
    COLUMN_EMAIL_SIZE    = 255,
    TABLE_MAX_PAGES      = 100
};

typedef struct {
    char*   Buffer;
    size_t  BufferLength;
    ssize_t InputLength;
} InputBuffer;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} EExecuteResult;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} EMetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} EPrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} EStatementType;

typedef struct {
    uint32_t ID;
    char     Username[COLUMN_USERNAME_SIZE + 1];
    char     Email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
    EStatementType Type;
    Row            RowToInsert;
} Statement;

const uint16_t ID_SIZE         = size_of_attribute(Row, ID);
const uint16_t USERNAME_SIZE   = size_of_attribute(Row, Username);
const uint16_t EMAIL_SIZE      = size_of_attribute(Row, Email);
const uint16_t ID_OFFSET       = 0;
const uint16_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint16_t EMAIL_OFFSET    = USERNAME_OFFSET + USERNAME_SIZE;
const uint16_t ROW_SIZE        = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const uint32_t PAGE_SIZE       = 4096;
const uint32_t ROWS_PER_PAGE   = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS  = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    int      FileDescriptor;
    uint32_t FileLength;
    void*    Pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    Pager*   Pager;
    uint32_t NumRows;
} Table;

InputBuffer* NewInputBuffer() {
    InputBuffer* inputBuffer  = (InputBuffer*)malloc(sizeof(InputBuffer));
    inputBuffer->Buffer       = NULL;
    inputBuffer->BufferLength = 0;
    inputBuffer->InputLength  = 0;

    return inputBuffer;
}

void PrintPrompt() { printf("db > "); }

ssize_t GetLine(char** lineptr, size_t* n, FILE* stream) {
    debug_assertFmt(lineptr != NULL && n != NULL && stream != NULL, "NULLs in parameters are not allowed");
    if (!*lineptr) {
        *lineptr = (char*)malloc(ALLOC_SIZE);
        if (!*lineptr) {
            return -1;
        }
        *n = ALLOC_SIZE;
    }
    char*  cur = NULL;
    size_t len = 0;
    while (!feof(stream)) {
        cur = *lineptr + len;
        if (!fgets(cur, (int)(*n - len), stream)) {
            return -1;
        }
        len += strlen(cur);
        if ((*lineptr)[len - 1] != '\n') { // * 개행 문자 확인
            char* new = (char*)realloc(*lineptr, *n += ALLOC_SIZE);
            if (!new) {
                return -1;
            }
            *lineptr = new;

        } else {
            return (*lineptr)[--len] = 0, len; // NOLINT
        }
    }
    return -1;
}

void ReadLine(InputBuffer* inputBuffer) {
    ssize_t bytesRead = GetLine(&(inputBuffer->Buffer), &(inputBuffer->BufferLength), stdin);
    if (bytesRead <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE); // NOLINT
    }
    // * Ignore trailing newline
    // inputBuffer->InputLength = bytesRead - 1;
    // inputBuffer->Buffer[bytesRead - 1] = 0;
}

Pager* OpenPager(const char* filename) {
    int fileDescriptor = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

    if (fileDescriptor == -1) {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t fileLength = lseek(fileDescriptor, 0, SEEK_END);

    Pager* pager          = malloc(sizeof(Pager));
    pager->FileDescriptor = fileDescriptor;
    pager->FileLength     = fileLength;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->Pages[i] = NULL;
    }

    return pager;
}

Table* OpenDB(const char* filename) {
    Pager*   pager   = OpenPager(filename);
    uint32_t numRows = pager->FileLength / ROW_SIZE;

    Table* table   = malloc(sizeof(Table));
    table->Pager   = pager;
    table->NumRows = numRows;

    return table;
}

void* GetPage(Pager* pager, uint32_t pageNum) {
    if (pageNum > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out of bounds. %d > %d\n", pageNum, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->Pages[pageNum] == NULL) {
        // Cache miss. Allocate memory and load from file.
        void*    page     = malloc(PAGE_SIZE);
        uint32_t numPages = pager->FileLength / PAGE_SIZE;

        if (pager->FileLength % PAGE_SIZE) {
            numPages += 1;
        }

        if (pageNum <= numPages) {
            lseek(pager->FileDescriptor, (i32)(pageNum * PAGE_SIZE), SEEK_SET);
            ssize_t bytes_read = read(pager->FileDescriptor, page, PAGE_SIZE);
            if (bytes_read == -1) {
                printf("Error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
        pager->Pages[pageNum] = page;
    }
    return pager->Pages[pageNum];
}

void* GetRowSlot(Table* table, uint32_t rowNum) {
    uint32_t pageNum     = rowNum / ROWS_PER_PAGE;
    void*    page        = GetPage(table->Pager, pageNum);
    uint32_t rowOffset   = rowNum % ROWS_PER_PAGE;
    uint32_t byte_offset = rowOffset * ROW_SIZE;
    return page + byte_offset;
}

void SerializeRow(Row* source, void* destination) {
    memcpy(destination + ID_OFFSET, &(source->ID), ID_SIZE);
    strncpy(destination + USERNAME_OFFSET, source->Username, USERNAME_SIZE);
    strncpy(destination + EMAIL_OFFSET, source->Email, EMAIL_SIZE);
}

void DeserializeRow(void* source, Row* destination) {
    memcpy(&(destination->ID), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->Username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->Email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void PrintRow(Row* row) {
    printf("%d, %s, %s\n", row->ID, row->Username, row->Email);
}

void CloseInputBuffer(InputBuffer* inputBuffer) {
    free(inputBuffer->Buffer);
    free(inputBuffer);
}

void FlushPager(Pager* pager, uint32_t pageNum, uint32_t size) {
    if (pager->Pages[pageNum] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->FileDescriptor, (i32)(pageNum * PAGE_SIZE), SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(pager->FileDescriptor, pager->Pages[pageNum], size);

    if (bytesWritten == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

void CloseDB(Table* table) {
    Pager*   pager        = table->Pager;
    uint32_t numFullPages = table->NumRows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < numFullPages; i++) {
        if (pager->Pages[i] == NULL) {
            continue;
        }
        FlushPager(pager, i, PAGE_SIZE);
        free(pager->Pages[i]);
        pager->Pages[i] = NULL;
    }

    uint32_t numAdditionalRows = table->NumRows % ROWS_PER_PAGE;
    if (numAdditionalRows > 0) {
        uint32_t pageNum = numFullPages;
        if (pager->Pages[pageNum] != NULL) {
            FlushPager(pager, pageNum, numAdditionalRows * ROW_SIZE);
            free(pager->Pages[pageNum]);
            pager->Pages[pageNum] = NULL;
        }
    }

    int result = close(pager->FileDescriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        void* page = pager->Pages[i];
        if (page) {
            free(page);
            pager->Pages[i] = NULL;
        }
    }
    free(pager);
    free(table);
}

EMetaCommandResult DoMetaCommand(InputBuffer* inputBuffer, Table* table) {
    if (strcmp(inputBuffer->Buffer, ".exit") == 0) {
        CloseInputBuffer(inputBuffer);
        CloseDB(table);
        exit(EXIT_SUCCESS); // NOLINT
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

EPrepareResult PrepareInsert(InputBuffer* inputBuffer, Statement* statement) {
    statement->Type = STATEMENT_INSERT;

    char* keyword          = strtok(inputBuffer->Buffer, " ");
    char* identifierString = strtok(NULL, " ");
    char* username         = strtok(NULL, " ");
    char* email            = strtok(NULL, " ");

    if (identifierString == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int identifier = atoi(identifierString);

    if (identifier < 0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->RowToInsert.ID = identifier;
    strcpy(statement->RowToInsert.Username, username);
    strcpy(statement->RowToInsert.Email, email);

    return PREPARE_SUCCESS;
}

EPrepareResult PrepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer->Buffer, "insert", 6) == 0) { // NOLINT
        return PrepareInsert(inputBuffer, statement);
    }
    if (strcmp(inputBuffer->Buffer, "select") == 0) {
        statement->Type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

EExecuteResult ExecuteInsert(Statement* statement, Table* table) {
    if (table->NumRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }
    Row* rowToInsert = &(statement->RowToInsert);
    SerializeRow(rowToInsert, GetRowSlot(table, table->NumRows));
    table->NumRows += 1;
    return EXECUTE_SUCCESS;
}

EExecuteResult ExecuteSelect(Statement* statement, Table* table) {
    pp_unused(statement);
    Row row;
    for (uint32_t i = 0; i < table->NumRows; i++) {
        DeserializeRow(GetRowSlot(table, i), &row);
        PrintRow(&row);
    }
    return EXECUTE_SUCCESS;
}

EExecuteResult ExecuteStatement(Statement* statement, Table* table) {
    switch (statement->Type) {
    case (STATEMENT_INSERT):
        // printf("This is where we would do an insert.\n");
        return ExecuteInsert(statement, table);
    case (STATEMENT_SELECT):
        // printf("This is where we would do a select.\n");
        return ExecuteSelect(statement, table);
    }
}

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        printf("Must supply a database filename.\new");
        exit(EXIT_FAILURE);
    }
    char const* filename = argv[1];
    Table*      table    = OpenDB(filename);

    InputBuffer* inputBuffer = NewInputBuffer();
    while (true) {
        PrintPrompt();
        ReadLine(inputBuffer);

        if (inputBuffer->Buffer[0] == '.') {
            switch (DoMetaCommand(inputBuffer, table)) {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'.\n", inputBuffer->Buffer);
                continue;
            }
        }

        Statement statement;
        switch (PrepareStatement(inputBuffer, &statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_NEGATIVE_ID):
            printf("ID must be positive.\n");
            continue;
        case (PREPARE_STRING_TOO_LONG):
            printf("String is too long.\n");
            continue;
        case (PREPARE_SYNTAX_ERROR):
            printf("Syntax error. Could not parse statement.\n");
            continue;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->Buffer);
            continue;
        }

        switch (ExecuteStatement(&statement, table)) {
        case (EXECUTE_SUCCESS):
            printf("Executed.\n");
            break;
        case (EXECUTE_TABLE_FULL):
            printf("Error: Table full.\n");
            break;
        }
    }
    return 0;
}
// Test Case : 40422, 김광호, unieye07@gmail.com
