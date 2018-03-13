#include <cstdio>
#include <stdlib.h>

#define MAX 112
#define IN_FILE 0

int main(void){
  FILE *entrada, *saida;
  char linha[MAX];

  entrada = fopen("/dev/ttyUSB0", "r");
  saida = fopen("./saida.txt", "a");
  if(entrada == NULL){ printf("Erro ao abrir arquivo de entrada\n"); return 0; }
  if(saida == NULL){ printf("Erro ao abrir arquivo de saida\n"); return 0; }
  while(1) {
    linha[0] = 0;
    fgets(linha, MAX, entrada);
    if(linha[0]){
      if(IN_FILE) fwrite(linha, sizeof(char), sizeof(linha), saida);
      else printf("%s", linha);
    }
  }
  fclose(entrada);
  fclose(saida);
  return 0;
}
