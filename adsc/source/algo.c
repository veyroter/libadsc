#include <ads_base.h>

size_t ads_fast_close_2(size_t i)
{
  if (i < 8)
    return 8;

  size_t k = 2;

  while ((i >>= 1) > 0)
    k <<= 1;

  return k;
}