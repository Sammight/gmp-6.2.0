/* Test mpz_nextprime.

Copyright 2009 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


#include <stdio.h>
#include <stdlib.h>

#include "gmp.h"
#include "gmp-impl.h"
#include "tests.h"

void
refmpz_nextprime (mpz_ptr p, mpz_srcptr t)
{
  mpz_add_ui (p, t, 1L);
  while (! mpz_probab_prime_p (p, 10))
    mpz_add_ui (p, p, 1L);
}

void
run (char *start, int reps, char *end, short diffs[])
{
  mpz_t x, y;
  int i;

  mpz_init_set_str (x, start, 0);
  mpz_init (y);

  for (i = 0; i < reps; i++)
    {
      mpz_nextprime (y, x);
      mpz_sub (x, y, x);
      if (diffs != NULL && diffs[i] != mpz_get_ui (x))
	{
	  gmp_printf ("diff list discrepancy\n");
	  abort ();
	}
      mpz_set (x, y);
    }

  mpz_set_str (y, end, 0);

  if (mpz_cmp (x, y) != 0)
    {
      gmp_printf ("got  %Zx\n", x);
      gmp_printf ("want %Zx\n", y);
      abort ();
    }

  mpz_clear (y);
  mpz_clear (x);
}

extern short diff1[];
extern short diff3[];
extern short diff4[];
extern short diff5[];

int
main (int argc, char **argv)
{
  int i;
  int reps = 200;
  gmp_randstate_ptr rands;
  mpz_t bs, x, nxtp, ref_nxtp;
  unsigned long bsi, size_range;

  tests_start();
  rands = RANDS;

  run ("2", 5000, "0xbdeb", diff1);

  run ("3", 5000 - 1, "0xbdeb", NULL);

  run ("0x8a43866f5776ccd5b02186e90d28946aeb0ed914", 100,
       "0x8a43866f5776ccd5b02186e90d28946aeb0f00dd", diff3);

  run ("0x10000000000000000000000000000000000000", 100,
       "0x100000000000000000000000000000000023cb", diff4);

  run ("0x1c2c26be55317530311facb648ea06b359b969715db83292ab8cf898d8b1b", 1000,
       "0x1c2c26be55317530311facb648ea06b359b969715db83292ab8cf89901519", diff5);

  mpz_init (bs);
  mpz_init (x);
  mpz_init (nxtp);
  mpz_init (ref_nxtp);

  if (argc == 2)
     reps = atoi (argv[1]);

  for (i = 0; i < reps; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 9 + 2; /* 0..1024 bit operands */

      mpz_urandomb (bs, rands, size_range);
      mpz_rrandomb (x, rands, mpz_get_ui (bs));

/*      gmp_printf ("%ld: %Zd\n", mpz_sizeinbase (x, 2), x); */

      mpz_nextprime (nxtp, x);
      refmpz_nextprime (ref_nxtp, x);
      if (mpz_cmp (nxtp, ref_nxtp) != 0)
	abort ();
    }

  mpz_clear (bs);
  mpz_clear (x);
  mpz_clear (nxtp);
  mpz_clear (ref_nxtp);

  tests_end ();
  return 0;
}

