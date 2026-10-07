#include <CUnit/Basic.h>

// Prototypes des fonctions d'ajout de suite définies dans les autres fichiers
extern int ajouter_suite_ville(void);
extern int ajouter_suite_direction(void);
extern int ajouter_suite_chemin(void);
extern int ajouter_suite_pilote(void);

int main() {
    // 1. Initialiser le registre
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // 2. Ajouter toutes les suites
    if (ajouter_suite_ville() != 0 || 
        ajouter_suite_direction() != 0 ||
        ajouter_suite_chemin() != 0 ||
        ajouter_suite_pilote() != 0) {
        
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 3. Lancer les tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // 4. Nettoyer
    CU_cleanup_registry();
    return 0;
}