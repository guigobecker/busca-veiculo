//RODRIGO SCHMIDT BECKER

#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <functional>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

struct Carro
{
    std::string placa;
    std::string marca;
    std::string ano;
};

template<typename T>
struct Abb {
    T carro;
    Abb<T>* esq;
    Abb<T>* dir;
    int altura;
};

template<typename T>
bool vazio(Abb<T>* no)
{
    return (no == nullptr);
}

template<typename T>
int altura(Abb<T>* no)
{
    if(vazio(no))
        return 0;
    
    return no->altura;
}

template<typename T>
int fator(Abb<T>* no)
{
    if(vazio(no))
        return 0;
    
    return (altura(no->esq) - altura(no->dir));
}

template<typename T>
void ajustar_altura(Abb<T>* no)
{
    if(vazio(no))
        return;

    ajustar_altura(no->esq);
    ajustar_altura(no->dir);

    no->altura = std::max(altura(no->esq), altura(no->dir)) + 1;
}

template<typename T>
Abb<T>* esq_rotacao(Abb<T>* x)
{
    Abb<T>* q = x->dir;
    Abb<T>* hold = q->esq;
    q->esq = x;
    x->dir = hold;
    ajustar_altura(x->dir);
    ajustar_altura(x);
    return q;
}

template<typename T>
Abb<T>* esq_dir_rotacao(Abb<T>* no)
{
    no->esq = esq_rotacao(no->esq);
    no = dir_rotacao(no);
    return no;
}

template<typename T>
Abb<T>* dir_esq_rotacao(Abb<T>* no)
{
    no->dir = dir_rotacao(no->dir);
    no = esq_rotacao(no);
    return no;
}

template<typename T>
Abb<T>* dir_rotacao(Abb<T>* x)
{
    Abb<T>* q = x->esq;
    Abb<T>* hold = q->dir;
    q->dir = x;
    x->esq = hold;
    ajustar_altura(x->esq);
    ajustar_altura(x);
    return q;
}

template<typename T>
bool caso1(Abb<T>* no) // esq/esq
{
    return (fator(no) > 1 && fator(no->esq) >= 0); //> ou >= 0?
}

template<typename T>
bool caso2(Abb<T>* no) // esq/dir
{
    return (fator(no) > 1 && !(fator(no->esq) >= 0));
}

template<typename T>
bool caso3(Abb<T>* no) // dir/esq
{
    return (fator(no) < -1 && !(fator(no->dir) <= 0));
}

template<typename T>
bool caso4(Abb<T>* no) // dir/dir
{
    return (fator(no) < -1 && fator(no->dir) <= 0);
}

template<typename T>
Abb<T>* balancear(Abb<T>* no)
{
    ajustar_altura(no);
    if(caso1(no))
        no = dir_rotacao(no);
    
    else if(caso2(no))
        no = esq_dir_rotacao(no);
    
    else if(caso3(no))
        no = dir_esq_rotacao(no);
    
    else if(caso4(no))
        no = esq_rotacao(no);
    
    return no;
}

template<typename T>
Abb<T>* insere(Abb<T>* no, T v, char id)
{
    if(vazio(no))
    {
        Abb<T>* no = new Abb<T>;
        no->carro = v;
        no->altura = 0;
        no->esq = nullptr;
        no->dir = nullptr;
        return no;
    }

    if(id == 'p')
    {
        if(v->placa < no->carro->placa)
                no->esq = insere(no->esq, v, id);

        else if (v->placa > no->carro->placa)
                no->dir = insere(no->dir, v, id);
    }

    if(id == 'm')
    {
        if(v->marca < no->carro->marca)
                no->esq = insere(no->esq, v, id);

        else if (v->marca > no->carro->marca)
                no->dir = insere(no->dir, v, id);
    }

    if(id == 'a')
    {
        if(v->ano < no->carro->ano)
                no->esq = insere(no->esq, v, id);

        else if (v->ano > no->carro->ano)
                no->dir = insere(no->dir, v, id);
    }

    else
        return no;

    ajustar_altura(no);

    no = balancear(no);

    return no;
}

template<typename T>
Abb<T>* no_minimo(Abb<T>* no)
{
    Abb<T>* curr = no->esq;
    while( curr->dir != nullptr ) //busca o filho mais à direita
        curr = curr->dir;
    return curr;
}

template<typename T>
Abb<T>* remove(Abb<T>* no, T v, char id)
{
    if(vazio(no))
        return no;
    
    if(id == 'p')
    {
        if(no->carro->placa > v->placa)
            no->esq = remove(no->esq, v, id);

        else if(no->carro->placa < v->placa)
            no->dir = remove(no->dir, v, id);
    }

    else if(id == 'm')
    {
        if(no->carro->marca > v->marca)
            no->esq = remove(no->esq, v, id);

        else if(no->carro->marca < v->marca)
            no->dir = remove(no->dir, v, id);
    }

    else if(id == 'a')
    {
        if(no->carro->ano > v->ano)
            no->esq = remove(no->esq, v, id);

        else if(no->carro->ano < v->ano)
            no->dir = remove(no->dir, v, id);
    }

    if(vazio(no->esq) && vazio(no->dir)) //nó sem filhos
    {
        delete no;
        no = nullptr;
    }

    else if (no->esq == nullptr) //filho na direita
    {
        Abb<T>* t = no;
        no = no->dir;
        delete t;
    }

    else if (no->dir == nullptr) //filho na esquerda
    {
        Abb<T>* t = no;
        no = no->esq;
        delete t;
    }

    else //nó com dois filhos
    {
        Abb<T>* t = no_minimo(no);
        no->carro = t->carro;
        t->carro = v;
        no->esq = remove(no->esq, v, id);
    }

    no = balancear(no);

    return no;
}

