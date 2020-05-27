#include <cstdio>
#include <cstdlib>
#include <iostream>

int history_size;
int arr_len;
float* prev_v_arr;
float to_be_popped_idx = -1;
float sum_xx, sum_x;

float poly_1dim_filter(float raw_v){
    float a, b;
    float sum_xy = 0, sum_y = 0;

    // 1. Case for full history
    if(arr_len > history_size){
        for(int i=1; i<history_size; i++){
            sum_xy += prev_v_arr[i]*i;
            sum_y += prev_v_arr[i];
        }
        sum_xy += raw_v*history_size;
        sum_y += raw_v;

        a = ( history_size * sum_xy - sum_x * sum_y ) / ( history_size * sum_xx - sum_x * sum_x);
        b = ( sum_y * sum_xx - sum_xy * sum_x ) / ( history_size * sum_xx - sum_x * sum_x);

        for(int i=0; i<history_size-1; i++){
            prev_v_arr[i] = prev_v_arr[i+1];
        }
        prev_v_arr[history_size-1] = raw_v;

        return a*history_size+b;
    }
    // 2. Case if array is not full
    else if(arr_len > 1){
        sum_xx += arr_len * arr_len;
        sum_x += arr_len;

        for(int i=1; i < arr_len; i++){
            sum_xy += prev_v_arr[i-1]*i;
            sum_y += prev_v_arr[i-1];
        }
        sum_xy += raw_v*arr_len;
        sum_y += raw_v;

        a = ( arr_len * sum_xy - sum_x * sum_y ) / ( arr_len * sum_xx - sum_x * sum_x);
        b = ( sum_y * sum_xx - sum_xy * sum_x ) / ( arr_len * sum_xx - sum_x * sum_x);
        prev_v_arr[arr_len-1] = raw_v;

        return a*arr_len+b;
    }
    // 3. Initial Case
    else{
        sum_xx += 1;
        sum_x += 1;
        prev_v_arr[0] = raw_v;
        return raw_v;
    }
}

int main(int argc, char **argv){
    if(argc != 4){
        printf("Usage : ./smoothing <raw file> <output file> <history_size>\n");
        return 1;
    }

    FILE* raw_file = fopen(argv[1], "r");
    FILE* filtered_file = fopen(argv[2], "w");

    if(raw_file == NULL){
        printf("File doesn't exists!");
        return 1;
    }

    history_size = atoi(argv[3]);
    prev_v_arr = (float *)malloc(sizeof(float) * history_size);

    char buffer[255];
    float v;

    while(fscanf(raw_file, "%f", &v) != EOF){
        if(arr_len <= history_size) arr_len++;
        //fprintf(filtered_file, "%f\t%f\n", v, poly_1dim_filter(v))
        fprintf(filtered_file, "%f\n", poly_1dim_filter(v));
    }

    // while(!feof(raw_file)){
    //     fgets(buffer, sizeof(buffer), raw_file);
    //     printf("%d\n", v);
    //     printf("%s\n", buffer);
    // }

    fclose(raw_file);
    fclose(filtered_file);
}