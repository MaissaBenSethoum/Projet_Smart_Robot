#include <CUnit/Basic.h>
#include "Ville.h"

void test_ajouter_connexion_simple(void) {
    Ville v;
    memset(&v, 0, sizeof(Ville));

    ajouter_connexion(&v, 1, 2);

    CU_ASSERT_EQUAL(v.grille[0].nb_voisins, 1);
    CU_ASSERT_EQUAL(v.grille[1].nb_voisins, 1);

    CU_ASSERT_EQUAL(v.grille[0].voisins[0], 2);
    CU_ASSERT_EQUAL(v.grille[1].voisins[0], 1);
}
void test_ajouter_connexion_max_voisins(void) {
    Ville v;
    memset(&v, 0, sizeof(Ville));

    for (int i = 2; i <= Max_Voisins + 2; i++) {
        ajouter_connexion(&v, 1, i);
    }

    CU_ASSERT_EQUAL(v.grille[0].nb_voisins, Max_Voisins);
}

void test_charger_ville_ok(void) {
    Ville v;
    FILE *f = fopen("test_ville.txt", "w");

    fprintf(f,
        "3\n"
        "1N\n"
        "2\n"
        "1-2\n"
        "2-3\n"
    );
    fclose(f);

    int res = charger_ville("test_ville.txt", &v);

    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(v.largeur, 3);
    CU_ASSERT_EQUAL(v.nb_total_case, 9);
    CU_ASSERT_EQUAL(v.position_initiale, 1);
    CU_ASSERT_EQUAL(v.direction_initiale, 'N');
    CU_ASSERT_EQUAL(v.nb_points_bip, 1);
    CU_ASSERT(v.grille[1].est_bip == 1);
}
void test_charger_ville_trop_grande(void) {
    Ville v;
    FILE *f = fopen("test_ville2.txt", "w");

    fprintf(f,
        "11\n"   // 121 cases > 100
        "1\n"
        "N\n"
    );
    fclose(f);

    int res = charger_ville("test_ville2.txt", &v);
    CU_ASSERT_EQUAL(res, 0);
}
void test_charger_ville_fichier_inexistant(void) {
    Ville v;
    int res = charger_ville("fichier_inexistant.txt", &v);
    CU_ASSERT_EQUAL(res, 0);
}
void test_afficher_resume_ville(void) {
    Ville v;
    memset(&v, 0, sizeof(Ville));

    v.largeur = 2;
    v.nb_total_case = 4;
    v.position_initiale = 1;
    v.direction_initiale = 'E';

    ajouter_connexion(&v, 1, 2);

    afficher_resume_ville(&v);
    CU_ASSERT_TRUE(1); // si on arrive ici, c'est OK
}


int ajouter_suite_ville(void) {
    CU_pSuite suite = CU_add_suite("Tests Ville", NULL, NULL);
    if (NULL == suite) return -1;

    if ((NULL == CU_add_test(suite, "ajouter connexion simple", test_ajouter_connexion_simple)) ||
        (NULL == CU_add_test(suite, "ajouter connexion max voisins", test_ajouter_connexion_max_voisins)) ||
        (NULL == CU_add_test(suite, "charger ville OK", test_charger_ville_ok)) ||
        (NULL == CU_add_test(suite, "charger ville trop grande", test_charger_ville_trop_grande)) ||
        (NULL == CU_add_test(suite, "charger ville fichier inexistant", test_charger_ville_fichier_inexistant)) ||
        (NULL == CU_add_test(suite, "afficher resume ville", test_afficher_resume_ville)))
    {
        return -1;
    }
    return 0;
}