template<typename T>
void ordena(Abb<T>* a, std::list<T>& saida)
{
    if(!vazio(a)){
        ordena(a->esq, saida);
        saida.push_back(a->carro);
        ordena(a->dir, saida);
    }
}   

template<typename T>
void destroi(Abb<T>* a)
{
    if(!vazio(a)){
        destroi(a->esq);
        destroi(a->dir);
        delete a;
    }
}

template<typename T>
Abb<T>* busca(Abb<T>* no, T v)
{
    if(vazio(no))
        return nullptr;

    if(no->carro->placa == v->placa)
        return no;

    else if(no->carro->placa > v->placa)
        return busca(no->esq, v);

    else
        return busca(no->dir, v);
}

void imprime(std::list<Carro*> saida)
{
    for(auto it = saida.begin(); it != saida.end(); it++)
        std::cout << (*it)->placa << " " << (*it)->marca << " " << (*it)->ano << std::endl;
    
    std::cout << std::endl;

    saida.clear();
}

void incluir_carro(Abb<Carro*>** arvore_placa, Abb<Carro*>** arvore_marca, Abb<Carro*>** arvore_ano)
{
    Carro* carro = new Carro;
    std::string placa, marca, ano;

    std::cout << "DIGITE A PLACA: ";
    std::cin >> carro->placa;

    std::cout << "DIGITE A MARCA: ";
    std::cin >> carro->marca;

    std::cout << "DIGITE O ANO: ";
    std::cin >> carro->ano;

    *arvore_placa = insere(*arvore_placa, carro, 'p');
    *arvore_marca = insere(*arvore_marca, carro, 'm');
    *arvore_ano = insere(*arvore_ano, carro, 'a');
}

void remover_carro(Abb<Carro*>** arvore_placa, Abb<Carro*>** arvore_marca, Abb<Carro*>** arvore_ano)
{
    Carro* carro = new Carro;
    std::string placa, marca, ano;

    std::cout << "DIGITE A PLACA: ";
    std::cin >> carro->placa;

    std::cout << "DIGITE A MARCA: ";
    std::cin >> carro->marca;

    std::cout << "DIGITE O ANO: ";
    std::cin >> carro->ano;

    *arvore_placa = remove(*arvore_placa, carro, 'p');
    *arvore_marca = remove(*arvore_marca, carro, 'm');
    *arvore_ano = remove(*arvore_ano, carro, 'a');
}

void buscar_placa(Abb<Carro*>* arvore_placa)
{
    Carro* carro = new Carro;
    std::string placa, marca, ano;

    std::cout << "DIGITE A PLACA: ";
    std::cin >> carro->placa;

    std::cout << "\n" << busca(arvore_placa, carro)->carro->placa << std::endl;
    std::cout << busca(arvore_placa, carro)->carro->marca << std::endl;
    std::cout << busca(arvore_placa, carro)->carro->ano << std::endl << std::endl;
}

int main()
{
    Abb<Carro*>* arvore_placa = nullptr;
    Abb<Carro*>* arvore_marca = nullptr;
    Abb<Carro*>* arvore_ano = nullptr;

    std::list<Carro*> saida;

    int n;
    std::string s;
    bool fim = false;


    do
    {
        std::cout << "OPERAÇÕES:\n\n"
                    "1 - INCLUIR\n"
                    "2 - EXCLUIR\n"
                    "3 - BUSCAR\n"
                    "4 - LISTAR POR PLACA\n"
                    "5 - LISTAR POR MARCA\n"
                    "6 - LISTAR POR ANO\n"
                    "7 - SAIR\n\n";
        
        std::cin >> n;

        switch (n)
        {
        case 1:
            incluir_carro(&arvore_placa, &arvore_marca, &arvore_ano);
            break;
        
        case 2:
            remover_carro(&arvore_placa, &arvore_marca, &arvore_ano);
            break;

        case 3:
            buscar_placa(arvore_placa);
            
            std::cout << "1 - VOLTAR\n\n";
            std::cin >> n;    
            
            if(n == 1)
                break;

        case 4:
            ordena(arvore_placa, saida);
            imprime(saida);
            saida.clear();

            std::cout << "1 - VOLTAR\n\n";
            std::cin >> n;

            if(n == 1)
                break;

        case 5:
            ordena(arvore_marca, saida);
            imprime(saida);
            saida.clear();

            std::cout << "1 - VOLTAR\n\n";
            std::cin >> n;

            if(n == 1)
                break;
        
        case 6:
            ordena(arvore_ano, saida);
            imprime(saida);
            saida.clear();

            std::cout << "1 - VOLTAR\n\n";
            std::cin >> n;

            if(n == 1)
                break;

        case 7:
            fim = true;
            break;

        default:
                break;
        }
    } while(fim == false);
}
