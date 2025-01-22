#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// 1-N aras� random say�lar olu�turup, unique dizi �retme
void unique_random_array(int *arr, int N) {
	int i;
	srand(time(NULL));
    for (i = 0; i < N; i++) {  // 1 den N ye kadar s�rayla diziye say�lar� atama
        arr[i] = i + 1;
    }
    for (i = N - 1; i > 0; i--) {    // say�lar� rastgele kar��t�rma
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// k-way Merge Sort (diziyi k par�aya b�ler ve her par�ay� s�ralar)
void k_way_merge_sort(int *arr, int left, int right, int k) {
	
     	int i;

        int num_elements = right - left + 1;  // toplam eleman say�s�
        int chunk_size = (num_elements + k - 1) / k;  // dizi boyutuna g�re her par�an�n uzunlu�u
        int *boundaries = (int *)malloc((k + 1) * sizeof(int));  // her alt par�an�n s�n�rlar�n� tutan dizi

        // Her par�an�n s�n�rlar�n� belirle
        boundaries[0] = left;  // ilk par�a left ile ba�lar
        for (i = 1; i < k; i++) {
            boundaries[i] = boundaries[i - 1] + chunk_size; // bir sonraki par�an�n ba�lang�c�,�nceki par�an�n ba�lang�c�na chunk_size eklenerek ayarlan�r.
            if (boundaries[i] > right) boundaries[i] = right + 1;  // s�n�r� ge�ip ge�medi�imiz kontrol edilir
        }
        boundaries[k] = right + 1; //   son s�n�r� ayarlamak i�in

        // her par�ay� ayr� ayr� s�ralar
        for (i = 0; i < k; i++) {
            if (boundaries[i] < boundaries[i + 1] - 1) {  // par�an�n uzunlu�u 1 den b�y�kse s�rala
                k_way_merge_sort(arr, boundaries[i], boundaries[i + 1] - 1, k);
            }
        }

        // k par�ay� s�rayla birle�tir
        int *temp = (int *)malloc(num_elements * sizeof(int)); // s�ralanm�� par�alar� birle�tirip ge�ici tutmak i�in
        int *indexes = (int *)malloc(k * sizeof(int));  // hangi elemana bak�ld���n�n takip edilmesi i�in
        for (i = 0; i < k; i++) indexes[i] = boundaries[i];

        int index = 0;
        while (index < num_elements) {  // t�m elemanlar s�ralanana kadar
            int min_value = INT_MAX;  // en k���k de�er
            int min_index = -1;     // min_valueye sahip de�erin indexi

            for (i = 0; i < k; i++) {  // k par�adan s�rayla en k���k eleman� se�mek
                if (indexes[i] < boundaries[i + 1] && arr[indexes[i]] < min_value) { 
                    min_value = arr[indexes[i]];
                    min_index = i;
                }
            }

            if (min_index != -1) { // en k���k eleman� bulduysak
                temp[index++] = arr[indexes[min_index]]; 
                indexes[min_index]++;
            }
        }

        for (i = 0; i < num_elements; i++) arr[left + i] = temp[i];  // temp dizisinde s�ralanm�� elemanlar� as�l diziye kopyalar

        free(temp);
        free(indexes);
        free(boundaries);
}
// zaman �l�me fonksiyonu
double zaman_olc(int *arr, int N, int k) {
    struct timespec start, end;  // ba�lang�� ve biti� zamanlar�n� tutar
    double total_time = 0.0;   // toplam ge�en s�reyi saklar
    int *temp_arr = (int *)malloc(N * sizeof(int));   // orjinal diziyi bozmadan s�ralama yapmak i�in
    int i,j;
   
    for (i = 0; i < 10; i++) {  // ort s�reyi almak i�in 10 kez �al��t�r
          for (j = 0; j < N; j++) temp_arr[j] = arr[j];  // diziyi ba�tan kopyala

        clock_gettime(CLOCK_MONOTONIC, &start);   // zaman �l�me ba�lang�c�
        k_way_merge_sort(temp_arr, 0, N - 1, k);  // merge sort i�lemi
        clock_gettime(CLOCK_MONOTONIC, &end);    // zaman �l�me biti�

        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;  // ba�lang�� biti� aras� saniye fark� hesaplar
        total_time += elapsed_time;  // toplam s�reye ekleme
    }

    free(temp_arr);
    return total_time / 10;  // ortalama s�reyi d�nd�r�r
}


int main() {
    int N,i;
    int k = 1;
    printf("Lutfen siralanacak eleman sayisini (N) girin: ");
    scanf("%d", &N);
   
    int *arr = (int *)malloc(N * sizeof(int));
    int *initial_arr = (int *)malloc(N * sizeof(int));  // ilk diziyi saklamak i�in
    unique_random_array(initial_arr, N);  // rastgele say� olu�turma (1-N aras�)
    while(k!=0) { // kullan�c� ��kmak istemedi�i s�rece

    for(i=0;i<N;i++) {
    	arr[i] = initial_arr[i];  
	}
	printf("Lutfen birlestirme isleminde kullanilacak k degerini girin.Programi sonlandirmak icin 0a basin. ");
    scanf("%d", &k);
   
    if (k == 0) {
        printf("Program sonlandirildi.\n");
        return 0; // programdan ��k��
    }

    printf("Siralama oncesi dizi:\n");
    for (i = 0; i < N; i++) {
    	printf("%d ", arr[i]);
	}	
    printf("\n");
        
        k_way_merge_sort(arr, 0, N - 1, k);  // diziyi merge sorta g�re s�ralar
        double time_taken = zaman_olc(arr, N, k);  // s�reyi hesaplar
    printf("Siralama sonrasi dizi:\n");
    for (i = 0; i < N; i++) {
    printf("%d ", arr[i]);   
     }
      printf("Siralama suresi (k = %d): %f saniye\n", k, time_taken);    
 }

     free(arr);
     free(initial_arr);
    return 0;
}