short diff1[] =
{
  1,2,2,4,2,4,2,4,6,2,6,4,2,4,6,6,
  2,6,4,2,6,4,6,8,4,2,4,2,4,14,4,6,
  2,10,2,6,6,4,6,6,2,10,2,4,2,12,12,4,
  2,4,6,2,10,6,6,6,2,6,4,2,10,14,4,2,
  4,14,6,10,2,4,6,8,6,6,4,6,8,4,8,10,
  2,10,2,6,4,6,8,4,2,4,12,8,4,8,4,6,
  12,2,18,6,10,6,6,2,6,10,6,6,2,6,6,4,
  2,12,10,2,4,6,6,2,12,4,6,8,10,8,10,8,
  6,6,4,8,6,4,8,4,14,10,12,2,10,2,4,2,
  10,14,4,2,4,14,4,2,4,20,4,8,10,8,4,6,
  6,14,4,6,6,8,6,12,4,6,2,10,2,6,10,2,
  10,2,6,18,4,2,4,6,6,8,6,6,22,2,10,8,
  10,6,6,8,12,4,6,6,2,6,12,10,18,2,4,6,
  2,6,4,2,4,12,2,6,34,6,6,8,18,10,14,4,
  2,4,6,8,4,2,6,12,10,2,4,2,4,6,12,12,
  8,12,6,4,6,8,4,8,4,14,4,6,2,4,6,2,
  6,10,20,6,4,2,24,4,2,10,12,2,10,8,6,6,
  6,18,6,4,2,12,10,12,8,16,14,6,4,2,4,2,
  10,12,6,6,18,2,16,2,22,6,8,6,4,2,4,8,
  6,10,2,10,14,10,6,12,2,4,2,10,12,2,16,2,
  6,4,2,10,8,18,24,4,6,8,16,2,4,8,16,2,
  4,8,6,6,4,12,2,22,6,2,6,4,6,14,6,4,
  2,6,4,6,12,6,6,14,4,6,12,8,6,4,26,18,
  10,8,4,6,2,6,22,12,2,16,8,4,12,14,10,2,
  4,8,6,6,4,2,4,6,8,4,2,6,10,2,10,8,
  4,14,10,12,2,6,4,2,16,14,4,6,8,6,4,18,
  8,10,6,6,8,10,12,14,4,6,6,2,28,2,10,8,
  4,14,4,8,12,6,12,4,6,20,10,2,16,26,4,2,
  12,6,4,12,6,8,4,8,22,2,4,2,12,28,2,6,
  6,6,4,6,2,12,4,12,2,10,2,16,2,16,6,20,
  16,8,4,2,4,2,22,8,12,6,10,2,4,6,2,6,
  10,2,12,10,2,10,14,6,4,6,8,6,6,16,12,2,
  4,14,6,4,8,10,8,6,6,22,6,2,10,14,4,6,
  18,2,10,14,4,2,10,14,4,8,18,4,6,2,4,6,
  2,12,4,20,22,12,2,4,6,6,2,6,22,2,6,16,
  6,12,2,6,12,16,2,4,6,14,4,2,18,24,10,6,
  2,10,2,10,2,10,6,2,10,2,10,6,8,30,10,2,
  10,8,6,10,18,6,12,12,2,18,6,4,6,6,18,2,
  10,14,6,4,2,4,24,2,12,6,16,8,6,6,18,16,
  2,4,6,2,6,6,10,6,12,12,18,2,6,4,18,8,
  24,4,2,4,6,2,12,4,14,30,10,6,12,14,6,10,
  12,2,4,6,8,6,10,2,4,14,6,6,4,6,2,10,
  2,16,12,8,18,4,6,12,2,6,6,6,28,6,14,4,
  8,10,8,12,18,4,2,4,24,12,6,2,16,6,6,14,
  10,14,4,30,6,6,6,8,6,4,2,12,6,4,2,6,
  22,6,2,4,18,2,4,12,2,6,4,26,6,6,4,8,
  10,32,16,2,6,4,2,4,2,10,14,6,4,8,10,6,
  20,4,2,6,30,4,8,10,6,6,8,6,12,4,6,2,
  6,4,6,2,10,2,16,6,20,4,12,14,28,6,20,4,
  18,8,6,4,6,14,6,6,10,2,10,12,8,10,2,10,
  8,12,10,24,2,4,8,6,4,8,18,10,6,6,2,6,
  10,12,2,10,6,6,6,8,6,10,6,2,6,6,6,10,
  8,24,6,22,2,18,4,8,10,30,8,18,4,2,10,6,
  2,6,4,18,8,12,18,16,6,2,12,6,10,2,10,2,
  6,10,14,4,24,2,16,2,10,2,10,20,4,2,4,8,
  16,6,6,2,12,16,8,4,6,30,2,10,2,6,4,6,
  6,8,6,4,12,6,8,12,4,14,12,10,24,6,12,6,
  2,22,8,18,10,6,14,4,2,6,10,8,6,4,6,30,
  14,10,2,12,10,2,16,2,18,24,18,6,16,18,6,2,
  18,4,6,2,10,8,10,6,6,8,4,6,2,10,2,12,
  4,6,6,2,12,4,14,18,4,6,20,4,8,6,4,8,
  4,14,6,4,14,12,4,2,30,4,24,6,6,12,12,14,
  6,4,2,4,18,6,12,8,6,4,12,2,12,30,16,2,
  6,22,14,6,10,12,6,2,4,8,10,6,6,24,14,6,
  4,8,12,18,10,2,10,2,4,6,20,6,4,14,4,2,
  4,14,6,12,24,10,6,8,10,2,30,4,6,2,12,4,
  14,6,34,12,8,6,10,2,4,20,10,8,16,2,10,14,
  4,2,12,6,16,6,8,4,8,4,6,8,6,6,12,6,
  4,6,6,8,18,4,20,4,12,2,10,6,2,10,12,2,
  4,20,6,30,6,4,8,10,12,6,2,28,2,6,4,2,
  16,12,2,6,10,8,24,12,6,18,6,4,14,6,4,12,
  8,6,12,4,6,12,6,12,2,16,20,4,2,10,18,8,
  4,14,4,2,6,22,6,14,6,6,10,6,2,10,2,4,
  2,22,2,4,6,6,12,6,14,10,12,6,8,4,36,14,
  12,6,4,6,2,12,6,12,16,2,10,8,22,2,12,6,
  4,6,18,2,12,6,4,12,8,6,12,4,6,12,6,2,
  12,12,4,14,6,16,6,2,10,8,18,6,34,2,28,2,
  22,6,2,10,12,2,6,4,8,22,6,2,10,8,4,6,
  8,4,12,18,12,20,4,6,6,8,4,2,16,12,2,10,
  8,10,2,4,6,14,12,22,8,28,2,4,20,4,2,4,
  14,10,12,2,12,16,2,28,8,22,8,4,6,6,14,4,
  8,12,6,6,4,20,4,18,2,12,6,4,6,14,18,10,
  8,10,32,6,10,6,6,2,6,16,6,2,12,6,28,2,
  10,8,16,6,8,6,10,24,20,10,2,10,2,12,4,6,
  20,4,2,12,18,10,2,10,2,4,20,16,26,4,8,6,
  4,12,6,8,12,12,6,4,8,22,2,16,14,10,6,12,
  12,14,6,4,20,4,12,6,2,6,6,16,8,22,2,28,
  8,6,4,20,4,12,24,20,4,8,10,2,16,2,12,12,
  34,2,4,6,12,6,6,8,6,4,2,6,24,4,20,10,
  6,6,14,4,6,6,2,12,6,10,2,10,6,20,4,26,
  4,2,6,22,2,24,4,6,2,4,6,24,6,8,4,2,
  34,6,8,16,12,2,10,2,10,6,8,4,8,12,22,6,
  14,4,26,4,2,12,10,8,4,8,12,4,14,6,16,6,
  8,4,6,6,8,6,10,12,2,6,6,16,8,6,6,12,
  10,2,6,18,4,6,6,6,12,18,8,6,10,8,18,4,
  14,6,18,10,8,10,12,2,6,12,12,36,4,6,8,4,
  6,2,4,18,12,6,8,6,6,4,18,2,4,2,24,4,
  6,6,14,30,6,4,6,12,6,20,4,8,4,8,6,6,
  4,30,2,10,12,8,10,8,24,6,12,4,14,4,6,2,
  28,14,16,2,12,6,4,20,10,6,6,6,8,10,12,14,
  10,14,16,14,10,14,6,16,6,8,6,16,20,10,2,6,
  4,2,4,12,2,10,2,6,22,6,2,4,18,8,10,8,
  22,2,10,18,14,4,2,4,18,2,4,6,8,10,2,30,
  4,30,2,10,2,18,4,18,6,14,10,2,4,20,36,6,
  4,6,14,4,20,10,14,22,6,2,30,12,10,18,2,4,
  14,6,22,18,2,12,6,4,8,4,8,6,10,2,12,18,
  10,14,16,14,4,6,6,2,6,4,2,28,2,28,6,2,
  4,6,14,4,12,14,16,14,4,6,8,6,4,6,6,6,
  8,4,8,4,14,16,8,6,4,12,8,16,2,10,8,4,
  6,26,6,10,8,4,6,12,14,30,4,14,22,8,12,4,
  6,8,10,6,14,10,6,2,10,12,12,14,6,6,18,10,
  6,8,18,4,6,2,6,10,2,10,8,6,6,10,2,18,
  10,2,12,4,6,8,10,12,14,12,4,8,10,6,6,20,
  4,14,16,14,10,8,10,12,2,18,6,12,10,12,2,4,
  2,12,6,4,8,4,44,4,2,4,2,10,12,6,6,14,
  4,6,6,6,8,6,36,18,4,6,2,12,6,6,6,4,
  14,22,12,2,18,10,6,26,24,4,2,4,2,4,14,4,
  6,6,8,16,12,2,42,4,2,4,24,6,6,2,18,4,
  14,6,28,18,14,6,10,12,2,6,12,30,6,4,6,6,
  14,4,2,24,4,6,6,26,10,18,6,8,6,6,30,4,
  12,12,2,16,2,6,4,12,18,2,6,4,26,12,6,12,
  4,24,24,12,6,2,12,28,8,4,6,12,2,18,6,4,
  6,6,20,16,2,6,6,18,10,6,2,4,8,6,6,24,
  16,6,8,10,6,14,22,8,16,6,2,12,4,2,22,8,
  18,34,2,6,18,4,6,6,8,10,8,18,6,4,2,4,
  8,16,2,12,12,6,18,4,6,6,6,2,6,12,10,20,
  12,18,4,6,2,16,2,10,14,4,30,2,10,12,2,24,
  6,16,8,10,2,12,22,6,2,16,20,10,2,12,12,18,
  10,12,6,2,10,2,6,10,18,2,12,6,4,6,2,24,
  28,2,4,2,10,2,16,12,8,22,2,6,4,2,10,6,
  20,12,10,8,12,6,6,6,4,18,2,4,12,18,2,12,
  6,4,2,16,12,12,14,4,8,18,4,12,14,6,6,4,
  8,6,4,20,12,10,14,4,2,16,2,12,30,4,6,24,
  20,24,10,8,12,10,12,6,12,12,6,8,16,14,6,4,
  6,36,20,10,30,12,2,4,2,28,12,14,6,22,8,4,
  18,6,14,18,4,6,2,6,34,18,2,16,6,18,2,24,
  4,2,6,12,6,12,10,8,6,16,12,8,10,14,40,6,
  2,6,4,12,14,4,2,4,2,4,8,6,10,6,6,2,
  6,6,6,12,6,24,10,2,10,6,12,6,6,14,6,6,
  52,20,6,10,2,10,8,10,12,12,2,6,4,14,16,8,
  12,6,22,2,10,8,6,22,2,22,6,8,10,12,12,2,
  10,6,12,2,4,14,10,2,6,18,4,12,8,18,12,6,
  6,4,6,6,14,4,2,12,12,4,6,18,18,12,2,16,
  12,8,18,10,26,4,6,8,6,6,4,2,10,20,4,6,
  8,4,20,10,2,34,2,4,24,2,12,12,10,6,2,12,
  30,6,12,16,12,2,22,18,12,14,10,2,12,12,4,2,
  4,6,12,2,16,18,2,40,8,16,6,8,10,2,4,18,
  8,10,8,12,4,18,2,18,10,2,4,2,4,8,28,2,
  6,22,12,6,14,18,4,6,8,6,6,10,8,4,2,18,
  10,6,20,22,8,6,30,4,2,4,18,6,30,2,4,8,
  6,4,6,12,14,34,14,6,4,2,6,4,14,4,2,6,
  28,2,4,6,8,10,2,10,2,10,2,4,30,2,12,12,
  10,18,12,14,10,2,12,6,10,6,14,12,4,14,4,18,
  2,10,8,4,8,10,12,18,18,8,6,18,16,14,6,6,
  10,14,4,6,2,12,12,4,6,6,12,2,16,2,12,6,
  4,14,6,4,2,12,18,4,36,18,12,12,2,4,2,4,
  8,12,4,36,6,18,2,12,10,6,12,24,8,6,6,16,
  12,2,18,10,20,10,2,6,18,4,2,40,6,2,16,2,
  4,8,18,10,12,6,2,10,8,4,6,12,2,10,18,8,
  6,4,20,4,6,36,6,2,10,6,24,6,14,16,6,18,
  2,10,20,10,8,6,4,6,2,10,2,12,4,2,4,8,
  10,6,12,18,14,12,16,8,6,16,8,4,2,6,18,24,
  18,10,12,2,4,14,10,6,6,6,18,12,2,28,18,14,
  16,12,14,24,12,22,6,2,10,8,4,2,4,14,12,6,
  4,6,14,4,2,4,30,6,2,6,10,2,30,22,2,4,
  6,8,6,6,16,12,12,6,8,4,2,24,12,4,6,8,
  6,6,10,2,6,12,28,14,6,4,12,8,6,12,4,6,
  14,6,12,10,6,6,8,6,6,4,2,4,8,12,4,14,
  18,10,2,16,6,20,6,10,8,4,30,36,12,8,22,12,
  2,6,12,16,6,6,2,18,4,26,4,8,18,10,8,10,
  6,14,4,20,22,18,12,8,28,12,6,6,8,6,12,24,
  16,14,4,14,12,6,10,12,20,6,4,8,18,12,18,10,
  2,4,20,10,14,4,6,2,10,24,18,2,4,20,16,14,
  10,14,6,4,6,20,6,10,6,2,12,6,30,10,8,6,
  4,6,8,40,2,4,2,12,18,4,6,8,10,6,18,18,
  2,12,16,8,6,4,6,6,2,52,14,4,20,16,2,4,
  6,12,2,6,12,12,6,4,14,10,6,6,14,10,14,16,
  8,6,12,4,8,22,6,2,18,22,6,2,18,6,16,14,
  10,6,12,2,6,4,8,18,12,16,2,4,14,4,8,12,
  12,30,16,8,4,2,6,22,12,8,10,6,6,6,14,6,
  18,10,12,2,10,2,4,26,4,12,8,4,18,8,10,14,
  16,6,6,8,10,6,8,6,12,10,20,10,8,4,12,26,
  18,4,12,18,6,30,6,8,6,22,12,2,4,6,6,2,
  10,2,4,6,6,2,6,22,18,6,18,12,8,12,6,10,
  12,2,16,2,10,2,10,18,6,20,4,2,6,22,6,6,
  18,6,14,12,16,2,6,6,4,14,12,4,2,18,16,36,
  12,6,14,28,2,12,6,12,6,4,2,16,30,8,24,6,
  30,10,2,18,4,6,12,8,22,2,6,22,18,2,10,2,
  10,30,2,28,6,14,16,6,20,16,2,6,4,32,4,2,
  4,6,2,12,4,6,6,12,2,6,4,6,8,6,4,20,
  4,32,10,8,16,2,22,2,4,6,8,6,16,14,4,18,
  8,4,20,6,12,12,6,10,2,10,2,12,28,12,18,2,
  18,10,8,10,48,2,4,6,8,10,2,10,30,2,36,6,
  10,6,2,18,4,6,8,16,14,16,6,14,4,20,4,6,
  2,10,12,2,6,12,6,6,4,12,2,6,4,12,6,8,
  4,2,6,18,10,6,8,12,6,22,2,6,12,18,4,14,
  6,4,20,6,16,8,4,8,22,8,12,6,6,16,12,18,
  30,8,4,2,4,6,26,4,14,24,22,6,2,6,10,6,
  14,6,6,12,10,6,2,12,10,12,8,18,18,10,6,8,
  16,6,6,8,16,20,4,2,10,2,10,12,6,8,6,10,
  20,10,18,26,4,6,30,2,4,8,6,12,12,18,4,8,
  22,6,2,12,34,6,18,12,6,2,28,14,16,14,4,14,
  12,4,6,6,2,36,4,6,20,12,24,6,22,2,16,18,
  12,12,18,2,6,6,6,4,6,14,4,2,22,8,12,6,
  10,6,8,12,18,12,6,10,2,22,14,6,6,4,18,6,
  20,22,2,12,24,4,18,18,2,22,2,4,12,8,12,10,
  14,4,2,18,16,38,6,6,6,12,10,6,12,8,6,4,
  6,14,30,6,10,8,22,6,8,12,10,2,10,2,6,10,
  2,10,12,18,20,6,4,8,22,6,6,30,6,14,6,12,
  12,6,10,2,10,30,2,16,8,4,2,6,18,4,2,6,
  4,26,4,8,6,10,2,4,6,8,4,6,30,12,2,6,
  6,4,20,22,8,4,2,4,72,8,4,8,22,2,4,14,
  10,2,4,20,6,10,18,6,20,16,6,8,6,4,20,12,
  22,2,4,2,12,10,18,2,22,6,18,30,2,10,14,10,
  8,16,50,6,10,8,10,12,6,18,2,22,6,2,4,6,
  8,6,6,10,18,2,22,2,16,14,10,6,2,12,10,20,
  4,14,6,4,36,2,4,6,12,2,4,14,12,6,4,6,
  2,6,4,20,10,2,10,6,12,2,24,12,12,6,6,4,
  24,2,4,24,2,6,4,6,8,16,6,2,10,12,14,6,
  34,6,14,6,4,2,30,22,8,4,6,8,4,2,28,2,
  6,4,26,18,22,2,6,16,6,2,16,12,2,12,4,6,
  6,14,10,6,8,12,4,18,2,10,8,16,6,6,30,2,
  10,18,2,10,8,4,8,12,24,40,2,12,10,6,12,2,
  12,4,2,4,6,18,14,12,6,4,14,30,4,8,10,8,
  6,10,18,8,4,14,16,6,8,4,6,2,10,2,12,4,
  2,4,6,8,4,6,32,24,10,8,18,10,2,6,10,2,
  4,18,6,12,2,16,2,22,6,6,8,18,4,18,12,8,
  6,4,20,6,30,22,12,2,6,18,4,62,4,2,12,6,
  10,2,12,12,28,2,4,14,22,6,2,6,6,10,14,4,
  2,10,6,8,10,14,10,6,2,12,22,18,8,10,18,12,
  2,12,4,12,2,10,2,6,18,6,6,34,6,2,12,4,
  6,18,18,2,16,6,6,8,6,10,18,8,10,8,10,2,
  4,18,26,12,22,2,4,2,22,6,6,14,16,6,20,10,
  12,2,18,42,4,24,2,6,10,12,2,6,10,8,4,6,
  12,12,8,4,6,12,30,20,6,24,6,10,12,2,10,20,
  6,6,4,12,14,10,18,12,8,6,12,4,14,10,2,12,
  30,16,2,12,6,4,2,4,6,26,4,18,2,4,6,14,
  54,6,52,2,16,6,6,12,26,4,2,6,22,6,2,12,
  12,6,10,18,2,12,12,10,18,12,6,8,6,10,6,8,
  4,2,4,20,24,6,6,10,14,10,2,22,6,14,10,26,
  4,18,8,12,12,10,12,6,8,16,6,8,6,6,22,2,
  10,20,10,6,44,18,6,10,2,4,6,14,4,26,4,2,
  12,10,8,4,8,12,4,12,8,22,8,6,10,18,6,6,
  8,6,12,4,8,18,10,12,6,12,2,6,4,2,16,12,
  12,14,10,14,6,10,12,2,12,6,4,6,2,12,4,26,
  6,18,6,10,6,2,18,10,8,4,26,10,20,6,16,20,
  12,10,8,10,2,16,6,20,10,20,4,30,2,4,8,16,
  2,18,4,2,6,10,18,12,14,18,6,16,20,6,4,8,
  6,4,6,12,8,10,2,12,6,4,2,6,10,2,16,12,
  14,10,6,8,6,28,2,6,18,30,34,2,16,12,2,18,
  16,6,8,10,8,10,8,10,44,6,6,4,20,4,2,4,
  14,28,8,6,16,14,30,6,30,4,14,10,6,6,8,4,
  18,12,6,2,22,12,8,6,12,4,14,4,6,2,4,18,
  20,6,16,38,16,2,4,6,2,40,42,14,4,6,2,24,
  10,6,2,18,10,12,2,16,2,6,16,6,8,4,2,10,
  6,8,10,2,18,16,8,12,18,12,6,12,10,6,6,18,
  12,14,4,2,10,20,6,12,6,16,26,4,18,2,4,32,
  10,8,6,4,6,6,14,6,18,4,2,18,10,8,10,8,
  10,2,4,6,2,10,42,8,12,4,6,18,2,16,8,4,
  2,10,14,12,10,20,4,8,10,38,4,6,2,10,20,10,
  12,6,12,26,12,4,8,28,8,4,8,24,6,10,8,6,
  16,12,8,10,12,8,22,6,2,10,2,6,10,6,6,8,
  6,4,14,28,8,16,18,8,4,6,20,4,18,6,2,24,
  24,6,6,12,12,4,2,22,2,10,6,8,12,4,20,18,
  6,4,12,24,6,6,54,8,6,4,26,36,4,2,4,26,
  12,12,4,6,6,8,12,10,2,12,16,18,6,8,6,12,
  18,10,2,54,4,2,10,30,12,8,4,8,16,14,12,6,
  4,6,12,6,2,4,14,12,4,14,6,24,6,6,10,12,
  12,20,18,6,6,16,8,4,6,20,4,32,4,14,10,2,
  6,12,16,2,4,6,12,2,10,8,6,4,2,10,14,6,
  6,12,18,34,8,10,6,24,6,2,10,12,2,30,10,14,
  12,12,16,6,6,2,18,4,6,30,14,4,6,6,2,6,
  4,6,14,6,4,8,10,12,6,32,10,8,22,2,10,6,
  24,8,4,30,6,2,12,16,8,6,4,6,8,16,14,6,
  6,4,2,10,12,2,16,14,4,2,4,20,18,10,2,10,
  6,12,30,8,18,12,10,2,6,6,4,12,12,2,4,12,
  18,24,2,10,6,8,16,8,6,12,10,14,6,12,6,6,
  4,2,24,4,6,8,6,4,2,4,6,14,4,8,10,24,
  24,12,2,6,12,22,30,2,6,18,10,6,6,8,4,2,
  6,10,8,10,6,8,16,6,14,6,4,24,8,10,2,12,
  6,4,36,2,22,6,8,6,10,8,6,12,10,14,10,6,
  18,12,2,12,4,26,10,14,16,18,8,18,12,12,6,16,
  14,24,10,12,8,22,6,2,10,60,6,2,4,8,16,14,
  10,6,24,6,12,18,24,2,30,4,2,12,6,10,2,4,
  14,6,16,2,10,8,22,20,6,4,32,6,18,4,2,4,
  2,4,8,52,14,22,2,22,20,10,8,10,2,6,4,14,
  4,6,20,4,6,2,12,12,6,12,16,2,12,10,8,4,
  6,2,28,12,8,10,12,2,4,14,28,8,6,4,2,4,
  6,2,12,58,6,14,10,2,6,28,32,4,30,8,6,4,
  6,12,12,2,4,6,6,14,16,8,30,4,2,10,8,6,
  4,6,26,4,12,2,10,18,12,12,18,2,4,12,8,12,
  10,20,4,8,16,12,8,6,16,8,10,12,14,6,4,8,
  12,4,20,6,40,8,16,6,36,2,6,4,6,2,22,18,
  2,10,6,36,14,12,4,18,8,4,14,10,2,10,8,4,
  2,18,16,12,14,10,14,6,6,42,10,6,6,20,10,8,
  12,4,12,18,2,10,14,18,10,18,8,6,4,14,6,10,
  30,14,6,6,4,12,38,4,2,4,6,8,12,10,6,18,
  6,50,6,4,6,12,8,10,32,6,22,2,10,12,18,2,
  6,4,30,8,6,6,18,10,2,4,12,20,10,8,24,10,
  2,6,22,6,2,18,10,12,2,30,18,12,28,2,6,4,
  6,14,6,12,10,8,4,12,26,10,8,6,16,2,10,18,
  14,6,4,6,14,16,2,6,4,12,20,4,20,4,6,12,
  2,36,4,6,2,10,2,22,8,6,10,12,12,18,14,24,
  36,4,20,24,10,6,2,28,6,18,8,4,6,8,6,4,
  2,12,28,18,14,16,14,18,10,8,6,4,6,6,8,22,
  12,2,10,18,6,2,18,10,2,12,10,18,32,6,4,6,
  6,8,6,6,10,20,6,12,10,8,10,14,6,10,14,4,
  2,22,18,2,10,2,4,20,4,2,34,2,12,6,10,2,
  10,18,6,14,12,12,22,8,6,16,6,8,4,12,6,8,
  4,36,6,6,20,24,6,12,18,10,2,10,26,6,16,8,
  6,4,24,18,8,12,12,10,18,12,2,24,4,12,18,12,
  14,10,2,4,24,12,14,10,6,2,6,4,6,26,4,6,
  6,2,22,8,18,4,18,8
};

