#include <CUnit/Basic.h>
#include "direction.h"

void test_direction_oppose(void) {
    CU_ASSERT_EQUAL(direction_oppose(NORD), SUD);
    CU_ASSERT_EQUAL(direction_oppose(SUD), NORD);
    CU_ASSERT_EQUAL(direction_oppose(EST), OUEST);
    CU_ASSERT_EQUAL(direction_oppose(OUEST), EST);
}
void test_direction_oppose_symetrie(void) {
    direction dirs[] = {NORD, SUD, EST, OUEST};

    for (int i = 0; i < 4; i++) {
        CU_ASSERT_EQUAL(
            direction_oppose(direction_oppose(dirs[i])),
            dirs[i]
        );
    }
}
void test_char_vers_dir_valide(void) {
    CU_ASSERT_EQUAL(char_vers_dir('N'), NORD);
    CU_ASSERT_EQUAL(char_vers_dir('S'), SUD);
    CU_ASSERT_EQUAL(char_vers_dir('E'), EST);
    CU_ASSERT_EQUAL(char_vers_dir('O'), OUEST);
}
void test_char_vers_dir_invalide(void) {
    CU_ASSERT_EQUAL(char_vers_dir('X'), NORD);
    CU_ASSERT_EQUAL(char_vers_dir(' '), NORD);
    CU_ASSERT_EQUAL(char_vers_dir('n'), NORD); // minuscule non gérée
}
void test_nouvelle_direction_horizontal(void) {
    CU_ASSERT_EQUAL(nouvelle_direction(2, 1, 4), EST);
    CU_ASSERT_EQUAL(nouvelle_direction(1, 2, 4), OUEST);
}
void test_nouvelle_direction_vertical(void) {
    CU_ASSERT_EQUAL(nouvelle_direction(5, 1, 4), SUD);
    CU_ASSERT_EQUAL(nouvelle_direction(1, 5, 4), NORD);
}
void test_nouvelle_direction_invalide(void) {
    CU_ASSERT_EQUAL(nouvelle_direction(3, 1, 4), NORD); // diff = 2
}

int ajouter_suite_direction(void) {
    CU_pSuite suite = CU_add_suite("Tests Direction", NULL, NULL);
    if (NULL == suite) return -1;

    if ((NULL == CU_add_test(suite, "direction oppose", test_direction_oppose)) ||
        (NULL == CU_add_test(suite, "nouvelle direction horizontale", test_nouvelle_direction_horizontal)) ||
        /* ... ajoutez tous les autres tests ici ... */
        (NULL == CU_add_test(suite, "nouvelle direction invalide", test_nouvelle_direction_invalide)))
    {
        return -1;
    }
    return 0;
}