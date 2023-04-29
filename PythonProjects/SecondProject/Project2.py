"""
2º Projeto de Fundamentos da Programação (mineswepper)
Sofia Dinis Pinto Piteira
sofia.piteira@tecnico.ulisboa.pt
11 de Novembro de 2022
"""


# Descrição do jogo, mineswepper.
# Existe um campo com minas escondidas em algumas das suas parcelas.
# Outras parcelas do campo contêm pistas sobre o número de minas que lhes são vizinhas (0 é omitido).
# Cada parcela pode se encontrar tapada (verde), limpa (castanha) ou marcada (bandeira vermelha).
# Inicialmente todas as parcelas estão tapadas. A cada ação o jogador pode escolher marcar ou limpar uma parcela.
# O jogo começa quando o jogador seleciona, pela primeira vez, uma parcela do campo que deseja limpar.
# É garantido que  nem a primeira parcela selecionada, nem as suas parcelas vizinhas escondem uma mina.
# Se o jogador escolhe limpar uma parcela que esconde uma mina, a mina rebenta e o jogo acaba.
# Se o jogador decidir limpar uma parcela que não esconde uma mina, esta é limpa e passa a mostrar uma pista sobre
# o número de minas vizinhas. Caso não exista nenhuma mina vizinha, as parcelas vizinhas são limpas automaticamente.
# Ao marcar uma parcela com uma bandeira o jogador assinala a possível existência de uma mina escondida nessa parcela.
# Para além do mais, o jogo mostra o número total de minas escondidas nas parcelas do campo.
# O objetivo do jogo é limpar o campo sem rebentar nenhuma das minas.


# Exercicio 2.1.1


def cria_gerador(b, s):  # construtor
    """
    Recebe um inteiro b correspondente ao n´umero de bits do gerador e um inteiro positivo s correspondente à seed.
    Devolve o gerador correspondente, em forma de lista.
    """
    if not isinstance(s, int) or s <= 0:
        raise ValueError("cria_gerador: argumentos invalidos")
    elif not isinstance(b, int) or (b != 64 and b != 32):
        raise ValueError("cria_gerador: argumentos invalidos")
    # Se a dimensão do estado do gerador for 32 a seed não pode ser maior que 2^32.
    # Se a dimensão do estado do gerador for 64 a seed não pode ser maior que 2^64.
    if b == 32 and abs(s) > 0xffffffff or b == 64 and abs(s) > 0xFFFFFFFFFFFFFFFF:
        raise ValueError("cria_gerador: argumentos invalidos")

    return [b, s]  # lista com o número de bits e o seed (argumentos de entrada).


def cria_copia_gerador(g):  # construtor
    """
    Recebe um gerador e retorna uma cópia shallow do mesmo.
    """
    return g.copy()


def obtem_estado(g):  # seletor
    """
    Retorna o estado atual do gerador, sem o alterar.
    """
    return g[1]

def obtem_dimensao(g):  # seletor
    """
    Retorna a dimensão do estado do gerador, sem o alterar.
    """
    return g[0]

def define_estado(g, s):  # modificador
    """
    Define o novo valor do estado do gerador g como sendo s.
    Retorna s.
    """
    g[1] = s
    return s


def atualiza_estado(g):  # modificador
    """
    Atualiza o estado do gerador, de acordo com o algoritmo xorshift
    """
    if obtem_dimensao(g) == 32:
        g[1] ^= (obtem_estado(g) << 13) & 0xFFFFFFFF
        g[1] ^= (obtem_estado(g) >> 17) & 0xFFFFFFFF
        g[1] ^= (obtem_estado(g) << 5) & 0xFFFFFFFF
    else:  # Se a dimensão do estado do gerador for 64.
        g[1] ^= (obtem_estado(g) << 13) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (obtem_estado(g) >> 7) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (obtem_estado(g) << 17) & 0xFFFFFFFFFFFFFFFF
    return obtem_estado(g)


def eh_gerador(arg):  # reconhecedor
    """
    Retorna True se o arg for um gerador e False se não for.
    """
    if type(arg) == list:
        if len(arg) == 2:
            if isinstance(obtem_dimensao(arg), int) and (obtem_dimensao(arg) == 32 or obtem_dimensao(arg) == 64):
                if isinstance(obtem_estado(arg), int) and obtem_estado(arg) > 0:
                    # Se a dimensão do estado do gerador for 32 a seed não pode ser maior que 2^32.
                    if obtem_dimensao(arg) == 32 and abs(obtem_estado(arg)) <= 0xffffffff:
                        return True
                    # Se a dimensão do estado do gerador for 64 a seed não pode ser maior que 2^64.
                    elif obtem_dimensao(arg) == 64 and abs(obtem_estado(arg)) <= 0xFFFFFFFFFFFFFFFF:
                        return True
        else:
            return False
    return False


