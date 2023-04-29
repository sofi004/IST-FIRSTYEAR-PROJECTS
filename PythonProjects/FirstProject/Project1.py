"""
1º Projeto de Fundamentos da Programação
Sofia Dinis Pinto Piteira
sofia.piteira@tecnico.ulisboa.pt
28 de outubro de 2022
"""


# Exercício 1 do projeto
# Justifica um texto utilizando uma largura de coluna configurável


def limpa_texto(dirty_text):
    """
    Substitui os caracteres brancos (\t, \n, \v, \f, \r) duma cadeia de 
    caracteres por espaços (" ") e remove qualquer repetição de espaços 
    consecutivos, bem como os espaços iniciais e finais.
    """
    # corpo da função
    return ' '.join(dirty_text.split())
    # Utilizando a função split() os caracteres brancos são automaticamente excluidos


def corta_texto(cadeia, num):
    """
    Recebe uma cadeia de caracteres e um número.
    Limpa a cadeia de caracteres.
    Retorna um tuplo, em que o primeiro elento tem todas as palavras completas, 
    desde o início da cadeia original, até um comprimento máximo igual ao número
    recebido e o segundo tem o resto do texto de entrada.
    No final, nenhuma destas cadeias, dentro do tuplo, pode começar ou terminar 
    com espaços, mas podem ser vazias.
    """
    # corpo da função
    if len(cadeia) <= num:
        final = (cadeia, "")
        # caso o número (2º argumento)  seja maior que o tamanho da cadeia(1º argumento)
        # a função retorna um tuplo em que o 2º elemento é uma string vazia
    else:
        cadeia_tuplo = cadeia.split()  # Como uso a função split() não preciso de usar a função limpa_texto
        conta_espaco = 0
        i = 0
        conta = 0
        while conta < num:
            # O ciclo tem como objetivo contar quantos caracteres vão pertencer ao primeiro elemento do tuplo a retornar
            conta = conta_espaco + len(cadeia_tuplo[i])
            conta_espaco = conta + 1
            # O (+1) é para contarmos com mais o espaço que é necessário colocar a seguir à palavra.
            i += 1
        if type(cadeia[num - 1]) == str and cadeia[num] == " ":
            primeira_cadeia = " ".join(cadeia_tuplo[:i])
            # A primeira_cadeia será igual à cadeia de entrada limpa até à posição i-1.
            segunda_cadeia = " ".join(cadeia_tuplo[i:])
            # A segunda_cadeia será igual à cadeia de entrada limpa da posição i até ao final.
            final = (primeira_cadeia, segunda_cadeia)
        else:
            primeira_cadeia = " ".join(cadeia_tuplo[:i - 1])
            # A primeira_cadeia será igual à cadeia de entrada limpa até à posição i-2.
            segunda_cadeia = " ".join(cadeia_tuplo[i - 1:])
            # A segunda_cadeia será igual à cadeia de entrada limpa da posição i-1 até ao final.
            final = (primeira_cadeia, segunda_cadeia)
    return final


