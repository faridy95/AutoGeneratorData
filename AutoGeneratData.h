#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"
#include <time.h>

#define lmax 80

int tail = 0, *tab, m = 0, size_p = 0, i, size_m = 0, size_n = 0;
cJSON *json, *regist, *suffixe, *name, *ind, *val_at, *val_rel, *val_ar,  *rel_i, *link, *nom,  **tabrel,**tab_rel, *fichierjson;
char *titleR , *titleA, **adresse;

//structure d'une relation

typedef struct
{
  int indic_don;
  int *tab_dep;
  int tail_dep;
} relation;

relation **depend;

typedef struct
{
  char car[lmax];
} tab_char;

tab_char **matrix;

typedef struct
{
  char *lnom;
  char *adress;
} liste;

//Enregistrement de nom de l'objet à l'indice j dans tab_elt à l'indice j
cJSON **record()
{
  cJSON **tab_elt;
  int j;
  //tail = cJSON_GetArraySize(json);
  tab_elt = malloc(tail*sizeof(cJSON*));
  for(j = 0; j < tail; j++)
    {
      rel_i = cJSON_GetArrayItem(json,j);
      nom = cJSON_GetObjectItem(rel_i,"NomRelation");
      tab_elt[j] = nom;
    }
  return tab_elt;
}

//Enregistrement des indices des rélations auxquelles sont liées les autres
int *conversion(cJSON **tab_elt, cJSON **depend, int tail_dep)
{
  int *convert,*p,j;

  convert = malloc(tail_dep*sizeof(int));
  for(j = 0; j < tail_dep; j++)
    {
      for(i = 0; i < tail; i++)
	{
	  if(strcmp(cJSON_Print(depend[j]),cJSON_Print(tab_elt[i])) == 0)
	    {
	      convert[j] = i;
	    }
	}
    }
  return convert;
}

//Fonction permettent de determiner l'ordre de génération des données
void ordre(int indic, relation **dep)
{
  if(dep[indic] -> tail_dep == 0)
    {
      if(dep[indic] -> indic_don != -1)
        {
          tab[m] = dep[indic] -> indic_don;
          m++;
          dep[indic] -> indic_don = -1;
        }
    }
  else
    {
      for(i = 0; i < dep[indic] -> tail_dep ; i++)
        {
          if(dep[indic] -> tab_dep[i] > dep[indic] -> indic_don)
            ordre(dep[indic] -> tab_dep[i],dep);
          if(i == (dep[indic] -> tail_dep)-1)
            {

              if(dep[indic] -> indic_don != -1)
                {
                  tab[m] = dep[indic] -> indic_don;
                  dep[indic] -> indic_don = -1;
                  m = m+1;
                }
            }
        }
    }
}

//Recherche de dependance et rélève des données
void recherche_link()
{
  int j, k, n, l, tail_tabat, cpt, *conv;

  tail = cJSON_GetArraySize(json);
  tab = malloc(tail*sizeof(int));
  depend = malloc(tail*sizeof(relation*));
  tabrel = record(json);

  //Recuperation des données de chaque relation
  for(j = 0; j < tail; j++)
    {
      rel_i = cJSON_GetArrayItem(json,j);
      val_at = cJSON_GetObjectItem(rel_i,"attributs"); //récupération du tableau d'attributs.
      tail_tabat = cJSON_GetArraySize(val_at);
      tab_rel = malloc(tail_tabat*sizeof(cJSON*));
      depend[j] = malloc(sizeof(relation));

      //instruction pour chaque attribut de la relation j
      k = 0;
      for(i = 0; i < tail_tabat; i++)
        {
          val_ar = cJSON_GetArrayItem(val_at,i);
          link = cJSON_GetObjectItem(val_ar,"link");
          cpt = 0;

          //Existence de liaison
          if(link != NULL)
            {
              link = cJSON_GetObjectItem(link,"NomRelation");
              for(n = 0; n < k; n++)
                {
                  if(tab_rel[n] == link)
                    {
                      cpt = 1;
                      break;
                    }
                }
                if(cpt == 0)
                {
                  tab_rel[k] = link;
                  k++;
                }
            }
        }//fin d'attribut

        if(k != 0)
        {
          conv = conversion(tabrel,tab_rel,k);
          depend[j] -> indic_don = j;
          depend[j] -> tab_dep = conv;
          depend[j] -> tail_dep = k;
        }
        else
        {
          depend[j] -> indic_don = j;
          depend[j] -> tab_dep = NULL;
          depend[j] -> tail_dep = 0;
        }
    }
}

