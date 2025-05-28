#include "clipp.h"
#include <iostream>
#include <string>

using namespace clipp;

int main(int argc, char* argv[]) 
{

    std::string CodigoSerie = "";
    int PortaSerial = 0;
    double tensao = 0.0;
    double corrente = 0.0;
    int tempo = 0;

    std::vector<int> numeros(10); // Vetor com 10 posições

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

        for (int i = 0; i < 10; ++i) {
        // Verifica se o caractere é um dígito
        if (!std::isdigit(CodigoSerie[i])) 
        {
            std::cout << "Erro: O caractere na posição " << i + 1 << " não é um dígito." << std::endl;
            return 1;
        }
        // Converte o caractere para inteiro (subtrai 48 para obter o valor numérico)
        numeros[i] = CodigoSerie[i] - 48;
        }
    } 
    else 
    {
        std::cout << "Uso incorreto!\n";
        std::cout << make_man_page(cli, argv[0]);
    }

    return 0;
}