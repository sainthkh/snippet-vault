#include <utest.h>
#include "add.h"

UTEST(add, add)
{
    ASSERT_EQ(3, add(1, 2));
}
