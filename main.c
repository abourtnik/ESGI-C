#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "compte.h"

#include "sqlite3.c"
#include "sqlite3.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s : %s   ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int callback_count(void *NotUsed, int argc, char **argv, char **azColName) {

    return atoi(argv[0] ? argv[0] : "NULL");
}

void cleaninput() {
    int p;
    while ((p=getchar())!='\n'&& p!=EOF);
}

int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    // On lit le texte saisi au clavier
    if (fgets(chaine, longueur, stdin) != NULL)  // Pas d'erreur de saisie ?
    {
        positionEntree = strchr(chaine, '\n'); // On recherche l'"Entrée"
        if (positionEntree != NULL) // Si on a trouvé le retour à la ligne
        {
            *positionEntree = '\0'; // On remplace ce caractère par \0
        }
        return 1; // On renvoie 1 si la fonction s'est déroulée sans erreur
    }
    else
    {
        return 0; // On renvoie 0 s'il y a eu une erreur
    }
}

int main() {

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql [500];
    const char* data = "Callback function called";

    char choix_bis [10];
    int choix = 0;

    int id = 0;
    int id2= 0;
    double somme = 0.0;

    char temp_nom[100] = "";
    char temp_prenom[100] = "";
    char temp_profession[1000] = "";
    char temp_telephone[1000] = "";

    char search[100];


    // Connexion

    rc = sqlite3_open("banque.db", &db);

    if( rc ) {
        fprintf(stderr, "Echec de connexion a la base de donness : %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        printf("Connexion a la base de donness reussi\n");
    }


    printf("\n\n ---------- BIENVENUE DANS LA BANQUE ! ---------- \n\n");


    // Afficher tant que l'utilisateur n'a pas choisit 0

    do {

        // Le menu


        printf("MENU GENERALE : \n\n");

        printf("*** GESTION DES CLIENTS ***\n\n");

        printf("1 ---------- Creer un client\n");
        printf("2 ---------- Liste des clients\n");
        printf("3 ---------- Modifier un client\n");
        printf("4 ---------- Supprimmer un client\n");
        printf("5 ---------- Rechercher un client\n\n");

        printf("*** GESTION DES COMPTES ***\n\n");

        printf("6 ---------- Creer un compte\n");
        printf("7 ---------- Liste des comptes\n");
        printf("8 ---------- Liste des comptes d'un client\n");
        printf("9 ---------- Fermer un compte\n\n");

        printf("*** GESTION DES OPERATIONS ***\n\n");

        printf("10 ---------- Crediter un compte\n");
        printf("11 ---------- Debiter un compte\n");
        printf("12 ---------- Effectuer un virement\n");
        printf("13 ---------- Liste des operations\n\n");

        printf("0 ---------- QUITTER\n\n");
        printf("---------- Votre choix : ");

        scanf("%s", &choix_bis);
        choix = atoi(choix_bis);

        // Choix de l'utilisateur

        switch (choix) {
            case 1:

                printf("\n---------- CREATION D'UN CLIENT ----------\n\n");

                Client c;

                printf("---------- Prenom : ");
                scanf("%s", &c.prenom);
                printf("---------- Nom : ");
                scanf("%s", &c.nom);
                printf("---------- Profession : ");
                scanf("%s", &c.profession);
                printf("---------- Telephone : ");
                scanf("%s", &c.telephone);

                sprintf(sql, "INSERT INTO Client (nom,prenom,profession,telephone) VALUES ('%s', '%s', '%s', '%s');" , c.nom , c.prenom , c.profession , c.telephone);

                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

                if( rc != SQLITE_OK ){
                    fprintf(stderr, "Echec d'insertion : %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    printf("\n---------- CLIENT CREE AVEC SUCCEES ! ---------- \n\n");;
                }

                break;
            case 2:

                printf("\n---------- LISTE DES CLIENTS DE LA BANQUE : ---------- \n\n");

                strcpy(sql, "SELECT * from Client");

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recupration: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");

                break;
            case 3:

                printf("\n---------- Entrez l'identifiant du client à modifier : ");
                scanf("%d", &id);

                Client c2;

                printf("---------- Prenom : ");
                scanf("%s", &c2.prenom);
                printf("---------- Nom : ");
                scanf("%s", &c2.nom);
                printf("---------- Profession : ");
                scanf("%s", &c2.profession);
                printf("---------- Telephone : ");
                scanf("%s", &c2.telephone);

                printf("ID : %d\n" , id);

                sprintf(sql, "UPDATE Client set nom = '%s' , prenom = '%s' , profession = '%s', telephone = '%s' WHERE id=%d;" , c2.nom , c2.prenom , c2.profession , c2.telephone , id);

                printf("SQL : %s\n" , sql);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN CLIENT CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE CLIENT A ETE MODIFIE AVEC SUCCESS ---------- \n\n");
                }

                break;
            case 4:

                printf("\n---------- Entrez l'identifiant du client à supprimer : ");
                scanf("%d", &id);

                printf("ID : %d\n" , id);

                sprintf(sql, "DELETE from Client WHERE id = %d;" , id);

                printf("SQL : %s\n" , sql);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN CLIENT CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE CLIENT A ETE SUPPRIME AVEC SUCCESS ---------- \n\n");
                }

                break;

                break;
            case 5:

                printf("\n---------- Entrez le nom du client : ");
                cleaninput();
                lire(search , 100);

                printf("\n---------- LISTE DES CLIENTS TROUVES corespondant a '%s' : ---------- \n\n" , search);

                sprintf(sql, "SELECT * FROM Client WHERE nom LIKE '%%%s%%';" , search);

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recuperation: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");


                break;
            case 6:

                printf("\n---------- CREATION D'UN COMPTE ----------\n\n");

                printf("\n---------- Entrez l'identifiant du client : ");
                scanf("%d", &id);

                Compte compte;

                printf("---------- Entrez le taux du compte : ");
                scanf("%lf", &compte.taux);

                sprintf(sql, "INSERT INTO Compte (taux,id_client) VALUES ('%lf', %d);" , compte.taux , id);

                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

                if( rc != SQLITE_OK ){
                    fprintf(stderr, "Echec d'insertion : %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    printf("\n---------- COMPTE CREE AVEC SUCCEES ! ---------- \n\n");;
                }

                break;

            case 7:

                // Liste des comptes

                printf("\n---------- LISTE DES COMPTES DE LA BANQUE : ---------- \n\n");

                strcpy(sql, "SELECT * from Compte");

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recupration: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");

                break;


            case 8:

                printf("\n---------- Entrez l'identifiant du client : ");
                scanf("%d", &id);

                // Liste des comptes d'u client

                printf("\n---------- LISTE DES COMPTES DU CLIENT %d : ---------- \n\n" , id);

                sprintf(sql, "SELECT Compte.id , Compte.solde , Compte.taux , Client.nom from Compte INNER JOIN Client on Client.id = Compte.id_client WHERE Client.id = %d;" , id);

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recupration: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");

                // Total solde

                printf("\n---------- TOTAL SOLDE : ---------- \n\n");

                sprintf(sql, "SELECT  SUM(solde) as Total From Compte WHERE id_client = %d;" , id);

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recupration: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");


                break;

            case 9:

                printf("\n---------- Entrez l'identifiant du compte à fermer : ");
                scanf("%d", &id);

                printf("ID : %d\n" , id);

                sprintf(sql, "DELETE from Compte WHERE id = %d;" , id);

                printf("SQL : %s\n" , sql);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN COMPTE CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE COMTE A ETE FERME AVEC SUCCESS ---------- \n\n");
                }


                break;

            case 10:

                printf("\n---------- Entrez l'identifiant du compte à créditer : ");
                scanf("%d", &id);

                printf("\n---------- Entrez la somme à créditer : ");
                scanf("%lf", &somme);

                sprintf(sql, "UPDATE Compte SET solde = solde + %lf WHERE id = %d;" , somme , id);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN COMPTE CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE COMTE A ETE CREDITE AVEC SUCCESS ---------- \n\n");
                }

                // Historique

                sprintf(sql, "INSERT INTO Operation (type, somme , compte_id_1) VALUES ('credit', %lf , %d);" , somme , id);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec d'insertion 1 : %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- ENREGISTRE ---------- \n\n");
                }

                break;

            case 11:

                printf("\n---------- Entrez l'identifiant du compte à debiter : ");
                scanf("%d", &id);

                printf("\n---------- Entrez la somme à debiter : ");
                scanf("%lf", &somme);

                sprintf(sql, "UPDATE Compte SET solde = solde - %lf WHERE id = %d;" , somme , id);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN COMPTE CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE COMTE A ETE DEBITE AVEC SUCCESS ---------- \n\n");
                }

                // Historique

                sprintf(sql, "INSERT INTO Operation (type, somme , compte_id_1) VALUES ('debit', %lf , %d);" , somme , id);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec d'insertion 2 : %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- ENREGISTRE ---------- \n\n");
                }

                break;

            case 12:

                printf("\n---------- VIREMENT ----------");

                printf("\n---------- Entrez l'identifiant du compte à debiter : ");
                scanf("%d", &id);

                printf("\n---------- Entrez l'identifiant du compte à crediter : ");
                scanf("%d", &id2);

                printf("\n---------- Entrer la somme du transfert : ");
                scanf("%lf", &somme);


                // Debit

                sprintf(sql, "UPDATE Compte SET solde = solde - %lf WHERE id = %d;" , somme , id);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN COMPTE CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE COMTE A ETE DEBITE AVEC SUCCESS ---------- \n\n");
                }

                // Credit

                sprintf(sql, "UPDATE Compte SET solde = solde + %lf WHERE id = %d;" , somme , id2);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    printf("\n-------- AUCUN COMPTE CORRESPONDANT A CETTE ID N A ETE TROUVE ---------- \n\n");
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- LE COMTE A ETE CREDITER AVEC SUCCESS ---------- \n\n");
                }

                // Historique

                sprintf(sql, "INSERT INTO Operation (type, somme , compte_id_1 , compte_id_2) VALUES ('virement', %lf , %d , %d);" , somme , id , id2);

                rc = sqlite3_exec(db, sql, NULL , (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec d'insertion 3 : %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }
                else {
                    printf("\n---------- ENREGISTRE ---------- \n\n");
                }

                break;

            case 13:

                printf("\n---------- LISTE DES OPERATIONS DE LA BANQUE : ---------- \n\n");

                strcpy(sql, "SELECT * FROM Operation");

                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "Echec recupration: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }

                printf("\n");

                break;

            default:

                printf("\n\n --------- Veuillez choisir un nombre correct !!! -------- \n\n");
                break;
        }

    } while (choix_bis != choix != 0);

    return 0;
}