//Génération des entiers consécutifs
int *serial(int min, int max, int N)
{
  int *tab_int, j;
  tab_int = malloc(N*sizeof(int));
  srand(time(NULL));
  for(j = 0; j < N; j++)
    {
      tab_int[j] = min + j;
    }
  return tab_int;
}

//Génération des entiers sans doublons
int *entier_dist(int min, int max, int N)
{
  int taille,nombre_tire=0, temp=0, * tableau, *tab_int,j;

  srand(time(NULL));
  taille = max-min;
  tableau= malloc((taille)*sizeof (int));
  tab_int = malloc(N*sizeof (int));
  for(j = 0; j < taille; j++)
    tableau[j] = j+min;
  for(j = 0; j < taille; j++)
    {
      nombre_tire = rand()%taille;
      temp = tableau[j];
      tableau[j] = tableau[nombre_tire];
      tableau[nombre_tire]=temp;
    }
  for(j = 0; j < N; j++)
    {
      tab_int[j] = tableau[j];
    }
  return tab_int;
}

//Génération des entiers avec doublons
int *entier_rep(int min, int max, int N)
{
  int j, * tableau;

  srand(time(NULL));
  tableau=malloc(N*sizeof (int));
  for(j = 0; j < N; j++)
    {
      tableau[j]= rand()%(max - min)+ min;
    }
  return tableau;
}

//Génération des flottants tous distincts
float *decimaux_dist(float min, float max, int N)
{
  int nombre_tire, i, size, taille;
  float temp, *tableau, *tab_float = calloc(sizeof(float),N);

  taille = (int)(max-min);
  printf("%d\n", taille);
  srand(time(NULL));
  tableau = calloc(sizeof(float),taille);
  for(i = 0; i < taille; i++)
    {
      tableau[i]= i + min+ (rand()/(float)RAND_MAX)*1;
    }
  for(i = 0; i< taille; i++)
    {
      nombre_tire = rand()%taille;
      temp = tableau[i];
      tableau[i] = tableau[nombre_tire];
      tableau[nombre_tire]=temp;
    }
  if(N <= taille)
    {
      for(i = 0; i < N; i++)
        {
          tab_float[i] = tableau[i];
        }
    }
  return tab_float;
}

//Génération des nombres flottant avec doublons
float *decimaux_rep(float min, float max, int N)
{
  float *tableau = malloc(N*sizeof(float));

  srand(time(NULL));
  for(i = 0; i < N; i++)
    {
      tableau[i]= (rand()/(float)RAND_MAX)*(max-min)+min;
    }
  return tableau;
}

//Fonction permettant d'oter les doubles cott entourant une chaine
char *oter_cot(char *chaine)
{
  int len = strlen(chaine), k = 0;
  char *point = strstr(chaine, "\""), *tmp = calloc(sizeof(char),len);
  if(point)
    for(i = 0; i < len; i++)
      {
	while(*point == '\"')
	  point = point +1;
	tmp[k] = *point;
	k++;
	point = point +1;
      }
  else
    strcpy(tmp, chaine);

  return tmp;
}


