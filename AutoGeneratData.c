#include "AutoGeneratData.h"

//parser le fichier contenant les relations à générer
void parse_file(char *text)
{
  json = cJSON_Parse(text);
  if (!json)
      printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //Noter la présence d'une erreur syntaxique
  else
      recherche_link(json);
}

//fonction qui parse le fichier json contenant les pays et leurs indicatifs
void parse_indicatif(char *contenu)
{

  regist = cJSON_Parse(contenu);
  size_p = cJSON_GetArraySize(regist);
  if (!regist)
      printf("Error before: [%s]\n",cJSON_GetErrorPtr());
}

//fonction qui parse le fichier json contenant les suffixe d'une adresse mail
void parse_mail(char *text)
{
  cJSON *ind;
  suffixe = cJSON_Parse(text);
  size_m = cJSON_GetArraySize(suffixe);
  if (!suffixe)
    {
      printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
}

//fonction qui parse le fichier json contenant les noms
void parse_nom(char *text)
{
  name  = cJSON_Parse(text);
  if (!name)
    {
      printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
}

//fonction qui lit le fichier contenant les relations
void read_file(char *filename)
{
  FILE *f;
  long len;
  char *data;

  f=fopen(filename,"rb");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  data=(char*)malloc(len+1);
  fread(data,1,len,f);
  data[len]='\0';
  parse_file(data);
  free(data);
  fclose(f);
}


void read_indicatif(char *filename)
{
  FILE *f;
  long len;
  char *data;
  f=fopen(filename,"rb");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  data=(char*)malloc(len+1);
  fread(data,1,len,f);
  data[len]='\0';
  parse_indicatif(data);
  free(data);
  fclose(f);
}


void read_mail(char *filename)
{
  FILE *f;
  long len;
  char *data;

  f=fopen(filename,"rb");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  data=(char*)malloc(len+1);
  fread(data,1,len,f);
  data[len]='\0';
  parse_mail(data);
  free(data);
  fclose(f);
}


void read_nom(char *filename)
{
  FILE *f;
  long len;
  char *data;

  f=fopen(filename,"rb");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  data=(char*)malloc(len+1);
  fread(data,1,len,f);
  data[len]='\0';
  parse_nom(data);
  free(data);
  fclose(f);
}

void free_space()
{
  free(json);
  free(regist);
  free(suffixe);
  free(name);
  free(depend);
  free(tabrel);
  free(ind);
  free(val_at);
  free(val_rel);
  free(val_ar);
  free(tab_rel);
  free(link);
}

int main (int argc, const char * argv[])
{
  char *file = calloc(sizeof(char),lmax), tmp[] = "Solution";
  printf("\nMERCI D'AVOIR OPTE POUR NOTRE SOLUTION\n\n");
  printf("Entrer le nom de votre fichier au format requis: ");
  gets(file);
  read_file(file);
  strcat(file,tmp);
  read_indicatif("pays");
  read_mail("domaine");
  read_nom("liste");
  generate_all(file);
  free_space();
  return 0;
}
