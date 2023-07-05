#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
float LOAD_FACTOR = 0.6; // Yük faktörü
// hash tablomuzu oluşturalım
int hashTablosu[8500] = {0};
// kelimemizi hashleyelim
int hashleme(char *satir, int tabloBoyutu, int boyut)
{
    int key = 0;
    for (int i = 0; i < boyut; i++)
    {
        key = 61 * (key + satir[i]);
    }
    return key % tabloBoyutu;
}
// farklı girdilerden aynı hash değerlerinin çıkması durumu için alınan önlem.
int doubleHashleme(char *satir, int tabloBoyutu, int i, int boyut)
{
    int h1 = hashleme(satir, tabloBoyutu, boyut);
    int h2 = 19 - (hashleme(satir, tabloBoyutu, boyut) % 19);
    int m = tabloBoyutu / LOAD_FACTOR;
    return ((h1 + i) * h2) % m;
}
// Anagram kontrolü yapacak fonksiyon
bool anagramMi(char *str1, char *str2)
{
    int sayac[256] = {0}; // ASCII karakterler için bir sayaç dizisi oluşturulur
    // İlk kelimenin her bir karakteri için sayaç artırılır
    for (int i = 0; str1[i] != '\0'; i++)
    {
        sayac[(int)str1[i]]++;
    }
    // İkinci kelimenin her bir karakteri için sayaç azaltılır
    for (int i = 0; str2[i] != '\0'; i++)
    {
        sayac[(int)str2[i]]--;
    }
    // Sayaçlar kontrol edilir, eğer her bir karakterin sayısı 0 ise anagramdır
    for (int i = 0; i < 256; i++)
    {
        if (sayac[i] != 0)
        {
            return false;
        }
    }
    return true;
}
int main()
{
    // kelimeler dosyasının işaretçisi
    FILE *kelimeDosyasi;
    // kelimeleri tutan dizi
    char kelimeler[6] = {""};
    // kelimenin boyutunu tutacak
    int boyut = 0;
    // İndex değeri tutan değişken
    int dizin;
    // kelimeler dosyasını okuma modunda açma
    kelimeDosyasi = fopen("kelimeler.txt", "r");
    if (kelimeDosyasi == NULL)
    {
        printf("Dosya açama hatası!!!");
        // programa yolunda gitmeyen bir şeyler olduğunu haber veriyoruz
        return 1;
    }
    int satirSayisi = 1;
    char gez;
    // döngü dosyanın sonuna kadar gider ve her \n gördüğünde (satır sonu ifadesi ) sayacı arttırır bu sayede satır sayısı bulunur
    while ((gez = fgetc(kelimeDosyasi)) != EOF)
    {
        if (gez == '\n')
        {
            satirSayisi++;
        }
    }
    // dosyanın sonuna kadar gittik şimdi geri başa dönelim
    rewind(kelimeDosyasi);
    // Tekrar tekrar dosyayı okumak yerine dosyayı 1 kez okuyup bir diziye kaydedelim
    char kelimelerDizisi[satirSayisi][6];
    int doubleHashSayisi[satirSayisi][1];
    int say = satirSayisi - 1;
    while (say >= 0)
    {
        doubleHashSayisi[say][0] = 0;
        say--;
    }

    int i = 0;
    // tüm kelimelerin hashlendiği yer
    while (fgets(kelimeler, sizeof(kelimeler), kelimeDosyasi) != NULL)
    {
        // okunan kelimeler diziye kaydediliyor
        strcpy(kelimelerDizisi[i], kelimeler);
        // kelimenin boyutu hesaplanıyor -1 ifadesi \n dahil etmemek için
        boyut = strlen(kelimeler) - 1;
        dizin = hashleme(kelimeler, satirSayisi, boyut);
        int j = 0;
        // adres boş mu dolu mu kontrol edilip doluysa double hashleme yapılıyor
        while (hashTablosu[dizin] != 0)
        {
            j++;
            dizin = doubleHashleme(kelimeler, satirSayisi, j, boyut);
            doubleHashSayisi[i][0] = j;
        }
        // hash tablosunu oluşturma
        hashTablosu[dizin] = dizin + 1;
        i++;
    }
    //  for (size_t i = 0; i < 8500; i++)
    //  {
    //      printf("%d-", hashTablosu[i]);
    //  }
    //  printf("\n");
    //  dosya boşa açık kalmasın doyayı kapatalım
    fclose(kelimeDosyasi);
    bool devamEt = true;
    // kullanıcı bitirene kadar oyunu devam ettiren döngü
    while (devamEt)
    {
        printf("\n1. Yeni bir anagram kontrolu yap\n");
        printf("2. Cikis\n");
        printf("3. oyun hakkinda\n");
        printf("Seciminizi yapin: ");
        int secim;
        scanf("%d", &secim);
        getchar();
        switch (secim)
        {
        case 1:
        {
            // rastgelelik için zamanın ayarlanması
            srand(time(NULL));
            // rastgele bir satır seçme 0. satır diye bir şey yok bu yüzden 1. satırdan başlıyoruz.
            int rastgeleSatir = rand() % satirSayisi;
            // rastgele seçilen kelimenin ekrana yazılması
            printf("Secilen rastgele kelime: %s", kelimelerDizisi[rastgeleSatir]);
            // yine şimdi geri başa dönelim
            char anagram[6] = ""; // Boş bir 6 karakterlik bir dizi tanımlanıyor. Bu dizi anagramı kontrol etmek için kullanılacak.
            printf("Anagram kelimeyi girin: ");
            fgets(anagram, sizeof(anagram), stdin); // Kullanıcıdan anagram kelimeyi alıyoruz ve anagram dizisine atıyoruz.
            int doubleHashCount = 0;                // Double hashing sayısını tutmak için bir sayaç tanımlanıyor.
            bool anagramBulundu = false;            // AnagramBulundu değeri anagram bulunursa true atanacak default olarak false atıyorum
            for (int i = 0; i < satirSayisi; i++)
            {
                if (anagramMi(kelimelerDizisi[rastgeleSatir], anagram)) // Anagramlık kontrolü yapıyorum
                {
                    anagramBulundu = true; // Anagram bulunduğu için false değer true ile değiştiriliyor

                    if (strcmp(kelimelerDizisi[i], anagram) == 0) // Girilen kelime dizide aranıyor ve varsa 0 döndürerek kontrol yapılıyor
                    {
                        dizin = hashleme(anagram, satirSayisi, boyut);
                        int j = 0;
                        // adres boş mu dolu mu kontrol edilip doluysa double hashleme yapılıyor
                        while ((doubleHashSayisi[i][0]) != 0) // kelimenin double hashing sayısı kadar double hashing yaptırılması
                        {
                            j++;
                            dizin = doubleHashleme(anagram, satirSayisi, j, boyut);
                            (doubleHashSayisi[i][0])--;
                        }
                        printf("Girdiginiz kelimenin hash tablosundaki indeksi: %d\n", dizin - 1); // Dizinin indexini ekrana basılıyor
                        if (j == 0)
                        {
                            printf("Cakisma yok.\n");
                        }
                        else
                        {
                            printf("Cakisma var: %d defa. \n", j);
                        }
                        break;
                    }
                    if (i + 1 == satirSayisi) // eğer satırın sonuna geldiysek ve hash tablosunda yoktur
                    {
                        printf("Girdiginiz kelime hash tablosunda yok.\n");
                        break;
                    }
                }
            }
            if (anagramBulundu) // default olarak false atamıştık eğer true olursa bu koşul çalışır ve ekrana bilgi basılır
            {
                printf("Girdiginiz kelime anagram.\n");
            }
            else // default durumda bu durum çalışır ve ekrana bilgi basılır
            {
                printf("Girdiginiz kelime anagram degil.\n");
            }
            break;
        }
        case 2:
        {
            devamEt = false; // folse atanarak döngü sonlandırılır ve programdan çıkış yapılır
            break;
        }
        case 3: // kullanıcı oyun hakkında bilgi almak için 3. seçtiği durumda çalışacak durum
        {
            printf("Her oyunun basinda bu secim ekranini goreceksiniz bu cok sinir bozucu biliyorum :) \n");
            printf("Eger 1. secenegi secerseniz oyun baslar ve ekrana rastgele bir kelime cikacak.\n");
            printf("Sizden o kelimenin anagramini girmeniz bekleniyor.\n");
            printf("Siz kelimeyi girdikten sonra sonuc ekranda gorunecek herhangi bir tusa basarak devam edebilirsiniz.\n");
            printf("Son olarak lutfen girdiginiz kelimelerin hepsi buyuk harflerden olussun.\n");
            printf("Artik hazirsiniz, herhangi bir tusa basip secim ekranina gidebilirsiniz.\n");
            printf("Iyi eglenceler.");
            break;
        }
        default: // yanlış girdilerde çalışacak durum
        {
            printf("Gecersiz secim!\n");
            break;
        }
        }
        getche(); // oyunu bir karaktere basana kadar duraklatıyor
        system("cls"); // eski oyundan ekranı temizliyor
    }
    return 0;
}