short diff3[] =
{
  33,32,136,116,24,22,104,114,76,278,238,162,36,44,388,134,
  130,26,312,42,138,28,24,80,138,108,270,12,330,130,98,102,
  162,34,36,170,90,34,14,6,24,66,154,218,70,132,188,88,
  80,82,84,44,64,2,76,138,54,6,266,88,98,144,66,60,
  202,36,126,38,52,60,30,48,2,4,72,200,4,390,44,4,
  56,36,124,74,108,12,336,40,30,374,160,72,20,6,18,24,
  30,178,152,280
};

short diff4[] =
{
  91,92,64,6,104,24,46,258,68,18,54,100,68,154,26,4,
  38,142,168,42,18,26,286,104,136,116,40,2,28,110,52,78,
  104,24,54,96,4,626,196,24,56,36,52,102,48,156,26,18,
  42,40,54,62,114,88,48,150,60,134,136,48,176,180,28,180,
  140,12,96,28,50,456,138,70,236,112,342,162,30,36,50,4,
  38,112,48,132,126,140,12,312,28,60,120,20,16,74,58,44,
  10,36,42,48
};

short diff5[] =
{
  268,120,320,184,396,2,94,108,20,318,274,14,64,122,220,108,
  18,174,6,24,348,32,64,116,268,162,20,156,28,110,52,428,
  196,14,262,30,194,120,300,66,268,12,428,370,212,198,192,130,
  30,80,154,30,194,460,282,42,236,592,24,140,18,52,216,72,
  126,84,416,24,112,146,180,10,72,212,126,64,8,268,162,198,
  110,34,38,78,148,50,84,6,120,252,4,246,2,6,112,62,
  256,422,624,298,68,34,440,60,88,72,66,150,158,24,190,90,
  236,154,72,110,34,74,132,168,130,38,54,28,42,374,36,54,
  72,252,118,168,320,162,282,100,42,330,156,18,344,36,96,36,
  234,220,66,258,80,292,300,12,126,56,148,512,66,460,12,32,
  100,50,90,220,260,154,140,124,206,130,342,162,216,162,128,514,
  80,6,106,144,110,424,92,184,32,310,132,48,42,92,340,108,
  120,20,58,188,190,270,170,312,94,80,214,344,156,144,136,44,
  90,282,366,54,58,56,322,38,214,32,10,86,154,290,58,138,
  14,18,12,180,358,26,420,174,6,156,196,488,54,30,22,14,
  580,36,30,204,224,202,504,54,212,30,114,190,36,38,16,14,
  250,96,32,364,366,30,192,122,64,164,112,120,158,18,12,292,
  98,100,348,200,154,314,540,366,294,4,80,138,108,4,108,60,
  642,60,56,60,72,18,150,100,132,14,190,618,98,270,234,240,
  150,48,82,446,268,206,58,116,210,136,50,66,628,146,154,140,
  174,46,146,180,58,26,336,60,28,282,18,476,4,170,142,380,
  240,220,86,82,6,56,448,246,132,390,42,78,380,634,296,52,
  222,54,26,136,8,154,132,80,516,432,166,114,50,432,16,188,
  226,86,148,50,390,232,120,170,70,200,442,962,168,66,94,362,
  16,14,124,26,46,90,1430,12,42,294,30,84,312,16,102,38,
  52,380,426,34,18,264,240,54,50,34,140,208,44,210,138,16,
  90,156,26,214,254,114,12,114,456,106,104,240,144,76,74,180,
  54,46,254,390,496,426,68,262,98,118,182,30,198,84,114,12,
  52,68,160,102,158,4,320,78,250,210,218,40,8,196,534,206,
  178,432,140,228,600,60,240,58,174,60,318,266,60,114,118,308,
  4,90,192,278,126,450,346,356,174,36,118,60,12,50,54,84,
  160,234,42,36,368,90,606,126,238,74,168,594,34,312,80,82,
  30,78,374,36,42,72,190,374,76,198,156,26,676,282,80,78,
  370,158,90,436,116,850,32,42,276,60,184,116,4,84,62,60,
  282,48,420,412,92,250,56,82,648,188,4,200,268,60,74,136,
  38,4,282,236,700,128,42,42,702,84,90,460,296,154,90,132,
  84,108,222,218,88,62,78,414,220,206,238,50,6,76,408,6,
  98,102,126,168,30,250,12,278,6,40,90,96,78,132,54,24,
  344,60,160,138,198,224,36,154,156,56,46,108,26,570,6,28,
  114,110,792,4,6,200,160,20,310,620,76,74,340,300,128,432,
  100,156,164,408,286,246,146,288,10,222,272,58,144,578,60,30,
  196,104,198,352,26,432,64,20,124,422,222,436,102,110,12,40,
  308,178,414,140,268,92,58,114,68,28,492,186,62,484,20,6,
  274,20,28,56,226,56,48,94,96,362,70,338,64,116,192,160,
  128,136,120,50,6,94,186,72,188,220,450,92,246,420,76,74,
  42,370,140,154,194,36,90,216,18,24,310,326,16,24,110,190,
  162,32,192,240,24,76,300,140,136,300,278,70,42,14,34,14,
  168,292,68,78,6,120,190,24,30,36,606,108,144,62,276,792,
  18,4,38,328,98,24,22,98,6,60,52,2,18,108,6,126,
  192,262,126,128,100,270,14,70,260,112,48,90,62,450,24,162,
  442,2,46,242,360,126,64,138,158,42,192,60,40,78,32,78,
  100,90,584,18,276,504,142,14,64,1080,198,114,66,1272,120,66,
  2,150,238,330,192,134,100,56,118,366,294,104,210,942,46,134,
  66,370,218,60,136,410,214,120,140,16,86,70,282,26,22,74,
  18,136,54,50,76,356,114,24,52,198,240,56,72,322,152,30,
  70,30,54,266,330,48,72,96,202,48,24,194,4,98,348,226,
  366,38,36,48,1708,162,26,22,372,846,396,164,4,36,476,910,
  84,2,28,36,8,366,114,82,74,12,28,30,206,144,112,18,
  30,434,42,240,270,84,24,72,54,100,234,30,140,342,138,148,
  14,390,268,20,376,290,46,14,136,48,278,58,54,38,238,810,
  114,42,278,76,86,574,362,108,154,116,144,406,24,26,16,60,
  24,152,24,310,176,40,20,60,250,432,32,18,222,58,336,114,
  50,490,186,384,86,130,170,90
};
