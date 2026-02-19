#include <generated/csr.h>
#include <stdio.h>

int main(void) {
    printf("Testing mock accelerator...\n");

    mock_value_write(41);
    int result = mock_result_read();

    printf("Result = %d\n", result);

    while (1);
}