def insere_espacos(cadeia, num):
    """
    Recebe uma cadeia de caracteres limpa e um numero inteiro, correspondente à 
    largura da coluna. Se a cadeia tiver duas ou mais palavras a função devolve 
    uma cadeia de carateres de comprimento igual à largura da coluna com espaços
    entre palavras que a compõem.
    Se a função apenas tiver uma palavra devolve a cadeia de comprimento igual à
    largura da coluna formada pela palavra seguida de espaços.
    """
    # corpo da função
    texto_separado = cadeia.split()
    if len(texto_separado) != 1:
        count_space = cadeia.count(' ')  # Conta os espaços na cadeia de entrada.
        quant_space = (num - len(cadeia)) // count_space  # Divisão inteira
        # Dividimos os espaços que nos faltam colocar pelos espaços já existentes.
        rest_space = (num - len(cadeia)) % count_space  # resto
        # Calcula quantos espaços vão sobrar se distribuirmos cada dos espaços que nos faltam colocar pelos
        # espaços já existentes.
        for h in range(rest_space):
            texto_separado[h] += " "  # Adicionamos um espaço às primeiras h palavras
        for j in range(len(texto_separado) - 1):
            espacos_add = (quant_space + 1) * " "
            texto_separado[j] += espacos_add
            # Distribuirmos cada um dos espaços que nos faltam colocar pelos espaços já existentes.
        espacos_inseridos = "".join(texto_separado)
        # Juntamos tudo sem espaços pelo porque os elementos de texto_separado já têm os espaços
    else:
        w = (num - len(cadeia)) * " "
        espacos_inseridos = (limpa_texto(cadeia) + w)
        # Se a cadeia de entrada apenas tiver uma palavra colacam-se os espaços que faltam todos à direita.
    return espacos_inseridos


def justifica_texto(cadeia, n):
    """
    Recebe uma cadeia de carateres não vazia e um inteiro positivo,
    correspondente à largura da coluna.
    Devolve um tuplo com cadeias de carateres, sem caracteres brancos, e de 
    comprimento igual à largura da coluna.
    """
    # corpo da função
    if type(cadeia) != str or cadeia == '' or type(n) != int or n < 1:
        raise ValueError("justifica_texto: argumentos invalidos")

    palavras = cadeia.split()
    if max(len(x) for x in palavras) > n or not isinstance(n, int) or len(cadeia) == 0:
        raise ValueError("justifica_texto: argumentos invalidos")

    cadeia = limpa_texto(cadeia)  # cadeia limpa, sem cracteres brancos
    if n >= len(cadeia):
        return (cadeia + ((n - len(cadeia)) * " "),)
    # Se o tamanho da cadeia for menor que o numero (2º argumento) então o 1º elemento,
    # do tuplo a retornar, será a cadeia

    tuplo1 = corta_texto(cadeia, n)
    justificado = (insere_espacos(tuplo1[0], n),)
    while len(tuplo1[1]) != 0:
        tuplo1 = corta_texto(tuplo1[1], n)
        if len(tuplo1[1]) == 0:
            # No tuplo1[1] está o resto da cadeia de caracteres que ainda não foi colocada dentro da dimensão de
            # larguradada como argunto de entrada. As instruções dentro da condição if só são executadas quando o
            # tuplo1[1] for vazio.
            cadeia_limpa = limpa_texto(tuplo1[0])
            cadeia_esq = cadeia_limpa + ((n - len(cadeia_limpa)) * " ")
            # A cadeia limpa fica encostada à esquerda e é seguida dos espaços necessários para preencher a largura
            justificado += (cadeia_esq,)
        else:
            justificado += ((insere_espacos(tuplo1[0], n)),)

    return justificado


# Exercício 2 do projeto
# Obtem o resultado de eleições


def calcula_quocientes(dic, num):
    """
    Recebe um dicionário com os votos apurados num círculo, com pelo menos um 
    partido, e um inteiro positivo correspondente ao número de deputados.
    Devolve um dicionário diferente do dicionário de entrada com as mesmas 
    chaves do dicionário original e com os valores iguais a listas que contêm os
    quocientes calculados com o método de Hondt ordenados em ordem decrescente.
    """
    # corpo da função
    d = {}
    for i in dic.keys():
        lista = []
        for m in range(1, num + 1):  # (num + 1) Porque também dividimos o valor pelo 2º argumento de entrada.
            lista += [dic[i] / m]
            # Vamos adicionando à lista o valor correspondente à chave dividido por todos os números, começando
            # no 1 e acabando no número introduzido como segundo argumento de entrada, incluindo.
        d[i] = lista
        # Adicionamos ao dicionário (d) o par (chave : elemento) em que a chave pertence ao dicionário (dic)
        # e o valor é a respetiva (lista).
    return d