def geradores_iguais(g1, g2):  # teste
    """
    Recebe dois geradores.
    Retorna True se os dois geradores são iguais e False se não são iguais.
    """
    return g1 == g2


def gerador_para_str(g):  # transformador
    """
    Devolve uma cadeia de carateres que representa o argumento da função.
    """
    return "xorshift" + str(obtem_dimensao(g)) + "(s=" + str(obtem_estado(g)) + ")"


def gera_numero_aleatorio(g, n):  # Função de alto nível
    """
    Atualiza o estado do gerador e devolve um número aleatório no intervalo [1, n] obtido a partir do novo estado
    com o resto da divisão inteira entre o novo estado e o 2º argumento de entrada + 1.
    """
    # Aplica-se o algoritmo xorshift
    s = define_estado(g, atualiza_estado(g))  # Atualiza o estado e devolve o novo valor do estado.
    numero_aleatorio = 1 + (s % n)
    if 1 <= numero_aleatorio <= n:  # Verificar se o número está no intervalo [1, n].
        return numero_aleatorio


def gera_carater_aleatorio(g, c):  # Função de alto nível
    """
    Atualiza o estado do gerador.
    Retorna um carater aleatório no intrevalo entre A e c, que corresponde a uma letra maiúscula.
    Sendo este carater o que está na posição s % l.
    """
    # Aplica-se o algoritmo xorshift
    s = define_estado(g, atualiza_estado(g))  # Atualiza o estado e devolve o novo valor do estado.
    cadeia = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  # Cadeia de caracteres com todos os carteres posíveis, ordenados.
    cadeia_utilizada = cadeia[:cadeia.find(c) + 1]  # "A...c"
    l = len(cadeia_utilizada)
    pos = s % l
    return cadeia_utilizada[pos]


# Exercicio 2.1.2


def cria_coordenada(col, lin):  # construtor
    """
    Recebe uma letra correspondente à coluna e um número correspondente à linha e cria uma coordenada.
    """
    if not isinstance(col, str) or len(col) != 1 or col not in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
        raise ValueError("cria_coordenada: argumentos invalidos")
    if not isinstance(lin, int) or 99 < lin or lin < 1:
        raise ValueError("cria_coordenada: argumentos invalidos")

    return (col, lin)  # Tuplo com uma letra maiúscula correspondente à coluna e com um número correspondente à linha.


def obtem_coluna(c):  # seletor
    """
    Retorna a coluna da coordenada dada como argumento de entrada.
    """
    return c[0]  # Uma letra maiúscula.


def obtem_linha(c):  # seletor
    """
    Retorna a linha da coordenada dada como argumento de entrada.
    """
    return c[1]  # um número entre 1 e 99


