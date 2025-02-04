#include <iostream>
#include <gmp.h>

int main() {
    mpz_t a, b, result;

    // Initialize GMP integers
    mpz_init(a);
    mpz_init(b);
    mpz_init(result);

    // Set values for a and b
    mpz_set_str(a, "123456789012345678901234567890", 10);  // Set large number (base 10)
    mpz_set_str(b, "987654321098765432109876543210", 10);  // Another large number (base 10)

    // Perform addition
    mpz_add(result, a, b);

    // Print the result
    std::cout << "Result of a + b: ";
    mpz_out_str(stdout, 10, result);  // Print result in base 10
    std::cout << std::endl;

    // Perform multiplication
    mpz_mul(result, a, b);
    std::cout << "Result of a * b: ";
    mpz_out_str(stdout, 10, result);
    std::cout << std::endl;

    // Clear GMP integers to free memory
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(result);

    return 0;
}