def atribui_mandatos(dic, num):
    """
    Recebe um dicionário com os votos apurados num círculo e um inteiro positivo
    correspondente ao número de deputados.
    Devolve uma lista ordenada, de dimensão igual ao número de deputados, que 
    contém o nome dos partidos que obtiveram cada mandato.
    """
    # corpo da função
    lista = []
    dic_mandatos = {}
    dic_quocientes = calcula_quocientes(dic, num)
    for i in dic.keys():
        dic_mandatos[i] = 0
        # Dicionário inicializado com todas as chaves, do dicionário de entrada,
        # com o valor 0 associado a cada uma, correspondente a 0 mandatos
    for i in range(num):
        # Os is são os mandatos.
        quocientes = 0
        k = ""
        for n in dic_quocientes.keys():
            # Os ns são os partidos.
            if dic_quocientes[n][dic_mandatos[n]] > quocientes:
                quocientes = dic_quocientes[n][dic_mandatos[n]]
                # Compara os elementos dos tuplos, correspondentes aos valores de dic_quocientes
                k = n  # k é igual ao partido que contém o maior quociente
            elif dic_quocientes[n][dic_mandatos[n]] == quocientes:
                # Caso ambos os partidos tenham o mesmo quociente executam-se as seguintes instruções.
                if dic_quocientes[n][0] < dic_quocientes[k][0]:
                    quocientes = dic_quocientes[n][dic_mandatos[n]]
                    k = n  # k é igual ao partido que tem o maior numero de votos, entre os que têm o mesmo quociente.
        dic_mandatos[k] = dic_mandatos[k] + 1  # Adicionamos um mandato ao/s que o partido já tinha
        lista += [k]
    return lista


def obtem_partidos(info):
    """
    Recebe um dicionário com a informação sobre as eleições num território com 
    vários círculos eleitorais.
    Devolve uma lista ordenada com o nome de todos os partidos que participaram 
    nas eleições.
    """
    # corpo da função
    def remove_repetidos(lst):
        """
        Remove os elementos repetidos de uma lista
        """
        # corpo da função
        l = []
        for a in lst:
            if a not in l:
                l.append(a)
                # Acrescenta à lista que vai ser retornada os elementos da lista de entrada que ainda não lhe pertencem.
        l.sort()
        return l

    lista = []
    for i in info.values():
        for n in i.values():
            # Os ns vão ser ou os números de deputados, ou os dicionários com a informação relativa aos votos.
            if type(n) == dict:  # Porque o nome de cada partido está dentro dos dicionários.
                for j in n.keys():
                    lista += [j]  # Esta lista vai conter o nome de cada partido e alguns podem estár repetidos.
    return remove_repetidos(lista)


