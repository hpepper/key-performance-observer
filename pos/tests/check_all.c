/** check_all.c
 * See: http://check.sourceforge.net/doc/check_html/check_3.html#SEC4
 */ 
#include <stdlib.h>
#include <check.h>

#include "check_Common.h"
#include "check_FileOps.h"
#include "check_GetData.h"
#include "check_Statistics.h"

int main (void)
{
  int number_failed;

  SRunner *srCommon = srunner_create (common_suite());

  SRunner *srFileOps = srunner_create (fileops_suite());

  SRunner *srStatistics = srunner_create (statistics_suite());

  SRunner *srGetData = srunner_create (getdata_suite());

  srunner_run_all (srCommon, CK_VERBOSE);  number_failed = srunner_ntests_failed (srCommon);

  srunner_run_all (srFileOps, CK_VERBOSE);   number_failed +=srunner_ntests_failed (srFileOps);

  srunner_run_all (srStatistics, CK_VERBOSE);   number_failed +=srunner_ntests_failed (srStatistics);

  srunner_run_all (srGetData, CK_VERBOSE);   number_failed +=srunner_ntests_failed (srGetData);


  srunner_free (srCommon);
  srunner_free (srFileOps);
  srunner_free (srStatistics);
  srunner_free (srGetData);


  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


