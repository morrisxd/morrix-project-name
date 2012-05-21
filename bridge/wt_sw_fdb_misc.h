#define TRUE     1
#define FALSE    0


#define WPI_TEST_CHECK_RESULT(result, exp_result, x) \
        do{ \
            result = (x); \
            if(result != exp_result) { \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n" \
                   "   Returned %s. Expected Value %s\n",  \
                   __FILE__, __FUNCTION__, __LINE__, \
                   WP_error_name[result & 0xFFFF], WP_error_name[exp_result & 0xFFFF]);\
            wpe_test_failures++; \
          } \
        } while(0)

#define WPE_TEST_CHECK_RESULT(result, exp_result, x) \
        do{ \
            result = (x); \
            if(result != exp_result) { \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n" \
                   "   Returned %s. Expected Value %s\n",  \
                   __FILE__, __FUNCTION__, __LINE__, \
                   WP_error_name[result & 0xFFFF], WP_error_name[exp_result & 0xFFFF]);\
            wpe_test_failures++; \
          } \
        } while(0)

#define WPI_TEST_CHECK_HANDLE(handle, x) \
        do{ \
            handle = (x); \
            if(WP_ERROR_P(handle)) { \
              printf("TEST FAILURE in file %s,  function %s,  line: %d \n" \
                     "   Expected Handle. Returned error %s\n",  \
                     __FILE__, __FUNCTION__, __LINE__, \
                     WP_error_name[handle & 0xFFFF]);\
              wpe_test_failures++;\
            }\
        } while(0)

#define WPI_TEST_ADD_ERROR(error_str) \
        do{ \
            wpe_test_failures++; \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n    %s\n", \
                  __FILE__, __FUNCTION__, __LINE__, error_str); \
        } while(0)