def eh_coordenada(arg):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma coordenada.
    """
    if isinstance(arg, tuple):
        if isinstance(obtem_coluna(arg), str) and len(obtem_coluna(arg)) == 1 and \
                obtem_coluna(arg) in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
            if isinstance(obtem_linha(arg), int) and 99 >= obtem_linha(arg) >= 1:
                return True
    return False  # Só é uma coordenada se for um tuplo.


def coordenadas_iguais(c1, c2):  # teste
    """
    Verifica se os dois argumentos de entrada são coordenadas e se são iguais.
    """
    return eh_coordenada(c1) and eh_coordenada(c2) and c1 == c2


def coordenada_para_str(c):  # transformador
    """
    Recebe uma coordenada e devolve uma cadeia de caracteres que a representa
    """
    num_str = str(obtem_linha(c))
    if len(num_str) == 1:  # Se o número, na coordenada, apenas tiver 1 digito ...
        return obtem_coluna(c) + "0" + num_str  # ... na representação da coordenada acrescentamos um zero antes desse número.
    else:
        return obtem_coluna(c) + num_str  # concatenação de strings


def str_para_coordenada(s):  # transformador
    """
    Dada uma cadeia de caracteres que representa uma coordenada, devolve a respetiva coordenada.
    """
    if obtem_linha(s) == "0":  # Porque a cadeia de caracteres vai sempre ter 3 elementos.
        return cria_coordenada(obtem_coluna(s), int(s[2]))
    else:
        return cria_coordenada(obtem_coluna(s), int(s[1:3]))


def obtem_coordenadas_vizinhas(c):  # Função de alto nível
    """
    Recebe uma coordenada e retorna um tuplo com as coordenadas vizinhas a essa.
    As coordenadas, dentro do tuplo, estão ordenadas pelo sentido horário, começando na diagonal superior esquerda.
    """
    tuplo = ()  # Tuplo que vai conter as coordenadas vizinhas.
    col = obtem_coluna(c)
    lin = obtem_linha(c)
    # A coluna da coordenada á identificada com um dos caracteres nesta cadeia de caracteres.
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    # Indice da letra, em alfabeto, que identifica a coluna onde se encontra a coordenada, permite identificar quais são
    # as letras que vão identificar as colunas onde se encontram as coordenadas vizinhas a essa coordenada.
    index_col = alfabeto.find(col)
    if index_col - 1 >= 0:  # - 1 para que uma coordenada na coluna Z não seja vizinha de uma na A.
        if lin - 1 <= 99 and lin - 1 >= 1:
            tuplo += ((alfabeto[index_col - 1], lin - 1),)
    if lin - 1 <= 99 and lin - 1 >= 1:
        tuplo += ((alfabeto[index_col], lin - 1),)
    if index_col + 1 <= len(alfabeto) - 1:  # - 1 para que uma coordenada na coluna A não seja vizinha de uma na Z.
        if lin - 1 <= 99 and lin - 1 >= 1:
            tuplo += ((alfabeto[index_col + 1], lin - 1),)
        tuplo += ((alfabeto[index_col + 1], lin),)
        if lin + 1 <= 99 and lin + 1 >= 1:
            tuplo += ((alfabeto[index_col + 1], lin + 1),)
    if lin + 1 <= 99 and lin + 1 >= 1:
        tuplo += ((alfabeto[index_col], lin + 1),)
    if index_col - 1 >= 0:  # - 1 para que uma coordenada na coluna Z não seja vizinha de uma na A.
        if lin + 1 <= 99 and lin + 1 >= 1:
            tuplo += ((alfabeto[index_col - 1], lin + 1),)
        tuplo += ((alfabeto[index_col - 1], lin),)
    # As instruções acima estão ordenadas, de forma aque as coordenas vizinhas, dentro do tuplo, estejam ordenadas pelo
    # sentido horário, começando na diagonal superior esquerda.
    return tuplo


def obtem_coordenada_aleatoria(c, g):  # Função de alto nível
    """
    Recebe um gerador e uma coordenada que define qual a coluna e linha máxima.
    Retorna uma coordenada aleatória.
    """
    # Aplica-se o algoritmo xorshift à coluna e à linha.
    col = gera_carater_aleatorio(g, obtem_coluna(c))
    lin = gera_numero_aleatorio(g, obtem_linha(c))
    return cria_coordenada(col, lin)


# Exercicio 2.1.3


def cria_parcela():  # construtor
    """
    Devolve um dicionário no qual as chaves correspondem aos estados possiveis e os valores são booleanos.
    Se a parcela estiver num certo estado o valor correspondente a essa chave será True, caso contrário será False.
    Esta parcela devolve uma parcela tapada sem mina escondida.
    Logo todos os valores do dicionário serão False, à excessão do associado à chave Tapada que será True.
    """
    return {"Tapada": True, "Minada": False, "Limpa": False, "Marcada": False}


def cria_copia_parcela(p):  # construtor
    """
    Recebe uma parcela e retorna uma cópia da mesma.
    """
    return p.copy()


def limpa_parcela(p):  # modificador
    """
    Esta função recebe uma parcela e devolve a mesma parcela após ter modificado o seu estado para limpa.
    """
    for k in p.keys():
        if k != "Limpa" and k != "Minada":  # Se a parcela tiver uma mina após ter sido limpa continua com a mina.
            p[k] = False
    p["Limpa"] = True
    return p


def marca_parcela(p):  # modificador
    """
    Esta função recebe uma parcela e devolve a mesma parcela após a ter marcado.
    """
    for k in p.keys():
        if k != "Marcada" and k != "Minada":  # Se a parcela tiver uma mina após marcada continua com a mina.
            p[k] = False
    p["Marcada"] = True
    return p


def desmarca_parcela(p):  # modificador
    """
    Esta função recebe uma parcela e devolve a mesma parcela tapada.
    """
    for k in p.keys():
        if k != "Tapada" and k != "Minada":  # Se a parcela tiver uma mina após ser desmarcada continua com a mina.
            p[k] = False
    p["Tapada"] = True
    return p


def esconde_mina(p):  # modificador
    """
    Esta função recebe uma parcela e devolve a mesma parcela com uma mina.
    """
    p["Minada"] = True  # O estado "Minada" é independente de todos os outros estados
    return p


# Em relação às ultimas funções modificadoras é importante ter em atenção que os estados "Tapada", "Limpa" e "Marcada"
# não coexistem e que pelo contrário o estado "Minada" é independente de todos os outros estados.


def eh_parcela(arg):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma parcela.
    Se for uma parcela retorna True, caso contrário retorna False.
    """
    # Todas as parcelas têm como chaves estas 4 strings.
    chaves = ["Tapada", "Minada", "Limpa", "Marcada"]
    if isinstance(arg, dict):  # Não é parcela senão for um dicionário.
        chaves_dadas = []
        for k in arg.keys():
            if type(arg[k]) != bool:  # Não é parcela se os valores do dicionário não forem booleanos.
                return False
            # Se o argumento de entrada for um dicionário e os valores forem booleanos cria uma lista com as chaves
            # do argumento de entrada.
            chaves_dadas += [k]
        if sorted(chaves_dadas) != sorted(chaves):
            return False
        return True
    return False


