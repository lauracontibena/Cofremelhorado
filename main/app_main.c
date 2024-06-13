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
#include "driver/adc.h"
#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "MP_hcf.h"   // Controles do ADC
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>
#include "ioplaca.h"
#ifndef __MP_HCF_H
#include "esp_err.h"

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------
#define MP1 GPIO_NUM_19
#define MP2 GPIO_NUM_21
#define MP3 GPIO_NUM_22
#define MP4 GPIO_NUM_23
#define TMP 10
#define DRV1 io_le_escreve((outputs|0b10000000)&0b10001111)
#define DRV2 io_le_escreve((outputs|0b11000000)&0b11001111)
#define DRV3 io_le_escreve((outputs|0b01000000)&0b01001111)
#define DRV4 io_le_escreve((outputs|0b01100000)&0b01101111)
#define DRV5 io_le_escreve((outputs|0b00100000)&0b00101111)
#define DRV6 io_le_escreve((outputs|0b00110000)&0b00111111)
#define DRV7 io_le_escreve((outputs|0b00010000)&0b00011111)
#define DRV8 io_le_escreve((outputs|0b10010000)&0b10011111)
#define DRV0 io_le_escreve(outputs&0b00001111)

#define FI (1<<FC_INI)
#define FF (1<<FC_FIN)
#define __MP_HCF_H

static const char *TAG = "MP_HCF";
int FC_FIN, FC_INI; 
uint8_t outputs;

//TECLADO 
#define T_C 'C'==le_teclado()
//#define T_0 '0'==le_teclado()

//#define T_1 '1'==le_teclado()
//#define T_2 '2'==le_teclado()

//#define T_3 '3'==le_teclado()
///#define T_4 '4'==le_teclado()

//#define T_5 '5'==le_teclado()
//#define T_6 '6'==le_teclado()
//#define T_7 '7'==le_teclado()
//#define T_8 '8'==le_teclado()


//#define LIGA_RELE1 saidas|0b00000001
//#define DESLIGA_RELE1 saidas&11111110

//#define LIGA_RELE2 saidas|0b00000010
//#define DESLIGA_RELE2 saidas&11111101

//#define LIGA_TRIAC1 saidas|0b00000100
//#define DESLIGA_TRIAC1 saidas&11111011

//#define LIGA_TRIAC2 saidas|0b00001000
//#define DESLIGA_TRIAC2 saidas&11110111

//#define LIGA_TRANSISTOR1 saidas|0b00010000
//#define DESLIGA_TRANSISTOR1 saidas&11101111

//#define LIGA_TRANSISTOR2 saidas|0b00100000
//#define DESLIGA_TRANSISTOR2 saidas&11011111

//#define LIGA_TRANSISTOR3 saidas|0b01000000
//#define DESLIGA_TRANSISTOR3 saidas&10111111

//#define LIGA_TRANSISTOR4 saidas|0b10000000
//#define DESLIGA_TRANSISTOR4 saidas&01111111
//#define Potenciometro
char mostra [40];
int valor = 0;
int controle = 0;
int num1 = 0; //numero 1 
int num2 = 0;
int coluna = 0; 
int resultado = 0; 
char operador; 
char tecla; 
char mostra[40];
int clear = 0; 


// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------
static const char *TAG = "Placa";
static uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas




// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------


// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------
void passo_1()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,1);
    gpio_set_level(MP3,1);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_2()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,1);
    gpio_set_level(MP4,1);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_3()
{
    gpio_set_level(MP1,1);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,1);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_4()
{
    gpio_set_level(MP1,1);
    gpio_set_level(MP2,1);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_fim()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void MP_horario(int passos)
{
    ESP_LOGI(TAG, "Rotacionando horário..");
    for (int i = passos * 4; i > 0; i--)
    {
        passo_4();
        passo_3();
        passo_2();
        passo_1();
    }
    passo_fim();
}
void MP_antihora(int passos)
{
    ESP_LOGI(TAG, "Rotacionando anti-horário..");
    for (int i = passos * 4; i > 0; i--)
    {   
        passo_1();
        passo_2();
        passo_3();
        passo_4();
    }
    passo_fim();
}

void rotacionar_MP(int horario, float graus) //64 passos por volta = 5,625 graus por passo
{
    float passos = graus / 2.8125;
    ESP_LOGI(TAG, "Recebido %.1f graus, %d passos", graus, (int)passos);
    horario == 0 ? MP_antihora((int)passos) : MP_horario((int)passos);
    ESP_LOGI(TAG, "Rotação concluída");
}

void DRV_abrir(int passos)
{
    ESP_LOGI(TAG, "Rotacionando horário..");
    for (int i = passos * 4; i > 0; i--)
    {
        if(DRV8 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV7 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV6 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV5 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV4 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV3 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV2 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV1 & FF)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
    }
    DRV0;
}
void DRV_fechar(int passos)
{
    ESP_LOGI(TAG, "Rotacionando horário..");
    for (int i = passos * 4; i > 0; i--)
    {
        if(DRV1 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV2 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV3 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV4 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV5 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV6 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV7 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
        if(DRV8 & FI)break;
        vTaskDelay(TMP / portTICK_PERIOD_MS); 
    }
    DRV0;
}

void rotacionar_DRV(int abrir, float graus, uint8_t saidas_atual) //64 passos por volta = 5,625 graus por passo
{
    float passos = graus / 2.8125;
    outputs = saidas_atual;
    ESP_LOGI(TAG, "Recebido %.1f graus, %d passos", graus, (int)passos);
    abrir == 0 ? DRV_fechar((int)passos) : DRV_abrir((int)passos);
    ESP_LOGI(TAG, "Rotação concluída");
}



void MP_init()
{
    gpio_reset_pin(MP1);
    gpio_set_direction(MP1, GPIO_MODE_OUTPUT);
    gpio_set_level(MP1, 0);
    
    gpio_reset_pin(MP2);
    gpio_set_direction(MP2, GPIO_MODE_OUTPUT);
    gpio_set_level(MP2, 0);
    
    gpio_reset_pin(MP3);
    gpio_set_direction(MP3, GPIO_MODE_OUTPUT);
    gpio_set_level(MP3, 0);
    
    gpio_reset_pin(MP4);
    gpio_set_direction(MP4, GPIO_MODE_OUTPUT);
    gpio_set_level(MP4, 0);

    ESP_LOGI(TAG, "MP Configurado modo Driver ULN2003");
}
void DRV_init(int Fim_de_curso_inicial, int Fim_de_curso_final)
{
    FC_INI = Fim_de_curso_inicial;
    FC_FIN = Fim_de_curso_final; 
}


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
    /*esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }*/

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();
    lcd595_init();
      void MP_init(void);
    void rotacionar_MP(int horario, float graus);

    void DRV_init(int Fim_de_curso_inicial, int Fim_de_curso_final);
    void rotacionar_DRV(int abrir, float graus, uint8_t saidas_atual);
#endif
  

    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados
  
    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {    
        
        char tecla = le_teclado (); //ler tecla pressionada 
        int resultado = 0; // armazena resultado 
        tecla = le_teclado();
        if (tecla>= '0' && tecla <= '9') 
{
        
        if (controle == 0)
        {
            num1 = num1 * 10 + tecla - '0';
        }

        else if (controle == 1 )
        {
            num2 = num2 * 10 + tecla - '0'; 
        }
}
        else if (tecla == '+')
        {
            operador = '+';
            controle = 1;
        }

        else if (tecla == '=')
        {
            if(operador == '+')
            {
                resultado = num1 + num2; 
                controle = 2; 
                sprintf(&mostra[0], "%d", resultado);
                lcd595_write(2,0, &mostra[0]);
            }
        }

        if (tecla != '_')
        {
            sprintf(&mostra[0], "%c", tecla);
            lcd595_write(1, coluna, &mostra[0]);
            coluna++;
        }

        if (T_C)
        {
            lcd595_clear();
            resultado = 0; 
            num1 = 0;
            num2 = 0;
            coluna = 0; 
            controle = 0; 
            operador = 0; 
        }
        
        if (tecla == '-')
        {
            operador = '-';
            controle = 1;
        }

        else if (tecla == '=')
        {
            if(operador == '-')
            {
                resultado = num1 - num2; 
                controle = 2; 
                sprintf(&mostra[0], "%d", resultado);
                lcd595_write(2,0, &mostra[0]);
            }
        }


        if (tecla == 'x')
        {
            operador = 'x';
            controle = 1;
        }

        else if (tecla == '=')
        {
            if(operador == 'x')
            {
                resultado = num1 * num2; 
                controle = 2; 
                sprintf(&mostra[0], "%d", resultado);
                lcd595_write(2,0, &mostra[0]);
            }
        }

        if (tecla == '/')
        {
            operador = '/';
            controle = 1;
        }

        else if (tecla == '=')
        {
            if(operador == '/')
            {
                resultado = num1 / num2; 
                controle = 2; 
                sprintf(&mostra[0], "%d", resultado);
                lcd595_write(2,0, &mostra[0]);
            }
        }



        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
                                                                                                                                                                  


        
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(30 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
    
    // caso erro no programa, desliga o módulo ADC
    //hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
