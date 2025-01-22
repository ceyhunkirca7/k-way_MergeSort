#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// 1-N arasý random sayýlar oluþturup, unique dizi üretme
void unique_random_array(int *arr, int N) {
	int i;
	srand(time(NULL));
    for (i = 0; i < N; i++) {  // 1 den N ye kadar sýrayla diziye sayýlarý atama
        arr[i] = i + 1;
    }
    for (i = N - 1; i > 0; i--) {    // sayýlarý rastgele karýþtýrma
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// k-way Merge Sort (diziyi k parçaya böler ve her parçayý sýralar)
void k_way_merge_sort(int *arr, int left, int right, int k) {
	
     	int i;

        int num_elements = right - left + 1;  // toplam eleman sayýsý
        int chunk_size = (num_elements + k - 1) / k;  // dizi boyutuna göre her parçanýn uzunluðu
        int *boundaries = (int *)malloc((k + 1) * sizeof(int));  // her alt parçanýn sýnýrlarýný tutan dizi

        // Her parçanýn sýnýrlarýný belirle
        boundaries[0] = left;  // ilk parça left ile baþlar
        for (i = 1; i < k; i++) {
            boundaries[i] = boundaries[i - 1] + chunk_size; // bir sonraki parçanýn baþlangýcý,önceki parçanýn baþlangýcýna chunk_size eklenerek ayarlanýr.
            if (boundaries[i] > right) boundaries[i] = right + 1;  // sýnýrý geçip geçmediðimiz kontrol edilir
        }
        boundaries[k] = right + 1; //   son sýnýrý ayarlamak için

        // her parçayý ayrý ayrý sýralar
        for (i = 0; i < k; i++) {
            if (boundaries[i] < boundaries[i + 1] - 1) {  // parçanýn uzunluðu 1 den büyükse sýrala
                k_way_merge_sort(arr, boundaries[i], boundaries[i + 1] - 1, k);
            }
        }

        // k parçayý sýrayla birleþtir
        int *temp = (int *)malloc(num_elements * sizeof(int)); // sýralanmýþ parçalarý birleþtirip geçici tutmak için
        int *indexes = (int *)malloc(k * sizeof(int));  // hangi elemana bakýldýðýnýn takip edilmesi için
        for (i = 0; i < k; i++) indexes[i] = boundaries[i];

        int index = 0;
        while (index < num_elements) {  // tüm elemanlar sýralanana kadar
            int min_value = INT_MAX;  // en küçük deðer
            int min_index = -1;     // min_valueye sahip deðerin indexi

            for (i = 0; i < k; i++) {  // k parçadan sýrayla en küçük elemaný seçmek
                if (indexes[i] < boundaries[i + 1] && arr[indexes[i]] < min_value) { 
                    min_value = arr[indexes[i]];
                    min_index = i;
                }
            }

            if (min_index != -1) { // en küçük elemaný bulduysak
                temp[index++] = arr[indexes[min_index]]; 
                indexes[min_index]++;
            }
        }

        for (i = 0; i < num_elements; i++) arr[left + i] = temp[i];  // temp dizisinde sýralanmýþ elemanlarý asýl diziye kopyalar

        free(temp);
        free(indexes);
        free(boundaries);
}
// zaman ölçme fonksiyonu
double zaman_olc(int *arr, int N, int k) {
    struct timespec start, end;  // baþlangýç ve bitiþ zamanlarýný tutar
    double total_time = 0.0;   // toplam geçen süreyi saklar
    int *temp_arr = (int *)malloc(N * sizeof(int));   // orjinal diziyi bozmadan sýralama yapmak için
    int i,j;
   
    for (i = 0; i < 10; i++) {  // ort süreyi almak için 10 kez çalýþtýr
          for (j = 0; j < N; j++) temp_arr[j] = arr[j];  // diziyi baþtan kopyala

        clock_gettime(CLOCK_MONOTONIC, &start);   // zaman ölçme baþlangýcý
        k_way_merge_sort(temp_arr, 0, N - 1, k);  // merge sort iþlemi
        clock_gettime(CLOCK_MONOTONIC, &end);    // zaman ölçme bitiþ

        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;  // baþlangýç bitiþ arasý saniye farký hesaplar
        total_time += elapsed_time;  // toplam süreye ekleme
    }

    free(temp_arr);
    return total_time / 10;  // ortalama süreyi döndürür
}


int main() {
    int N,i;
    int k = 1;
    printf("Lutfen siralanacak eleman sayisini (N) girin: ");
    scanf("%d", &N);
   
    int *arr = (int *)malloc(N * sizeof(int));
    int *initial_arr = (int *)malloc(N * sizeof(int));  // ilk diziyi saklamak için
    unique_random_array(initial_arr, N);  // rastgele sayý oluþturma (1-N arasý)
    while(k!=0) { // kullanýcý çýkmak istemediði sürece

    for(i=0;i<N;i++) {
    	arr[i] = initial_arr[i];  
	}
	printf("Lutfen birlestirme isleminde kullanilacak k degerini girin.Programi sonlandirmak icin 0a basin. ");
    scanf("%d", &k);
   
    if (k == 0) {
        printf("Program sonlandirildi.\n");
        return 0; // programdan çýkýþ
    }

    printf("Siralama oncesi dizi:\n");
    for (i = 0; i < N; i++) {
    	printf("%d ", arr[i]);
	}	
    printf("\n");
        
        k_way_merge_sort(arr, 0, N - 1, k);  // diziyi merge sorta göre sýralar
        double time_taken = zaman_olc(arr, N, k);  // süreyi hesaplar
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


