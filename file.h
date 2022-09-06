#pragma once

#include "str.h"
#include "sum.h"

typedef RESULT(str, str) SlurpFileResult;

SlurpFileResult slurp_file(str filename);
