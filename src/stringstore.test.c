#include <utest.h>
#include "stringstore.h"

UTEST(stringstore, get_getstring_correctly)
{
    gss_init();

    string_id id = gss_add("Hello, World!");
    const char *str = gss_get(id);

    ASSERT_EQ(0, strcmp(str, "Hello, World!"));

    gss_release();
}
