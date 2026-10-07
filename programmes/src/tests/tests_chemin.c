#include <CUnit/Basic.h>
#include <stdio.h>
#include "Chemin.h"
#include "Ville.h"


static void initialiser_ville_simple(Ville* v, int nb_cases) {
    v->nb_total_case = nb_cases;
    v->largeur = nb_cases;

    for (int i = 0; i < nb_cases; i++) {
        v->grille[i].id = i + 1;
        v->grille[i].nb_voisins = 0;
        v->grille[i].est_bip = 0;
    }
}

static void connecter(Ville* v, int a, int b) {
    v->grille[a - 1].voisins[v->grille[a - 1].nb_voisins++] = b;
    v->grille[b - 1].voisins[v->grille[b - 1].nb_voisins++] = a;
}

void test_inverser_tableau(void) {
    int t[] = {1, 2, 3, 4, 5};
    inverser_tableau(t, 5);

    CU_ASSERT_EQUAL(t[0], 5);
    CU_ASSERT_EQUAL(t[1], 4);
    CU_ASSERT_EQUAL(t[2], 3);
    CU_ASSERT_EQUAL(t[3], 2);
    CU_ASSERT_EQUAL(t[4], 1);
}


void test_chercher_chemin_simple(void) {
    Ville v;
    int chemin[Max_Cases];

    initialiser_ville_simple(&v, 3);
    connecter(&v, 1, 2);
    connecter(&v, 2, 3);

    int longueur = chercher_chemin(&v, 1, 3, -1, chemin);

    CU_ASSERT_EQUAL(longueur, 3);
    CU_ASSERT_EQUAL(chemin[0], 1);
    CU_ASSERT_EQUAL(chemin[1], 2);
    CU_ASSERT_EQUAL(chemin[2], 3);
}

void test_chercher_chemin_case_interdite(void) {
    Ville v;
    int chemin[Max_Cases];

    initialiser_ville_simple(&v, 3);
    connecter(&v, 1, 2);
    connecter(&v, 2, 3);

    int longueur = chercher_chemin(&v, 1, 3, 2, chemin);

    CU_ASSERT_EQUAL(longueur, 0);
}


void test_chercher_chemin_depart_arrivee_identique(void) {
    Ville v;
    int chemin[Max_Cases];

    initialiser_ville_simple(&v, 1);

    int longueur = chercher_chemin(&v, 1, 1, -1, chemin);

    CU_ASSERT_EQUAL(longueur, 1);
    CU_ASSERT_EQUAL(chemin[0], 1);
}


void test_chercher_chemin_cycle(void) {
    Ville v;
    int chemin[Max_Cases];

    initialiser_ville_simple(&v, 3);
    connecter(&v, 1, 2);
    connecter(&v, 2, 3);
    connecter(&v, 3, 1);

    int longueur = chercher_chemin(&v, 1, 3, -1, chemin);

    CU_ASSERT(longueur >= 2);
    CU_ASSERT_EQUAL(chemin[0], 1);
    CU_ASSERT_EQUAL(chemin[longueur - 1], 3);
}


void test_chercher_chemin_aucun(void) {
    Ville v;
    int chemin[Max_Cases];

    initialiser_ville_simple(&v, 3);
    connecter(&v, 1, 2);

    int longueur = chercher_chemin(&v, 1, 3, -1, chemin);

    CU_ASSERT_EQUAL(longueur, 0);
}

/* ========================= */
/*   MAIN CUNIT              */
/* ========================= */

int ajouter_suite_chemin(void) {
    CU_pSuite suite = CU_add_suite("Tests Chemin", NULL, NULL);
    if (NULL == suite) return -1;

    if ((NULL == CU_add_test(suite, "Inverser tableau", test_inverser_tableau)) ||
        (NULL == CU_add_test(suite, "Chemin simple", test_chercher_chemin_simple)) ||
        (NULL == CU_add_test(suite, "Chemin avec case interdite", test_chercher_chemin_case_interdite)) ||
        (NULL == CU_add_test(suite, "Depart = Arrivee", test_chercher_chemin_depart_arrivee_identique)) ||
        (NULL == CU_add_test(suite, "Graphe cyclique", test_chercher_chemin_cycle)) ||
        (NULL == CU_add_test(suite, "Aucun chemin", test_chercher_chemin_aucun)))
    {
        return -1;
    }
    return 0;
}