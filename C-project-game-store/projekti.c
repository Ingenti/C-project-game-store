#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct peli {
	char *pelinimi;
	float hinta;
	float tuotto;
};


struct peli *lisaa_peli(struct peli *tietokanta, char *nimi, float hinta)
{
	int i;
	for (i=0;tietokanta[i].pelinimi!=NULL;i++);		//Etsitään NULL-jäsen (i:s indeksi),jonka paikalle lisätään uusi peli
	struct peli *uusi_tietokanta = realloc(tietokanta,(i+2)*sizeof(struct peli));
	char *asetettava_nimi=malloc(sizeof(nimi)+1);		//varataan dynaamisesti tila pelin nimen osoittimelle (,koska struct pelin "pelinimi"-kenttä on _osoitin_ merkkijonoon, joka edustaa pelin nimeä)
	if (asetettava_nimi==NULL)							//// +1 merkkijonon päättävälle '\0'
	{
		printf("Muistin varaaminen epäonnistui.\n");
		return NULL;
	}
	strcpy(asetettava_nimi,nimi);
	if (uusi_tietokanta==NULL)
	{
		printf("Muistin varaaminen epäonnistui.\n");
		return NULL;
	}

	uusi_tietokanta[i].pelinimi=asetettava_nimi;
	uusi_tietokanta[i].hinta=hinta;
	uusi_tietokanta[i].tuotto=0;
	uusi_tietokanta[i+1].pelinimi=NULL;		//asetetaan seuraava pelin nimi NULLiksi

	return uusi_tietokanta;
}

struct peli *osta_pelia(struct peli *tietokanta, char nimi[20], int n)
{
	int i;
	for (i=0;tietokanta[i].pelinimi!=NULL;i++)  //etsitään peli taulukosta, joka vastaa annettua nimeä
	{
		if (strcmp(tietokanta[i].pelinimi,nimi)==0)
		{
			tietokanta[i].tuotto+=n*tietokanta[i].hinta;
			printf("Osto onnistui.\n");
			return tietokanta;
		}
	}
	printf("Annetulla nimellä ei löytynyt peliä taulukosta.\n");
	return tietokanta;
}

void tulosta_tietokanta(const struct peli *tietokanta)
{
	int i;
	for (i=0;tietokanta[i].pelinimi!=NULL;i++)
	{
		printf("Nimi: %s " "Hinta: %.2f " "Tuotto: %.2f\n",tietokanta[i].pelinimi, tietokanta[i].hinta, tietokanta[i].tuotto);
	}
	printf("\n");
}

void tallenna_tiedot(const struct peli *tietokanta, const char *tiedostonimi)
{
	FILE *tiedosto=fopen(tiedostonimi,"w");
	if (!tiedosto)
	{
		printf("Tiedoston avaaminen epäonnistui.\n");
		exit(EXIT_SUCCESS);
	}
	int i;
	for (i=0;tietokanta[i].pelinimi!=NULL;i++)
	{
		fprintf(tiedosto, "%s " "%.2f " "%.2f " "\n",tietokanta[i].pelinimi, tietokanta[i].hinta, tietokanta[i].tuotto);
	}
	printf("Tallennus onnistui!\n");
	fclose(tiedosto);
}

struct peli *lataa_tiedot(struct peli *tietokanta, const char *tiedostonimi)
{
	FILE *tiedosto = fopen(tiedostonimi,"r");
	if (!tiedosto)
	{
		printf("Tiedoston avaaminen epäonnistui.\n");
		exit(EXIT_SUCCESS);
	}

