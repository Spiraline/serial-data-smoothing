#include <cstdio>
#include <cstdlib>
#include <iostream>

int pool_num;
int arr_len;
float* prev_v_arr;
float to_be_popped_idx = -1;
float xx, sum_x;

float poly_1dim_filter(float raw_v){
    float a, b;
    float xy = 0;
    float sum_y = 0;

    if(arr_len > pool_num){
        for(int i=1; i<pool_num; i++){
            xy += prev_v_arr[i]*i;
            sum_y += prev_v_arr[i];
        }
        xy += raw_v*pool_num;
        sum_y += raw_v;

        a = ( pool_num * xy - sum_x * sum_y ) / ( pool_num * xx - sum_x * sum_x);
        b = ( sum_y * xx - xy * sum_x ) / ( pool_num * xx - sum_x * sum_x);

        for(int i=0; i<pool_num-1; i++){
            prev_v_arr[i] = prev_v_arr[i+1];
        }
        prev_v_arr[pool_num-1] = a*pool_num+b;

        return a*pool_num+b;
    }
    else if(arr_len > 1){
        float temp_xx = 0;
        float temp_sum_x = 0;
        for(int i=1; i < arr_len; i++){
            xy += prev_v_arr[i-1]*i;
            sum_y += prev_v_arr[i-1];
            temp_xx += i*i;
            temp_sum_x += i;
        }
        xy += raw_v*arr_len;
        sum_y += raw_v;
        temp_xx += arr_len * arr_len;
        temp_sum_x += arr_len;

        a = ( arr_len * xy - temp_sum_x * sum_y ) / ( arr_len * temp_xx - temp_sum_x * temp_sum_x);
        b = ( sum_y * temp_xx - xy * temp_sum_x ) / ( arr_len * temp_xx - temp_sum_x * temp_sum_x);
        prev_v_arr[arr_len-1] = a*arr_len+b;

        return a*arr_len+b;
    }
    else{
        prev_v_arr[0] = raw_v;
        return raw_v;
    }
}

int main(int argc, char **argv){
    if(argc != 4){
        printf("Usage : ./smoothing <raw file> <output file> <pool_num>\n");
        return 1;
    }

    FILE* raw_file = fopen(argv[1], "r");
    FILE* filtered_file = fopen(argv[2], "w");

    if(raw_file == NULL){
        printf("File doesn't exists!");
        return 1;
    }

    pool_num = atoi(argv[3]);
    prev_v_arr = (float *)malloc(sizeof(float) * pool_num);

    for(int i=1; i<=pool_num; i++){
        sum_x += i;
        xx += i*i;
    }

    char buffer[255];
    float v;

    while(fscanf(raw_file, "%f", &v) != EOF){
        if(arr_len <= pool_num) arr_len++;
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