def obtem_resultado_eleicoes(info):
    """
    Recebe um dicionário com a informação sobre as eleições num território com 
    vários círculos eleitorais.
    Devolve uma lista ordenada de comprimento igual ao número de partidos que 
    participaram nas eleições.
    Cada elemento da lista é um tuplo, de tamanho 3, contendo o nome de um 
    partido, o número de deputados
    obtidos e o número total de votos.
    A lista encontra-se ordenada por ordem descendente de acordo com o número de
    deputados obtidos por cada partido.
    É ainda de salientar que em caso de empate o deputado escolhido é o do 
    partido com menos votos.
    """
    # corpo da função
    dicm = {}
    dicv = {}
    tuplo = ()
    votos = []
    deputados = 0

    if type(info) != dict:
        raise ValueError("obtem_resultado_eleicoes: argumento invalido")

    if len(info) < 1:
        raise ValueError("obtem_resultado_eleicoes: argumento invalido")
    else:
        for i in info.keys():
            if type(i) != str:
                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
            x = info[i]
            if len(x) != 2 or type(x) != dict:
                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
            else:
                d = 'NOK'
                v = 'NOK'
                for j in x.keys():
                    r = x[j]
                    if type(r) == dict:  # Estamo-nos a referir aos votos
                        v = 'OK'
                        if len(r) < 1:
                            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
                        for l in r.keys():
                            v = r[l]
                            if type(v) != int:
                                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
                    else:  # Estamo-nos a referir aos deputados
                        if type(r) != int:
                            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
                        else:
                            d = 'OK'
                            if r < 1:
                                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
                if d == 'NOK' or v == 'NOK':
                    raise ValueError("obtem_resultado_eleicoes: argumento invalido")
    listap = obtem_partidos(info)
    for i in listap:
        dicm[i] = 0  # Inicialização de dicionário com os mandatos.
        dicv[i] = 0  # Inicialização de dicionário com os votos.

    for n in info.keys():
        # Os ns são os nomes dos circulos eleitorais.
        x = info[n]
        for m in x.keys():
            r = x[m]
            if type(r) == dict:
                votos = r
            else:
                deputados = x[m]

        mandatos = atribui_mandatos(votos, deputados)  # Achamos os mandatos para cada circulo eleitoral.
        for i in listap:
            if i in votos:
                dicv[i] = dicv[i] + votos[i]  # Para cada partido somamos o numero de votos.
            dicm[i] = dicm[i] + mandatos.count(i)  # Para cada partido somamos o numero de mandatos.

    for j in sorted(dicv, key=dicv.get, reverse=True):  # Ordena o dicionário por ordem decrescente do total de votos.
        tuplo += ((j, dicm[j], dicv[j]),)
    return list(tuplo)

# Exercício 3 do projeto
# Soluciona um SEL (sistema de equações lineares)


def produto_interno(vetor1, vetor2):
    """
    Recebe 2 tuplos, do mesmo tamanho, e devolve um número real
    correspondente ao produto interno entre os dois tuplos.
    """
    # corpo da função
    if len(vetor1) == len(vetor2):
        result = 0
        for i in range(len(vetor1)):
            result += float(vetor1[i]) * float(vetor2[i])
            # Multiplicação do 1º elemento de um vetor com o 1º de outro e assim sucessivamente vai-se multiplicando
            # os respetivos elementos dos dois vetores e no final soma-se tudo.
    else:
        raise ValueError("argumentos inválidos")
        # Não é possivel fazer o produto interno entre vetores que não são do mesmo tamanho.
    return result


def verifica_convergencia(tuploa, tuploc, tuplox, precisao):
    """
    Recebe três tuplos do mesmo tamanho e um valor real positivo.
    O primeiro tuplo é um tuplo de tuplos cada um representando uma linha da 
    matriz quadrada (A).
    Os outros dois tuplos de entrada representam respetivamente o vetor de 
    constantes (c)e a solução atual (x).
    O valor real corresponde à precisão pretendida (e).
    A função retorna True se o valor absoluto do erro de todas as equações for 
    inferior a (e) e False caso seja maior
    """
    # corpo da função
    produtos = ()  # Tuplo que posteriormente vai conter a multiplicação entre uma linha da matriz e a solução (x).
    tuplo = ()  # Tuplo que irá conter todos os valores de |fi(x)−ci|
    res = True
    for i in range(len(tuploa)):
        Ax = produto_interno(tuploa[i], tuplox)  # Ax = fi(x)
        produtos += (Ax,)
        dif = abs(produtos[i] - tuploc[i])  # |fi(x)−ci|
        tuplo += (dif,)
    for m in tuplo:  # Para cada elemento do tuplo verifico se algum deles é maior que a precisão (e).
        if m > precisao:
            res = False
    return res


