#include <assert.h>
#include <stdio.h>   // For remove function
#include "../../src/config/config.h"

void test_read_encrypted_file() {
    BYTE *data;
    DWORD size;

    // Test 1: Valid file
    FILE *file = NULL;
    errno_t err = fopen_s(&file, "test.txt", "wb");
    assert(err == 0);

    char content[] = "Hello, world!";
    fwrite(content, 1, sizeof(content) - 1, file);  // -1 to avoid writing the null terminator
    fclose(file);
    
    char* decrypted_content = get_decrypted_config("test.txt");
    data = (BYTE *)decrypted_content; 
    size = (DWORD)strlen(decrypted_content);
    
    assert(size == strlen(content));
    assert(memcmp(data, content, size) == 0);
    free(data);

    // Test 2: Non-existing file
    decrypted_content = get_decrypted_config("nonexistent.txt");
    assert(decrypted_content == NULL);
}

int main() {
    test_read_encrypted_file();

    // Deleting the test file after the tests are done
    if (remove("test.txt") != 0) {
        perror("Error deleting file");
    }

    // Add calls to other test functions here
    printf("All tests passed.\n");
    return 0;
}