def eh_parcela_tapada(p):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma parcela tapada.
    Se for uma parcela tapada retorna True, caso contrário retorna False.
    """
    return p["Tapada"]


def eh_parcela_marcada(p):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma parcela marcada.
    Se for uma parcela  marcada retorna True, caso contrário retorna False.
    """
    return p["Marcada"]


def eh_parcela_limpa(p):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma parcela limpa.
    Se for uma parcela limpa retorna True, caso contrário retorna False.
    """
    return p["Limpa"]


def eh_parcela_minada(p):  # reconhecedor
    """
    Verifica se o argumento de entrada é uma parcela com mina.
    Se for uma parcela com mina retorna True, caso contrário retorna False.
    """
    return p["Minada"]


def parcelas_iguais(p1, p2):  # teste
    """
    Recebe duas parcelas e verifica se estas são iguais.
    Se ambas forem iguais retorna True, caso contrário retorna False.
    """
    return eh_parcela_minada(p1) == eh_parcela_minada(p2) and eh_parcela_limpa(p1) == eh_parcela_limpa(p2) and \
           eh_parcela_marcada(p1) == eh_parcela_marcada(p2) and eh_parcela_tapada(p1) == eh_parcela_tapada(p2)


def parcela_para_str(p):  # transformador
    """
    Recebe uma pacela.
    Retorna uma cadeia de caracteres que representa o estado da parcela.
    parcela tapada ’#’
    parcela marcada ’@’
    parcela limpa sem mina ’?’
    parcela limpa com mina ’X’
    """
    if eh_parcela_tapada(p):
        return "#"
    elif eh_parcela_marcada(p):
        return "@"
    elif eh_parcela_limpa(p) and not eh_parcela_minada(p):
        return "?"
    elif eh_parcela_limpa(p) and eh_parcela_minada(p):
        return "X"


def alterna_bandeira(p):  # Função de alto nível
    """
    Recebe uma parcela.
    Se a parcela estiver marcada desmarca-a e retorna True.
    Se a parcela estiver tapada marca-a e retorna True.
    Se a parcela não estiver marcada nem tapada, não modifica a parcela e retorna False.
    """
    if eh_parcela_marcada(p):
        desmarca_parcela(p)
        return True
    elif eh_parcela_tapada(p):
        marca_parcela(p)
        return True
    else:
        return False  # A parcela não foi modificada


# Exercicio 2.1.4


def cria_campo(c, l):  # construtor
    """
    Recebe uma cadeia de caracteres (uma letra maiúscula) e um número entre 1 e 99 que correspodem, respetivamente,
    à última coluna e à última linha de um campo.
    Retorna um campo, com o tamanho indicado pelos argumentos de entrada, com parcelas tapadas e sem minas.
    """
    if not isinstance(c, str) or len(c) != 1 or c not in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
        raise ValueError("cria_campo: argumentos invalidos")
    if not isinstance(l, int) or not 99 >= l or not l >= 1:
        raise ValueError("cria_campo: argumentos invalidos")
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    index_col = alfabeto.find(c)
    # Novo alfabeto com as letras correspondentes às colunas do campo, que é suposto criar.
    # +1, porque a letra "A" é o indice 0 da cadeia de caracteres alfabeto.
    alfabeto_utilizado = alfabeto[:index_col + 1]
    dic = {}  # Dicionário que vai ter as coordenas do campo como chaves e parcelas como valores.
    for letra in alfabeto_utilizado:
        for num in range(1, l + 1):  # Números correspondentes às linhas do campo, que é suposto criar.
            dic[cria_coordenada(letra, num)] = {"Tapada": True, "Minada": False, "Limpa": False, "Marcada": False}
    return dic


def cria_copia_campo(m):  # construtor
    """
    Recebe um campo e retorna uma cópia do mesmo.
    """
    dic = {}  # Novo dicionário.
    for c in m.keys():
        dic[c] = cria_copia_parcela(obtem_parcela(m, c))
    return dic


def obtem_ultima_coluna(m):  # seletor
    """
    Recebe um campo e retorna uma letra maiúscula, que corresponde à ultima coluna do respetivo campo.
    """
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    lista_col = []  # Lista que vai conter o indice, em alfabeto, de todas as letras que são colunas do campo.
    for k in m.keys():
        index_col = alfabeto.find(obtem_coluna(k))
        lista_col += [index_col]
    # Maior número da lista, que corresponde ao indice, em alfabeto, da letra que representa a ultima coluna do campo.
    index_maior = max(lista_col)
    return alfabeto[index_maior]


def obtem_ultima_linha(m):  # seletor
    """
    Recebe um campo e retorna um número, que corresponde à ultima linha do respetivo campo.
    """
    lista_lin = []  # Lista que vai conter todas as linhas do campo.
    for k in m.keys():
        lista_lin += [obtem_linha(k)]
    return max(lista_lin)  # Maior número da lista que contém todas as linhas do campo.


def obtem_parcela(m, c):  # seletor
    """
    Recebe um campo e uma coordenada e retorna a parcela associada à respetiva coordenada.
    """
    return m[c]


def obtem_coordenadas(m, s):
    """
    Recebe um campo e uma string.
    Retorna um tuplo formado com coordenadas ordenadas ascendentemente da esquerda para a  direita e de cima a baixo,
    que dependem da string (argumento de entrada).
    ’limpas’ - retorna tuplo com as parcelas limpas.
    ’tapadas’ - retorna tuplo com as parcelas tapadas.
    ’marcadas’ - retorna tuplo com as parcelas marcadas.
    ’minadas’ - retorna tuplo com as parcelas minadas.
    """
    tuplo = ()  # Lista que vai conter as coordenadas pedidas pela string (argumento de entrada).
    ultima_col = obtem_ultima_coluna(m)
    ultima_lin = obtem_ultima_linha(m)
    # Tem espaços entre as letras maiúculas para depois se poder aplicar a função split().
    alfabeto = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
    index_col = alfabeto.find(ultima_col)
    # Novo alfabeto com as letras correspondentes às colunas do campo (argumento de entrada).
    alfabeto_novo = (alfabeto[:index_col + 1])  # +1, porque a letra "A" é o indice 0 da cadeia de caracteres alfabeto.
    for num in range(1, ultima_lin + 1):  # Números correspondentes às linhas do campo (argumento de entrada).
        for letra in alfabeto_novo.split():
            cord = cria_coordenada(letra, num)
            # s.capitalize()[:len(s) - 1], com isto estou a transformar a string (argumento de entrada) numa string
            # equivalente a uma das chaves do campo (argumento de entrada).
            if m[cord][s.capitalize()[:len(s) - 1]]:
                # Se a parcela estiver no estado indicado pela string (argumento de entrada), então acrescenta-se
                # a coordenada ao tuplo.
                tuplo += (cord, )
    return tuplo


def obtem_numero_minas_vizinhas(m, c):  # seletor
    """
    Recebe um campo e uma coordenada.
    Retorna um número correspondente ao número de parcelas vizinhas que se encontram Minadas.
    """
    # Tuplo com as coordenadas vizinhas ordenadas pelo sentido horário, começando na diagonal superior esquerda.
    cord_vizinhas = obtem_coordenadas_vizinhas(c)
    contador = 0
    # Conta das coordenadas no tuplo têm uma mina.
    for cord in cord_vizinhas:
        if cord in m and eh_parcela_minada(obtem_parcela(m, cord)):
            contador += 1
    return contador


def eh_campo(arg):  # reconhecedor
    """
    Verifica se o argumento de entrada é um campo, se for retorna True, senão retorna False.
    """
    if type(arg) == dict:
        if len(arg) != 0:
            if all(isinstance(key, tuple) for key in arg.keys()):
                for e in arg.keys():
                    # Só é um campo se o primeiro elemento das chaves for uma letra maiúscula e o segundo for um número
                    # inteiro ente 1 e 99.
                    if not isinstance(obtem_coluna(e), str) or len(obtem_coluna(e)) != 1 or \
                            obtem_coluna(e) not in "ABCDEFGHIJKLMNOPQRSTUV":
                        return False
                    if not isinstance(obtem_linha(e), int) or 99 < obtem_linha(e) < 1:
                        return False
                for val in arg.values():
                    # Só é um campo se os valores forem parcelas.
                    if not eh_parcela(val):
                        return False
            else:
                return False  # Só é um campo se as chaves forem tuplos.
        else:
            return False  # Só é campo se o dicionário não for vazio.
    else:
        return False  # Só é um campo se for um dicionário.
    return True


def eh_coordenada_do_campo(m, c):  # reconhecedor
    """
    Recebe um campo e uma coordenada.
    Retorna True se a respetiva coordenada pertence ao campo dado e False caso contrário.
    """
    tuplo_k = ()  # Tuplo que vai conter todas as coordenadas do campo.
    for k in m.keys():
        tuplo_k += (k,)
    # Se a coordenada estiver no tuplo retorna-se True, senão retorna-se False.
    if c in tuplo_k:
        return True
    else:
        return False


def campos_iguais(m1, m2):  # teste
    """
    Recebe dois campos.
    Retorna True se forem ambos iguais e False caso contrário.
    """
    return m1 == m2


def campo_para_str(m):  # transformador
    """
    Recebe um campo e retorna uma cadeia de caracteres que o representa.
    """
    cadeia_total = ""  # Vai ter os números das linhas seguido dos estados (simbolos) das parcelas na respetiva linha.
    ultima_col = obtem_ultima_coluna(m)
    ultima_lin = obtem_ultima_linha(m)
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    # Tem espaços entre as letras maiúsculas para depois se poder aplicar a função split().
    alfabeto1 = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
    index_col = alfabeto.find(ultima_col)
    index_col1 = alfabeto1.find(ultima_col)
    alfabeto_novo = (alfabeto[:index_col + 1])  # Letras correspondentes às colunas do campo sem espaços entre elas.
    alfabeto_novo1 = (alfabeto1[:index_col1 + 1])  # Letras correspondentes às colunas do campo com espaços entre elas.
    tam = len(alfabeto_novo)  # Número de colunas do campo.
    for num in range(1, ultima_lin + 1):  # +1, porque queremos que contablize a ultima linha do campo.
        # Se o número, que representa a linha da parcela, só tiver um digito tem-se de acrescentar um 0 a sse número.
        if len(str(num)) == 1:
            s = "0" + str(num)
        else:
            s = str(num)
        cadeia = s + "|"  # Após o número, da linha, coloca-se uma barra vertical.
        for letra in alfabeto_novo1.split():
            cord = cria_coordenada(letra, num)
            cad = parcela_para_str(obtem_parcela(m, cord))
            if cad == "?":
                # Se a parcela estiver limpa e sem mina e se o número de minas nas coordenadas vizinhas for diferente
                # de 0 troca-se o ponto de interrogação pelo número de minas vizinhas.
                if obtem_numero_minas_vizinhas(m, cord) != 0:
                    cad = str(obtem_numero_minas_vizinhas(m, cord))
                # Se a parcela estiver limpa e sem mina e não tiver minas vizinhas troca-se o ponto de interrogação
                # por um espaço.
                else:
                    cad = " "
            # A cadeia é composta por um número que diz respeito a uma linha, seguido de uma barra vertical, seguida
            # dos estados (simbolos) das parcelas na respetiva linha.
            cadeia += cad
        cadeia_total += cadeia + "|\n"
    return 3 * " " + alfabeto_novo + "\n" + 2 * " " + "+" + tam * "-" + "+\n" + cadeia_total + 2 * " " + "+" + \
           tam * "-" + "+"


def coloca_minas(m, c, g, n):  # função de alto nivel
    """
    Recebe, por esta ordem, um campo, uma coordenada, um gerador e um número coorespondente ao número de minas no campo.
    São geradas em sequência, com o gerador (argumento de entrada), tantas cordenadas quanto o último argumento de
    entrada indicar.
    Retorna um campo com minas nas coordenadas que foram geradas.
    """
    # Aplica-se o algoritmo xorshift à coluna e à linha.
    lista = []  # Lista que vai conter as coordenadas, onde é suposto esconder uma mina.
    coordenadas_vizinhas = obtem_coordenadas_vizinhas(c)
    # Enquanto a lista não tiver o mesmo número de coordenadas, que o campo é suposto ter de minas.
    while len(lista) < n:
        coordena_al = obtem_coordenada_aleatoria(cria_coordenada(obtem_ultima_coluna(m), obtem_ultima_linha(m)), g)
        if coordena_al not in lista and coordena_al not in coordenadas_vizinhas and \
                not coordenadas_iguais(coordena_al, c):
            lista.append(coordena_al)  # Se cumprir todas as condições acima adiciona-se a coordenada à lista.
    # Após o ciclo while acabar, coloca-se uma mina em cada uma das coordenas da lista.
    for cord in lista:
        m[cord] = esconde_mina(obtem_parcela(m, cord))
    return m


def limpa_campo(m, c):  # função de alto nivel
    """
    Recebe um campo e uma coordenada.
    Limpa a parcela associada à coordenada (argumento de entrada), se nenhuma das parcelas vizinhas tiver uma mina
    limpa-as iterativamente.
    Caso alguma das parcelas se encontre já limpa a operação não tem efeito.
    """
    vizinhas = [c, ]
    # Se a coordenada tiver uma mina, as vizinhas não são limpas. Só a parcela associada à coordenada é que é limpa.
    if eh_parcela_minada(obtem_parcela(m, c)):
        limpa_parcela(obtem_parcela(m, c))
    if not eh_parcela_minada(obtem_parcela(m, c)):
        while len(vizinhas) > 0:  # Enquanto a lista vizinhas tiver coordenadas.
            # Mesmo que alguma das vizinhas tenha uma mina limpa-se a coordenada.
            m[vizinhas[0]] = limpa_parcela(obtem_parcela(m, vizinhas[0]))
            # Só se limpam todas as parcelas vizinhas, se nenhuma delas tiver uma mina.
            if obtem_numero_minas_vizinhas(m, vizinhas[0]) == 0:
                for cordv in obtem_coordenadas_vizinhas(vizinhas[0]):
                    if eh_coordenada_do_campo(m, cordv) and \
                            not eh_parcela_marcada(obtem_parcela(m, cordv)) and \
                            not eh_parcela_limpa(obtem_parcela(m, cordv)) and \
                            not eh_parcela_minada(obtem_parcela(m, cordv)) and cordv not in vizinhas:
                        vizinhas += [cordv, ]
            # Depois de limpar a parcela e adicionar as suas vizinhas à lista vizinhas, caso  nenhuma destas tenha uma
            # mina, remove-se a coordenada da lista, passando outra coordenada a ser a viinhas[0].
            vizinhas.remove(vizinhas[0])
    return m


# Exercicio 2.2 - Funções adicionais
# Exercicio 2.2.1


def jogo_ganho(m):  # função auxiliar
    """
    Recebe um campo e retorna True caso todas as parcelas que não têm minas estejam limpas e False caso contrário.
    """
    n_limpa = 0  # contador
    for coordenada in m.keys():
        if not eh_parcela_minada(obtem_parcela(m, coordenada)) and not eh_parcela_limpa(obtem_parcela(m, coordenada)):
            n_limpa += 1
    return n_limpa == 0  # Se houver alguma parcela sem mina por limpar n_limpa vai ser diferente de 0


# Exercicio 2.2.2

# Tanto na função turno_jogador, como na função minas usam-se ciclos while para pedir ao jogador coordenadas e ações,
# porque é suposto repetir a pergunta até que o jogador introduza uma resposta válida.


def turno_jogador(m):  # função auxiliar
    """
    Esta função recebe um campo.
    Dá ao jogador a hipotese de limpar ou marcar uma coordenada e pede-lhe para indicar qual a coordenada
    à qual quer aplicar a ação escolhida.
    Caso o jogador escolha limpar a coordenada a função limpa a coordenada, se esta tiver uma mina retorna False,
    caso contrário limpa o campo nessa coordenada (limpa_campo(m, coordenada))e retorna True.
    Se o jogador decidir marcar a coordenada a função muda o estado da parcela para Marcada e retorna True.
    """
    # As 3 cadeias de caracteres, a baixo, indicam quais os caracteres possiveis para cada um dos caracteres das
    # coordenadas quando estas estão na forma de string.
    numeros0 = "0123456789"
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    act, cord = "A", "ABC"  # Duas strings que não vão cumprir as condições atribuidas aos respetivos ciclos while
    while act != "L" and act != "M":
        act = input("Escolha uma ação, [L]impar ou [M]arcar:")
    while len(str(cord)) != 3 or str(cord)[0] not in alfabeto or str(cord)[1] not in numeros0 or \
            str(cord)[2] not in numeros0 or cord[1:3] == "00" or \
            not eh_coordenada_do_campo(m, str_para_coordenada(str(cord))) or \
            eh_parcela_limpa(obtem_parcela(m, str_para_coordenada(str(cord)))):
        cord = input("Escolha uma coordenada:")
    if act == "L":
        limpa_campo(m, str_para_coordenada(cord))
        # Mesmo que a parcela tenha uma mina limpa-se a parcela.
        if eh_parcela_minada(obtem_parcela(m, str_para_coordenada(cord))):
            return False
    if act == "M":
        if not eh_parcela_marcada(obtem_parcela(m, str_para_coordenada(cord))):
            marca_parcela(obtem_parcela(m, str_para_coordenada(cord)))
        else:
            desmarca_parcela(obtem_parcela(m, str_para_coordenada(cord)))
    return True


# Exercicio 2.2.2


def minas(c, l, n, d, s):  # função principal
    """
    Esta função é a função que permite jogar o jogo.
    Recebe, por esta ordem, um caracter (letra maiúscula), correspondente à ultima coluna do campo, um número entre
    1 e 99, correspondente à ultima linha do campo, um número correspondente ao número de parcelas com mina,
    o número 32 ou o 64 que corresponde à dimensão do gerador e um número correspondente ao estado inicial (seed)
    Retorna True, se o jogador ganhar o jogo e False caso o jogador limpe uma parcela com mina.
    """
    # As 3 cadeias de caracteres, a baixo, indicam quais os caracteres possiveis para cada um dos caracteres das
    # coordenadas quando estas estão na forma de string.
    numeros0 = "0123456789"
    alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    # Primeiro é necessário fazer estas validações, porque senão não dá para fazer as contas a baixo.
    if not eh_coordenada((c, l)) or not eh_gerador([d, s]) or not isinstance(n, int) or n <= 0:
        raise ValueError("minas: argumentos invalidos")
    # Só depois de fazer estas contas é que posso fazer a validação a baixo.
    col_index = alfabeto.find(c)
    n_parcelas = (col_index + 1) * l  # Número de parcelas do campo.
    # O número de minas no campo não pode ser maior que o número de parcelas do campo menos 9, porque a primeira parcela
    # selecionada pelo jogador e as suas vizinhas não podem ter minas.
    if n > (n_parcelas - 9):
        raise ValueError("minas: argumentos invalidos")
    m = cria_campo(c, l)
    n_marcadas = 0  # contador
    print("   [Bandeiras " + str(n_marcadas) + "/" + str(n) + "]")
    print(campo_para_str(m))
    coord = "ABC"  # string que não cumpra a condição seguinte.
    while len(str(coord)) != 3 or str(coord)[0] not in alfabeto or str(coord)[1] not in numeros0 or\
            str(coord)[2] not in numeros0 or coord[1:3] == "00" or \
            not eh_coordenada_do_campo(m, str_para_coordenada(str(coord))):
        coord = input("Escolha uma coordenada:")
    # O campo começa sem minas. Após o jogador escolher uma coordenada, são colocadas minas consoante a coordenada
    # escolhida e de seguida o campo é limpo nessa coordenada.
    coloca_minas(m, str_para_coordenada(coord), cria_gerador(d, s), n)
    limpa_campo(m, str_para_coordenada(coord))
    # Indica quantas parcelas estão marcadas e quantas minas existem.
    print("   [Bandeiras " + str(n_marcadas) + "/" + str(n) + "]")
    print(campo_para_str(m))
    while not jogo_ganho(m):  # Enquanto houver parcelas sem mina por limpar.
        n_marcadas = 0
        if turno_jogador(m):
            # Ciclo for para saber quantas parcelas do campo se encontram marcadas.
            for k in m.keys():
                if eh_parcela_marcada(obtem_parcela(m, k)):
                    n_marcadas += 1  # Número de parcelas marcadas.
            print("   [Bandeiras " + str(n_marcadas) + "/" + str(n) + "]")
            print(campo_para_str(m))
        else:  # Perdeu o jogo
            # Ciclo for para saber quantas parcelas do campo se encontram marcadas.
            for k in m.keys():
                if eh_parcela_marcada(obtem_parcela(m, k)):
                    n_marcadas += 1
            # Antes de se apresentar a derrota aparece o campo com a parcela minada limpa (X).
            print("   [Bandeiras " + str(n_marcadas) + "/" + str(n) + "]")
            print(campo_para_str(m))
            print("BOOOOOOOM!!!")  # Mina rebentou.
            return False
    print("VITORIA!!!")  # Ganhou o jogo
    return True