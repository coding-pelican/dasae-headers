#include "dh/Err.h"

#include <stdio.h>

void Err_print(Err self) {
    $ignore = fprintf(
        stderr,
        "Program failed: [%s] %s(%d)\n",
        Err_domainToCStr(self),
        Err_codeToCStr(self),
        self.ctx
    );
}