//Création d'un tableau de noms générer aléatoirement
char **make_name(int N)
{

  int taille = 32, *tabl = calloc(sizeof(int), taille), j, k, l, temp, t, vowel[] = {97,101,105,117,111,121};
  char *nom, **tab_nom = calloc(sizeof(char*), N);
  srand(time(NULL));

  for(k = 0; k < 6; k++)
    {
      tabl[k]= vowel[k];
    }
  for(j = k; j < taille; j++)
    tabl[j] = rand()%(123-97)+97;
  for(k = 0; k< taille ;k++)
    {
      t = rand()%taille;
      temp = tabl[k];
      tabl[k] = tabl[t];
      tabl[t] = temp;
    }
  for(l = 0; l < N; l++)
    {
      tab_nom[l] = calloc(sizeof(char), taille);
      nom = calloc(sizeof(char),taille);
      j = (rand()%(9 - 4)+4);

      for(t = 0; t < 5; t++)
	{
	  k = rand()%taille;
	  nom[t] = tabl[k];
	}
      printf("\n");
      strcpy(tab_nom[l], nom);

    }
  return tab_nom;
}


//Création d'une adresse mail
char *email(char *nom, int k)
{
  char *mail = calloc(sizeof(char),lmax);
  strcpy(mail,nom);
  strcat(mail,oter_cot(cJSON_Print(cJSON_GetArrayItem(suffixe,k))));
  return mail;
}

char **make_mail(int N)
{
  int l;
  char **mail = calloc(sizeof(char*),N), *tmp = calloc(sizeof(char*),lmax);
  srand(time(NULL));
  mail = make_name(N);
  for(l = 0; l < N; l++)
    {
      strcpy(mail[l],email(mail[l], rand()%size_m));
    }
  return mail;
}

liste **make_nom(int min, int max, int flag, int N, int flag2)
{
  int n, k = 0, taille = 32, j;
  char *nam;
  liste **tab_nom = calloc(sizeof(liste*),N);

  srand(time(NULL));

  while(k < N)
    {
      nam = calloc(sizeof(char),lmax);
      ind = cJSON_GetArrayItem(name,rand()%2);
      n = cJSON_GetArraySize(ind);
      ind = cJSON_GetArrayItem(ind,rand()%n);
      nam = oter_cot(cJSON_Print(ind));
      n = strlen(nam);
      if(n <= max && n >= min)
        {
          tab_nom[k] = calloc(sizeof(liste),n);
          if(flag2 == 1)
            {
              for(j = 0; j < k; j++)
                {
                  if(strcmp(tab_nom[j] -> lnom ,nam) == 0)
                    {
                      n = -1;
                      break;
                    }
                }
            }
          if(n != -1)
            {
              j = rand()%size_m;
              tab_nom[k] -> lnom = calloc(sizeof(char),lmax);
              strcpy(tab_nom[k] -> lnom,nam);
              if(flag == 1)
                {
                  tab_nom[k] -> adress = calloc(sizeof(char),lmax);
                  tab_nom[k] -> adress = email(tab_nom[k] -> lnom,j);

                }
              k++;
            }
        }
    }
  return tab_nom;
}

//Génération de date et de l'heure
char **date(int N)
{
  int heure, min, sec, jr,mois,annee;
  char *cal,tmp[4], **tab_date = calloc(sizeof(char*),N);

  srand(time(NULL));
  for(i = 0; i < N; i++)
    {
      cal = calloc(sizeof(char),lmax);
      tab_date[i] = calloc(sizeof(char),lmax);
      heure = rand()%24;
      min = rand()%60;
      sec = rand()%60;
      mois =rand()%(13 - 1)+1;
      annee =rand()%(2021-1900)+1900;
      if((mois%2 == 0 && mois != 10 && mois != 8) || mois == 11 || mois == 9)
	{
	  if(mois == 2 && ((annee%4 == 0 && annee%100 != 0) || annee%400 == 0))
	    jr =rand()%(30-1)+1;
	  else if(mois == 2)
	    jr =rand()%(31-1)+1;
	  else
	    jr =rand()%(31-1)+1;
	}
      else
	jr =rand()%(32-1)+1;
      sprintf(tmp,"%d",jr);
      strcat(strcat(cal,tmp),"/");
      sprintf(tmp,"%d",mois);
      strcat(strcat(cal,tmp),"/");
      sprintf(tmp,"%d",annee);
      strcat(strcat(cal,tmp),"  ");
      sprintf(tmp, "%d", heure);
      strcat(strcat(cal,tmp),":");
      sprintf(tmp, "%d", min);
      strcat(strcat(cal,tmp),":");
      sprintf(tmp, "%d", sec);
      strcat(cal,tmp);
      strcpy(tab_date[i], cal);
    }
  return tab_date;
}

