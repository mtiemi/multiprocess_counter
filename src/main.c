/* Mariane Tiemi Iguti RA: 147279
Contador de numeros primos usando multiprocessos
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int primo(unsigned long int entrada)
{
    unsigned long int i, div = 0;
    
    for(i = 1; i <= entrada; i++)
    {
        if(entrada % i == 0)
        {
            div++;
        }
    }
    
    if(div == 2)
    {
        return 1; // primo
    }else
    {
        return 0; // nao eh primo
    }

}

int main() 
{
    pid_t filho[3];
    unsigned long int entrada;
    int filhos_criados = 0;
    int i, aux = 0;
    int n_primos = 0;
    
    /* Definir flags de protecao e visibilidade de memoria */
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    /* Criar area de memoria compartilhada */
    // criando um vetor compartilhado para armazenar se o numero eh primo ou nao
    int *primos;
    primos = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
    if ((long int)primos == -1) printf("Erro de alocacao!\n");
    memset(primos, 0, 100*sizeof(int));

    // criando variavel compartilhado para usar como indice auxiliar
    int *j;
    j = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
    if ((long int)j ==- 1) printf("Erro de alocacao!\n");
    *j = 0;
    
    do 
    {
        // lendo o numeros de entrada
        scanf("%lu", &entrada);
        
        // enquanto tiverem menos de 4 processos filhos
        if(filhos_criados < 4)
        {
        
            filho[filhos_criados] = fork();
            
            // eh processo filho
            if(filho[filhos_criados] == 0)
            {
                (*j)++;
                aux = *j;
                //printf("Filho atual é %d e j %d\n", filhos_criados, *j);
                if(primo(entrada) == 1) // se for primo
                {
                    primos[aux] = 1;
  
                }else
                {
                    primos[aux] = 0;
                }
                
                exit(0);
            // eh processo pai    
            }else
            {
                filhos_criados++;
            }
            
        }else // quando tiver mais 4 processos filhos, termina os processos e começa um outro ciclo de forks
        {
             
            for(i = 0; i < filhos_criados; i++)
            {
                waitpid(filho[i], NULL, 0);

            }
            filhos_criados = 0;
            filho[filhos_criados] = fork();
            
            // eh processo filho
            if(filho[filhos_criados] == 0)
            {
                (*j)++;
                aux = *j;
                if(primo(entrada) == 1)
                {
                    primos[aux] = 1;

                }else
                {
                    primos[aux] = 0;
                }
                
               exit(0);
            // eh processo pai    
            }else
            {
                filhos_criados++;
   
            }
            
        }
        
    }while(getchar() != '\n');
    
    for(i = 0; i < filhos_criados; i++)
    {
        waitpid(filho[i], NULL, 0);
    }
      
    for(i = 1; i <= *j ; i++)
    {
        n_primos = n_primos + primos[i];
        
    }
    
    printf("%d\n", n_primos);   
    
    return 0;
     
}
