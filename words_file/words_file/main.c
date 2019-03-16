#include <stdio.h>
#include <string.h>
#include <locale.h>//Для printf, fgets, fopen, fclose, feof

int main(void)
{
    setlocale(LC_ALL, "RUS");
    FILE *ptr;//объявляе указатель на файл
    int c, c2;
    int count_of_words = 1;
    
    printf("Открытие файла : ");
    ptr = fopen("aaa.txt", "r+");//открываем для чтения
    if (ptr == NULL) {
        printf("ошибка\n");
        return 1;
    }
    else printf("выполнено\n");
    
    printf("Содержимое файла : \n");
    if (ptr) {
        while ((c = getc(ptr)) != EOF) {
            putchar(c);
        }
    }
    
    rewind(ptr);
    
    char a = 'a';
    int found_starting_with_a = 0;
    int is_first_word_char = 1;
//    long a_location;
    fpos_t a_word_position;
    fpos_t a_word_position_end;
    int length_of_a_word = 1;
    while ((c = getc(ptr)) != EOF) {
        if (c == (int) a) {
            if (is_first_word_char == 1) {
                found_starting_with_a = 1;
                
                while ((c2 = getc(ptr)) != '\n') {
                    length_of_a_word++;
                    fgetpos(ptr, &a_word_position_end);
                }
                break;
            }
        } else {
            printf("\nNot an 'a'\n");
            putchar(c);
            if (c == '\n') {
                is_first_word_char = 1;
                fgetpos(ptr, &a_word_position);
            } else {
                is_first_word_char = 0;
            }
        }
    }
    fseek(ptr, 0, SEEK_SET);
    
    while ((c = getc(ptr)) != EOF) {
        if (c == '\n') {
            count_of_words++;
        }
    }
    printf("Количество слов = %d \n", count_of_words);
    
    if (found_starting_with_a == 0) {
        fclose(ptr);
        printf("Cлов на 'a' не найдено");
        puts("\n");
        return 0;
    }
    
    printf("Найдено слово на 'a' : ");
    fsetpos (ptr, &a_word_position);
    
    while ((c = getc(ptr)) != '\n') {
        putchar(c);
    }
    
    printf("\nДлина слова на 'a' : %d", length_of_a_word);
    
    rewind(ptr);
    
    long cur_word_beginning, cur_word_end = 0;
    char first_letter, last_letter;
    int a_word_chr, tmp_chr;
    
    while (c != EOF) {
        cur_word_beginning = ftell(ptr);
        
        c = getc(ptr);
        
        int cur_word_length = 1;
        cur_word_end = ftell(ptr);
        first_letter = c;
        
        while ((c2 = getc(ptr)) != '\n' && c2 != EOF) {
            cur_word_length++;
            cur_word_end = ftell(ptr);
            last_letter = c2;
            //            fgetpos(ptr, &cur_word_position_end);
        }
        
        printf("\nСлово %i:%i len=%i", cur_word_beginning, cur_word_end, cur_word_length);
        if (first_letter == last_letter) {
            printf("\tHooray");
            // replace
            fseek(ptr, cur_word_beginning, SEEK_SET);
            
            int len_diff = length_of_a_word - cur_word_length;
            
            for (int i = 0; i < length_of_a_word; i++) {
                fseek(ptr, (long) a_word_position + i * sizeof(char), SEEK_SET);
                a_word_chr = getc(ptr);
                fseek(ptr, cur_word_beginning + i * sizeof(char), SEEK_SET);
                putc(a_word_chr, ptr);
            }
            fflush(ptr);
            
            if (a_word_position > cur_word_beginning) {
                // a_word location will be changed after the moving of the rest of the file
                a_word_position += len_diff * sizeof(char);
            }
            
            // a_word is shorter than current word
            if (len_diff < 0) {
                // move rest of the file to the left from the cur_word_end
                fseek(ptr, cur_word_end * sizeof(char), SEEK_SET);
                while ((c2 = getc(ptr)) != EOF ) {
                    fseek(ptr, (len_diff - 1) * sizeof(char), SEEK_CUR);
                    putc(c2, ptr);
                    fseek(ptr, -1 * len_diff * sizeof(char), SEEK_CUR);
                }
                fflush(ptr);
                // cut out the length difference
                
            }
            if (len_diff > 0) {
                // move rest of the file towards the end from the end
                fseek(ptr, -1 * sizeof(char), SEEK_END);
                long cur_pos;
                while ((cur_pos = ftell(ptr)) > cur_word_end) {
                    c2 = getc(ptr);
                    
                    fseek(ptr, (-1 * len_diff) * sizeof(char), SEEK_CUR);
                    putc(c2, ptr);
                    fseek(ptr, (len_diff - 1) * sizeof(char), SEEK_CUR);
                }
                fflush(ptr);
            }
            fseek(ptr, cur_word_end * sizeof(char), SEEK_SET);
        }
    }
    
    
    printf("\nГотово\n");
    return 0;
    
}