//Génération du suffixe du numéro de téléphone
char *cell_phone()
{
  int j = 0,tabl[8];
  char tell[1]="", *fin = calloc(sizeof(char),lmax);
  for(i = 0; i < 8; i++)
    tabl[i] = rand()%10;
  for(i = 0; i < 8; i++)
    {
      sprintf(tell,"%d",tabl[i]);
      if(j == 2)
	{
	  j = 0;
	  strcat(fin," ");
	}
      strcat(fin,tell);
      j++;
    }
  return fin;
}

//Génération du préfixe puis numéro de téléphone
char **telephone(cJSON *pays,int N)
{
  int i,n;
  cJSON *ind, *pay;
  cJSON *obj;
  char *fone, num[lmax], **tab_num = calloc(sizeof(char*),N);
  srand(time(NULL));
  if(pays == NULL)
    {
      for(i = 0; i < N; i++)
        {
          tab_num[i] = calloc(sizeof(char),lmax);
          n = rand()%size_p;
          ind =cJSON_GetObjectItem(cJSON_GetArrayItem(regist,n),"indicatif");
          strcpy(num, oter_cot(cJSON_Print(ind)));
          fone = cell_phone();
          strcat(num, " ");
          strcat(num,fone);
          strcpy(tab_num[i],num);
        }
    }
  else
    {
      for(i = 0; i < size_p; i++)
        {
          obj = cJSON_GetArrayItem(regist,i);
          pay = cJSON_GetObjectItem(obj,"pays");
          if(pays == pay)
            {
              ind = cJSON_GetObjectItem(obj,"indicatif");
              break;
            }
        }
          for(i = 0; i < N; i++)
        {
          tab_num[i] = calloc(sizeof(char),lmax);
          strcpy(num, oter_cot(cJSON_Print(ind)));
          fone = cell_phone();
          strcat(num, " ");
          strcat(num,fone);
          strcpy(tab_num[i],num);
        }
    }
  return tab_num;
}

//Génération du genre
char **sexe(int N)
{
  char **tab_sexe;
  tab_sexe = calloc(sizeof(char*),N);
  srand(time(NULL));
  for(i = 0; i < N; i++)
    {
      tab_sexe[i] = calloc(sizeof(char),lmax);
      if(rand()%2 == 0)
	strcpy(tab_sexe[i],"Masculin");
      else if(rand()%2 == 1)
	strcpy(tab_sexe[i],"Féminin");
      else
	strcpy(tab_sexe[i],"Non repertorié");

    }
  return tab_sexe;
}

//Génération de l'age
int *age(int N)
{
  int i, *tabl;
  srand(time(NULL));
  tabl = calloc(sizeof(int),N);
  for(i = 0; i < N; i++)
    {
      tabl[i] = rand()%121;
    }
  return tabl;
}

//Génération des booléens
char **boolean(int N)
{
  char **tab_bool = calloc(sizeof(char*),N);
  for(i = 0; i < N; i++)
    {
      tab_bool[i] = calloc(sizeof(char),lmax);
      if(rand()%2 == 1)
	strcpy(tab_bool[i], "true");
      else
	strcpy(tab_bool[i],"false");
    }
  return tab_bool;
}

