//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "MP_hcf.h"   // Controles do ADC

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------
#define T_C 'C'== le_teclado()
char mostra [40];
int valor = 0;
int digito = 0; // numero de digitos 
int controle = 0; 
int num1 = 0; //numero 1 
int senha = 2504; // senha do cofre 
int coluna = 0; 
char tecla; // para tecla apertada 
char mostra[40];
int clear = 0; 
int i = 0; // para funcionamento do motor  
int tentativas = 0; 

#define Potenciometro

// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------
uint32_t  valorpoten;
static const char *TAG = "Placa";
static uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas


// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------


 void exibe_lcd (void)
    {
    sprintf (&mostra[0],"  %"PRIu32"   ", valorpoten);
    lcd595_write(1,2, &mostra[0]);
    }

void abrir_cofre (void) // para o laco while nao ficar tao longo e abrir o cofre 
{
            for (i = 0; i<1964;i= valorpoten)
        {
            rotacionar_DRV (1,10, saidas);
            hcf_adc_ler (& valorpoten);
        }
}


void fechar_cofre (void) // para o laco while nao ficar tao longo e fechar o cofre 
{
    for (i = valorpoten ; i>400; i= valorpoten)
        {
            rotacionar_DRV (0,10, saidas);
            hcf_adc_ler (& valorpoten);
             
        }
}


void cofre_aberto (void) // para o laco while nao ficar tao longo e detectar caso o crofe esteja aberto 
{
    hcf_adc_ler (& valorpoten);
    for (i = valorpoten ; i>400; i= valorpoten)
        {
            rotacionar_DRV (0,10, saidas);
            hcf_adc_ler (& valorpoten);
             
        }
}


// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal


    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
    entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas

    // inicializar o display LCD 
    lcd595_init();
    lcd595_write(1,1,"    Jornada 1   ");
    lcd595_write(2,1," Programa Basico");
    
    // Inicializar o componente de leitura de entrada analógica
    esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();
    lcd595_init();
  
    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados

    DRV_init (6,7); // motor 
    cofre_aberto(); // detectar cofre aberto
    lcd595_write (1,1, "Insira sua"); 
    lcd595_write (2,2, "senha"); 
    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
        
        char tecla = le_teclado (); //ler tecla pressionada
       
        
     if (tecla>= '0' && tecla <= '9') // ter acesso a todos os numeros do teclado  
    {
        if (controle == 0)
        {
  
            num1 = num1 * 10 + tecla - '0'; // conseguir fazer um numero com mais de um algarismo 
            digito = digito + 1; // gravar os digitos apertados 
        }  
        if (digito == 1) // caso tenha 1 digito apertado 
        {
            lcd595_clear();
            lcd595_write (1,1, "[*] [] [] []"); 
        }
        if (digito == 2) // caso tenha 2 digitos apertado 
        {
            lcd595_clear();
            lcd595_write (1,1, "[*] [*] [] []");  
        }
        if (digito == 3) // caso tenha 3 digitos apertado 
        {
            lcd595_clear();
            lcd595_write (1,1, "[*] [*] [*] []");  
        }
        if (digito == 4) // caso tenha 4 digitos apertado 
        {
            lcd595_clear();
            lcd595_write (1,1, "[*] [*] [*] [*]");  
        }
        if (digito == 4) // se tiverem 4 digitos digitados 
        {
            if (num1 == senha) // se esses digitos forem igual a senha 
            {
            lcd595_write (1,1, "Senha correta"); 
            lcd595_clear();
            lcd595_write (1,1, "Abrindo cofre");
            abrir_cofre(); // funcao void escrita la em cima para abrir o cofre 
            lcd595_clear();
            lcd595_write (1,1, "10 segundos..."); // iniciando contagem regressiva
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "9 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "8 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "7 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "6 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "5 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "4 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "3 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "2 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "1 segundos...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd595_clear();
            lcd595_write (1,1, "Fechando cofre");
            fechar_cofre(); // funcao void escrita la em cima para fechar o cofre
            }
        }
            if (digito == 4) // se 4 digitos tiverem sido digitados 
            {
                if (num1 != senha) // se a senha digitada for diferente da correta 
                {
                lcd595_clear();
                lcd595_write (1,1, "Senha"); 
                lcd595_write (2,2, "incorreta");
                lcd595_clear();
                lcd595_write (1,1, "Insira sua"); 
                lcd595_write (2,2, "senha"); 
                tentativas = tentativas + 1; 
                if (tentativas == 0) // para tentativas feitas no cofre
                {
                }
                if (tentativas == 1)
                {
                lcd595_clear();
                lcd595_write (2,1, "1 tentativa");
                }
                if (tentativas == 2)
                {
                lcd595_clear();
                lcd595_write (2,1, "2 tentativas");
                }
                if (tentativas == 3)
                {
                lcd595_clear();
                lcd595_write (2,1, "3 tentativas");
                lcd595_write (1,1, "Cofre bloqueado");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                lcd595_clear();
                lcd595_write (1,1, "Espere 1");
                lcd595_write (2,2, "minuto");
                vTaskDelay(1000000 / portTICK_PERIOD_MS);
                lcd595_write (1,1, "Insira sua"); 
                lcd595_write (2,2, "senha"); 
                }
                }
                
               
            }       
          
}
        if (T_C) // para limpar a senha digitada 
         {
            lcd595_clear();
            num1 = 0; 
            digito = 0;
            controle = 0;
         }
    
      
                  
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(200 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
    
    // caso erro no programa, desliga o módulo ADC
    hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