def retira_zeros_diagonal(tuploa, tuploc):
    """
    Recebe um tuplo de tuplos, representando uma matriz e um tuplo de números, 
    representando o vetor das constantes.
    Retorna uma nova matriz com as mesmas linhas que a de entrada, mas com estas
    reordenadas para que não existam zeros na diagonal da matriz. O tuplo das 
    constantes é também reordenado em função das trocas na matriz original.
    """
    # corpo da função
    def swap(lst, pos1, pos2):
        """
        Recebe uma lista com 2 ou mais elementos e duas posições da lista.
        Troca os dois elementos correspondentes às posições de local.
        """
        # corpo da função
        lst[pos1], lst[pos2] = lst[pos2], lst[pos1]  # Troca dois elementos de posição.
        return lst

    tuploa = list(tuploa)
    tuploc = list(tuploc)
    # Mudamos tipo de tuploa e de tuploc para listas porque as listas são mutaveis e os tuplos não.
    for m in range(len(tuploa)):
        for i in range(len(tuploa[m])):
            if tuploa[i][i] == 0 and tuploa[m][i] != 0:
                tuploa = swap(tuploa, i, m)
                tuploc = swap(tuploc, i, m)
    return tuple(tuploa), tuple(tuploc)


def eh_diagonal_dominante(tuplo):
    """
    Recebe um tuplo de tuplos representando uma matriz quadrada.
    Retorna True se a diagonal da matriz for dominante e False caso contrario.
    A diagonal de uma matriz diz-se dominante quando o valor absoluto do numero na diagonal é maior ou igual
    ao valor absoluto da soma dos restantes números nessa linha.
    """
    res = True

    for i in range(len(tuplo)):
        if 2*abs(tuplo[i][i]) - sum([abs(a) for a in list(tuplo[i])]) < 0:
            # Soma-se todos os elementos de uma linha, depois subtrai-se duas vezes o elemento na diagonal da mesma,
            # de modo a que do lado esquerdo da comparação (<=) fique 2 vezes o elemento da linha que pertence à
            # diagonal (-) a soma dos elementos da linha que não lhe pertencem.
            # Assim, do lado direito da comparação terá que estár um 0.
            res = False
            # Basta existir um caso para o qual a condição seguinte se verifique, para que a função retorne False.
    return res


def resolve_sistema(matrix, tuploc, precisao):
    """
    Recebe um tuplo de tuplos representando uma matriz quadrada, um tuplo 
    correspondente ao vetor das constantes, e um número real positivo 
    correspondente à precisão.
    Retorna um tuplo que é a solução do sistema de equações de entrada aplicando
    o método de Jacobi
    """
    if type(matrix) != tuple or type(tuploc) != tuple or type(precisao) != float or precisao < 0:
        raise ValueError("resolve_sistema: argumentos invalidos")

    nlinhas = len(tuploc)
    nmatrix = len(matrix)

    if nmatrix != nlinhas:
        raise ValueError("resolve_sistema: argumentos invalidos")

    for row in matrix:
        if type(row) != tuple or len(row) != nlinhas:
            raise ValueError("resolve_sistema: argumentos invalidos")
        for b in row:
            if type(b) != int and type(b) != float:
                raise ValueError("resolve_sistema: argumentos invalidos")

    for k in tuploc:
        if type(k) != int and type(k) != float:
            raise ValueError("resolve_sistema: argumentos invalidos")

    w = retira_zeros_diagonal(matrix, tuploc)
    x = [0] * len(w[0])  # Tuplo com tantos zeros quanto a matriz tem de elementos, vai sendo alterado.
    if not eh_diagonal_dominante(w[0]):
        raise ValueError("resolve_sistema: matriz nao diagonal dominante")

    while not verifica_convergencia(w[0], w[1], x, precisao):
        r = x.copy()
        # É necessário criar uma copia shallow para que os elementos do tuplo x sejam  todos atualizados ao mesmo tempo.
        for i in range(len(w[0])):
            r[i] = (x[i] + (w[1][i] - produto_interno(w[0][i], x)) / w[0][i][i])  # Método de Jacobi.
        x = r

    return tuple(x)