//si l'attribut contient un champ de choix
char **choix(cJSON *tabchoix, int N)
{
  int l;
  char **tab_choix = calloc(sizeof(char*),N);
  int nbre;
  nbre = cJSON_GetArraySize(tabchoix);

  for(l = 0; l < N; l++)
    {
      tab_choix[l] = calloc(sizeof(char),lmax);
      strcpy(tab_choix[l], oter_cot(cJSON_Print(cJSON_GetArrayItem(tabchoix,rand()%nbre))));
    }
  return tab_choix;
}

/*int *choixint(cJSON *tabchoix, int N)
{
  int l;
  int *tab_choix = calloc(sizeof(int),N);
  int nbre;
  nbre = cJSON_GetArraySize(tabchoix);

  for(l = 0; l < N; l++)
      tab_choix[l] = otoi(cJSON_Print(cJSON_GetArrayItem(tabchoix,rand()%nbre))));
  return tab_choix;
  }*/

//Fonctions qui fait appel aux petites fonctions de génération en fonction du types de l'attribut
void global_generator(cJSON *attribut, int indiceRel, int col, int N)
{
  int tabat_tail, min, max, flag = 0, flagnul = 0, flagmail = 0, flagserial = 0, repere = 0, j = 0;
  int *entier = calloc(sizeof(int),N) ;
  float min1, max1, *flottant = calloc(sizeof(float),N);
  char *type = calloc(sizeof(char),lmax), tmp[1] = "", **temp = calloc(sizeof(char*),N), *val = "vide";
  liste **list = calloc(sizeof(liste*),N);

  srand(time(NULL));
  titleA = calloc(sizeof(char),lmax);
  titleA = oter_cot(cJSON_Print(cJSON_GetObjectItem(attribut,"name")));
  strcpy((matrix[0]+col)-> car,titleA);
  type =  oter_cot(cJSON_Print(cJSON_GetObjectItem(attribut,"type")));
  rel_i = cJSON_GetObjectItem(attribut,"options");
  tabat_tail = cJSON_GetArraySize(rel_i);
  if(strcmp(type,"int") == 0)
    {
      min = atoi(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemin")));
      max = atoi(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemax")));
      if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"not null")),"true") == 0)
	flagnul = 1;
      if(cJSON_GetObjectItem(rel_i,"unique"))
	{
	  if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"unique")),"true") == 0)
	    flagmail = 1;
	}
      if(cJSON_GetObjectItem(rel_i,"serial"))
        {
          if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"serial")),"true") == 0)
            flagserial = 1;
        }
      if(cJSON_GetObjectItem(rel_i,"age"))
        {
          if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"age")),"true") == 0)
            flag = 1;
        }
      if(flag)
	entier = age(N);
      else if(flagserial)
	entier = serial(min, max, N);
      else if(flagmail)
	entier = entier_dist(min, max, N);
      else
	entier = entier_rep(min, max, N);
       for(i = 1; i < N; i++)
	    {
	      sprintf(tmp,"%d", entier[i-1]);
		  strcpy((matrix[i]+col)-> car, tmp);
	    }
    }

  //Si l'attribut est un flottant
  else if(strcmp(type,"float") == 0)
    {
      min1 = atof(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemin")));
      max1 = atof(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemax")));
      if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"not null")),"true") == 0)
	flagnul = 1;
      if(cJSON_GetObjectItem(rel_i,"unique"))
        {
          if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"unique")),"true") == 0)
            flagmail = 1;
        }
      if(flagmail)
	flottant = decimaux_dist(min1, max1, N);
      else
	flottant = decimaux_rep(min1, max1, N);
      for(i = 1; i < N; i++)
        {
          sprintf(tmp,"%.2f", flottant[i-1]);
          strcpy((matrix[i]+col)-> car,tmp);
        }
    }

  //S'il s'agit des booléens
  else if(strcmp(type,"boolean") == 0)
    {
      if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"not null")),"true") == 0)
	flagnul = 1;
      temp = boolean(N);
      min = strlen(titleA);
      for(i = 1; i < N; i++)
	strcpy((matrix[i]+col)-> car,temp[i-1]);
    }

  //Si l'attribut est de type string
  else if(strcmp(type,"string") == 0)
    {
      min = atoi(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemin")));
      max = atoi(cJSON_Print(cJSON_GetObjectItem(rel_i,"taillemax")));
      if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"not null")),"true") == 0)
	flagnul = 1;
      if(cJSON_GetObjectItem(rel_i,"unique"))
        {
          if(strcmp(cJSON_Print(cJSON_GetObjectItem(rel_i,"unique")),"true") == 0)
            flagmail = 1;
        }
      //S'il y'a un tableau de choix
      if(cJSON_GetObjectItem(rel_i,"choix"))
        {
          temp = choix(cJSON_GetObjectItem(rel_i,"choix"), N);
        }
      //S'il s'agit du numéro de téléphone

      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"telephone") == 0)
        {
          if(cJSON_GetObjectItem(rel_i,"pays"))
            ind = cJSON_GetObjectItem(rel_i,"pays");
          else
            ind = NULL;
          temp = telephone(ind,N);
        }

      //S'il s'agit du sexe
      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"sexe") == 0)
	temp = sexe(N);

      //S'il s'agit de la date
      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"date") == 0)
        {
          temp = date(N);
        }

      //S'il s'agit d'un identifiant
      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"identifiant") == 0)
        {
          repere = 1;
          flagserial = 1;
        }

      //S'il s'agit du nom
      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"nom") == 0)
        {
          repere = 1;
          val_ar = cJSON_GetArrayItem(json,indiceRel);
          link = cJSON_GetObjectItem(val_ar,"attributs");
          j = cJSON_GetArraySize(link);
          for(i = 0; i < j; i++)
            {
              val_at = cJSON_GetArrayItem(link,i);
              if(cJSON_GetObjectItem(val_at,"link"))
            flagserial = 1;
              val_rel = cJSON_GetObjectItem(val_at,"type");
              if(strcmp(oter_cot(cJSON_Print(val_rel)),"string") == 0)
            {
              val_rel = cJSON_GetObjectItem(val_at,"options");
              if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(val_rel,"type"))),"email") == 0 && flagserial == 0)
                {
                  flag = 1;
                  break;
                }
            }
            }
        }

      else if(strcmp(oter_cot(cJSON_Print(cJSON_GetObjectItem(rel_i,"type"))),"email") == 0)
        {
          if(adresse)
            {
              repere = 1;
              for(i = 1; i < N; i++)
            strcpy((matrix[i]+col)-> car, adresse[i-1]);
            }
          else
            {
              repere = 0;
              temp = make_mail(N);
            }
        }
      if(repere == 1)
	{
	  list = make_nom(min, max, flag, N, flagserial);
	      for(i = 1; i < N; i++)
		strcpy((matrix[i]+col)-> car,list[i-1] -> lnom);
	  if(flag == 1)
	    {
	      adresse = calloc(sizeof(char*), N);
	      for(i = 0; i < N; i++)
		strcpy(adresse[i],list[i] -> adress);
	    }
	}
      else
       for(i = 1; i < N; i++)
	 strcpy((matrix[i]+col)-> car, temp[i-1]);
    }
}

