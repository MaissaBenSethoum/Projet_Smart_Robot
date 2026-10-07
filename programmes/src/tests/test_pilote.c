#include <CUnit/Basic.h>
#include <stdio.h>
#include "Ville.h"
#include "Chemin.h"
#include "direction.h"
#include "pilote.h"

static void initialiser_ville(Ville* v, int nb_cases, int largeur) {
    v->nb_total_case = nb_cases;
    v->largeur = largeur;
    v->nb_points_bip = 0;

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


void test_est_intersection(void) {
    Case c;

    c.nb_voisins = 2;
    CU_ASSERT_EQUAL(est_intersection(&c, NULL), 0);

    c.nb_voisins = 3;
    CU_ASSERT_EQUAL(est_intersection(&c, NULL), 1);

    c.nb_voisins = 4;
    CU_ASSERT_EQUAL(est_intersection(&c, NULL), 1);
}

void test_traduction_ordres_ligne_droite(void) {
    Ville v;
    int chemin[] = {1, 2, 3};
    TypeOrdre ordres[10];
    int nb_ordres = 0;

    initialiser_ville(&v, 3, 3);
    connecter(&v, 1, 2);
    connecter(&v, 2, 3);

    direction dir_fin = traduction_des_ordres(
        chemin,
        EST,
        3,
        &v,
        ordres,
        &nb_ordres
    );

    CU_ASSERT_EQUAL(nb_ordres, 1);
    CU_ASSERT_EQUAL(ordres[0], O_AV);
    CU_ASSERT_EQUAL(dir_fin, EST);
}


void test_traduction_virage_droite(void) {
    Ville v;
    int chemin[] = {1, 2, 4};
    TypeOrdre ordres[10];
    int nb_ordres = 0;

    initialiser_ville(&v, 4, 2);
    connecter(&v, 1, 2);
    connecter(&v, 2, 4);

    direction dir_fin = traduction_des_ordres(
        chemin,
        NORD,
        3,
        &v,
        ordres,
        &nb_ordres
    );

    CU_ASSERT_EQUAL(ordres[0], O_TD);
    CU_ASSERT_EQUAL(ordres[1], O_AV);
    CU_ASSERT_EQUAL(ordres[2], O_TD);
    CU_ASSERT_EQUAL(ordres[3], O_AV);
    CU_ASSERT_EQUAL(nb_ordres, 4);
    CU_ASSERT_EQUAL(dir_fin, SUD);
}


void test_optimiser_ordre_simple(void) {
    Ville v;
    int resultat[3];

    initialiser_ville(&v, 4, 4);

    connecter(&v, 1, 2);
    connecter(&v, 2, 3);
    connecter(&v, 3, 4);
    connecter(&v, 4, 1);

    v.Points_Bip[0] = 2;
    v.Points_Bip[1] = 3;
    v.Points_Bip[2] = 4;
    v.nb_points_bip = 3;

    int total = optimiser_ordre(&v, 1, resultat);

    CU_ASSERT(total > 0);
    CU_ASSERT(resultat[0] != resultat[1]);
    CU_ASSERT(resultat[1] != resultat[2]);
    CU_ASSERT(resultat[0] != resultat[2]);
}

int ajouter_suite_pilote(void) {
    CU_pSuite suite = CU_add_suite("Tests Ordres", NULL, NULL);
    if (NULL == suite) return -1;

    if ((NULL == CU_add_test(suite, "est_intersection", test_est_intersection)) ||
        (NULL == CU_add_test(suite, "traduction ligne droite", test_traduction_ordres_ligne_droite)) ||
        (NULL == CU_add_test(suite, "traduction virage droite", test_traduction_virage_droite)) ||
        (NULL == CU_add_test(suite, "optimiser ordre simple", test_optimiser_ordre_simple)))
    {
        return -1;
    }
    return 0;
}