	char kauppatiedot[1000];					// Tallennetaan tänne ladattavat tiedot, oletetaan tiedoissa olevan maksimissaan 1000 merkkiä
	int i=0;
	while (fgets(kauppatiedot,1000,tiedosto))				//luetaan ja tallennetaan tietokantaan pelien tietoja, kunnes sscanf ei saa enää kolme argumenttia (eli ollaan luettu kaikki tarpeellinen)
	{
		tietokanta=realloc(tietokanta,(i+2)*sizeof(struct peli));
		if (tietokanta==NULL)
		{
			printf("Muistin varaaminen epäonnistui.\n");
			exit(EXIT_SUCCESS);
		}

		char nimi[20];
		char *asetettava_nimi=malloc(sizeof(nimi)+1);   // +1 merkkijonon päättävälle '\0'
		float hinta;
		float tuotto;

		int sscanf_ret=sscanf(kauppatiedot,"%s" "%f" "%f", nimi, &hinta, &tuotto);
		if (sscanf_ret!=3)
		{
			fclose(tiedosto);
			return tietokanta;
		}

		strcpy(asetettava_nimi,nimi);
		tietokanta[i].tuotto=tuotto;
		tietokanta[i].hinta=hinta;
		tietokanta[i].pelinimi=asetettava_nimi;
		tietokanta[i+1].pelinimi=NULL;
		i++;
	}
	return tietokanta;
}

void vapauta_pelitiedot(struct peli *tietokanta)
{
	int i;
	for (i=0;tietokanta[i].pelinimi!=NULL;i++)
	{
		free(tietokanta[i].pelinimi);
	}
	free(tietokanta);
}


int main(void)
{
	struct peli *tietokanta=malloc(sizeof(struct peli));
	tietokanta[0].pelinimi = NULL;

	char toiminto;
	float hinta;
	int n_kpl=0;
	char uusi_nimi[20];					
	char pelin_nimi[20];					

	char tiedot[1000];

	fgets(tiedot,1000,stdin);
	sscanf(tiedot,"%c", &toiminto);
	
	while(toiminto!='Q')
	{
		switch(toiminto)
		{
			case 'A': ; //statement ";", jotta return_value voidaan määritellä sscanf-funktion paluuarvoksi
				int return_value=sscanf(tiedot, "%c" "%s" "%f",&toiminto, uusi_nimi, &hinta);
				if(return_value==3)
				{
					tietokanta=lisaa_peli(tietokanta,uusi_nimi,hinta);
					if (tietokanta==NULL)
					{
						printf("Pelin lisääminen epäonnistui.\n");
						break;
					}
				}
				else
				{
					printf("Liian vähän argumentteja.\n");
					break;
				}
				break;

			case 'B': ;
				int ret=sscanf(tiedot,"%c" "%s" "%d", &toiminto, pelin_nimi, &n_kpl);
				if (n_kpl>0 && ret==3) //tarkistetaan, että n_kpl on positiivinen ja syöte on luettu
				{
					tietokanta=osta_pelia(tietokanta,pelin_nimi,n_kpl);
				}
				else 
				{
					printf("Virhe kappalemäärässä tai liian vähän argumentteja.\n");
					break;
				}
				break;

			case  'L':
				tulosta_tietokanta(tietokanta);
				break;


			case 'W': ;
				char tiedosto[1000];
				int ret_s=sscanf(tiedot,"%c" "%s",&toiminto, tiedosto);
				if (ret_s==2)
				{
					tallenna_tiedot(tietokanta, tiedosto);
				}
				else
				{
					printf("Liian vähän argumentteja.\n");
				}
				break;


			case 'O': ;
				char tiedostonimi[1000];
				int ret_ss=sscanf(tiedot, "%c" "%s",&toiminto,tiedostonimi);
				if (ret_ss==2)
				{
					tietokanta=lataa_tiedot(tietokanta,tiedostonimi);
				}
				else 
				{
					printf("Liian vähän argumentteja.\n");
				}
				break;

			default:
				printf("Väärä syöte!\n");
		}
		fgets(tiedot,1000,stdin);
		sscanf(tiedot,"%c",&toiminto);
	}
	vapauta_pelitiedot(tietokanta);
	return 0;
}