//Fonction qui crée un tableau permettant de faire l'enregistrement des données de chaque table
void matrice(int lin, int col)
{
  int t,j;

  matrix = calloc(sizeof(tab_char*), (lin+1));
  if(!matrix)
    exit(0);
  for(j = 0; j < (lin+1); j++)
    {
      matrix[j] = calloc(sizeof(tab_char), col);
    }
}

//tester si la chaîne est numérique
int isnum(char *carac)
{
  char *tmp1 = calloc(sizeof(char),lmax);
  sprintf(tmp1, "%d", atoi(carac));
  return strcmp(tmp1,carac);
}

//tester si la chaîne est numérique
int isfloat(char *carac)
{
  char *tmp1 = calloc(sizeof(char),lmax),*tmp2 = calloc(sizeof(char),lmax);
  sprintf(tmp2,"%f", atof(carac));
  //printf("tmp2 = %s carac = %s  %d\n",tmp2, carac, strncmp(tmp2,carac,strlen(carac)));
  return strncmp(tmp2,carac,strlen(carac));
}

// fonction qui crée les filenamesjson
void  writtingjson(char *filename, int Ndata, int tail_at)
{
  int t, j, num = 0, num1 = 0;
  char *tmp1 =" ",  *tmp3 = "\n\t", *crocF = "]", *cote = "\"", *jso = "json";
  char *js = calloc(sizeof(char),lmax), *virgule = ",", *deuxp = ":", *accoO = "{", *accoF = "}", *crocO = "[";
  FILE *fil;

  strcpy(js, filename);
  strcat(js, jso);
  //printf("tail_at = %d, \t Ndata = %d\n", tail_at, Ndata);
  fil = fopen(js,"w");

  fprintf(fil, "%s", accoO);
  fprintf(fil, "%s", tmp3);
  for(t = 0 ; t < tail_at; t++)
    {
      fprintf(fil, "%s", cote);
      fprintf(fil, "%s", matrix[0][t].car);
      fprintf(fil, "%s", cote);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", deuxp);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", crocO);
      num = isnum(matrix[1][t].car);
      num1 = isfloat(matrix[1][t].car);
      for(j = 1; j < Ndata; j++)
	{
	  if(num == 0 || num1 == 0)
	      fprintf(fil, "%s", matrix[j][t].car);
	  else
	    {
	      fprintf(fil, "%s", cote);
	      fprintf(fil, "%s", matrix[j][t].car);
	      fprintf(fil, "%s", cote);
	    }
	  if(j < Ndata - 1)
	    {
	      fprintf(fil, "%s", virgule);
	      fprintf(fil, "%s", tmp1);
	    }
	}
     fprintf(fil, "%s", crocF);
     if(t  < tail_at - 1)
       {
	 fprintf(fil, "%s", virgule);
	 fprintf(fil, "%s", tmp3);
       }
    }
  fprintf(fil, "%s", "\n");
  fprintf(fil, "%s", accoF);
  fclose(fil);
}

