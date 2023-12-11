# Tietoliikenteen sovellusprojekti

# Projektin kuvaus

Projektissa käytetyt ohjelmat ja komponentit:
  - Visual Studio Coden C-kieliset ohjelmat: Kiihtyvyysanturin dataa mittaava ohjelma, Bluetoothin toiminnan mahdollistava ohjelma, sekä nRF5430-DK alustan käytön ohjelma.
  - Visual Studio Coden Python ohjelmat: Mittausdatan luku, käsittely ja lähettäminen MySql tietokantaan(Raspberry pi 3:n kautta), K-means opetusalgoritmin toteutus 3D-mallinnuksella ja opetusalgoritmin tehokkuuden     testaus ohjelma.
  - Kiihtyvyysanturi GY-61
  - Nrf Connect
  - Nordic nRF5340-DK alusta
  - Raspberry pi 3
  - MySql tietokanta
  - Linux serveri

Projektin tehtävänä on suunnitella nRF5340 Development Kit -alustalle client, joka mittaa anturidataa kiihtyvyysanturilta ja välittää tietoa langattomasti IoT-reitittimelle (Raspberry Pi). Raspberry välittää dataa Oamkin MySQL-palvelimelle.

Tietokantaan tallentuvaan dataan on TCP-sokettirajapinta ja yksinkertainen HTTP API. Kerättyä dataa haetaan HTTP-rajanpinnasta omaan kannettavaan koodatulla ohjelmalla ja käsitellään koneoppimistarkoituksiin.

## Projektin arkkitehtuurikuva
<img src="arkkitehtuurikuva.png">

## K-means algoritmin toiminta, sekä 3D-mallin kuva

