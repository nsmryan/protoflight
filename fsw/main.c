#include "stdio.h"

#include "unistd.h"

#include "em_interface.h"
#include "mb_interface.h"
#include "tlm_interface.h"
#include "tm_interface.h"



int main(void)
{
    FSW_RESULT_ENUM result = FSW_RESULT_OKAY;

    result = em_initialize();
    if (result != FSW_RESULT_OKAY)
    {
        printf("EM Init failure (%d)\n", result);
    }

    result = mb_initialize();
    if (result != FSW_RESULT_OKAY)
    {
        printf("MB Init failure (%d)\n", result);
    }

    result = tlm_initialize();
    if (result != FSW_RESULT_OKAY)
    {
        printf("TLM Init failure (%d)\n", result);
    }

    result = tm_initialize();
    if (result != FSW_RESULT_OKAY)
    {
        printf("TM Init failure (%d)\n", result);
    }

    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;

    tm_result = tm_start();
    if (tm_result != TM_RESULT_OKAY)
    {
        printf("TM start failed (%d)!\n", tm_result);
    }

    os_task_delay(9000);
}