// fonction qui crée les filenamessql
void  writtingsql(char *filename, int Ndata, int tail_at)
{
  int t, j, num = 0, num1 = 0;
  char *tmp1 =" ",  *tmp3 = "\n", *pointv = ";", *jso = "sql", *parou = "(", *parf = ")";
  char *js = calloc(sizeof(char),lmax), *virgule = ",", *insert= "insert", *into = "into", *value = "values", *cote = "'";
  FILE *fil;

  strcpy(js, filename);
  strcat(js, jso);
  fil = fopen(js,"w");

  for(t = 1 ; t < Ndata; t++)
    {
      fprintf(fil, "%s", insert);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", into);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", filename);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", value);
      fprintf(fil, "%s", tmp1);
      fprintf(fil, "%s", parou);
      for(j = 0; j < tail_at; j++)
        {
           num = isnum(matrix[t][j].car);
           num1 = isfloat(matrix[t][j].car);
          if(num == 0 || num1 == 0)
              fprintf(fil, "%s", matrix[t][j].car);
          else
            {
              fprintf(fil, "%s", cote);
              fprintf(fil, "%s", matrix[t][j].car);
              fprintf(fil, "%s", cote);
            }
          if(j < tail_at - 1)
            {
              fprintf(fil, "%s", virgule);
              fprintf(fil, "%s", tmp1);
            }
        }
	 fprintf(fil, "%s", parf);
	 fprintf(fil, "%s", pointv);
	 fprintf(fil, "%s", tmp3);
    }
  fclose(fil);
}

