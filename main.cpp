#include "clipp.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace clipp;

//------------------------------------Declarações de Protótipos de Funções-----------------------------------
void ConstruirPacoteTensao(std::vector<unsigned char> ParSerialCode, double ParTensao);
std::vector<unsigned char> SepDigitos(uint16_t valor);
//-----------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) 
{

    std::string CodigoSerie = "0206050809";
    int PortaSerial = 0;
    double tensao = 0.0;
    double corrente = 0.0;
    int tempo = 0;

    std::vector<unsigned char> identificador(10); // Vetor com 10 posições

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
        

        PortaSerial = 12;
        tensao = 232.6;
        corrente = 156.65;
        tempo = 5;

        ConstruirPacoteTensao(identificador, tensao);
    
    } 
    else 
    {
        std::cout << "Uso incorreto!\n";
        std::cout << make_man_page(cli, argv[0]);
    }

    return 0;
}

    void ConstruirPacoteTensao(std::vector<unsigned char> ParSerialCode, double ParTensao)
    {
        
        std::vector<unsigned char> Preambulo = {0xAA, 0x55};
        std::vector<unsigned char> Pacote;
        std::vector<unsigned char> Tamanho = {0x04};
        std::vector<unsigned char> EscopoIndice = {0x0B, 0x01};
        
        //Preambulo destino.insert(destino.end(), origem.begin(), origem.end());
        
        Pacote.insert(Pacote.end(), Preambulo.begin(), Preambulo.end());     
        Pacote.insert(Pacote.end(), ParSerialCode.begin(), ParSerialCode.end());
        Pacote.insert(Pacote.end(), Tamanho.begin(), Tamanho.end());
        Pacote.insert(Pacote.end(), EscopoIndice.begin(), EscopoIndice.end());

        uint16_t Tensao;
    
        std::vector<unsigned char> DigitosDaTensao(4);

        //-------------------------------------------------------------------------------------
        //Cálculo para quebrar o valor de tensão elétrica em BCD
        //-------------------------------------------------------------------------------------

        Tensao = uint16_t(ParTensao * 10);
        
        DigitosDaTensao = SepDigitos(Tensao);

        Pacote.insert(Pacote.end(), DigitosDaTensao.begin(), DigitosDaTensao.end());

        std::cout << "Pacote de Tensao (em decimal): ";
        
        for (size_t i = 0; i < Pacote.size(); ++i) 
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(Pacote[i]) << " ";
        }

        
        std::cout << "Pacote de Tensao: " << std::endl;

        std::cout << std::endl;
    }

    std::vector<unsigned char> SepDigitos(uint16_t valor) 
    {
        std::vector<unsigned char> Digitos(4);

        Digitos[3] = valor / 1000;
        Digitos[2] = (valor % 1000)/100;
        Digitos[1] = (valor % 100)/10;
        Digitos[0] = valor % 10;

        return Digitos;
    }