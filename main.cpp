#include "clipp.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace clipp;

std::vector<unsigned char> identificador(10); // Vetor com 10 posições para identificação do medidor
std::vector<unsigned char> bytesID;

unsigned char DecParaBCD(unsigned char high, unsigned char low);

int main(int argc, char* argv[]) 
{

    std::string CodigoSerie = "0206050809";
    int PortaSerial = 0;
    double tensao = 0.0;
    double corrente = 0.0;
    int tempo = 0;

    //---------------------------------------Configuração da Comunicação Serial-------------------------------------------------------

    std::vector<unsigned char> PacoteTensao;
    std::vector<unsigned char> PacoteCorrente;
    std::vector<unsigned char> PacotePotencia;
    
    //---------------------------------------Fim das configurações de comunicação serial----------------------------------------------
    

    auto cli = (
        option("-p", "--porta") & value("PortaCOM", PortaSerial),
        option("-c", "--codigo") & value("Codigo", CodigoSerie),
        option("-v", "--tensao") & value("Tensao", tensao),
        option("-i", "--corrente") & value("Corrente", corrente),
        option("-t", "--tempo") & value("Tempo", tempo)
               );

    if (parse(argc, argv, cli)) 
    {
        std::cout << "Porta serial: " << PortaSerial << std::endl;
        std::cout << "Codigo serial do medidor: " << CodigoSerie << std::endl;
        std::cout << "Tensao: " << tensao << std::endl;
        std::cout << "Corrente: " << corrente << std::endl;
        std::cout << "Tempo: " << tempo << std::endl;

        for (int i = 0; i < 10; ++i) 
        {
            // Verifica se o caractere é um dígito
            if (!std::isdigit(CodigoSerie[i])) 
            {
                std::cout << "Erro: O caractere na posição " << i + 1 << " não é um dígito." << std::endl;
                return 1;
            }
            // Converte o caractere para inteiro (subtrai 48 para obter o valor numérico)
            identificador[i] = CodigoSerie[i] - 48;
        }
        
        // Combinar os números em pares para formar bytes BCD
        for (size_t i = 0; i < 10; i += 2) 
        {
            unsigned char high = identificador[i];     // Primeiro número do par (nibble superior)
            unsigned char low = identificador[i + 1];  // Segundo número do par (nibble inferior)
            unsigned char ByteEmBCD = DecParaBCD(high, low);
            bytesID.push_back(ByteEmBCD);
        }

    return 0;
}
}


// Função para combinar dois números decimais (0-9) em um byte BCD
    unsigned char DecParaBCD(unsigned char high, unsigned char low) 
    {
        // Verificar se os valores estão no intervalo válido (0-9)
        if (high > 9 || low > 9) 
        {
            std::cerr << "Erro: Valores devem estar entre 0 e 9!" << std::endl;
            return 0;
        }
        // Combinar os nibbles: high nos 4 bits superiores, low nos 4 bits inferiores
        return (high << 4) | low;
    }