//Fonction pour ecriture des enregistrements dans un fichier
void writting(char *filename, int Ndata, int tail_at)
{
  int t, j, len, len0;
  char *tmp = "\t" , *tmp1 =" ", *tmp2 = "\n\n";
  FILE *f = fopen(filename, "w");
  for(t = 0; t < Ndata; t++)
    {
      for(j = 0 ;j < tail_at; j++)
        {
	  len = strlen(matrix[0][j].car)-strlen(matrix[t][j].car);
	  if(len >= 0)
	    {
	      fputs((matrix[t]+j) -> car,f);
	      for(i = 0; i < len; i++)
		fprintf(f,"%s",tmp1);
	    }
	  else
	    {
	      fputs((matrix[t]+j) -> car,f);
	      for(i = 0; i < (-len); i++)
		fprintf(f,"%s",tmp1);
	    }
	  fprintf(f,"%s",tmp);
	}
      fprintf(f,"%s",tmp2);
    }
    fclose(f);
}

//fonction qui parse lit les filenamejson créés
void parse_filenamejson(char *text)
{
  fichierjson = cJSON_Parse(text);
  if (!fichierjson)
    {
      printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
}

void read_filenamejson(char *filenamejson)
{
  FILE *f;
  long len;
  char *data;

  f=fopen(filenamejson,"rb");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  data=(char*)malloc(len+1);
  fread(data,1,len,f);
  data[len]='\0';
  parse_filenamejson(data);
  free(data);
  fclose(f);
}

//Fonction qui parcours l'objet json parsé
void generate_all(char *filename)
{
  int k, l,j, t, indice_rel, indice_at, tail_at, Ndata, repere;
  cJSON *relat, *attribut, *objet, *otre;
  FILE *f;
  char *nomjs = calloc(sizeof(char),lmax), *js = "json", **tempjson, *format = calloc(sizeof(char),5) ;
  printf("Sous quel format renvoyer les fichiers(json ou sql ou txt)? ");
  gets(format);
  srand(time(NULL));
  for(j = 0; j < tail; j++)
    ordre(j,depend);
  for(k = 0; k < tail; k++)
    {
      Ndata = rand()%(26-5)+5;
      relat = cJSON_GetArrayItem(json,tab[k]);
      titleR = oter_cot(cJSON_Print(cJSON_GetObjectItem(relat,"NomRelation")));
      objet = cJSON_GetObjectItem(relat,"attributs");
      tail_at = cJSON_GetArraySize(objet);
      matrice(Ndata,tail_at);
      for(l = 0; l < tail_at; l++)
	{
	  attribut = cJSON_GetArrayItem(objet,l);
	  if(!cJSON_GetObjectItem(attribut, "link"))
	    {
	      global_generator(attribut, k, l, Ndata);
	    }
	  else
	    {
	      link = cJSON_GetObjectItem(cJSON_GetObjectItem(attribut, "link"),"NomRelation");
	      otre = cJSON_GetObjectItem(cJSON_GetObjectItem(attribut, "link"),"name");
	      strcpy(nomjs,oter_cot(cJSON_Print(link)));
	      strcat(nomjs, js);
	      read_filenamejson(nomjs);
	      otre = cJSON_GetObjectItem(fichierjson,oter_cot(cJSON_Print(otre)));
	      tempjson = choix(otre, Ndata);
	      strcpy((matrix[0]+l) -> car, oter_cot(cJSON_Print(cJSON_GetObjectItem(attribut, "name"))));
	      for(t = 1; t < Ndata; t++)
            {
              strcpy((matrix[t]+l)-> car, tempjson[t-1]);
            }
	    }
	}
	  writtingjson(titleR, Ndata, tail_at);
	  if(strcmp(format,"txt") == 0)
	    writting(titleR,Ndata,tail_at);
	  else if(strcmp(format,"sql") == 0)
	    writtingsql(titleR, Ndata, tail_at);
	  else if(strcmp(format,"json") == 0)
	    continue;
	  else
	    {
	      printf("format non autorisé!\n");
	      exit(0);
	    }
    }
}
