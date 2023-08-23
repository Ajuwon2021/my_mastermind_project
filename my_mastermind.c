#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int const MAXIMUM_GAME_ROUNDS_SET_TO_DEFAULT = 10;

int length_of_code_or_str(char *code_or_str) 
{
    int total_code_len = 0;
    while(code_or_str[total_code_len]) total_code_len++;
    return total_code_len;
}

bool char_or_str_content(char* code_or_str, char code_input) 
{
    int i = 0;
    while(i < length_of_code_or_str(code_or_str))
    {
        if(code_or_str[i] == code_input) return false;
        else
        {
            i++;
            continue;
        }
        i++;
    }
    return true;
}

int flag_t(int flags_len, char** char_flags)
{
    int game_res = MAXIMUM_GAME_ROUNDS_SET_TO_DEFAULT;
    for(int i = 1; i < flags_len; i++)
        if(strcmp(char_flags[i] ,"-t") == 0) 
        {
            i++;
            game_res = atoi(char_flags[i]);
        }
    return game_res;
}


char *codes_or_nbr_rand_gen()
{
    int i = 0;
    char* game_res = (char*)malloc(5 * sizeof(char));
    srand(time(NULL));
    while(i < 4)
    {
        char ascii_nbr =  rand() % 8 + '0';
        if(char_or_str_content(game_res, ascii_nbr)) 
        {
            game_res[i++] = ascii_nbr;
        }   
    }
    game_res[i] = '\0';
    return game_res;
}

char* flag_c(int input_counter, char** rec_val)
{
    char* output_code = NULL;
    for(int i = 1; i < input_counter; i++)
    { 
        if(strcmp(rec_val[i] ,"-c") == 0)
        {
            i++;
            output_code = rec_val[i];
        }
        else
        {
            output_code = NULL;
        }
    }
    if (output_code == NULL) 
    {
        output_code = codes_or_nbr_rand_gen();
    }
    return output_code;   
}

int rec_wrong_codes(char* player_input_code, char* code_target_by_player)
{
    int code_count = 0;
    int i = 0;
    while(i < length_of_code_or_str(player_input_code))
    {
        if(!char_or_str_content(player_input_code, code_target_by_player[i]) && player_input_code[i] != code_target_by_player[i])
        {
            code_count++; 
        }
        i++;
    }
    return code_count;
}

int rec_right_code(char* player_input_code, char* code_target_by_player)
{
    int player_res = 0;
    int i = 0;
    while (i < length_of_code_or_str(player_input_code))
    {
        if (player_input_code[i] == code_target_by_player[i])
        {
            player_res++;
        }
        i++;
    }
    return player_res;
}

int check_input_code(char* player_input_nbr)
{
    for(int i = 0; i < length_of_code_or_str(player_input_nbr) - 1; i++)
    {
        for(int j = i+1; j < length_of_code_or_str(player_input_nbr); j++)
        {
            if (player_input_nbr[i] == player_input_nbr[j])
            {
                return 1;
            }
        }
    }
    return 0;
}


bool secret_evaluator(char* digit_1, char digit_2)
{
    int values = 0; 
    while (values < length_of_code_or_str(digit_1)){
        if (digit_1[values] == digit_2)
            return false;
        values++;
    }
    return true;
}

char* rec_input_from_cli()
{
    char *resp_buffer_settings = (char*)malloc(sizeof(char)* 255);
    int i = 0, infomation_return_speed = 0;
    char code_input;
    while ((infomation_return_speed = read(0, &code_input, 1)) > 0)
    { 
        if(code_input != '\n')
        {
            resp_buffer_settings[i] = code_input;
            i++;
        }
        else 
        {
            return resp_buffer_settings;
        }
    }
    if(!infomation_return_speed)
    {
        exit(0);
    }
    return resp_buffer_settings;
}

int buffer_eval(char* player_input_nbr)
{
    if(length_of_code_or_str(player_input_nbr) != 4) 
    {
        return 1;
    }
    int i = 0;
    while (i < length_of_code_or_str(player_input_nbr)) 
    {
        if((player_input_nbr[i] > '7' && player_input_nbr[i] < '0') ) 
            return 1;
        i++;
    }
    return 0;
}

void display_game_rounds(int game_ro_cnt)
{
    printf("\n---\nRound %d\n", game_ro_cnt); 
    write(1, ">", 1);
}

void success_msg()
{
    printf("Congratz! You did it!"); 
    exit(0);
}

void display_nbr_positions(int rightly_positioned, int wrongly_positioned)
{
    printf("\nWell placed pieces: %d\n", rightly_positioned);
    printf("Misplaced pieces: %d", wrongly_positioned);
}


int main(int input_counter, char** rec_val)
{
    int game_tried = 0;
    int rigthly_positioned_counter;
    int wrongly_positioned_counter;
    int nbr_of_time_game_played = flag_t(input_counter, rec_val);
    char* code_target_by_player = flag_c(input_counter, rec_val);
    printf("Will you find the secret code? \nPlease enter a valid guess");
    
    while(game_tried < nbr_of_time_game_played) 
    {
        display_game_rounds(game_tried + 1);
        char *player_input_nbr = rec_input_from_cli();
        if (buffer_eval(player_input_nbr)) 
        {
            printf("\nWrong input!\n");
            continue;
        }
        if (check_input_code(player_input_nbr)) 
        {
            printf("\nWrong input!\n");
            continue;
        }
        rigthly_positioned_counter = rec_right_code(player_input_nbr, code_target_by_player);
        if (rigthly_positioned_counter == 4) 
        {
            success_msg();
        }
        wrongly_positioned_counter = rec_wrong_codes(player_input_nbr, code_target_by_player);
        display_nbr_positions(rigthly_positioned_counter, wrongly_positioned_counter);
        game_tried++;
    }
    printf("\nYou lost the game! Try again.\n");
    return 0;
}